# Known Issues
- In `levels.prg` the `c` action to complete the level doesn't always work
- In `levels.prg` at some times the last level in a bank (e.g. 36, 72, ...) display as blank
- In `runner.prg` the runner movement isn't exactly game-like but does exercise the sprite animations except for falling

# Rough plans
- Complete runner sprite images, get all movement and actions on a level fully working
- Do guard sprite images, start adding runner AI based on other LodeRunner ports
- Longer term: consider incorporating tilesets and concepts from LodeRunner Mad Monks Revenge

# TODO
- Cleanup python scripts
- Complete the alphabetic tiles versus just those needed for status line.  
- Redo tiles.txt so TILES.BIN can be built using the same `graphic.py` script as sprites.txt
- Code cleanup/refactoring


