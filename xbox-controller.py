import pygame
import serial
from time import sleep
from pygame.constants import JOYBUTTONDOWN, JOYAXISMOTION

motion = [0, 0]
joysticks = []

def connectController(arduino):
    while len(joysticks) == 0:
        print("Searching for controller...")
        cmd = "1: BEEP BEEP BEEP BOOP\n"
        pygame.quit()
        pygame.init()
        for i in range(0, pygame.joystick.get_count()):
            cmd = None
            joysticks.append(pygame.joystick.Joystick(i))
            joysticks[-1].init()
        sendCommand(cmd, arduino)
        sleep(2)

    print("Controller found: ", pygame.joystick.Joystick(0).get_name())

def sendCommand(cmd, arduino):
    if cmd is not None:
        print(cmd)
        arduino.write(cmd.encode())

if __name__ == '__main__':
    with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
        sleep(0.5)
        if arduino.isOpen:
            print("{} connected".format(arduino.port))
            try:
                connectController(arduino)
                while True:
                    for event in pygame.event.get():
                        cmd = None
                        if event.type == JOYBUTTONDOWN:
                            print(event.button)
                            if event.button == 0:
                                cmd = "1: BEEP BEEP BEEP BOOP\n"
                            if event.button == 4:
                                cmd = "2: Use Force\n"
                            if event.button == 3:
                                cmd = "3: Look left\n"
                            if event.button == 1:
                                cmd = "4: Look right\n"

                        if event.type == JOYAXISMOTION:
                            if event.axis < 2: # Left stick
                                if abs(event.value) > 0.3:
                                    motion[event.axis] = -1 * event.value
                                    cmd = "5: {}\n".format(motion)
                                else:
                                    if motion[event.axis] == 0:
                                        cmd = None
                                    else:
                                        motion[event.axis] = 0
                                        cmd = "5: {}\n".format(motion)
                        sendCommand(cmd, arduino)
            except KeyboardInterrupt:
                print("Program closing...")