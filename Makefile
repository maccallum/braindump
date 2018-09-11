all: neurolator

CC=clang
O=-O3

neurolator: neurolator.c
	$(CC) -std=c99 -Wall $(CFLAGS) $(O) -o $@ $<
#$(CC) -std=c99 -Wall $(CFLAGS) $(O) -o c -I/System/Library/Frameworks/IOKit.framework/Headers -framework IOKit -framework CoreFoundation -framework Carbon -framework CoreMIDI c.c

.PHONY: clean
clean:
	rm -rf neurolator

