F :=
CC := g++
T := 1
CCFLAGS := -std=c++17 -g -Wall $(if $(T),-DRSPC_TRACE_HINT=1)

ifeq ($(F),)
	$(error Empty filename)
endif

E := $(basename $(F))

$(E): $(F)
	$(CC) $(CCFLAGS) -o $@ $<
