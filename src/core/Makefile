TOPDIR = ../..

OBJS = cw.o cw_string.o cw_cycle.o
EXTRA_CFLAGS = -DCW_LINUX -I. -I../os/unix
TARGET_O_CP = libcore.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf *.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make