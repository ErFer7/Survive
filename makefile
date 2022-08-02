release: src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c main.c
	@echo Building the game with release options.
	gcc src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c main.c -o Survive -O3 -s -DNDEBUG

debug: src/core.c src/utilities.c src/core.c src/graphics.c src/interface.c src/entity.c src/world.c main.c
	@echo Building the game with debug options.
	gcc src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c main.c -o Survive -O3 -g -Wall

dis_release: src/core.c src/utilities.c src/core.c src/graphics.c src/interface.c src/entity.c src/world.c main.c
	@echo Building the game with assembly output.
	gcc src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c main.c -o Survive -O3 -s -DNDEBUG
	objdump -drwC -Mintel Survive.exe > Survive.s

dis_debug: src/core.c src/utilities.c src/core.c src/graphics.c src/interface.c src/entity.c src/world.c main.c
	@echo Building the game with assembly output.
	gcc src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c main.c -o Survive -O3 -g -Wall
	objdump -drwC -Mintel Survive.exe > Survive.s
