release: src/entity.c src/renderer.c src/survive.c main.c
	@echo Building the game with release options.
	gcc src/entity.c src/renderer.c src/survive.c main.c -o Survive -O3 -s -DNDEBUG

debug: src/entity.c src/renderer.c src/survive.c main.c
	@echo Building the game with debug options.
	gcc src/entity.c src/renderer.c src/survive.c main.c -o Survive -O3 -g -Wall

dis_release: src/entity.c src/renderer.c src/survive.c main.c
	@echo Building the game with assembly output.
	gcc src/entity.c src/renderer.c src/survive.c main.c -o Survive -O3 -s -DNDEBUG
	objdump -drwC -Mintel Survive.exe > Survive.s

dis_debug: src/entity.c src/renderer.c src/survive.c main.c
	@echo Building the game with assembly output.
	gcc src/entity.c src/renderer.c src/survive.c main.c -o Survive -O3 -g -Wall
	objdump -drwC -Mintel Survive.exe > Survive.s
