#-*- coding: utf-8 -*-
# gcode2thetas.py
import pyutils

import gcode2points
import points2thetas

import sys
import argparse


def gcode2thetas(gcode_file, l, r):
    print '... extracting points data from Gcode'
    points = gcode2points.gcode2points(gcode_file=gcode_file)

    print '... computing thetas of arms from data points'
    thetas = points2thetas.points2thetas(points=points, l=l, r=r)

    print '... saving thetas to csv file'
    filename = pyutils.file.get_filename_frompath(gcode_file)
    filename = pyutils.file.change_filename(filename, extension='.csv')
    filename = './output/thetas_' + filename
    pyutils.data.save_tocsv(filename, thetas,
                            header=['theta1', 'theta2', 'theta3'])


def main(gcode_file, l1, l2, r1, r2):
    l = (l1, l2)
    r = (r1, r2)

    gcode2thetas(gcode_file=gcode_file, l=l, r=r)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('gcode_file', type=str, help='gcode file path')
    parser.add_argument('l1', type=int, help='l1')
    parser.add_argument('l2', type=int, help='l2')
    parser.add_argument('r1', type=int, help='r1')
    parser.add_argument('r2', type=int, help='r2')
    args = parser.parse_args(sys.argv[1:])

    main(**vars(args))
