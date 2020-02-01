all: runELF_inMem victim

CC = gcc
# CFLAGS = 
runELF_inMem: runELF_inMem.c
	$(CC) -o runELF_inMem runELF_inMem.c 

victim: victim.c
	$(CC) -o victim victim.c 

.PHONY: clean,run
clean:
	rm runELF_inMem victim

run:
	./runELF_inMem

