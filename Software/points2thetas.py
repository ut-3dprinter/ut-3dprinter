#-*- coding: utf-8 -*-
# points2angle.py
import numpy

def calc_theta(x, y, z, l, r):
    """Calculate theta using x, y, z, l and r"""
    num = 2 * (x + r[1] - r[0]) * l[0]

    D = (2 * (x + r[1] - r[0]) * l[0])**2 + (2 * z * l[0])**2 \
        - ((x + r[1] - r[0])**2 + y**2 + z**2 + l[0]**2 - l[1]**2)**2 
    if D < 0:
        return None

    num += numpy.sqrt(D)

    denum = (x + r[1] - r[0])**2 + y**2 + z**2 + l[0]**2 - l[1]**2 -2*z*l[0]
    if denum == 0:
        return None

    res = num / denum
    theta = 2. * numpy.arctan(res) # [rad]

    return theta


def points2thetas(points, l, r):
    thetas = numpy.array([calc_theta(point[0], point[1], point[2], l, r) for point in points])
    return thetas
