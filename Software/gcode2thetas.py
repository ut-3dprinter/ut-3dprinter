#-*- coding: utf-8 -*-
# gcode2thetas.py
import gcode2points
import points2thetas

import sys
import argparse


def gcode2thetas(gcode_file, l, r):
    gcode_file = './sample_stldata/DD_carriage_for_atom.gcode'

    points = gcode2points.gcode2points(gcode_file=gcode_file)

    thetas = points2thetas.points2thetas(points=points, l=l, r=r)

    return thetas


def main(gcode_file, l1, l2, r1, r2):
    l = (l1, l2)
    r = (r1, r2)
    thetas = gcode2thetas(gcode_file=gcode_file, l=l, r=r)
    print thetas


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('gcode_file', type=str, help='gcode file path')
    parser.add_argument('l1', type=int, help='l1')
    parser.add_argument('l2', type=int, help='l2')
    parser.add_argument('r1', type=int, help='r1')
    parser.add_argument('r2', type=int, help='r2')
    args = parser.parse_args(sys.argv[1:])

    main(**vars(args))
