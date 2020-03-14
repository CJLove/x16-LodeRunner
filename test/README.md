## Lode Runner test programs
Test programs can be built from the test directory:
```bash
cd libX16Test/src
make
cd ../../test
make all
```

* `test_levels.prg` - browse levels in any supported world.  Use the `+` and `-` keys to browse the next and previous levels.  Use `1` - `5` to change worlds (Classic, Championship, Professional, Fanbook, Revenge).  Use `c` to "complete" any given level (changing hidden ladder tiles to regular ladder tiles)

* `test_runner.prg` - tests the runner's animations and basic movements on level 1.  Use the `j` and `k` for left/right, `i` and `m` for up/down.  Debug information about the current surrounding tiles and the runner's current position are displayed to the right.

* `test_tiles.prg` - displays tiles (alphabetic characters, numbers, game tiles).  Hit any key to cycle through the tile animation for digging holes to left and right of runner.

* `test_sprites.prg` - displays sprite images for the runner and guards (images are reversed as necessary for moving left)

* `test_keys.prg` - displays values from key (joystick) input

* `test_splash.prg` - displays splash screen tilemap

* `test_unitRunner.prg` - initial unit test of runner code
