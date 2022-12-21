F :=
CC := g++
T := 1

BTIM := $(shell date +%D-%Hh-%Mm-%Ss)
CCFLAGS := -std=c++17 -g -Wall $(if $(T),-DRSPC_TRACE_HINT=1) -D RSPC_TRACE_BTIME='"$(BTIM)"'

ifeq ($(F),)
	$(error Empty filename)
endif

E := $(basename $(F))

$(E): $(F)
	$(CC) $(CCFLAGS) -o $@ $<
