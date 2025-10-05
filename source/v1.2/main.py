import numpy as np
import pygame as pg
import time

# initialize pygame
pg.init()

# define basic things
width, height = 1280, 720
screen = pg.display.set_mode((width, height))
clock = pg.time.Clock()

# hide mouse
pg.mouse.set_visible(False)
pg.event.set_grab(True)

# rotation matrices
def rotXmat(ang):
    return np.array([
        [1, 0, 0],
        [0, np.cos(ang), -np.sin(ang)],
        [0, np.sin(ang), np.cos(ang)]
    ])
def rotYmat(ang):
    return np.array([
        [np.cos(ang), 0, np.sin(ang)],
        [0, 1, 0],
        [-np.sin(ang), 0, np.cos(ang)]
    ])

# moving matrices
def forward(yaw):
    return np.array([
        np.sin(yaw),
        0,
        np.cos(yaw)
    ])
def right(yaw):
    return np.array([
        np.cos(yaw),
        0,
        -np.sin(yaw)
    ])

# skeleton of a block
blockskl = np.array([
    [0, 0, 0],
    [1, 0, 0],
    [1, 0, 1],
    [0, 0, 1],
    [0, 1, 0],
    [1, 1, 0],
    [1, 1, 1],
    [0, 1, 1]
])

# edges of a block
blockedges = [
    [0, 1],
    [1, 2],
    [2, 3],
    [3, 0],
    [4, 5],
    [5, 6],
    [6, 7],
    [7, 4],
    [0, 4],
    [1, 5],
    [2, 6],
    [3, 7]
]

# check for aabb intersection
def aabb_intersect(mina, maxa, minb, maxb):
    return (mina[0] < maxb[0] and maxa[0] > minb[0] and
            mina[1] < maxb[1] and maxa[1] > minb[1] and
            mina[2] < maxb[2] and maxa[2] > minb[2])

# clip a line to screen edge
def clip_line_to_near_plane(pt1, pt2, near=0.1):
    z1, z2 = pt1[2], pt2[2]
    if z1 >= near and z2 >= near:
        return pt1, pt2, True, False
    if z1 < near and z2 < near:
        return pt1, pt2, False, False
    t = (near - z1) / (z2 - z1)
    intersection = pt1 + t * (pt2 - pt1)
    if z1 < near: pt1 = intersection
    else: pt2 = intersection
    return pt1, pt2, True, True

# get the position of the point relative to the camera
def project_to_camera(npos, origin, rot):
    npos = npos - origin
    R = rotYmat(rot[1]) @ rotXmat(rot[0])
    npos = R.T @ npos
    return npos

# use perspective projecttion to project 3D to 2D
def perspective_projection(npos, f):
    if npos[2] <= 0: return None
    nx = (f * npos[0]) / npos[2]
    ny = (f * npos[1]) / npos[2]
    npos = np.array([nx, ny])
    return npos

# get the points position on the screen
def project_to_screen(npos):
    if npos is None: return None
    u = (width / 2) + npos[0]
    v = (height / 2) - npos[1]
    npos = np.array([u, v])
    return npos

# class that deines a point
class Point:
    # initialize the point
    def __init__(self, world, pos):
        self.world = world
        self.pos = np.array(pos, float)
        self.gcp = np.zeros(3)
        self.gppp = np.zeros(2)
        self.gsp = np.zeros(2)
    # get the position of the point relative to the camera
    def get_camera_pos(self):
        npos = project_to_camera(self.pos, self.world.camera.pos, self.world.camera.rot)
        self.gcp = npos
        return npos
    # get the perspective projection position of the point
    def get_persp_proj_pos(self):
        npos = self.get_camera_pos()
        npos = perspective_projection(npos, self.world.camera.f)
        self.gppp = npos
        return npos
    # get the points position on the screen
    def get_screen_pos(self):
        npos = self.get_persp_proj_pos()
        npos = project_to_screen(npos)
        self.gsp = npos
        return npos
    # update gcp, gppp and gsp
    def updateall(self):
        self.get_camera_pos()
        self.get_persp_proj_pos()
        self.get_screen_pos()
    # draw the point onto the screen
    def draw(self):
        return
        if self.screenpos is None: return
        pg.draw.circle(screen, "white", self.screenpos, 3)

