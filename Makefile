OUTPUT = Oktoberfest

COMPILER = /usr/bin/gcc
LD = $(COMPILER)

INCLUDE = -I /usr/lib

CFLAGS += -std=c99

LDFLAGS = -L/usr/include

SOURCES = main.c
SOURCES += menu.c

Release: TARGET_CFLAGS := -O3
Release: OUTDIR := Release
Release: mkdir
Release: link

Debug: TARGET_CFLAGS := -O1 -g
Debug: OUTDIR := Debug
Debug: mkdir
Debug: link


mkdir:
	mkdir -p $(OUTDIR)

OBJS = $(SOURCES:.c=.o)
$(OBJS): %.o: %.c
	$(COMPILER) -c $(CFLAGS) $(TARGET_CFLAGS) $< -o $@

link: $(OBJS)
	$(LD) -o $(OUTDIR)/$(OUTPUT) $^ $(LDFLAGS)

clean:
	rm -rf *.o
