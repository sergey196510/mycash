PRAGMA foreign_keys=OFF
BEGIN TRANSACTION
CREATE TABLE account_type(id integer primary key autoincrement, name text)
INSERT INTO "account_type" VALUES(1,'Актив')
INSERT INTO "account_type" VALUES(2,'Пассив')
INSERT INTO "account_type" VALUES(3,'Доход')
INSERT INTO "account_type" VALUES(4,'Расход')
CREATE TABLE account(id integer primary key autoincrement, name text, type integer, balance double, descr text, ccod int default 1, hidden int default 0, parent int default 0)
INSERT INTO "account" VALUES(1,'Начальное сальдо',3,0,NULL,1,'false')
INSERT INTO "account" VALUES(2,'Корректировка',4,0,NULL,1,'false')
CREATE TABLE operation(id integer primary key autoincrement, dt date, acc_from integer, acc_to integer, summ double, descr text, agent int default 0, plan_id int default 0)
CREATE TABLE agent(id integer primary key autoincrement, name text, city text, address text, phone text, contact text)
CREATE TABLE currency(id integer primary key autoincrement, name text, icod int, scod text, kurs double)
INSERT INTO "currency" VALUES(1,'Российский рубль',NULL,'RUB',1.0)
INSERT INTO "currency" VALUES(2,'Доллар США',NULL,'USD',32.2676)
INSERT INTO "currency" VALUES(3,'Евро',NULL,'EUR',43.7645)
CREATE TABLE account_oper(id integer primary key autoincrement, a_id int, o_id int, summ double)
CREATE TABLE plan_oper(id integer primary key autoincrement, day int, month int, year int, acc_from int, acc_to int, summ double, descr text)
DELETE FROM sqlite_sequence
INSERT INTO "sqlite_sequence" VALUES('account_type',4)
INSERT INTO "sqlite_sequence" VALUES('account',1)
INSERT INTO "sqlite_sequence" VALUES('currency',3)
COMMIT
