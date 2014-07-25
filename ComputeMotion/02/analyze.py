#!/usr/local/bin/python
#-*- coding: utf-8 -*-
# Filename: analyze.py
import csv
from math import *
import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
def calc_theta(x, y, z, l, r):
    num = 2 * (x + r[1] - r[0]) * l[0]

    D = (2 * (x + r[1] - r[0]) * l[0])**2 + (2 * z * l[0])**2 \
        - ((x + r[1] - r[0])**2 + y**2 + z**2 + l[0]**2 - l[1]**2)**2 
    if D < 0:
        return None

    num += sqrt(D)

    denum = (x + r[1] - r[0])**2 + y**2 + z**2 + l[0]**2 - l[1]**2 -2*z*l[0]
    if denum == 0:
        return None

    res = num / denum
    theta = 2 * atan(res) / pi * 180

    return theta

def save_points_data(Xs, thetas):
    f = open('points_and_theta.csv', 'wb')
    writer = csv.writer(f)
    writer.writerow(['x','y','z','theta1','theta2','theta3'])
    for (x, y, z), (th1, th2, th3) in zip(Xs, thetas):
        writer.writerow([x, y, z, th1, th2, th3])
    f.close()

def plot_thetas(thetas):
    theta1, theta2, theta3 = thetas[:, 0], thetas[:, 1], thetas[:, 2]
    N = len(theta1)
    x = [i for i in range(N)]
    plt.figure()
    plt.plot(x, theta1, color='r', alpha=0.3)
    plt.plot(x, theta2, color='g', alpha=0.3)
    plt.plot(x, theta3, color='b', alpha=0.3)
    plt.show()

def main(sparseness, l1, l2, r1, r2):
    # 定数定義部分
    l = (l1, l2)
    r = (r1, r2) # r1, r2
    rad = 120. / 180. * pi
    A = np.array([[cos(rad), -sin(rad)],
                  [sin(rad), cos(rad)]])
    B = np.array([[cos(-rad), -sin(-rad)],
                  [sin(--rad), cos(rad)]])

    Xs = []
    thetas = []
    rng = {'x': range(-50, 51, sparseness),
           'y': range(-50, 51, sparseness),
           'z': range(-l[0]-l[1], -l[0]+1, sparseness)} # 可動範囲候補

    cube = [[],[],[]]
    for z in rng['z']:
        for y in rng['y']:
            for x in rng['x']:
                cube[0].append(x)
                cube[1].append(y)
                cube[2].append(z)
                theta = calc_theta(x, y, z, l, r)
                x2, y2 = np.dot(A, np.array([x, y]).T)
                theta2 = calc_theta(x2, y2, z, l, r)
                x3, y3 = np.dot(B, np.array([x, y]).T)
                theta3 = calc_theta(x3, y3, z, l, r)
                if None not in (theta, theta2, theta3) \
                        and 0 <= theta <= 120 \
                        and 0 <= theta2 <= 120 \
                        and 0 <= theta3 <= 120:
                    #print x, y, z, theta
                    thetas.append([theta, theta2, theta3])
                    Xs.append([x, y, z])
    save_points_data(Xs, thetas)
    Xs, thetas = np.array(Xs), np.array(thetas)

    X, Y, Z = Xs[:, 0], Xs[:, 1], Xs[:, 2]
    print "Limits: X({0}:{1}), Y({2}:{3}) Z:({4}:{5})" \
        .format(X.min(), X.max(), Y.min(), Y.max(), Z.min(), Z.max())

    #plot_thetas(thetas)

    theta_deltas_min = []
    theta_deltas_max = []
    theta_deltas_self = []
    for i in range(len(thetas)):
        current = thetas[i]
        prev = thetas[i-1]
        if i == 0 or None in list(current) or None in list(prev):
            theta_deltas_min.append(1000)
            theta_deltas_max.append(1000)
            theta_deltas_self.append(1000)
            continue
        delta = [abs(current[0]-prev[0]), abs(current[1]-prev[1]), abs(current[2]-prev[2])]
        theta_deltas_min.append(min(delta))
        theta_deltas_max.append(max(delta))
        theta_deltas_self.append(delta[0])
    print "Min Self theta change: {0}".format(min(theta_deltas_self))
    print "Min MinOf3 theta change: {0}".format(min(theta_deltas_min))
    print "Min MaxOf3 theta change: {0}".format(min(theta_deltas_max))

    theta_deltas_order = np.log10(theta_deltas_min)
    theta_deltas_floor = np.floor(theta_deltas_order)

    print "Order", [-3.0, -2.0, -1.0, 0.0, 1.0]
    print "Color", ['b', 'g', 'r', 'c', 'm']

    fig = plt.figure()
    ax = Axes3D(fig)
    ax.scatter(X[theta_deltas_floor==-3.0], Y[theta_deltas_floor==-3.0], Z[theta_deltas_floor==-3.0], c='b', marker='^', alpha=0.5)
    ax.scatter(X[theta_deltas_floor==-2.0], Y[theta_deltas_floor==-2.0], Z[theta_deltas_floor==-2.0], c='g', marker='^', alpha=0.5)
    ax.scatter(X[theta_deltas_floor==-1.0], Y[theta_deltas_floor==-1.0], Z[theta_deltas_floor==-1.0], c='r', marker='^', alpha=0.5)
    ax.scatter(X[theta_deltas_floor==0.0], Y[theta_deltas_floor==0.0], Z[theta_deltas_floor==0.0], c='c', marker='^', alpha=0.5)
    ax.scatter(X[theta_deltas_floor==1.0], Y[theta_deltas_floor==1.0], Z[theta_deltas_floor==1.0], c='m', marker='^', alpha=0.5)

    cube = np.array(cube)
    ax.scatter(cube[0], cube[1], cube[2], c='w', marker='o', alpha=0.05)

    plt.show()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyze about motion of parallel link')
    parser.add_argument('--sparseness', '-s', dest='sparseness', type=int, default=1,
            help='The sparseness when analyzing points and theta. Default is 1 and this means x moving is per 1mm.')
    parser.add_argument('l1', metavar='l1', type=int, help='l1')
    parser.add_argument('l2', metavar='l2', type=int, help='l2')
    parser.add_argument('r1', metavar='r1', type=int, help='r1')
    parser.add_argument('r2', metavar='r2', type=int, help='r2')
    args = parser.parse_args(sys.argv[1:])

    main(**vars(args))
