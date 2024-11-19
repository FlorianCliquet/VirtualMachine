flags=-O2 -std=c2x
ldflags=-lbu

.PHONY: all clean

all: clean VM

VM: simple_vm.o
	cc $(flags) $^ -o $@ $(ldflags)

simple_vm.o: simple_vm.c simple_vm.h
	cc $(flags) -c $<

clean:
	rm -f *.o VM