# LodeRunner for the Commander X16

This in-progress port is based on other open source LodeRunner ports: [LodeRunner TotalRecall](https://github.com/SimonHung/LodeRunner_TotalRecall) and [LodeRunner for Roku](https://github.com/lvcabral/Lode-Runner-Roku).

This port is (so far :) ) being written in C using the [CC65](https://cc65.github.io/) toolchain.  Also using the [libX16](https://github.com/CJLove/libX16) library for various utility functions.

![](runner.gif)

## Game Play
From the splash screen use cursor up/down keys to select the world.  Use `ENTER` to begin play.

Use cursor arrow keys to move the runner, `CTRL` to dig left and `ALT` to dig right.  Use `ENTER` to kill the runner if trapped.

## Building and Running
Building requires the CC65 toolchain.  After cloning the repo, do the following:
```bash
git submodule update --init
cd libX16/src
make
cd ../../src
make all
/path/to/x16emu -prg lode_runner.prg
```

## Test programs
* `test_levels.prg` - browse levels in any supported world.  Use the `+` and `-` keys to browse the next and previous levels.  Use `1` - `5` to change worlds (Classic, Championship, Professional, Fanbook, Revenge).  Use `c` to "complete" any given level (changing hidden ladder tiles to regular ladder tiles)

* `test_runner.prg` - tests the runner's animations and basic movements on level 1.  Use the `j` and `k` for left/right, `i` and `m` for up/down.  Debug information about the current surrounding tiles and the runner's current position are displayed to the right.

* `test_tiles.prg` - displays tiles (alphabetic characters, numbers, game tiles).  Hit any key to cycle through the tile animation for digging holes to left and right of runner.

* `test_sprites.prg` - displays sprite images for the runner and guards (images are reversed as necessary for moving left)

* `test_keys.prg` - displays values from key (joystick) input

* `test_splash.prg` - displays splash screen tilemap