# class that defines a block
class Block:
    # initialize the block
    def __init__(self, world, pos):
        self.world = world
        self.pos = np.array(pos, float)
        self.pts = blockskl + self.pos
        self.pts = [Point(self.world, pt) for pt in self.pts]
    # draw the block onto the screen
    def draw(self):
        for pt in self.pts: pt.updateall()
        for i1, i2 in blockedges:
            pt1 = self.pts[i1]
            pt2 = self.pts[i2]
            pt1c, pt2c, valid, needed = clip_line_to_near_plane(pt1.gcp, pt2.gcp, near=0.1)
            if not valid: continue
            if needed:
                pt1_proj = perspective_projection(pt1c, self.world.camera.f)
                pt2_proj = perspective_projection(pt2c, self.world.camera.f)
                pt1_screen = project_to_screen(pt1_proj)
                pt2_screen = project_to_screen(pt2_proj)
                if pt1_screen is None or pt2_screen is None: continue
                pg.draw.line(screen, "gray", pt1_screen, pt2_screen, 2)
            else:
                if pt1.gsp is None or pt2.gsp is None: continue
                pg.draw.line(screen, "gray", pt1.gsp, pt2.gsp, 2)

# class that defines the camera
class Camera:
    # initialize the camera
    def __init__(self, world):
        self.world = world
        self.pos = np.zeros(3)
        self.aabbpos = np.zeros(3)
        self.rot = np.zeros(3)
        self.fov = np.deg2rad(80)
        self.f = height / (2 * np.tan(self.fov / 2))
        self.sensitivity = 0.0015
        self.speed = 5
        self.rdist = 5
        self.lookingatblock = False
        self.canremove = False
        self.canplace = False
        self.size = np.array([0.6, 1.8, 0.6])
    # make sure camera doesnt clip into blocks
    def move_with_collisions(self, vel, dt):
        npos = self.aabbpos.copy()
        upos = npos + vel * dt
        mina = upos
        maxa = upos + self.size
        for i in range(3):
            tpos = npos.copy()
            tpos[i] = upos[i]
            mina = tpos
            maxa = tpos + self.size
            px, py, pz = np.floor(tpos)
            collided = False
            for x in range(int(px - 2), int(px + 2)):
                for y in range(int(py - 2), int(py + 3)):
                    for z in range(int(pz - 2), int(pz + 2)):
                        bpos = (x, y, z)
                        if bpos in self.world.blocks:
                            minb = np.array(bpos)
                            maxb = minb + 1
                            if aabb_intersect(mina, maxa, minb, maxb):
                                collided = True
                                if vel[i] > 0:
                                    tpos[i] = min(tpos[i], minb[i] - self.size[i])
                                elif vel[i] < 0:
                                    tpos[i] = max(tpos[i], maxb[i])
            npos[i] = tpos[i]
        self.aabbpos = npos
        self.pos = self.aabbpos + np.array([0.3, 1.6, 0.3])
    # update the cameras position, check for placing or removing blocks
    def update(self, dt):
        vel = np.zeros(3)
        keys = pg.key.get_pressed()
        if keys[pg.K_w]: vel += forward(self.rot[1])
        if keys[pg.K_s]: vel -= forward(self.rot[1])
        if keys[pg.K_a]: vel -= right(self.rot[1])
        if keys[pg.K_d]: vel += right(self.rot[1])
        if keys[pg.K_SPACE]: vel[1] += 1
        if keys[pg.K_LSHIFT]: vel[1] -= 1
        velmag = np.linalg.norm(vel)
        if velmag != 0: vel /= velmag
        #self.pos += vel * self.speed * dt
        self.move_with_collisions(vel * self.speed, dt)
        mx, my = pg.mouse.get_rel()
        self.rot[1] += mx * self.sensitivity
        self.rot[0] += my * self.sensitivity
        self.rot[0] = np.clip(self.rot[0], -np.pi/2, np.pi/2)
        rc = self.raycast()
        if rc is not None:
            self.lookingatblock = True
        else:
            self.lookingatblock = False
        if rc is not None and self.canremove:
            rb = rc[0]
            rb = tuple(rb)
            if rb in self.world.blocks:
                self.world.remove(rb)
        if rc is not None and self.canplace:
            rb = rc[0]
            rf = rc[1]
            if rf == '+x': rb[0] += 1
            elif rf == '-x': rb[0] -= 1
            elif rf == '+y': rb[1] += 1
            elif rf == '-y': rb[1] -= 1
            elif rf == '+z': rb[2] += 1
            elif rf == '-z': rb[2] -= 1
            rb = tuple(rb)
            mina = self.aabbpos
            maxa = self.aabbpos + self.size
            minb = np.array(rb)
            maxb = minb + 1
            if not aabb_intersect(mina, maxa, minb, maxb) and rb not in self.world.blocks:
                self.world.add(rb)
        self.canremove = False
        self.canplace = False
    # raycast to check which block is infront of the camera
    def raycast(self):
        dx = np.cos(self.rot[0]) * np.sin(self.rot[1])
        dy = -np.sin(self.rot[0])
        dz = np.cos(self.rot[0]) * np.cos(self.rot[1])
        d = np.array([dx, dy, dz])
        d /= np.linalg.norm(d)
        bx, by, bz = np.floor(self.pos[0]), np.floor(self.pos[1]), np.floor(self.pos[2])
        sx = 1 if dx > 0 else -1
        sy = 1 if dy > 0 else -1
        sz = 1 if dz > 0 else -1
        if dx != 0:
            nx = (bx + 1) if dx > 0 else bx
            tmx = (nx - self.pos[0]) / dx
            tdx = 1 / abs(dx)
        else:
            tmx = float('inf')
            tdx = float('inf')
        if dy != 0:
            ny = (by + 1) if dy > 0 else by
            tmy = (ny - self.pos[1]) / dy
            tdy = 1 / abs(dy)
        else:
            tmy = float('inf')
            tdy = float('inf')
        if dz != 0:
            nz = (bz + 1) if dz > 0 else bz
            tmz = (nz - self.pos[2]) / dz
            tdz = 1 / abs(dz)
        else:
            tmz = float('inf')
            tdz = float('inf')
        t = 0
        mintm = 0
        face = ''
        while t <= self.rdist:
            mintm = min(tmx, tmy, tmz)
            if mintm == tmx:
                bx += sx
                t = tmx
                tmx += tdx
                face = ('+x' if sx < 0 else '-x')
            elif mintm == tmy:
                by += sy
                t = tmy
                tmy += tdy
                face = ('+y' if sy < 0 else '-y')
            elif mintm == tmz:
                bz += sz
                t = tmz
                tmz += tdz
                face = ('+z' if sz < 0 else '-z')
            if (bx, by, bz) in self.world.blocks:
                return [[bx, by, bz], face]
        return None
    # draw the camera crosshair onto the screen
    def draw(self):
        if self.lookingatblock: pg.draw.circle(screen, "green", (width / 2, height / 2), 3)
        else: pg.draw.circle(screen, "red", (width / 2, height / 2), 3)

