CC=g++ 
CCFLAGS=-Wall -Wextra -Werror 
INC=-I./include/ -I./include/external
LIN=
DEF=
STD=-std=c++17

src=src/main.cc src/directoryItem.cc
exe=build/test.exe
prexe=build/cinitpp.exe

all: com 

dbg: dbgc 

prod: $(src) 
	mkdir -p build/test
	rm -f $(prexe) 
	$(CC) $(CCFLAGS) -o$(prexe) $(src) $(INC) $(LIN) $(DEF) $(STD) -O3

com: $(src)
	mkdir -p build/test
	rm -f $(exe)
	$(CC) $(CCFLAGS) -o$(exe) $(src) $(INC) $(LIN) $(DEF) $(STD)

dbgc: $(src) 
	mkdir -p build/test
	rm -f $(exe)
	$(CC) $(CCFLAGS) -o$(exe) $(src) $(INC) $(LIN) $(DEF) $(STD) -g3 

run: $(exe)
	./$(exe)

dbgr: $(exe)
	gdb $(exe)

clear: 
	rm -rf build/*

reset: 
	mkdir tmp
	cp build/*.exe tmp 
	rm -rf build/test/* 
	cp tmp/*.exe build
	rm -rf tmp
