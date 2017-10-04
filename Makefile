TOPDIR=.

SUBDIRS:=src demo

all: subdirs

clean: subdirs
	@-rm -rf *.o *.a *~ apsimg.out lib/* obj

.PHONY: all

include $(TOPDIR)/rules.Make

install:
	echo "todo install"