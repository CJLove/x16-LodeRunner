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

PALETTE.BIN: ../assets/palette.txt
	../scripts/ascii2bin.py ../assets/palette.txt PALETTE.BIN 

TILES.BIN: ../assets/tiles.txt
	../scripts/ascii2bin.py ../assets/tiles.txt TILES.BIN

CLASSIC.BIN: ../assets/classic.json
	../scripts/levels.py ../assets/classic.json CLASSIC.BIN

FANBOOK.BIN: ../assets/fanbook.json
	../scripts/levels.py ../assets/fanbook.json FANBOOK.BIN

clean:
	rm -f *.prg *.o