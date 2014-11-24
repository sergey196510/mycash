#include "global.h"
//#include "database.h"

//QString version = "0.1";
//int     current_account = 0;
//int     current_currency;
QLocale *default_locale;
//Database *db;
//QLocale *english;
//QFont   fnt;

int Globals::account = 0;
int Globals::currency = 0;
int Globals::correct_account = 0;
int Globals::initial_account = 0;
int Globals::precision = 2;

const int Direction::from = 1;
const int Direction::to = 2;

const int Account_Type::active = 1;
const int Account_Type::passive = 2;
const int Account_Type::debet = 3;
const int Account_Type::credit = 4;
const int Account_Type::initial = 6;

const int Plan_Status::actual = 1;
const int Plan_Status::committed = 2;
const int Plan_Status::minimum = 3;
const int Plan_Status::expired = 4;
const int Plan_Status::cancelled = 5;

double Globals::kurs = 1;
QString Globals::symbol = "";
QString Globals::list_font = "";
QString Globals::version = "0.1";
bool Globals::database_opened = false;

int Globals::proxy_enable = 0;
QString Globals::proxy_server = "";
int Globals::proxy_port = 0;
QString Globals::proxy_user = "";
QString Globals::proxy_password = "";
