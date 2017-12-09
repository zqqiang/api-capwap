#!/bin/bash

make clean;

./configure --conf-path=./conf/cw.conf \
            --with-zlog=../zlog;

make;