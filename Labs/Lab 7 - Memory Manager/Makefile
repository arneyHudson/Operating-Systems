CC=gcc
CFLAGS=-Wall -g -c
LDFLAGS=-pthread

all: testmemmgr pthread_testmemmgr

testmemmgr: memory_manager.o testmemmgr.o
	$(CC) -o testmemmgr memory_manager.o testmemmgr.o $(LDFLAGS)

pthread_testmemmgr: memory_manager.o pthread_testmemmgr.o
	$(CC) -o pthread_testmemmgr memory_manager.o pthread_testmemmgr.o $(LDFLAGS)

memory_manager.o: memory_manager.c memory_manager.h
	$(CC) $(CFLAGS) -o memory_manager.o memory_manager.c

testmemmgr.o: testmemmgr.c memory_manager.h
	$(CC) $(CFLAGS) -o testmemmgr.o testmemmgr.c

pthread_testmemmgr.o: pthread_testmemmgr.c memory_manager.h
	$(CC) $(CFLAGS) -o pthread_testmemmgr.o pthread_testmemmgr.c

clean:
	rm -f testmemmgr pthread_testmemmgr *.o
