TOPDIR = ..

SUBDIRS = core event http os/unix

EXTRA_CFLAGS = -I./core -I./os/unix
EXTRA_SYSLIBS = -lzlog

OBJS = core/libcore.o os/unix/libunix.o event/libevent.o http/libhttp.o ../obj/libobj.o
TARGET_PROG = capwap
TARGET_PROG_OBJECTS=$(OBJS)

all: $(SUBDIRS) $(TARGET_PROG)

clean: subdirs
	@-rm -rf *.o *.a *~ .deps $(TARGET_PROG)

include $(TOPDIR)/rules.Make