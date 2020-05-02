# LodeRunner for the Commander X16

This in-progress port is based on other open source LodeRunner ports: [LodeRunner TotalRecall](https://github.com/SimonHung/LodeRunner_TotalRecall) and [LodeRunner for Roku](https://github.com/lvcabral/Lode-Runner-Roku).

This port is (so far :smile:) being written in C using the [CC65](https://cc65.github.io/) toolchain.  Also using the [libX16](https://github.com/CJLove/libX16) library for various utility functions and using the [libX16Test](https://github.com/CJLove/libX16Test) library for unit tests.

![](runner.gif)

## Game Play
From the splash screen the following keys to select game options:
* Cursor up/down select the world
* Cursor left/right toggle sound
* `<` and `>` adjust the game speed
* `-` and `+` adjust the starting level

Use `ENTER` to begin play.

Use cursor arrow keys to move the runner, `CTRL` to dig left and `ALT` to dig right.  Use `ENTER` to kill the runner if trapped.

## Building and Running
Building requires the CC65 toolchain with r37 support (commit [`cbf0c1d`](https://github.com/cc65/cc65/commit/cbf0c1d1dddc9d201ef3bf6ce9f3d5b54bc6e325)) or later. Python3 scripts are used to build binary files from various assets. After cloning the repo, do the following:
```bash
git submodule update --init
cd libX16/src
make
cd ../../src
make all
/path/to/x16emu -prg lode_runner.prg
```

