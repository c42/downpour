drop table if exists Test1;
create table Test1 (
  name varchar(20)
);
insert into Test1 values ('foo');
insert into Test1 values ('bar');
insert into Test1 values ('baz');

drop table if exists Test2;
create table Test2 (
  id int auto_increment primary key,
  name varchar(20)
);
