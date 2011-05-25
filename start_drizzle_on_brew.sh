#!/bin/sh

mkdir tmp
exec /usr/local/sbin/drizzled --no-defaults --datadir=$PWD/tmp
