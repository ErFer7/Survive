release: src/vector2D.c src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c
	@echo Building the game with release options.
	gcc src/vector2D.c src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c -o Survive -O3 -s -DNDEBUG -fopenmp

debug: src/core.c src/utilities.c src/core.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c
	@echo Building the game with debug options.
	gcc src/vector2D.c src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c -o Survive -O3 -g -Wall -fopenmp

dis_release: src/core.c src/utilities.c src/core.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c
	@echo Building the game with assembly output.
	gcc src/vector2D.c src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c -o Survive -O3 -s -DNDEBUG -fopenmp
	objdump -drwC -Mintel Survive.exe > Survive.s

dis_debug: src/core.c src/utilities.c src/core.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c
	@echo Building the game with assembly output.
	gcc src/vector2D.c src/core.c src/utilities.c src/graphics.c src/interface.c src/entity.c src/world.c src/statemachine.c main.c -o Survive -O3 -g -Wall -fopenmp
	objdump -drwC -Mintel Survive.exe > Survive.s
