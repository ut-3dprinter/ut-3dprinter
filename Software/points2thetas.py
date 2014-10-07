#-*- coding: utf-8 -*-
# points2angle.py
import numpy
from numpy import cos, sin, pi


def calc_theta(point, l, r):
    """Calculate theta using x, y, z, l and r
    seen from (r[0], 0)
    """
    x, y, z = point

    num = 2. * (x + r[1] - r[0]) * l[0]

    D = 1. * (2 * (x + r[1] - r[0]) * l[0])**2 + (2 * z * l[0])**2 \
        - ((x + r[1] - r[0])**2 + y**2 + z**2 + l[0]**2 - l[1]**2)**2 
    if D < 0:
        return None

    num += numpy.sqrt(D)

    denum = 1. * (x + r[1] - r[0])**2 + y**2 + \
            z**2 + l[0]**2 - l[1]**2 -2*z*l[0]
    if denum == 0:
        return None

    res = num / denum
    theta = 2. * numpy.arctan(res) # [rad]

    return theta


def points2thetas(points, l, r):
    rotation = numpy.array([[cos(2*pi/3), -sin(2*pi/3)],
                            [sin(2*pi/3), cos(2*pi/3)]])

    thetas = numpy.zeros((len(points), 3)
    for i, point1 in enumerate(points):
        point2 = rotation.dot(point1) # rotate 2*pi/3
        point3 = rotation.dot(point2) # rotate 4*pi/3
        theta1 = calc_theta(point1, l, r)
        theta2 = calc_theta(point2, l, r)
        theta3 = calc_theta(point3, l, r)
        thetas[i] = numpy.array([theta1, theta2, theta3])
    return thetas
