TOPDIR = ../..

EXTRA_CFLAGS = -I.

EXTRA_CFLAGS += -DCW_LINUX -I../os/unix

OBJS = cw.o cw_log.o
TARGET_PROG = core
TARGET_PROG_OBJECTS = $(OBJS)

all: $(TARGET_PROG)

clean:
	-rm -rf *.o *.a *~ core .deps $(TARGET_PROG)

include $(TOPDIR)/rules.Make