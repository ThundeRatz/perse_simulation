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

import pygame
import argparse
import sys

import rospy
from geometry_msgs.msg import Twist

# Vamos acompanhar o estado dessas teclas
KEYS = [pygame.K_w, pygame.K_a, pygame.K_s, pygame.K_d]

# Indice dos eixos x e y do joystick
X_AXIS = 0
Y_AXIS = 4
INVERT_X_AXIS = False
INVERT_Y_AXIS = True

# Namespace dos tópicos que iremos publicar
DEFAULT_NAMESPACE = "/thunderwaze"

DEFAULT_DEBUG = False

# A vel máxima do robô é 2 m/s
# Deixando a velocidade máxima abaixo disso para evitar que ele empine
MAX_LIN_VEL = 1  # m/s
MAX_ROT_VEL = 3 # rad/s

# Os comandos vão de -126 até 126 de modo que os bytes 0xFE e 0xFF
# nunca são utilizados
SCALE = 126


def getNamespace():
    return DEFAULT_NAMESPACE


def drawConsole(win, font, console):
    """
    Fills window console with the sentences stored in the list console
        :param win: pygame.display Window object to be filled
        :param font: pygame.Font Font style to be used
        :param console: list<font.render> List of text to write
    """
    img = font.render("Event console Area", 1, (155, 155, 155), (0, 0, 0))
    win.blit(img, (2, 132))
    y_pos = 450
    text = list(console)
    text.reverse()
    for line in text:
        r = win.blit(line, (10, y_pos))
        win.fill(0, (r.right, r.top, 620, r.height))
        y_pos -= font.get_height()


def main(namespace, debug=DEFAULT_DEBUG):

    vel_pub = None
    vel_msg = None
    rate = None

    # Inicia configs do ROS
    rospy.init_node('thunderwaze_human_controller')

    vel_pub = rospy.Publisher(
            getNamespace() + '/robot_drive_controller/cmd_vel',
            Twist, queue_size=2)

    rate = rospy.Rate(60)  # 60hz

    pygame.init()

    # Cria a janela
    win = pygame.display.set_mode((640, 480), pygame.RESIZABLE)
    pygame.display.set_caption("nRFDongle Comunication Interface")

    # Lista de frases a serem mostradas no console
    console = []
    font = pygame.font.Font(None, 26)

    # Dicionário para guardar o estado de algumas teclas
    state = {}
    for key in KEYS:
        state[key] = False

    # Vamos detectar os Joysticks conectados ao computador
    axis = [0.0, 0.0]
    using_joystick = True
    for n in range(pygame.joystick.get_count()):
        joystick = pygame.joystick.Joystick(n)
        joystick.init()
        txt = "Enabled joystick: " + joystick.get_name()
        print(txt)
        img = font.render(txt, 1, (50, 200, 50), (0, 0, 0))
        console.append(img)

    if not pygame.joystick.get_count():
        using_joystick = False
        print("No Joysticks to Initialize")
        img = font.render("No Joysticks to Initialize", 1,
                          (50, 200, 50), (0, 0, 0))
        console.append(img)

    running = True
    while running:

        for event in pygame.event.get():

            # Movimento dos botões do teclado
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                if event.key in KEYS:
                    state[e.key] = True
            elif event.type == pygame.KEYUP:
                if event.key in KEYS:
                    state[e.key] = False

            # Movimento dos direcionais do joystick
            if event.type == pygame.JOYAXISMOTION:
                if event.dict['axis'] in (X_AXIS, Y_AXIS):
                    if event.dict['axis'] == X_AXIS:
                        if INVERT_X_AXIS:
                            axis[0] = -event.value
                        else:
                            axis[0] = event.value

                    elif event.dict['axis'] == Y_AXIS:
                        if INVERT_Y_AXIS:
                            axis[1] = -event.value
                        else:
                            axis[1] = event.value

            # Caso algum botão do joystick seja apertado
            if event.type == pygame.JOYBUTTONDOWN \
                or event.type == pygame.JOYBUTTONUP \
                    or event.type == pygame.JOYHATMOTION:

                txt = "%s: %s" % (pygame.event.event_name(event.type), event.dict)
                print(txt)
                img = font.render(txt, 1, (50, 200, 50), (0, 0, 0))
                console.append(img)
                console = console[-13:]

            # L1 pressionado
            if event.type == pygame.VIDEORESIZE:
                win = pygame.display.set_mode(e.size, pygame.RESIZABLE)

            elif event.type == pygame.QUIT:
                running = False

        drawConsole(win, font, console)
        pygame.display.flip()

        if using_joystick:
            txt = "X: {} Y: {}".format(int(axis[0]*SCALE), int(axis[1]*SCALE))
            if debug:
                print(txt)
                img = font.render(txt, 1, (50, 200, 50), (0, 0, 0))
                console.append(img)
                console = console[-13:]

            vel_msg = Twist()
            vel_msg.linear.x = axis[1]*MAX_LIN_VEL
            vel_msg.angular.z = axis[0]*MAX_ROT_VEL

            vel_pub.publish(vel_msg)

        else:
            vel_x = 0.0
            if state[pygame.K_w]:
                vel_x += 0.35
            if state[pygame.K_s]:
                vel_x -= 0.35

            vel_y = 0.0
            if state[pygame.K_a]:
                vel_y += 0.35
            if state[pygame.K_d]:
                vel_y -= 0.35

            txt = "X: {} Y: {}".format(int(vel_x*SCALE), int(vel_y*SCALE))
            if debug:
                print(txt)
                img = font.render(txt, 1, (50, 200, 50), (0, 0, 0))
                console.append(img)
                console = console[-13:]

            vel_msg = Twist()
            vel_msg.linear.x = vel_x*MAX_LIN_VEL
            vel_msg.angular.z = vel_y*MAX_ROT_VEL

            vel_pub.publish(vel_msg)

        rate.sleep()


if __name__ == "__main__":
    rospy.loginfo("Começando a brincadeira!")

    # Clean ROS parameters from command line
    myargv = rospy.myargv(argv=sys.argv)
    print(myargv)
    rospy.loginfo(myargv)

    main(DEFAULT_NAMESPACE)
