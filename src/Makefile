TOPDIR = ..

SUBDIRS = core event os

EXTRA_CFLAGS = -I./core -I./os/unix

OBJS = core/libcore.o os/libos.o
TARGET_PROG = cw
TARGET_PROG_OBJECTS=$(OBJS)

all: $(SUBDIRS) $(TARGET_PROG)

clean: subdirs
	@-rm -rf *.o *.a *~ .deps cw

include $(TOPDIR)/rules.Make