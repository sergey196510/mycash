PRAGMA foreign_keys=ON;
BEGIN TRANSACTION;
CREATE TABLE account_type(id integer primary key autoincrement, name text);
INSERT INTO "account_type" VALUES(1,'Наличные');
INSERT INTO "account_type" VALUES(2,'Дебетовая карта');
INSERT INTO "account_type" VALUES(3,'Кредитная карта');
INSERT INTO "account_type" VALUES(4,'Долги');
CREATE TABLE account(id integer primary key autoincrement, name text, type integer, balance real, descr text, ccod int default 1, hidden int default 0, parent int default 0, top int, system int default 0, dt date);
INSERT INTO "account" VALUES(1,'Актив',0,0,'',1,0,0,1,1,'');
INSERT INTO "account" VALUES(2,'Пассив',0,0,'',1,0,0,2,1,'');
INSERT INTO "account" VALUES(3,'Доход',0,0,'',1,0,0,3,1,'');
INSERT INTO "account" VALUES(4,'Расход',0,0,'',1,0,0,4,1,'');
INSERT INTO "account" VALUES(5,'Корректировка',4,0,'',1,0,4,1,1,'');
INSERT INTO "account" VALUES(6,'Собственные средства',4,0,'',1,1,0,6,1,'');
INSERT INTO "account" VALUES(7,'Начальное сальдо',5,0.0,'',1,1,6,6,1,'');
CREATE TABLE agent(id integer primary key autoincrement, name text, city text, address text, phone text, contact text);
CREATE TABLE currency(id integer primary key autoincrement, name text, icod int, scod text, kurs real);
INSERT INTO "currency" VALUES(1,'Российский рубль',NULL,'RUB',1.0);
INSERT INTO "currency" VALUES(2,'Доллар США',NULL,'USD',32.2676);
INSERT INTO "currency" VALUES(3,'Евро',NULL,'EUR',43.7645);
CREATE TABLE oper(id integer primary key autoincrement, dt date, descr text);
CREATE TABLE account_oper(id integer primary key autoincrement, a_id int, o_id int, summ real, direction int default 0, agent int default 0);
CREATE TABLE plan_oper(id integer primary key autoincrement, day int, month int, year int, auto int default 0, dt date, descr text);
CREATE TABLE plan_oper_acc(id integer primary key autoincrement, a_id int, o_id int, summ real, direction int default 0, agent int default 0);
CREATE TABLE plan_oper_mon(id integer primary key autoincrement, mon int, year int, p_id int, dt date, status int default 0);
CREATE TABLE budget_plan(id integer primary key autoincrement, mon int, a_id int, summ real);
CREATE TABLE budget_fact(id integer primary key autoincrement, year int, mon int, a_id int, summ real);
INSERT INTO "sqlite_sequence" VALUES('account_type',4);
INSERT INTO "sqlite_sequence" VALUES('account',7);
INSERT INTO "sqlite_sequence" VALUES('currency',3);
COMMIT;
