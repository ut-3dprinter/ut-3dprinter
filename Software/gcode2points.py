#-*- coding: utf-8 -*-
# gcode2points.py
#
# Reference : http://reprap.org/wiki/G-code
import pyutils

import sys
import argparse
import numpy


def reverse_bylayers(points):
    Z_reverse = []
    Z = points[:, 2]
    for z in sorted(set(Z), reverse=True):
        Z_reverse += list(points[Z == z])
    return numpy.array(Z_reverse)


def get_points_from_gcode_file(gcode_file):
    """Get XYZ moving points from gcode file"""
    with open(gcode_file, 'r') as f:
        lines = f.readlines()

    # read gcode file
    points = []
    for line in lines:
        line = line.strip()

        # first validation
        if line == '':
            continue # skip blank lines
        elif line.startswith(';'):
            continue # skip comment line

        # extract code and arguments
        code, arguments = line.partition(' ')[::2]
        arguments = arguments.split(' ')

        # code validation
        if code.startswith('M'):
            continue
        elif code.startswith('T'):
            continue
        elif code == 'G21':
            continue # G21 - Programming in [mm]
        elif code == 'G90':
            continue # G90 - Absolute programming
        elif code != 'G1':
            # G1  - Linear interpolation
            raise ValueError('Cannot understand the G code', code)

        # get points used for linear interpolation
        point = {}
        for arg in arguments:
            if arg.startswith('X'):
                point['X'] = float(arg[1:])
            elif arg.startswith('Y'):
                point['Y'] = float(arg[1:])
            elif arg.startswith('Z'):
                point['Z'] = float(arg[1:])
        if point != {}:
            points.append(point)

    ret = []
    x, y, z = 0, 0, 0 # initialize
    for point in points:
        x = point['X'] if 'X' in point else x
        y = point['Y'] if 'Y' in point else y
        z = point['Z'] if 'Z' in point else z
        ret.append([x, y, z])

    ret = numpy.array(ret)
    # ret = reverse_bylayers(ret)
    return ret


def get_interpolated_points(points):
    """Zはoffset引くことによって座標の原点を下に持ってくる"""
    offsets = [100, 100, 220]
    # initialize
    start = points[0]

    # linear interpolation from Point0 to Point1
    interpolated = []
    for point in points[2:]:
        # print 'start:', start, 'end:', end
        current = start
        end = point

        # move from start to end
        while True:
            # print 'current:', current
            interpolated.append([current[0]-offsets[0], current[1]-offsets[1], current[2]-offsets[2]])

            # decide how to move from current position
            for i in range(0, 3): # i means x,y,z
                if end[i] - current[i] > 1:
                    current[i] += 1
                elif end[i] - current[i] < -1:
                    current[i] -= 1
                else:
                    current[i] = end[i]

            if (current == end).all():
                break

        # update start and end points
        start = end

    return numpy.array(interpolated)


def gcode2points(gcode_file):
    points = get_points_from_gcode_file(gcode_file)

    points = get_interpolated_points(points)

    return points


def test_gcode2points(gcode_file, n_frame):
    print '==> params:', gcode_file, n_frame
    fname = pyutils.get_filename_frompath(gcode_file)

    print '... getting points'
    Xs = gcode2points(gcode_file=gcode_file)
    X, Y, Z = Xs[:, 0], Xs[:, 1], Xs[:, 2]
    savefig = pyutils.change_filename(fname, '.png')
    pyutils.plot_3D(X, Y, Z, savefig=savefig, show=False)
    for z in set(Z):
        Xz, Yz, Zz = X[Z == z], Y[Z == z], Z[Z == z]
        savefig = pyutils.change_filename(fname, '{}.png'.format(z))
        pyutils.plot_2D(Xz, Yz, xlim=[X.min(), X.max()],
                ylim=[Y.min(), Y.max()], savefig=savefig, show=False)

    Xs = get_points_from_gcode_file(gcode_file=gcode_file)
    X, Y, Z = Xs[:, 0], Xs[:, 1], Xs[:, 2]-250
    print("Limits: X({0}:{1}), Y({2}:{3}) Z:({4}:{5})"
          .format(X.min(), X.max(), Y.min(), Y.max(), Z.min(), Z.max()))

    print '... making plotting animation'
    for z in set(Z):
        print '... z == {}'.format(z)
        Xz, Yz, Zz = X[Z == z], Y[Z == z], Z[Z == z]
        saveanime = pyutils.change_filename(fname, '{}.mp4'.format(z))
        pyutils.plot_3D_animation(X=Xz, Y=Yz, Z=Zz, step=None, n_frame=n_frame,
                                  view_init=(90, 0),
                                  xlim=[X.min(), X.max()],
                                  ylim=[Y.min(), Y.max()],
                                  zlim=[Z.min(), Z.max()],
                                  saveanime=saveanime, show=False)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('gcode_file', type=str, help='gcode file path')
    parser.add_argument('--nframe', dest='n_frame', type=int)
    args = parser.parse_args(sys.argv[1:])

    test_gcode2points(**vars(args))
