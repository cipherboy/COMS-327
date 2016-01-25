#Possible targets:
# 
# all:			recompiles code, runs tests
# format: 		formats source files
# build:		builds and compiles code
# clean: 		removes old object files
# changelog: 	updates changelog
# run:			makes all, executes code
# execute:		runs current executable
# test: 		runs unit test samples

ELF = mapgen
CFLAGS = -O2 -march=native -Wall -std=c11 -ggdb

# Default target
run: all execute

all: clean format changelog build test

clean:
	rm -f ./src/*.o

format:
	astyle --style=linux ./src/*.c || true
	rm -f ./src/*.c.orig

changelog:
	git log --full-history > CHANGELOG

test:
	./tests/mapgen.sh

build: mapgen

mapgen:
	gcc ${CFLAGS} -o bin/${ELF} src/*.c

execute:
	./bin/mapgen
