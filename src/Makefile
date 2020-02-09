.PHONY: all clean

LIBRARIES := ../libX16/lib

INCLUDE := ../libX16/include

SRC = \
	loader.c screen.c levels.c

OBJ = $(SRC:%.c=%.o)

#.c.o:
#	cl65 -t cx16 -I $(INCLUDE) -Oi $<


levels: browser.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o levels.prg browser.c $(SRC) libX16.lib cx16.lib

runner: animation.c $(SRC)
	cl65 -t cx16 -I $(INCLUDE) -L $(LIBRARIES) -Oi -o runner.prg animation.c $(SRC) libX16.lib cx16.lib

PALETTE.BIN: ../assets/palette.txt
	../scripts/ascii2bin.py ../assets/palette.txt PALETTE.BIN 1000

TILES.BIN: ../assets/tiles.txt
	../scripts/graphic.py --input ../assets/tiles.txt --output TILES.BIN --x 8 --y 8 --bpp 2 --addr 0000

CLASSIC.BIN: ../assets/classic.json
	../scripts/levels.py ../assets/classic.json CLASSIC.BIN

CHAMP.BIN: ../assets/championship.json
	../scripts/levels.py ../assets/championship.json CHAMP.BIN

PRO.BIN: ../assets/professional.json
	../scripts/levels.py ../assets/professional.json PRO.BIN

FANBOOK.BIN: ../assets/fanbook.json
	../scripts/levels.py ../assets/fanbook.json FANBOOK.BIN

REVENGE.BIN: ../assets/revenge.json
	../scripts/levels.py ../assets/revenge.json REVENGE.BIN

SPRITES.BIN: ../assets/sprites.txt ../scripts/graphic.py
	../scripts/graphic.py --input ../assets/sprites.txt --output SPRITES.BIN --x 8 --y 8 --bpp 4 --addr e000

clean:
	rm -f *.prg *.o

bins: CLASSIC.BIN CHAMP.BIN PRO.BIN FANBOOK.BIN REVENGE.BIN