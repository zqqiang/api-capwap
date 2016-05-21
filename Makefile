TOPDIR=.

SUBDIRS:=capwap app demo

all: subdirs

.PHONY: all

include $(TOPDIR)/rules.Make