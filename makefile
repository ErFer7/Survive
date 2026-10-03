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

# TODO: Fix this insanity
.PHONY: gdb
gdb:
	@kitty sh -c "tty > /tmp/survive_tty && sleep infinity" & \
	while [ ! -f /tmp/survive_tty ]; do sleep 0.05; done; \
	TTY=$$(cat /tmp/survive_tty); \
	rm -f /tmp/survive_tty; \
	gdb -ex "set inferior-tty $$TTY" ./build/survive

.PHONY: valgrind
valgrind:
	valgrind --leak-check=full ./build/survive

.PHONY: clean
clean:
	cmake --build build --target clean

.PHONY: veryclean
veryclean:
	rm -rf build .cache
