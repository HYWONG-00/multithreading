CFLAGS = -Wall -ansi -pedantic -Werror
OBJS = question2.o scheduler.o simulator.o
CC = gcc -pthread  
EXEC = q2

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

scheduler.o : scheduler.c simulator.h
	$(CC) $(CFLAGS) -c scheduler.c -D DEBUG=1
	
simulator.o : simulator.c scheduler.h simulator.h 
	$(CC) $(CFLAGS) -c simulator.c -D DEBUG=1

question2.o : question2.c scheduler.h simulator.h question2.h
	$(CC) $(CFLAGS) -c question2.c

clean:
	rm -f $(OBJS) $(EXEC)
