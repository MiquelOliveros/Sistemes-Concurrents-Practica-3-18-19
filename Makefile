################################################################
#   Codi Font													#
#	Practica 1 Sistemes Concurrents i Paralels					#
#	Alumnes:													#
#		Joan Palau Oncins		- 47692135W						#
#		Miquel Oliveros Muelas	- 0024201						#
################################################################ 

# compiler
CC=gcc
CFLAGS=-Wall -g -DNODEBUG -pthread

# solaris
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),SunOS)
	CFLAGS=$(CFLAGS) -lnsl -lsocket -lresolv -D_PLATFORM_SOLARIS -pthread
endif

# targets
DEPS=siftp.o service.o #stats.o
TARGETS=siftp siftpd

all: $(TARGETS)

siftpd: $(DEPS) server.o
	$(CC) $(CFLAGS) -o $@ $^
	#$(DEPS) stats.o
	#$(CC) $(CFLAGS) -o $@ $^ 

siftp: $(DEPS) client.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGETS)

