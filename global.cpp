#include "global.h"

//QString version = "0.1";
//int     current_account = 0;
//int     current_currency;
QLocale *default_locale;
//QLocale *english;
//QFont   fnt;

int Globals::account = 0;
int Globals::currency = 0;
int Globals::correct_account = 0;
int Globals::initial_account = 0;
int Globals::precision = 2;
double Globals::kurs = 1;
QString Globals::symbol = "";
QString Globals::list_font = "";
QString Globals::version = "0.1";
bool Globals::database_opened = false;
