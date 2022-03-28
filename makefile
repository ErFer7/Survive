all: src/Source.c Main.c
	@echo Building the game with release options.
	gcc src/Source.c Main.c -o Survive -O2 -s -DNDEBUG

debug: src/Source.c Main.c
	@echo Building the game with debug options.
	gcc src/Source.c Main.c -o Survive -g -Wall
