#-*- coding: utf-8 -*-
# points2angle.py
import numpy as np
from numpy import cos, sin, pi, sqrt
import ctypes


def calc_theta(point, l, r):
    """Calculate theta using x, y, z, l and r
    seen from (r[0], 0)
    """
    x, y, z = point
    x -= 15. / sqrt(2)
    y -= 15. / sqrt(2)

    num = 2. * (x + r[1] - r[0]) * l[0]

    D = 1. * (2 * (x + r[1] - r[0]) * l[0])**2 + (2 * z * l[0])**2 \
        - ((x + r[1] - r[0])**2 + y**2 + z**2 + l[0]**2 - l[1]**2)**2 
    if D < 0:
        print (x, y, z)
        return None

    num += np.sqrt(D)

    denum = 1. * (x + r[1] - r[0])**2 + y**2 + \
            z**2 + l[0]**2 - l[1]**2 -2*z*l[0]
    if denum == 0:
        print (x, y, z)
        return None

    res = num / denum
    theta = 2. * np.arctan(res) # [rad]

    step = 25600. * theta / (2. * pi)

    return step


def points2thetas(points, l, r):
    rotation = np.array([[cos(2*pi/3), -sin(2*pi/3), 0],
                         [sin(2*pi/3),  cos(2*pi/3), 0],
                         [          0,            0, 1],])

    # thetas = np.zeros((len(points), 3))
    thetas = []
    for i, point1 in enumerate(points):
        point1 = np.array(point1)
        point2 = rotation.dot(point1.T).T # rotate 2*pi/3
        point3 = rotation.dot(point2.T).T # rotate 4*pi/3
        theta1 = calc_theta(point1, l, r)
        theta2 = calc_theta(point2, l, r)
        theta3 = calc_theta(point3, l, r)

        if None in [theta1, theta2, theta3]:
            continue
        # print [theta1, theta2, theta3]
        thetas.append([theta1, theta2, theta3])
        # thetas[i] = np.array([theta1, theta2, theta3])
    thetas = np.array(thetas)
    return thetas
