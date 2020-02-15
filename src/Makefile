.PHONY: all clean

LIBRARIES := ../libX16/lib

INCLUDE := ../libX16/include

SRC = \
	loader.c screen.c levels.c runner.c key.c

OBJ = $(SRC:%.c=%.o)

#.c.o:
#	cl65 -t cx16 -I $(INCLUDE) -Oi $<

lode_runner: mainloop.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o lode_runner.prg mainloop.c $(SRC) libX16.lib cx16.lib

test_keys: test_keys.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o test_keys.prg test_keys.c $(SRC) libX16.lib cx16.lib


test_levels: test_levels.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o test_levels.prg test_levels.c $(SRC) libX16.lib cx16.lib

test_runner: test_runner.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o test_runner.prg test_runner.c $(SRC) libX16.lib cx16.lib

test_tiles: test_tiles.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o test_tiles.prg test_tiles.c $(SRC) libX16.lib cx16.lib

test_sprites: test_sprites.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o test_sprites.prg test_sprites.c $(SRC) libX16.lib cx16.lib

PALETTE.BIN: ../assets/palette.txt
	../scripts/ascii2bin.py --input ../assets/palette.txt --output PALETTE.BIN --addr 1000

TILES.BIN: ../assets/tiles.txt
	../scripts/graphic.py --input ../assets/tiles.txt --output TILES.BIN --x 8 --y 8 --bpp 2 --addr 0000

CLASSIC.BIN: ../assets/classic.json
	../scripts/levels.py --input ../assets/classic.json --output CLASSIC.BIN

CHAMP.BIN: ../assets/championship.json
	../scripts/levels.py --input ../assets/championship.json --output CHAMP.BIN

PRO.BIN: ../assets/professional.json
	../scripts/levels.py --input ../assets/professional.json --output PRO.BIN

FANBOOK.BIN: ../assets/fanbook.json
	../scripts/levels.py --input ../assets/fanbook.json --output FANBOOK.BIN

REVENGE.BIN: ../assets/revenge.json
	../scripts/levels.py --input ../assets/revenge.json --output REVENGE.BIN

SPRITES.BIN: ../assets/sprites.txt ../scripts/graphic.py
	../scripts/graphic.py --input ../assets/sprites.txt --output SPRITES.BIN --x 8 --y 8 --bpp 4 --addr e000

clean:
	rm -f *.prg *.o

bins: CLASSIC.BIN CHAMP.BIN PRO.BIN FANBOOK.BIN REVENGE.BIN SPRITES.BIN TILES.BIN PALETTE.BIN