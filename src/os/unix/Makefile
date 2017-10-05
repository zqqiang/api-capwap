TOPDIR = ../../..

OBJS = cw_process_cycle.o cw_process.o cw_thread_id.o
EXTRA_CFLAGS = -I. -I../../core -DCW_LINUX
TARGET_O_CP = libunix.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf *.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make