#-*- coding: utf-8 -*-
# gcode2angle.py
#
# Reference : http://reprap.org/wiki/G-code


with open('./sample_stldata/DD_carriage_for_atom.gcode', 'r') as f:
    lines = f.readlines()

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
    elif not code.startswith('G'):
        raise ValueError('Cannot understand the G code', code)

    if code == 'G21':
        continue # G21 - Programming in [mm]
    elif code == 'G90':
        continue # G90 - Absolute programming
    elif code != 'G1':
        # G1  - Linear interpolation
        raise ValueError('Cannot understand the G code', code)

    point = {}
    for arg in arguments:
        for letter in 'XYZEF':
            if arg.startswith(letter):
                point[letter] = float(arg[1:])
    print point
