TOPDIR = ../..

OBJS = cw_http.o
EXTRA_CFLAGS = -DCW_LINUX -I. -I../os/unix -I../core
TARGET_O_CP = libhttp.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf *.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make