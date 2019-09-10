
CC=gcc
CFLAGS=-Wall -O -pthread -lm
LDFLAGS=-static
LIBPATH = -L../libiec61850-1.3.3/build/
LIB = -liec61850
EXEC = sv_subscriber
#EXEC = goose_publisher
INCLUDE = -I../inc

all: $(EXEC)
<<<<<<< HEAD
#$(EXEC): sv_subscriber_multi_channel.c
$(EXEC): sv_subscriber_1MU_acquisition.c
#			$(CC) $(INCLUDE)  sv_subscriber.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)
			$(CC) $(INCLUDE) sv_subscriber_1MU_acquisition.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)
=======
#$(EXEC): sv_subscriber.c
$(EXEC): sv_subscriber_phasor_extract.c
#			$(CC) $(INCLUDE)  sv_subscriber.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)
			$(CC) $(INCLUDE) sv_subscriber_phasor_extract.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)
>>>>>>> ec8fcc9e65e881565b24fbc0d3df713224ea6c65
#only goose publisher

#$(EXEC): goose_publisher.c
#				$(CC) $(INCLUDE)  goose_publisher.c -o $(EXEC) $(CFLAGS) $(LIBPATH) $(LIB)

clean:
	rm *.o



