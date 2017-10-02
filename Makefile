TOPDIR=.

SUBDIRS:=src/core demo

all: subdirs

clean: subdirs
	@-rm -f *.o core *.a *~ apsimg.out lib/* obj/*

.PHONY: all

include $(TOPDIR)/rules.Make

install:
	echo "todo install"