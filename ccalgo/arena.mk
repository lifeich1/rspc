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


.PHONY: genchk
genchk: sf := ./t0.sh
genchk: pf := ./t1.py
genchk: s := echo >> $(sf)
genchk: p := echo >> $(pf)
genchk:
	test ! -f $(sf) && test ! -f $(pf)
	$(p) 'from random import *'
	$(p) 'n = randint(2, 9)'
	$(s) '#!/bin/bash'
	$(s) 'touch keeptest'
	$(s) 'g++ main.cc -o 1main'
	$(s) 'g++ ans.cc -o 1ans'
	$(s) 'while [ -f keeptest ]; do'
	$(s) '	python3 ./t1.py > 1.in'
	$(s) '	./1ans < 1.in > 1.ans'
	$(s) '	./1main < 1.in > 1.out'
	$(s) '	diff 1.out 1.ans || break'
	$(s) '	echo -n "OK "'
	$(s) 'done'
	$(s) 'echo'
	$(s) 'if [ -f keeptest ]; then echo WA 1.in; head 1.in; fi'
	chmod +x $(sf)
