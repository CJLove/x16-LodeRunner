# LodeRunner for the Commander X16

This in-progress port is based on other open source LodeRunner ports: [LodeRunner TotalRecall](https://github.com/SimonHung/LodeRunner_TotalRecall) and [LodeRunner for Roku](https://github.com/lvcabral/Lode-Runner-Roku).

This port is (so far :) ) being written in C using the [CC65](https://cc65.github.io/) toolchain.  Also using the [libX16](https://github.com/CJLove/libX16) library for various utility functions.

## Test programs
* `levels.prg` - browse levels in any supported world.  Use the `+` and `-` keys to browse the next and previous levels.  Use `1` - `5` to change worlds (Classic, Championship, Professional, Fanbook, Revenge).  Use `c` to "complete" any given level (changing hidden ladder tiles to regular ladder tiles)

![](levels.gif)

* `runner.prg` - tests the runner's animations and basic movements on level 1.  Use the `j` and `k` for left/right, `i` and `m` for up/down.  The tiles directly under and directly below the runner are shown to the right, along with the runner's sprite's X and Y locations.

![](runner.gif)
