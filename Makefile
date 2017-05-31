CC=clang
ifeq ($(which clang), "/usr/bin/clang")
	CC=cc
endif
CFLAGS=-c -ggdb -std=c99 -Wall
LDFLAGS=-lcurl -ljansson
SOURCES=main.c zagruzka.c zapros.c feed2rss.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=vkfeed2rss

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
