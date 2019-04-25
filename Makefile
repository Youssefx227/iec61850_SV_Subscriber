
CC=gcc
CFLAGS=-Wall -O -pthread -lm
LDFLAGS=-static
LIBPATH = -L../libiec61850-1.3.3/build/
LIB = -liec61850
EXEC=sv_subscriber
INCLUDE = -I../inc


all: $(EXEC)
$(EXEC): sv_subscriber_example.c
				$(CC) $(INCLUDE)  sv_subscriber_example.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)

clean:
	rm *.o

