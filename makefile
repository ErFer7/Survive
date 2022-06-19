all: src/survive.c main.c
	@echo Building the game with release options.
	gcc src/survive.c main.c -o Survive -O2 -s -DNDEBUG

debug: src/survive.c main.c
	@echo Building the game with debug options.
	gcc src/survive.c main.c -o Survive -O2 -g -Wall

dis_release: src/survive.c main.c
	@echo Building the game with assembly output.
	gcc src/survive.c main.c -o Survive -O2 -s -DNDEBUG
	objdump -drwC -Mintel Survive.exe > Survive.s

dis_debug: src/survive.c main.c
	@echo Building the game with assembly output.
	gcc src/survive.c main.c -o Survive -O2 -g -Wall
	objdump -drwC -Mintel Survive.exe > Survive.s
