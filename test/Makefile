.PHONY: all clean

LIBRARIES := -L ../libX16/lib -L ../libX16Test/lib

INCLUDES := -I ../libX16/include -I ../libX16Test/include -I ../src

SRC = \
	../src/state.c ../src/loader.c ../src/screen.c ../src/levels.c ../src/runner.c ../src/key.c ../src/guard.c ../src/splash.c ../src/sound.c

test_keys: test_keys.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_keys.prg test_keys.c $(SRC) libX16.lib cx16.lib

test_levels: test_levels.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_levels.prg test_levels.c $(SRC) libX16.lib cx16.lib

test_runner: test_runner.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_runner.prg test_runner.c $(SRC) libX16.lib cx16.lib

test_tiles: test_tiles.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_tiles.prg test_tiles.c $(SRC) libX16.lib cx16.lib

test_splash: test_splash.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_splash.prg test_splash.c $(SRC) libX16.lib cx16.lib

test_sprites: test_sprites.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_sprites.prg test_sprites.c $(SRC) libX16.lib cx16.lib

test_unitRunner: test_main.c testRunner.c ../src/levels.c ../src/runner.c ../src/loader.c
	cl65 -t cx16 --listing list.txt $(INCLUDES) $(LIBRARIES) -Oi -o test_unitRunner.prg test_main.c testRunner.c ../src/levels.c ../src/loader.c ../src/runner.c ../src/state.c  libX16.lib libX16Test.lib cx16.lib


PALETTE.BIN: ../assets/palette.txt
	../scripts/ascii2bin.py --input ../assets/palette.txt --output PALETTE.BIN --addr 1000

SPLASH.BIN: ../assets/splash.txt
	../scripts/ascii2bin.py --input ../assets/splash.txt --output SPLASH.BIN --addr a000

TILEMAP.BIN: ../assets/tilemap.txt
	../scripts/ascii2bin.py --input ../assets/splash.txt --output TILEMAP.BIN --addr 0000	

TILES.BIN: ../assets/tiles.txt
	../scripts/graphic.py --input ../assets/tiles.txt --output TILES.BIN --x 8 --y 8 --bpp 2 --addr 0000

CLASSIC.BIN: ../assets/classic.json ../scripts/levels.py
	../scripts/levels.py --input ../assets/classic.json --output CLASSIC.BIN

CHAMP.BIN: ../assets/championship.json ../scripts/levels.py
	../scripts/levels.py --input ../assets/championship.json --output CHAMP.BIN

PRO.BIN: ../assets/professional.json ../scripts/levels.py
	../scripts/levels.py --input ../assets/professional.json --output PRO.BIN

FANBOOK.BIN: ../assets/fanbook.json ../scripts/levels.py
	../scripts/levels.py --input ../assets/fanbook.json --output FANBOOK.BIN

REVENGE.BIN: ../assets/revenge.json ../scripts/levels.py
	../scripts/levels.py --input ../assets/revenge.json --output REVENGE.BIN

CUSTOM.BIN: custom.json ../scripts/levels.py
	../scripts/levels.py --input ./custom.json --output CUSTOM.BIN


SPRITES.BIN: ../assets/sprites.txt ../scripts/graphic.py
	../scripts/graphic.py --input ../assets/sprites.txt --output SPRITES.BIN --x 8 --y 8 --bpp 4 --addr e000

clean:
	rm -f *.prg *.o

all: bins test_keys test_levels test_runner test_tiles test_splash test_sprites

bins: CLASSIC.BIN CHAMP.BIN PRO.BIN FANBOOK.BIN REVENGE.BIN SPRITES.BIN TILES.BIN PALETTE.BIN CUSTOM.BIN SPLASH.BIN
