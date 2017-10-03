TOPDIR = ../..

OBJS = unix/cw_process_cycle.o
EXTRA_CFLAGS = -DCW_LINUX -I./unix -I../core
TARGET_O_CP = libos.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf unix/*.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make