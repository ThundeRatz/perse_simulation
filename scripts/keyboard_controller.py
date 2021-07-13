#!/usr/bin/env python3
# coding=utf-8
"""
    File:
        keyboard_node.py

    Description:
        Simple python routine to watch the keyboard or a joystick
    to send velocity commands to a Gazebo simulation
        Adapted from https://github.com/ThundeRatz/travesim/blob/main/scripts/keyboard_node.py
"""

import sys
import pygame

import rospy
from geometry_msgs.msg import Twist

# Keep Track of these keyboard Keys (W A S D)
KEYS = [pygame.K_w, pygame.K_a, pygame.K_s, pygame.K_d]

# Joystick X and Y axis indexes
X_AXIS = 0
Y_AXIS = 4
INVERT_X_AXIS = False
INVERT_Y_AXIS = True

# Namespace we are going to publish, used in the simulation
DEFAULT_NAMESPACE = "/perse"

DEFAULT_DEBUG = False

# Robot max speed is 2 m/s
# Keep it lower so he doesnt capote
MAX_LIN_VEL = 2.0  # m/s
MAX_ROT_VEL = 3 # rad/s

# Defines the acceleration's ramp when using keyboard
KEYBOARD_LINEAR_STEP = 0.03
KEYBOARD_LINEAR_MAX = 1.0

KEYBOARD_ANGULAR_STEP = 0.35
KEYBOARD_ANGULAR_MAX = 0.6

# Commands go from -126 to 126 in a way that bytes 0xFE e 0xFF aren't used
SCALE = 126

def draw_console(win, font, console):
    """
    Fills window console with the sentences stored in the list console
        :param win: pygame.display Window object to be filled
        :param font: pygame.Font Font style to be used
        :param console: list<font.render> List of text to write
    """
    img = font.render("Event console Area", 1, (155, 155, 155), (0, 0, 0))
    win.blit(img, (2, 132))
    ypos = 450
    text = list(console)
    text.reverse()
    for line in text:
        r = win.blit(line, (10, ypos))
        win.fill(0, (r.right, r.top, 620, r.height))
        ypos -= font.get_height()


def get_joysticks():
    """
    Check if there are any joysticks connected
    """
    using_joystick = False
    for x in range(pygame.joystick.get_count()):
        j = pygame.joystick.Joystick(x)
        j.init()
    if pygame.joystick.get_count():
        using_joystick = True
    return using_joystick

def joystick_movement(axis):
    """
    Controls speed when moving with a joystick
        :param axis: list List contains the linear and angular axis
    """
    vel_cmd_twist = Twist()
    vel_cmd_twist.linear.x = axis[1]*MAX_LIN_VEL
    vel_cmd_twist.angular.z = axis[0]*MAX_ROT_VEL

    return vel_cmd_twist

def keyboard_movement(velocities,state):
    """
    Controls speed when moving with a keyboard
        :param axis: list List contains the linear and angular velocities
        :param state: list List the state of the keys
    """
    vel_lin = velocities[0]
    vel_ang = velocities[1]
    if state[pygame.K_w] and not state[pygame.K_s]:
        vel_lin += KEYBOARD_LINEAR_STEP
        vel_lin = max(vel_lin, KEYBOARD_LINEAR_MAX)
    elif state[pygame.K_s] and not state[pygame.K_w]:
        vel_lin -= KEYBOARD_LINEAR_STEP
        vel_lin = min(vel_lin, -KEYBOARD_LINEAR_MAX)
    else:
        vel_lin = 0.0

    if state[pygame.K_a] and not state[pygame.K_d]:
        vel_ang += KEYBOARD_ANGULAR_STEP
        vel_ang = max(vel_ang, KEYBOARD_ANGULAR_MAX)
    elif state[pygame.K_d] and not state[pygame.K_a]:
        vel_ang -= KEYBOARD_ANGULAR_STEP
        vel_ang = min(vel_ang, -KEYBOARD_ANGULAR_MAX)
    else:
        vel_ang = 0.0

    vel_cmd_twist = Twist()
    vel_cmd_twist.linear.x = vel_lin * MAX_LIN_VEL
    vel_cmd_twist.angular.z = vel_ang * MAX_ROT_VEL

    return vel_cmd_twist

def main(debug=DEFAULT_DEBUG):

    vel_pub = []
    rate = None

    # Start ROS configs
    rospy.init_node('perse_human_controller')
    vel_pub = rospy.Publisher(
       'perse/robot_drive_controller/cmd_vel',
        Twist, queue_size=2)

    rate = rospy.Rate(60)  # 60hz

    pygame.init()

    # Create the window
    win = pygame.display.set_mode((640, 480), pygame.RESIZABLE)
    pygame.display.set_caption("Keyboard Comunication Interface")

    # List of sentences to be displayed
    console = []
    font = pygame.font.Font(None, 26)

    # Dictionarys to hold the current state of some keys
    state = {}
    for key in KEYS:
        state[key] = False

    axis = [0.0, 0.0]
    # Detect Joysticks connected to the computer
    using_joystick = get_joysticks()

    velocities = [0.0,0.0]

    running = True
    while running:
        if using_joystick:
            for e in pygame.event.get():
                # If a joystick button is pressed
                if e.type == pygame.JOYAXISMOTION:
                    if e.dict['axis'] in (X_AXIS, Y_AXIS):
                        if e.dict['axis'] == X_AXIS:
                            if INVERT_X_AXIS:
                                axis[0] = -e.value
                            else:
                                axis[0] = e.value

                        elif e.dict['axis'] == Y_AXIS:
                            if INVERT_Y_AXIS:
                                axis[1] = -e.value
                            else:
                                axis[1] = e.value
                # L1 pressed
                if e.type == pygame.VIDEORESIZE:
                    win = pygame.display.set_mode(e.size, pygame.RESIZABLE)

                elif e.type == pygame.QUIT:
                    running = False

            vel_cmd_twist = joystick_movement(axis)
        else:
            for e in pygame.event.get():
                # Keyboard key movement
                if e.type == pygame.KEYDOWN:
                    if e.key == pygame.K_ESCAPE:
                        running = False
                    if e.key in KEYS:
                        state[e.key] = True
                elif e.type == pygame.KEYUP:
                    if e.key in KEYS:
                        state[e.key] = False
            vel_cmd_twist = keyboard_movement(velocities,state)

        draw_console(win, font, console)
        pygame.display.flip()

        vel_pub.publish(vel_cmd_twist)

        txt = f"Linear: {int(vel_cmd_twist.linear.x * SCALE)} Angular: {int(vel_cmd_twist.angular.z * SCALE)}"
        img = font.render(txt, 1, (50, 200, 50), (0, 0, 0))
        console.append(img)
        console = console[-13:]

        if debug:
            print(txt)

        rate.sleep()

if __name__ == "__main__":
    rospy.loginfo("Starting manual controller!")

    # Clean ROS parameters from command line
    myargv = rospy.myargv(argv=sys.argv)

    print(myargv)
    rospy.loginfo(myargv)

    main()
