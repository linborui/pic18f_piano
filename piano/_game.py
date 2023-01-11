import pygame
from piano._io import get_port, get_baudrate, Serial

IO_CURR = 0
IO_KEY = 1
IO_SERIAL = 2

WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600
FULLSCREEN = False

surface = None

piano = {}
piano['keys'] = [pygame.K_a, pygame.K_w, pygame.K_s, pygame.K_e, pygame.K_d,
                 pygame.K_f, pygame.K_u, pygame.K_j, pygame.K_i, pygame.K_k, pygame.K_o, pygame.K_l]
piano['pressed'] = [False] * len(piano['keys'])

keys = {}
keys['white'] = {}
keys['black'] = {}
keys['white']['idx'] = [0, 2, 4, 5, 7, 9, 11]
keys['black']['idx'] = [1, 3, 6, 8, 10]
keys['black']['pos'] = [0.7, 1.7, 3.7, 4.7, 5.7]

key_edge = 2
key_start_height = WINDOW_HEIGHT * 0.2
keys['white']['width'] = WINDOW_WIDTH / len(keys['white']['idx'])
keys['white']['height'] = WINDOW_HEIGHT - key_start_height
keys['black']['width'] = keys['white']['width'] * 0.6
keys['black']['height'] = keys['white']['height'] * 0.6

level_keys = [pygame.K_1, pygame.K_2, pygame.K_3, pygame.K_4, pygame.K_5]
level = 2
block_edge = 10
block_width = WINDOW_WIDTH / 5
block_height = key_start_height

serial = None
EVENT_SERIAL = pygame.USEREVENT + 1


def init(io):
    global serial, surface, IO_CURR
    IO_CURR = io
    if IO_CURR & IO_SERIAL:
        serial = Serial(port=get_port(), baudrate=get_baudrate())

    pygame.init()
    pygame.display.set_caption('Piano')
    surface = pygame.display.set_mode(
        (WINDOW_WIDTH, WINDOW_HEIGHT), pygame.SCALED)


def start():
    while events():
        draw()
    pygame.quit()


def events() -> bool:
    global FULLSCREEN, level
    
    if IO_CURR & IO_SERIAL:
        serial_buf = serial.read()
        if serial_buf:
            pygame.event.post(pygame.event.Event(EVENT_SERIAL, data=serial_buf))

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            return False

        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                return False

            elif event.key == pygame.K_F11:
                if (WINDOW_WIDTH, WINDOW_HEIGHT) in pygame.display.list_modes():
                    FULLSCREEN = not FULLSCREEN
                    flags = pygame.FULLSCREEN if not FULLSCREEN else pygame.SCALED
                    pygame.display.set_mode(
                        (WINDOW_WIDTH, WINDOW_HEIGHT), flags)

            elif IO_CURR & IO_KEY:
                for i in range(len(piano['keys'])):
                    if event.key == piano['keys'][i]:
                        press(i)
                        break
                for i in range(len(level_keys)):
                    if event.key == level_keys[i]:
                        level = i
                        break

        elif event.type == pygame.KEYUP and IO_CURR & IO_KEY:
            for i in range(len(piano['keys'])):
                if event.key == piano['keys'][i]:
                    release(i)
                    break

        elif event.type == EVENT_SERIAL and IO_CURR & IO_SERIAL:
            data = event.data.replace('\r', '')
            if data[0] == 'c':
                level = int(data[1])
            elif data[0] == 'r':
                press_reset()
            else:
                press(int(data) - 1)

    return True


def press(idx: int):
    global piano
    piano['pressed'][idx] = True


def release(idx: int):
    global piano
    piano['pressed'][idx] = False


def press_reset():
    global piano
    piano['pressed'] = [False] * len(piano['keys'])


def draw():
    pygame.draw.rect(surface, (0, 0, 0), (0, 0, WINDOW_WIDTH, WINDOW_HEIGHT))

    for i in range(len(keys['white']['idx'])):
        color = (192, 192, 192) if piano['pressed'][keys['white']['idx'][i]] else (
            255, 255, 255)
        pygame.draw.rect(surface, color, (key_edge + keys['white']['width'] * i,
                         key_start_height, keys['white']['width'] - key_edge * 2, keys['white']['height']))

    for i in range(len(keys['black']['idx'])):
        color = (50, 50, 50) if piano['pressed'][keys['black']['idx'][i]] else (
            0, 0, 0)
        pygame.draw.rect(surface, color, (key_edge / 2 + keys['white']['width'] * keys['black']['pos']
                         [i], key_start_height, keys['black']['width'] - key_edge * 2, keys['black']['height']))

    pygame.draw.rect(surface, (230, 230, 230), (block_edge + block_width * level,
                     block_edge, block_width - block_edge * 2, block_height - block_edge * 2))

    pygame.display.flip()
