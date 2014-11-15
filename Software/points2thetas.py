#-*- coding: utf-8 -*-
# points2angle.py
import numpy as np
from numpy import cos, sin, pi, sqrt
import ctypes

thresh = -0.1997885430712449

def calc_theta(point, l, r):
    """Calculate theta using x, y, z, l and r
    seen from (r[0], 0)
    """
    x, y, z = point
    x_ = x + r[1] * cos(pi/6) - r[0]

    num = 2. * x_ * l[0]

    D = 1. * (2 * x_ * l[0])**2 + (2 * z * l[0])**2 \
        - (x_**2 + y**2 + z**2 + l[0]**2 - l[1]**2)**2
    if D < 0:
        print (x, y, z)
        return None

    num1 = num + np.sqrt(D)
    num2 = num - np.sqrt(D)

    denum = 1. * x_**2 + y**2 + \
            z**2 + l[0]**2 - l[1]**2 -2*z*l[0]
    if denum == 0:
        print (x, y, z)
        return None

    res1 = num1 / denum
    res2 = num2 / denum

    theta1 = 2. * np.arctan(res1) # [rad]
    theta2 = 2. * np.arctan(res2) # [rad]

    # print theta1 / pi * 360
    # print theta2 / pi * 360

    theta = max(theta1, theta2)
    if theta < thresh:
        return None

    theta += -thresh
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
        theta = [theta1, theta2, theta3]

        if None in theta:
            continue
        # print [theta1, theta2, theta3]
        thetas.append(theta):
        # thetas[i] = np.array([theta1, theta2, theta3])
    thetas = np.array(thetas)
    return thetas
