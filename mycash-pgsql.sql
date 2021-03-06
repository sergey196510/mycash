CREATE TABLE account_type(id serial, name text);
INSERT INTO "account_type" VALUES(1,'Наличные');
INSERT INTO "account_type" VALUES(2,'Дебетовая карта');
INSERT INTO "account_type" VALUES(3,'Кредитная карта');
INSERT INTO "account_type" VALUES(4,'Долги');
CREATE TABLE account(id serial, name text, type integer, balance real, descr text, ccod int default 1, hidden int default 0, parent int default 0, top int, system int default 0, dt date);
INSERT INTO "account" VALUES(1,'Актив',0,0,'',1,0,0,1,1,NULL);
INSERT INTO "account" VALUES(2,'Пассив',0,0,'',1,0,0,2,1,NULL);
INSERT INTO "account" VALUES(3,'Доход',0,0,'',1,0,0,3,1,NULL);
INSERT INTO "account" VALUES(4,'Расход',0,0,'',1,0,0,4,1,NULL);
INSERT INTO "account" VALUES(5,'Корректировка',4,0,'',1,0,4,4,1,NULL);
INSERT INTO "account" VALUES(6,'Собственные средства',4,0,'',1,1,0,6,1,NULL);
INSERT INTO "account" VALUES(7,'Начальное сальдо',5,0.0,'',1,1,6,6,1,NULL);
CREATE TABLE agent(id serial, name text, city text, address text, phone text, contact text);
CREATE TABLE currency(id serial, name text, icod int, scod text, kurs real, nominal int);
INSERT INTO "currency" VALUES(1,'Российский рубль',NULL,'RUB',1.0);
INSERT INTO "currency" VALUES(2,'Доллар США',NULL,'USD',32.2676);
INSERT INTO "currency" VALUES(3,'Евро',NULL,'EUR',43.7645);
CREATE TABLE oper(id serial, dt date, descr text);
CREATE TABLE account_oper(id serial, a_id int, o_id int, summ real, direction int default 0, agent int default 0);
CREATE TABLE plan_oper(id serial, day int, month int, year int, auto int default 0, dt date, descr text);
CREATE TABLE plan_oper_acc(id serial, a_id int, o_id int, summ real, direction int default 0, agent int default 0);
CREATE TABLE plan_oper_mon(id serial, mon int, year int, p_id int, dt date, status int default 0);
CREATE TABLE budget_plan(id serial, mon int, a_id int, summ real);
CREATE TABLE budget_fact(id serial, year int, mon int, a_id int, summ real);
CREATE TABLE payment_codif(id serial, name text);
CREATE TABLE payment_real(id serial, type int, p_id int, value real, price real, summ real, dt date);
