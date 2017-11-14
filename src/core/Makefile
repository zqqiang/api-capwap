TOPDIR = ../..

OBJS = cw.o cw_string.o cw_cycle.o cw_log.o cw_module.o cw_connection.o cw_palloc.o cw_conf_file.o cw_array.o
EXTRA_CFLAGS = -DCW_LINUX -I. -I../os/unix -I../event
TARGET_O_CP = libcore.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf *.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make