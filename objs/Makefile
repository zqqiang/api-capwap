TOPDIR = ..

OBJS = cw_modules.o
EXTRA_CFLAGS = -DCW_LINUX -I../src/core -I../src/os/unix
TARGET_O_CP = libobj.o
OBJECTS_O = $(OBJS)

all: $(TARGET_O_CP)

clean:
	-rm -rf *.o *.a *~ $(TARGET_O_CP) .deps

include $(TOPDIR)/rules.Make