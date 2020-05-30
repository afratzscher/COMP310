CC=gcc -g

all: interpreter.o shell.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o
	$(CC) interpreter.o shell.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o -o mykernel

interpreter.o: interpreter.c
	$(CC) -c interpreter.c

shell.o: shell.c
	$(CC) -c shell.c

shellmemory.o: shellmemory.c
	$(CC) -c shellmemory.c

kernel.o: kernel.c
	$(CC) -c kernel.c

cpu.o: cpu.c
	$(CC) -c cpu.c

pcb.o: pcb.c
	$(CC) -c pcb.c

ram.o: ram.c
	$(CC) -c ram.c

memorymanager.o: memorymanager.c
	$(CC) -c memorymanager.c

.PHONY:
clean: 
	rm interpreter.o shell.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o
	rm mykernel

run:  mykernel
	./mykernel

debug: mykernel
	gdb mykernel

test: mykernel testfile.txt
	./mykernel < testfile.txt
