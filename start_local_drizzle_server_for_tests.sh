#!/bin/sh

export PATH=$PATH:/usr/sbin:/usr/local/sbin

DATABASE=$PWD/tmp/database
rm -rf $DATABASE
mkdir -p $DATABASE
drizzled --no-defaults --datadir=$DATABASE -d --mysql-unix-socket-protocol.path "$DATABASE/mysql.socket"

echo Creating a new test database
echo "create database test;" | drizzle -u $USER
drizzle -u $USER test < spec/bootstrap.sql
