.PHONY: all clean

LIBRARIES := -L ../libX16/lib

INCLUDES := -I ../libX16/include

SRC = \
	loader.c screen.c levels.c runner.c key.c guard.c splash.c sound.c

OBJ = $(SRC:%.c=%.o)

#.c.o:
#	cl65 -t cx16 -I $(INCLUDE) -Oi $<

lode_runner: mainloop.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o lode_runner.prg mainloop.c $(SRC) libX16.lib cx16.lib

PALETTE.BIN: ../assets/palette.txt
	../scripts/ascii2bin.py --input ../assets/palette.txt --output PALETTE.BIN --addr 1000

SPLASH.BIN: ../assets/splash.txt
	../scripts/ascii2bin.py --input ../assets/splash.txt --output SPLASH.BIN --addr a000

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

CUSTOM.BIN: ../assets/custom.json ../scripts/levels.py
	../scripts/levels.py --input ../assets/custom.json --output CUSTOM.BIN


SPRITES.BIN: ../assets/sprites.txt ../scripts/graphic.py
	../scripts/graphic.py --input ../assets/sprites.txt --output SPRITES.BIN --x 8 --y 8 --bpp 4 --addr e000

clean:
	rm -f *.prg *.o

all: bins lode_runner

bins: CLASSIC.BIN CHAMP.BIN PRO.BIN FANBOOK.BIN REVENGE.BIN SPRITES.BIN TILES.BIN PALETTE.BIN CUSTOM.BIN SPLASH.BIN

artifacts: all
	rm -f ../lode_runner.zip
	zip ../lode_runner.zip lode_runner.prg *.BIN