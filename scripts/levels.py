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
    ' ': 0,    # Blank
    '0': 1,    # Guard (marker only)
    '&': 2,    # Runner (marker only)
    '#': 3,    # Brick
    '@': 4,    # Block
    'X': 5,    # Trap (displays as brick)
    'H': 6,    # Ladder
    'S': 7,    # Hidden ladder (displays as blank)
    '-': 8,    # Rope
    '$': 9,    # Gold
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

    total = 0

    with open(args.output,"wb") as out:
        # 2 byte header which is ignored when loading file
        header = bytearray([0x00,0xa0])
        total += out.write(header)

        # 2 byte field specifying first level of this bank
        header2 = bytearray([1, num_levels & 0xff  ])
        total += out.write(header2)
        for i in range(1,num_levels+1):

            print ("    Processing level %d" % i)
            label = format("level-%03d" % i)
            data = levels[label]
            for row_idx in range(0,16):
                row = data[row_idx]
                row_data=[]
                for idx in range(0,28,2):
                    tile1 = row[idx]
                    tile2 = row[idx+1]
                    tile_data1 = 0
                    if tile1 in tile_map:
                        tile_data1 = tile_map[tile1]
                    tile_data2 = 0
                    if tile2 in tile_map:
                        tile_data2 = tile_map[tile2]
                    tile_data = (tile_data1 << 4) | tile_data2
                    row_data.append(tile_data)
                if len(row_data) != 14:
                    print("Bad data for row")
                total += out.write(bytearray(row_data))
            if (i % 36) == 0:
#                print("Padding after level %d" % i)
                total += out.write(bytearray(126))
                if (i+1 < num_levels):
                    # 2 byte field specifying first level of this bank
                    header2 = bytearray([i+1, num_levels & 0xff  ])
                    total += out.write(header2)

    print("Wrote %d bytes to %s" % (total,args.output))
