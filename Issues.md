# Known Issues
- key input handling likely needs work as control of the runner isn't very smooth,
  especially doing repeated digging or repeated movement
- timing of hole filling needs to be tuned
- main loop timing may needs work - is waitvsync() enough?


# Rough plans
- Start adding guard AI based on other LodeRunner ports
- Longer term: consider incorporating tilesets and concepts from LodeRunner Mad 
  Monks Revenge
- Figure out sound
- Add opening screen with world/level selection, support for jumping to specific level
- Improve transition between levels
- Add game over screen
- Add game editor?  This would require kernel SAVE support from banked RAM

# TODO
- Code cleanup/reorg among files
- TBD: refactoring to enable unit testing of core functions that aren't graphics-related

