all: src/Source.c Main.c
	@echo Building the game with release options.
	gcc src/Source.c Main.c -o Survive -O2 -s -DNDEBUG

debug: src/Source.c Main.c
	@echo Building the game with debug options.
	gcc src/Source.c Main.c -o Survive -O2 -g -Wall

dis_release: src/Source.c Main.c
	@echo Building the game with assembly output.
	gcc src/Source.c Main.c -o Survive -O2 -s -DNDEBUG
	objdump -drwC -Mintel Survive.exe > Survive.s

dis_debug: src/Source.c Main.c
	@echo Building the game with assembly output.
	gcc src/Source.c Main.c -o Survive -O2 -g -Wall
	objdump -drwC -Mintel Survive.exe > Survive.s
