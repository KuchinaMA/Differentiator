CC = g++
CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal\
-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum\
-Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

all: DiffRes.exe

DiffRes.exe: Main.o Tree.o TreeDump.o
	$(CC) $(CFLAGS) $^ -o $@

Main.o: Main.cpp Tree.h TreeDump.h   
	$(CC) $(CFLAGS) $< -c -o $@

Tree.o: Tree.cpp Tree.h
	$(CC) $(CFLAGS) $< -c -o $@

TreeDump.o: TreeDump.cpp Tree.h TreeDump.h
	$(CC) $(CFLAGS) $< -c -o $@ 

.PHONY: all clean

clean:
	rm *.o