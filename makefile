CC=g++ 
CCFLAGS=-Wall -Wextra -Werror 
INC=-I./include/
LIN=-ljsoncpp
DEF=
STD=-std=c++17

src=src/main.cc src/file.cc
exe=build/test.exe
prexe=build/cinitpp.exe

all: com 

dbg: dbgc 

prod: $(src) 
	rm -f $(prexe) 
	$(CC) $(CCFLAGS) -o$(prexe) $(src) $(INC) $(LIN) $(DEF) $(STD) -O3

com: $(src)
	rm -f $(exe)
	$(CC) $(CCFLAGS) -o$(exe) $(src) $(INC) $(LIN) $(DEF) $(STD)

dbgc: $(src) 
	rm -f $(exe)
	$(CC) $(CCFLAGS) -o$(exe) $(src) $(INC) $(LIN) $(DEF) $(STD) -g3 

run: $(exe)
	./$(exe)

dbgr: $(exe)
	gdb $(exe)

clear: 
	rm -rf build/*

# Doesn't work. 
reset: 
	rm -rf */ !*.exe
