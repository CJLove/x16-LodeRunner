#!/usr/bin/env python3
#
# Parse .txt file and create .bin file suitable for loading
# by x16 application
#

import argparse
import sys

def get_nibble(c):
    nibble = 0
    try:
        nibble = int(c,16)
    except:
        pass
    return nibble

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
    description='Converts .txt file to .bin file')
parser.add_argument('--input', help='.json filename')
parser.add_argument('--output', help='.bin filename')
parser.add_argument('--addr', help='hex load address')
args = parser.parse_args()

addr=0
try:
    addr = int(args.addr,16)
except:
    pass


with open(args.output,"wb") as out:
    # Handle 2-byte header
    header=[addr & 0xff, (addr >> 8) & 0xff]
    out.write(bytearray(header))

    with open(args.input,"r") as file:
        for line in file:
            if line[0] != '#':
                i = 0
                while ord(line[i]) >= 32 and ord(line[i]) != 35:
                    if line[i] == ' ':
                        i+=1
                    else:
                        nibble1 = get_nibble(line[i])
                        nibble2 = get_nibble(line[i+1])
                        i+=2
                        nibble = (nibble1 << 4) | nibble2
                        out.write(bytearray([nibble]))
    
