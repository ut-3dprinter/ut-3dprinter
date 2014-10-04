#-*- coding: utf-8 -*-
# gcode2angle.py
#
# Reference : http://reprap.org/wiki/G-code


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
            for cd in 'XYZ':
                if arg.startswith(cd):
                    point[cd] = float(arg[1:])
        if point != {}:
            points.append(point)

    return points


def get_interpolated_points(points):
    # initialize
    start = [0, 0, 0]
    start[0] = points[0]['X']
    start[1] = points[0]['Y']
    start[2] = points[0]['Z']
    end = [0, 0, 0]
    end[0] = points[1]['X'] if 'X' in points[1] else start[0]
    end[1] = points[1]['Y'] if 'Y' in points[1] else start[1]
    end[2] = points[1]['Z'] if 'Z' in points[1] else start[2]

    # linear interpolation from Point0 to Point1
    interpolated = []
    for point in enumerate(points[2:]):
        # print 'start:', start, 'end:', end

        current = start

        # move from start to end
        while True:
            # print 'current:', current
            interpolated.append(current)

            # decide how to move from current position
            for i in range(0, 3):
                if end[i] - current[i] > 1:
                    current[i] += 1
                elif end[i] - current[i] < -1:
                    current[i] -= 1
                else:
                    current[i] = end[i]

            if current == end:
                break

        # update start and end points
        start[0] = end[0] if 'X' in point else start[0]
        start[1] = end[1] if 'Y' in point else start[1]
        start[2] = end[2] if 'Z' in point else start[2]
        end[0] = point['X'] if 'X' in point else end[0]
        end[1] = point['Y'] if 'Y' in point else end[1]
        end[2] = point['Z'] if 'Z' in point else end[2]

    return interpolated


def main():
    gcode_file = './sample_stldata/DD_carriage_for_atom.gcode'

    points = get_points_from_gcode_file(gcode_file)

    interpolated = get_interpolated_points(points)

    print interpolated


if __name__ == '__main__':
    main()
