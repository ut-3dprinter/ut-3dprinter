#-*- coding: utf-8 -*-
# utils.py
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


def plot_3D(X, Y, Z):
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.scatter(X, Y, Z, c='b', marker='^', alpha=0.5)
    plt.show()
