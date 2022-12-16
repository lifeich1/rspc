F :=
CC := g++
CCFLAGS := -std=c++14 -g -Wall

ifeq ($(F),)
	$(error Empty filename)
endif

E := $(basename $(F))

$(E): $(F)
	$(CC) $(CCFLAGS) -o $@ $<