# class that defines the world
class World:
    # initialize the world
    def __init__(self):
        self.camera = Camera(self)
        self.blocks = {}
    # add a block to the world
    def add(self, pos):
        pos = tuple(pos)
        self.blocks[pos] = Block(self, pos)
    # remove a block from the world
    def remove(self, pos):
        if pos in self.blocks: del self.blocks[pos]
    # draw the world onto the screen
    def draw(self):
        for block in self.blocks.values(): block.draw()
        self.camera.draw()

# time before frame end
lt = time.time()
# exit trigger
xt = False

# create the world
world = World()

# add blocks
world.add([1, 0, 5])
world.add([1, 1, 5])
world.add([3, 0, 2])
world.add([2, 0, 2])
world.add([1, 0, 2])

# main loop
while True:
    # calculate delta time
    dt = time.time() - lt
    # update time before frame end
    lt = time.time()
    # check for events
    for event in pg.event.get():
        if event.type == pg.QUIT: xt = True
        elif event.type == pg.MOUSEBUTTONDOWN:
            if event.button == 1:
                world.camera.canremove = True
            if event.button == 3:
                world.camera.canplace = True
    # exit if triggered
    if xt: break
    # update camera
    world.camera.update(dt)
    # refresh the screen
    screen.fill((0, 0, 0))
    # draw the world
    world.draw()
    # update the screen
    pg.display.flip()
    # set fps
    clock.tick(60)

# exit at the end
pg.quit()