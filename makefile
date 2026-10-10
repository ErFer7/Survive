TERMINAL ?= kitty
VALGRIND_FLAGS ?= --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

.PHONY: default
default:
	cmake --build build -j$(nproc)

.PHONY: setup
setup:
	cmake -B build

.PHONY: debug
debug:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build -j$(nproc)

.PHONY: release
release:
	cmake -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j$(nproc)

.PHONY: run
run:
	./build/survive

.PHONY: gdb
gdb:
	@set -e; \
	dir=$$(mktemp -d); fifo="$$dir/tty"; mkfifo "$$fifo"; \
	$(TERMINAL) sh -c 'tty > "$$1"; exec sleep infinity' sh "$$fifo" & \
	term=$$!; \
	trap 'kill $$term 2>/dev/null; rm -rf "$$dir"' EXIT; \
	tty=$$(cat "$$fifo"); \
	gdb -ex "set inferior-tty $$tty" ./build/survive

.PHONY: valgrind
valgrind: build/survive
	@set -e; \
	dir=$$(mktemp -d); fifo="$$dir/tty"; mkfifo "$$fifo"; \
	$(TERMINAL) sh -c 'tty > "$$1"; exec sleep infinity' sh "$$fifo" & \
	term=$$!; \
	trap 'kill $$term 2>/dev/null; rm -rf "$$dir"' EXIT; \
	tty=$$(cat "$$fifo"); \
	valgrind $(VALGRIND_FLAGS) --log-file=valgrind.log \
		./build/survive $(ARGS) <"$$tty" >"$$tty" 2>&1

.PHONY: clean
clean:
	cmake --build build --target clean

.PHONY: veryclean
veryclean:
	rm -rf build .cache
