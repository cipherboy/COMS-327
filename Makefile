#Possible targets:
#
# all:				recompiles code, runs tests
# format: 			formats source files
# build:			builds and compiles code
# clean: 			removes old object files
# changelog: 		updates changelog
# run:				makes all, executes code
# execute:			runs current executable

VERSION = Scheel_Alexander.assignment-1.07
NAME = rlg327
CC ?= gcc
CXX ?= g++
CFLAGS = -O0 -mtune=native -Wall -std=gnu99 -ggdb -fdiagnostics-color=always
CXXFLAGS = -O0 -mtune=native -Wall -std=gnu++98 -ggdb -fdiagnostics-color=always
LFLAGS = -lm -lncurses

CSOURCES = $(wildcard src/*.c)
CXXSOURCES = $(wildcard src/*.cc)
COBJECTS = $(CSOURCES:.c=.o)
CXXOBJECTS = $(CXXSOURCES:.cc=.o)

# Default target
run: all execute

all: format changelog build

clean:
	rm -rf ./src/*.o ./bin/*.dSYM ./autom4te.cache ./configure ./bin/$(NAME)

format:
	astyle --style=linux ./src/*.c || true
	astyle --style=linux ./src/*.cc || true
	astyle --style=linux ./src/*.h || true
	rm -f ./src/*.orig

changelog: .git
	git log --source --log-size --all --cherry --decorate=full --full-history \
		--date-order --show-notes --relative-date  --abbrev-commit --children \
		--stat --no-color > CHANGELOG

build: main

main: $(COBJECTS) $(CXXOBJECTS)
	$(CXX) $(LFLAGS) $(COBJECTS) $(CXXOBJECTS) -o bin/$(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

gzip:
	tar -cf ../$(VERSION).tar ../$(VERSION)
	gzip ../$(VERSION).tar

execute:
	./bin/$(NAME) --error || true
