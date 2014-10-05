#-*- coding: utf-8 -*-
# utils.py
import cv2
import time
import numpy
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as p3
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D


def plot_3D(X, Y, Z):
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.scatter(X, Y, Z, c='b', marker='^', alpha=0.5)
    plt.savefig('3dprinted.png')

def update_lines(num, dataLines, lines) :
    for line, data in zip(lines, dataLines) :
        line.set_data(data[0:2, :num])
        line.set_3d_properties(data[2,:num])
    return lines

def plot_3D_video(X, Y, Z):
    fig = plt.figure()
    ax = p3.Axes3D(fig)

    data = [numpy.vstack((X, Y, Z))]

    lines = [ax.plot(dat[0, 0:1], dat[1, 0:1], dat[2, 0:1])[0] for dat in data]

    # Setting the axes properties
    ax.set_xlim3d([X.min(), X.max()])
    ax.set_xlabel('X')
    ax.set_ylim3d([Y.min(), Y.max()])
    ax.set_ylabel('Y')
    ax.set_zlim3d([Z.min(), Z.max()])
    ax.set_zlabel('Z')
    ax.set_title('3D Printing')

    # Creating the Animation object
    anim = animation.FuncAnimation(fig, update_lines, len(X),
                                       fargs=(data, lines),
                                       interval=1, blit=False)

    mywriter = animation.FFMpegWriter()
    anim.save('3dprinting.mp4',writer=mywriter)

    # plt.show()
