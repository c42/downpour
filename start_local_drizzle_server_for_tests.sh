#!/bin/sh

export PATH=$PATH:/usr/sbin:/usr/local/sbin

rm -rf tmp/database
mkdir -p tmp/database
drizzled --no-defaults --datadir=$PWD/tmp/database -d

echo Creating a new test database
echo "create database test;" | drizzle -u $USER
drizzle -u $USER test < spec/bootstrap.sql
