#!/usr/bin/env python3
#
# Parse LodeRunner levels file (json) and build a .bin file
# suitable for loading by x16 application
#
import argparse
import json
import sys

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
    description='Converts levels file (json) to .bin file')
parser.add_argument('input', help='.json filename')
parser.add_argument('output', help='.bin filename')
args = parser.parse_args()

# Note: these tile assignments must match those used in the runtime code
tile_map = {
    '0': 36,    # Guard (marker only)
    '&': 37,    # Runner (marker only)
    '#': 64,    # Brick
    '@': 65,    # Block
    'X': 66,    # Trap (displays as brick)
    'H': 67,    # Ladder
    'S': 68,    # Hidden ladder (displays as blank)
    '-': 69,    # Rope
    '$': 70,    # Gold
    ' ': 32     # Blank
}

top = None
try:
    with open(args.input,"r") as f:
        top = json.load(f)
except:
    print("Error reading %s" % input)
    sys.exit(1)

if "levels" in top:
    levels = top["levels"]
    num_levels = levels["total"]
    name = levels["name"]

    print ("Processing %d levels\n" % num_levels)

    with open(args.output,"wb") as out:
        # 2 byte header which is ignored when loading file
        header = bytearray([0x00,0xa0])
        out.write(header)

        # 2 byte field specifying number of levels
        header2 = bytearray([num_levels & 0xff, (num_levels >> 8)  ])
        out.write(header2)

        for i in range(1,num_levels+1):
            print ("    Processing level %d" % i)
            label = format("level-%03d" % i)
            data = levels[label]
            for row in data:
                row_data=[]
                for tile in row:
                    if tile in tile_map:
                        row_data.append(tile_map[tile])
                    else:
                        row_data.append(32)
                if (len(row_data) != 28):
                    print ("Bad data for row: %s",row)
                out.write(bytearray(row_data))


