
CC=gcc
CFLAGS=-Wall -O -pthread -lm
LDFLAGS=-static
LIBPATH = -L../libiec61850-1.3.3/build/
LIB = -liec61850
EXEC=sv_subscriber
INCLUDE = -I../inc


all: $(EXEC)
$(EXEC): sv_subscriber.c
				$(CC) $(INCLUDE)  sv_subscriber.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)

clean:
	rm *.o

