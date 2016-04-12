#Possible targets:
#
# all:				recompiles code, runs tests
# format: 			formats source files
# build:			builds and compiles code
# clean: 			removes old object files
# changelog: 		updates changelog
# run:				makes all, executes code
# execute:			runs current executable

VERSION = Scheel_Alexander.assignment-1.09
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
CXXMAINSOURCES = $(wildcard main/*.cc)
CXXMAINOBJECTS = $(CXXTESTSOURCES:.cc=.o)
CXXTESTSOURCES = $(wildcard tests/*.cc)
CXXTESTOBJECTS = $(CXXTESTSOURCES:.cc=.o)

# Default target
run: all execute

all: format changelog build

clean:
	rm -rf ./*/*.o ./bin/*.dSYM ./autom4te.cache ./configure ./bin/tests-$(NAME) ./bin/$(NAME)

format:
	astyle --style=linux ./src/*.c || true
	astyle --style=linux ./src/*.cc || true
	astyle --style=linux ./src/*.h || true
	rm -f ./src/*.orig

changelog: .git
	git log --source --log-size --all --cherry --decorate=full --full-history \
		--date-order --show-notes --relative-date  --abbrev-commit --children \
		--stat --no-color > CHANGELOG

build: main tests

main: $(COBJECTS) $(CXXOBJECTS) $(CXXTESTOBJECTS) $(CXXMAINOBJECTS)
	$(CXX) $(LFLAGS) $(COBJECTS) $(CXXOBJECTS) $(CXXMAINOBJECTS) -o bin/$(NAME)
	$(CXX) $(LFLAGS) $(COBJECTS) $(CXXOBJECTS) $(CXXTESTOBJECTS) -o bin/tests-$(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

gzip:
	tar -cf ../$(VERSION).tar ../$(VERSION)
	gzip ../$(VERSION).tar

execute:
	./bin/tests-$(NAME) || true
