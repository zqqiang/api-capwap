TOPDIR=.

SUBDIRS:=obj src

all: subdirs

clean: subdirs
	@-rm -rf *.o *.a *~ apsimg.out lib/*

.PHONY: all

include $(TOPDIR)/rules.Make

install:
	echo "todo install"