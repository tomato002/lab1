CC = gcc
CFLAGS = -Wall -std=c11
RM = rm
PYTHON = python3

OBJS = strace.o

all: clean gen_h $(OBJS)
	$(CC) $(CFLAGS) -o my_strace $(OBJS)

gen_h:
	$(RM) -f syscall_table.h
	$(PYTHON) gen_syscall_table.py

clean:
	$(RM) -f strace my_strace *.o syscall_table.h