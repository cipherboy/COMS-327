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

NAME = rlg327
CC ?= gcc
CFLAGS = -O0 -march=native -Wall -std=gnu99 -ggdb

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)



# Default target
run: all execute

all: format changelog build test

clean:
	rm -rf ./src/*.o ./bin/mapgen.dSYM ./autom4te.cache ./configure

format:
	astyle --style=linux ./src/*.c || true
	astyle --style=linux ./src/*.h || true
	rm -f ./src/*.orig

changelog:
	git log --source --log-size --all --cherry --decorate=full --full-history \
		--date-order --show-notes --relative-date  --abbrev-commit --children \
		--stat --no-color > CHANGELOG

test:
	true

build: main

main: $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/$(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


execute:
	./bin/$(NAME) || true
