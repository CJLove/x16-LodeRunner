#!/usr/bin/env python3
#
# Parse .txt file and create .bin file suitable for loading
# by x16 application
#

import argparse
import sys

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
    description='Converts .txt file to .bin file')
parser.add_argument('input', help='.json filename')
parser.add_argument('output', help='.bin filename')
args = parser.parse_args()

