#!/bin/sh
.PHONY: all clean
all:
	cd ./Server/; make
	cd ./Monitor.qt4/; make
	cd ./Agent/baj/; make
clean:
	cd ./Server/; make clean
	cd ./Monitor.qt4/; make clean
	cd ./Agent/baj/; make clean
