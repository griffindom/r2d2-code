import pygame
import serial
from time import sleep
from pygame.constants import JOYBUTTONDOWN, JOYAXISMOTION

motion = [0, 0]
joysticks = []

# pygame.init()
# while len(joysticks) == 0:
#     for i in range(0, pygame.joystick.get_count()):
#         joysticks.append(pygame.joystick.Joystick(i))
#         joysticks[-1].init()
#     print("Searching for controller...")
#     sleep(2)
#     pygame.init()

# # Print out the name of the controller
# print("Controller found: ", pygame.joystick.Joystick(0).get_name())

# # Xbox Joystick Axis:
# # Axis 0 up down, down value is -1, up value is 1
# # Axis 1 Left, Right, Left value is: -1, right value is 1
# # center is always 0

# # Main Loop
# while True or KeyboardInterrupt:

#     # Check for joystick events
#     for event in pygame.event.get():
#         if event.type == JOYBUTTONDOWN:
#             if event.button == 1:
#                 print("BEEP BEEP BEEP BOOP")

#         if event.type == JOYAXISMOTION:
#             if event.axis < 2: # Left stick
#                 if abs(event.value) > 0.3:
#                     motion[event.axis] = -1 * event.value
#                 else:
#                     motion[event.axis] = 0

#     print(motion)
#     sleep(0.1)

if __name__ == '__main__':
    pygame.init()
    while len(joysticks) == 0:
        for i in range(0, pygame.joystick.get_count()):
            joysticks.append(pygame.joystick.Joystick(i))
            joysticks[-1].init()
        print("Searching for controller...")
        sleep(2)
        pygame.init()

    print("Controller found: ", pygame.joystick.Joystick(0).get_name())

    with serial.Serial("/dev/cu.usbmodem142201", 9600, timeout=1) as arduino:
        sleep(0.1)
        if arduino.isOpen:
            print("{} connected".format(arduino.port))
            try:
                while True:
                    for event in pygame.event.get():
                        cmd = None
                        if event.type == JOYBUTTONDOWN:
                            if event.button == 1:
                                cmd = "1: BEEP BEEP BEEP BOOP\n"
                            if event.button == 2:
                                cmd = "2: Use Force\n"
                            if event.button == 13:
                                cmd = "3: Look left\n"
                            if event.button == 14:
                                cmd = "4: Look right\n"

                        if event.type == JOYAXISMOTION:
                            if event.axis < 2: # Left stick
                                if abs(event.value) > 0.1:
                                    # cmd = "{}\n".format(-1 * event.value)
                                    motion[event.axis] = -1 * event.value
                                else:
                                    # cmd = "{}\n".format(-1 *event.value)
                                    motion[event.axis] = 0
                                cmd = "5: {}\n".format(motion)
                        if cmd is not None:
                            print(cmd)
                            arduino.write(cmd.encode())
            except KeyboardInterrupt:
                print("Program closing...")