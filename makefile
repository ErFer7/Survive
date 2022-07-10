release: src/renderer.c src/survive.c main.c
	@echo Building the game with release options.
	gcc src/renderer.c src/survive.c main.c -o Survive -O2 -s -DNDEBUG -fopenmp

debug: src/renderer.c src/survive.c main.c
	@echo Building the game with debug options.
	gcc src/renderer.c src/survive.c main.c -o Survive -O2 -g -Wall -fopenmp

dis_release: src/renderer.c src/survive.c main.c
	@echo Building the game with assembly output.
	gcc src/renderer.c src/survive.c main.c -o Survive -O2 -s -DNDEBUG -fopenmp
	objdump -drwC -Mintel Survive.exe > Survive.s

dis_debug: src/renderer.c src/survive.c main.c
	@echo Building the game with assembly output.
	gcc src/renderer.c src/survive.c main.c -o Survive -O2 -g -Wall -fopenmp
	objdump -drwC -Mintel Survive.exe > Survive.s
