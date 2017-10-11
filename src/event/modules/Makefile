TOPDIR = ../..

OBJS = cw_epoll_module.o
EXTRA_CFLAGS = -DCW_LINUX -I.
TARGET_O_CP = libmodules.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf *.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make