.PHONY: init build clean format

all: build

init:
	test -d build || (mkdir build && cmake -S . -B build)

build: init
	make -C build

clean:
	rm -rf build

format: 