CC=m68k-amigaos-gcc
LINK=m68k-amigaos-gcc
ASS=vasmm68k_mot
VLINK=vlink
STRIP=m68k-amigaos-strip

CFLAGS = -noixemul -O3 --std=c99
LFLAGS = -noixemul

# Allow instructions to be emitted in runtime detection cases
AFLAGS = -Fhunk -m68040 -linedebug -chklabels -align -L listing.txt
AFLAGS += -I../../../../amiga/m68k-amigaos/ndk-include

VFLAGS = -b amigahunk -sc -l amiga -L m68k-amigaos/ndk/lib/libs

OBJS = main.o \
	   test.o
test: test.o
	$(VLINK) $(VFLAGS) $< -o $@

test:	${OBJS}
	$(LINK) $(LFLAGS) $^ -o $@
	$(STRIP) $@


clean:
	rm -f *.o
	rm -f ${OBJS}

#c/%.o: %.s Makefile
#	$(ASS) $(AFLAGS) $< -o $@

%.o: %.s Makefile
	$(ASS) $(AFLAGS) $< -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

