/*
=============================================================================
РКК "ЭНЕРГИЯ"          БНОПО       БИБЛИОТЕКА ДЛЯ РАБОТЫ
                                   С ДВУХСТРОЧНЫМИ ЭЛЕМЕНТАМИ (TLE)

ОТДЕЛ 291               ИCПOЛHИTEЛЬ: БОРОВИХИН        ЯЗЫK: C++

-----------------------------------------------------------------------------
БИБЛИОТЕКА libtle                BEPCИЯ 1.0      ДATA COЗДAHИЯ 31.03.2025

ИCПOЛЬЗОВАННЫE MATEPИAЛЫ:  "СПРАВОЧНОЕ РУКОВОДСТВО ПО НЕБЕСНОЙ
                           МЕХАНИКЕ" ПОД РЕДАКЦИЕЙ Г.Н.ДУБОШИНА
                           М., "НАУКА", 1976.
                           "СПРАВОЧНИК ПО АЛГОРИТМАМ И ПРОГРАММАМ
                           НА ЯЗЫКЕ БЕЙСИК ДЛЯ ПЕРСОНАЛЬНЫХ ЭВМ"
                           В.П.ДЬЯКОНОВ
                           М., "НАУКА", 1989.
                           "ОСНОВЫ ТЕОРИИ ПОЛЕТА И ЭЛЕМЕНТЫ
                           ПРОЕКТИРОВАНИЯ ИСКУССТВЕННЫХ СПУТНИКОВ
                           ЗЕМЛИ" ПОД РЕДАКЦИЕЙ М.К.ТИХОНРАВОВА
                           М., "МАШИНОСТРОЕНИЕ", 1967.
                           "АСТРОДИНАМИКА" В ТРЕХ ТОМАХ
                           М., "МИР", 1976,1977,1978.
                           МАТЕРИАЛЫ ПО ДВУХСТРОЧНЫМ ЭЛЕМЕНТАМ
                           НА САЙТЕ HTTP://WWW.CELESTRAK.COM.
                           ИСХОДНЫЕ ТЕКСТЫ РАЗРАБОТАННЫХ РАНЕЕ
                           БАЛЛИСТИЧЕСКИХ ПРОГРАММ НА ЯЗЫКАХ
                           PASCAL И PL1.

-----------------------------------------------------------------------------

*/


#include "libtle.h"

Libtle::Libtle()
{
}


bool Libtle::read_tle(string strDataFile, string sat_data[2], string* NAME_TLE)
/*
Копирование содержимого заданного файла с двухстрочными элементами
в строковые переменные (с целью в дальнейшем вычислить по ним
вектор состояния).
Исходный файл должен состоять (как минимум) из трех строк.
Дальнейшие строки файла не читаются.
Первая строка - название для двухстрочных элементов
(как правило, название КА).
Вторая строка - строка 1 TLE.
Третья строка - строка 2 TLE.
Строка названия TLE копируется в строку NAME_TLE.
Строки TLE, содержащие данные, переписываются в одномерный массив sat_data длиной 2.
Если не удается открыть файл с заданным именем или прочесть из него строки,
создается файл READ_TLE_ERROR и в него записывается сообщение об ошибке.
Проверка формата читаемых строк не производится.
*/
{

ifstream fread;
ofstream ferror, fwrite;
string strELEM0, strELEM1, strELEM2;

fread.open(strDataFile, ios::in);

if (fread.fail())
{
    ferror.open("READ_TLE_ERROR", ios::out);
    ferror << "open file error" << endl;
    ferror.close();
    return false;
}

// fwrite.open("copy.tle", ios::out); // в целях отладки создавалась копия файла TLE

if (!getline(fread, strELEM0))
{ferror.open("READ_TLE_ERROR", ios::out);
    ferror << "read line 0 error" << endl;
    ferror.close();
    return false;
}

// fwrite << strELEM0 << endl; // запись в копию файла TLE

if (!getline(fread, strELEM1))
{ferror.open("READ_TLE_ERROR", ios::out);
    ferror << "read line 1 error" << endl;
    ferror.close();
    return false;
}

// fwrite << strELEM1 << endl; // запись в копию файла TLE

if (!getline(fread, strELEM2))
{ferror.open("READ_TLE_ERROR", ios::out);
    ferror << "read line 2 error" << endl;
    ferror.close();
    return false;
}

// fwrite << strELEM2 << endl;  // запись в копию файла TLE

fread.close();

// fwrite.close(); // закрытие копии файла TLE

//Копирование прочитанных данных (подготовка к вычислению вектора состояния)

  *NAME_TLE = strELEM0;
  sat_data[0] = strELEM1;
  sat_data[1] = strELEM2;

return true;

} // read_tle


int Libtle::read_date_arr(string StrDate, string StrMark, int int3Date[3])
/*
Данная функция предназначена для чтения из строки StrDate даты,
которая должна задаваться в виде двузначных чисел, первое из которых
означает год, второе - месяц, третье - день.
Годы, месяцы и дни должны разделяться строками StrMark (в таком качестве
рекомендуется выбирать одиночный символ, например ":").
Таким образом, ИСХОДНАЯ СТРОКА ДОЛЖНА ИМЕТЬ ВИД: "25:07:30"
Полученные значения года, месяца и дня заносятся подпрограммой
в разные ячейки целочисленного массива int3Date.
При отсутствии ошибок функция возвращает 0.
При некорректной входной строке функция возвращает отличное от нуля
значение, зависящее от того, на каком этапе возникла ошибка.
*/
{
string StrBuf;
int N, ind, intReturn, LengthStrMark;

intReturn = 0; //значение 0 будет возвращено после штатной работы подпрограммы
StrBuf = StrDate; // первоначально буферная строка совпадает с исходной
LengthStrMark = StrMark.length();  //маркер рекомендуется обозначать одним символом,
                                   //но, в принципе, он может быть и длиннее

// ЦИКЛ РАЗБОРА СТРОКИ, В КОТОРОЙ ЗАДАНА ДАТА:

N = 0;
while (N<=2)
{

if (N<2)
   {
    ind = StrBuf.find(StrMark);
    if (ind == -1) {intReturn = N+1; N = 10; } //если маркер не найден, по возвращаемому значению N+1
                                             //можно понять, на каком шаге это произошло;
                                             //N=10 - для того, чтобы выйти из цикла while(N<=3)
    else if (ind != 2) {intReturn = 10*(N+1); N = 10; }  //если маркер найден не через два символа
                                             // от текущей позиции, по возвращаемому значению 10*(N+1)
                                             // можно понять, на каком шаге нарушился формат даты
         else {int3Date[N] = stoi(StrBuf.substr(0,2)); StrBuf.erase(0,(2+LengthStrMark)); }
                        //записывается в целочисленный массив очередная компонента формата даты,
                        //из буферной строки удаляется эта компонента и следующий за ней маркер
   }
else //текущее значение N равно 2
   {if ( StrBuf.length() != 2) {intReturn = 10*(N+1); N = 10; } //по возвращаемому значению можно понять, что здесь
                                              //обнаружено нарушение формата даты
    else { int3Date[N] = stoi(StrBuf.substr(0,2)); }
    }

N = N + 1;
}

return intReturn;

} // read_date_arr

struct_date Libtle::read_date(string StrDate, int p_y, int l_y, int p_m, int p_d)
/*
Данная функция аналогична функции read_date_arr, но отличается от нее тем,
что записывает дату не в массив, а в структуру, причем структура
является возвращаемым значением функции.
*/
{
struct_date dat;

dat.year = Libtle::Integer_Value(StrDate, p_y, l_y);
dat.month = Libtle::Integer_Value(StrDate, p_m, 2);
dat.day = Libtle::Integer_Value(StrDate, p_d, 2);

return dat;
} // read_date

void Libtle::read_date_arr_alt(string StrDate, int p_y, int l_y, int p_m, int p_d, int int3Date[3])
/*
Чтение даты из заданной строки StrDate в целочисленный массив.
Значение года читается с позиции p_y, длина представления года - l_y,
обычно значение l_y равно 2 или 4.
Значение месяца читается с позиции p_m, длина представления месяца - 2,
Значение дня читается с позиции p_m, длина представления дня - 2.
*/
{

int3Date[0] = Libtle::Integer_Value(StrDate, p_y, l_y);
int3Date[1] = Libtle::Integer_Value(StrDate, p_m, 2);
int3Date[2] = Libtle::Integer_Value(StrDate, p_d, 2);

} // read_date_arr_alt

int Libtle::read_time_arr(string StrTime, string StrMark, int int4Time[4])
/*
Данная функция предназначена для чтения из строки StrTime времени,
которое должно задаваться в виде двузначных чисел, первое из которых
означает час, второе - минуту, третье - секунду, четвертое - сотые доли
секунды. При этом часы от минут, минуты от секунд и секунды от сотых
долей должны отделяться разделителем StrMark (в таком качестве рекомендуется
выбирать одиночный символ, например ":").
Таким образом, ИСХОДНАЯ СТРОКА ДОЛЖНА ИМЕТЬ ВИД: "23:14:25:50"
Полученные часы, минуты, секунды и сотые доли заносятся подпрограммой
в разные ячейки целочисленного массива int4Time.
При отсутствии ошибок функция возвращает 0.
При некорректной входной строке функция возвращает отличное от нуля
значение, зависящее от того, на каком этапе возникла ошибка.
*/
{
string StrBuf;
int N, ind, intReturn, LengthStrMark;

intReturn = 0; //значение 0 будет возвращено после штатной работы подпрограммы
StrBuf = StrTime; // первоначально буферная строка совпадает с исходной
LengthStrMark = StrMark.length();  //маркер рекомендуется обозначать одним символом,
                                   //но, в принципе, он может быть и длиннее

// ЦИКЛ РАЗБОРА СТРОКИ, В КОТОРОЙ ЗАДАНО ВРЕМЯ:

N = 0;
while (N<=3)
{

if (N<2)
   {
    ind = StrBuf.find(StrMark);
    if (ind == -1) {intReturn = N+1; N = 10; } //если маркер не найден, по возвращаемому значению N+1
                                             //можно понять, на каком шаге это произошло;
                                             //N=10 - для того, чтобы выйти из цикла while(N<=3)
    else if (ind != 2) {intReturn = 10*(N+1); N = 10; }  //если маркер найден не через два символа
                                             // от текущей позиции, по возвращаемому значению 10*(N+1)
                                             // можно понять, на каком шаге нарушился формат времени
         else {int4Time[N] = stoi(StrBuf.substr(0,2)); StrBuf.erase(0,(2+LengthStrMark)); }
                        //записывается в целочисленный массив очередная компонента формата времени,
                        //из буферной строки удаляется эта компонента и следующий за ней маркер
   }
else //текущее значение N равно 2 или 3
   {if (N==2) {
        ind = StrBuf.find(".");
        if (ind == -1) {intReturn = N+1; N = 10; }
        else if (ind != 2) {intReturn = 10*(N+1); N = 10; }
             else {int4Time[N] = stoi(StrBuf.substr(0,2)); StrBuf.erase(0,(2+LengthStrMark)); }
              }
    else
              {
    if ( StrBuf.length() != 3) {intReturn = 10*(N+1); N = 10; } //по возвращаемому значению можно понять, что здесь
                                              //обнаружено нарушение формата времени
    else { int4Time[N] = stoi(StrBuf.substr(0,3)); }
               }
   }

N = N + 1;
}

return intReturn;

} // read_time_arr


struct_time Libtle::read_time(string StrTime, int p_h, int p_m, int p_s, int p_ss, int l_ss)
/*
Данная функция аналогична функции read_time_arr, но отличается от нее тем,
что записывает дату не в массив, а в структуру, причем структура
является возвращаемым значением функции.
*/
{
string StrBUF;
double doubleBUF;
struct_time tim;

tim.hour = Libtle::Real_Value(StrTime, p_h, 2);
tim.min = Libtle::Real_Value(StrTime, p_m, 2);
tim.sec = Libtle::Real_Value(StrTime, p_s, 2);
StrBUF = "0."+StrTime.substr(p_ss,l_ss);
doubleBUF = Libtle::Real_Value(StrBUF, 0, StrBUF.length());
tim.sec = tim.sec + doubleBUF;

return tim;
} // read_time

void Libtle::read_time_arr_alt(string StrTime, int p_h, int p_m, int p_s, int p_ss, int l_ss, int int4Time[4])
/*
Чтение времени из заданной строки StrTime в целочисленный массив.
Значение часа читается с позиции p_h, длина представления часа - 2,
Значение минуты читается с позиции p_m, длина представления минуты - 2,
Значение секунды читается с позиции p_s, длина представления секунды - 2.
Значение долей секунды читается с позиции p_ss, длина представления долей секунды - l_ss,
обычно значение l_ss равно 3 или 2.
*/
{

int4Time[0] = Libtle::Integer_Value(StrTime, p_h, 2);
int4Time[1] = Libtle::Integer_Value(StrTime, p_m, 2);
int4Time[2] = Libtle::Integer_Value(StrTime, p_s, 2);
int4Time[3] = Libtle::Integer_Value(StrTime, p_ss,l_ss);

} // read_time_arr_alt

int Libtle::Date_to_Arr(int Y, int M, int D, int int3Date[3])
/*
Функция для облегчения ввода значений даты в массив.
Первые три параметра - вводимые значения лет, месяцев и дней.
Последний параметр - тот массив, в который будут переписаны значения остальных параметров.
*/
  {

    if ((Y > 2050) || (Y < 1950)) return 1;
    if ((M < 1) || (M > 12)) return 2;
    if ((D < 1) || (D > 31)) return 3;

    int3Date[0] = Y;     int3Date[1] = M;    int3Date[2] = D;

    return 0;

  } // Date_to_Arr

int Libtle::Time_to_Arr(int H, int M, int S, int SS, int int4Time[4])
/*
Функция для облегчения ввода значений времени в массив.
Первые четыре параметра - вводимые значения часов, минут, секунд и долей секунд.
Последний параметр - тот массив, в который будут переписаны значения остальных параметров.
В каких единицах задаются доли секунд, надо помнить, чтобы правильно интерпретировать данные в дальнейшем.
Обычно задавались миллисекунды, то есть 1/1000 секунды.
*/

  {

   if ((H < 0) || (H > 23)) return 1;
   if ((M < 0) || (M > 59)) return 2;
   if ((S < 0) || (S > 59)) return 3;
   if (SS < 0) return 4;

   int4Time[0] = H;    int4Time[1] = M;    int4Time[2] = S;    int4Time[3] = SS;

   return 0;

  } // Time_to_Arr

int Libtle::Arr_to_Date(int int3Date[3], int Y, int M, int D)
/*
Функция для облегчения вывода значений даты из массива.
Первый параметр - тот массив, из которого будут переписаны значения остальных параметров.
Следующие три параметра - копируемые из массива значения лет, месяцев и дней.

*/
  {
    Y = int3Date[0];     M = int3Date[1];    D = int3Date[2];

    if ((Y > 2050) || (Y < 1950)) return 1;
    if ((M < 1) || (M > 12)) return 2;
    if ((D < 1) || (D > 31)) return 3;

    return 0;

  } // Arr_to_Date

int Libtle::Arr_to_Time(int int4Time[4], int H, int M, int S, int SS)
/*
Функция для облегчения вывода значений времени из массива.
Первый параметр - тот массив из которого будут переписаны значения остальных параметров.
Следующие четыре параметра - вводимые значения часов, минут, секунд и долей секунд.

В каких единицах задаются доли секунд, надо помнить, чтобы правильно интерпретировать данные в дальнейшем.
Обычно задавались миллисекунды, то есть 1/1000 секунды.
*/

  {

    H = int4Time[0];     M = int4Time[1];     S = int4Time[2];     SS = int4Time[3];

   if ((H < 0) || (H > 23)) return 1;
   if ((M < 0) || (M > 59)) return 2;
   if ((S < 0) || (S > 59)) return 3;
   if (SS < 0) return 4;

   return 0;

  } // Arr_to_Time


string Libtle::Date_to_Str_arr(int int3Date[3])
/*
Преобразование даты, заданной в целочисленном массиве, в строковое представление.
*/

  {
    stringstream ss;
    string SBuf;

    ss <<  setw(2) << setfill('0') << int3Date[0] << ":" <<  setw(2) << setfill('0') << int3Date[1] << ":";
    ss << setw(2) << setfill('0') << int3Date[2];
    ss >> SBuf;
    ss.str("");

    return SBuf;

  } // Date_to_Str_arr

string Libtle::Time_to_Str_arr(int int4Time[4])
/*
Преобразование времени, заданного в целочисленном массиве, в строковое представление.
*/
  {
    stringstream ss;
    string SBuf;

    ss <<  setw(2) << setfill('0') << int4Time[0] << ":" <<  setw(2) << setfill('0') << int4Time[1] << ":";
    ss << setw(2) << setfill('0') << int4Time[2] << "." <<  setw(3) << setfill('0') << int4Time[3];
    ss >> SBuf;
    ss.str("");

    return SBuf;

  } // Time_to_Str_arr

void Libtle::Curr_Date_Time_Loc(int int3Date[3], int int4Time[4])
/*
Занесение текущих даты и времени (местных) в массивы
*/
  {
    QDate DATE; DATE = DATE.currentDate();
    QTime TIME; TIME = TIME.currentTime();

        int3Date[0] = DATE.year();  int3Date[1] = DATE.month(); int3Date[2] =  DATE.day();
    int4Time[0] = TIME.hour(); int4Time[1] = TIME.minute(); int4Time[2] = TIME.second(); int4Time[0] = TIME.msec();

  } // Curr_Date_Time_Loc

void Libtle::Curr_Date_Time_UTC(int int3Date[3], int int4Time[4])
/*
Занесение текущих даты и времени (гринвичского) в массивы
*/
  {
    int intBuf;
    QDateTime DATE_TIME; DATE_TIME = DATE_TIME.currentDateTimeUtc();

    QDate DATE; DATE = DATE_TIME.date();
    QTime TIME; TIME = DATE_TIME.time();

    int3Date[0] = DATE.year();  int3Date[1] = DATE.month(); int3Date[2] =  DATE.day();
    int4Time[0] = TIME.hour(); int4Time[1] = TIME.minute(); int4Time[2] = TIME.second(); int4Time[3] = TIME.msec();

  } // Curr_Date_Time_UTC

void Libtle::Convert_Blanks(string* field)
/*
Замена пробелов в строке нулями.
*/
  {
    int i;
    char ch1, ch2;
    ch1 = ' ';
    ch2 = '0';
    for (i=(field->length()-1);i>=0;i--) { if (field->at(i) == ch1) {field[i] = ch2;} };
    //replace(field->begin,field->end,ch1,ch)
  } // Convert_Blanks

int Libtle::Integer_Value(string buffer, int start, int leng)
/*
Функция преобразовывает в целое число часть строки buffer.
Эта часть начинается с позиции start и имеет длину leng
*/
 {
   int answer;
   //int result1;
   string buf;

  buf = buffer.substr(start, leng); // Copy(buffer,start,leng);

  std::replace(buf.begin(),buf.end(),' ','0');  //вместо Libtle::Convert_Blanks(&buf);
  answer = stoi(buf.substr(0,buf.length())); // Val(buffer,answer,result1);
  // if result1 = 0 then Integer_Value := answer else Integer_Value := 0;
  return answer;
 } // Integer_Value


double Libtle::Real_Value(string buffer, int start, int leng)
/*
Функция преобразовывает в действительное число часть строки buffer.
Эта часть начинается с позиции start и имеет длину leng
*/
{
  //int result1;
  double answer;
  string buf;

  buf = buffer.substr(start,leng); // Copy(buffer,start,length);
//  std::cout  << "buf: " << buf << endl;

  std::replace(buf.begin(),buf.end(),' ','0');  //вместо Libtle::Convert_Blanks(&buf);
//  std::cout  << "buf1 " << buf << endl;
  if (buf == "") { buf = "0"; };
  answer = stod(buf.substr(0,buf.length())); // Val(buffer,answer,result1);
  // if (result1 == 0) { return answer; } else { return 0.0; };
  return answer;
 } // Real_Value}

   double Libtle::lambda_360_to_180(double lam360)
/*
Функция предназначена для перевода положительной географической долготы,
которая отсчитывается от 0 до 360 градусов, в долготу, лежащую
в диапазоне от -180 до +180 градусов.
Если исходная долгота превышает 360 градусов, она возвращается без изменений.
В случае, когда исходная долгота отрицательна, никаких действий не производится!!!!!!!!!
*/
  {
   double lam180;

   if (lam360>180.0) { lam180 = lam360 - 360.0; }
   else { lam180 = lam360; };

   if (lam360>360) { lam180 = lam360; };

   return lam180;

   } // lambda_360_to_180


double Libtle::AcTan(double sinx, double cosx)
/*
Дополнительная функция для расчета арктангенса
*/
{
    if (cosx == 0) { if (sinx > 0) {return (const_PI/2);} else { return (3*const_PI/2);} }
    else { if (cosx > 0) {return  atan(sinx/cosx);} else { return (const_PI + atan(sinx/cosx)); } }; //ArcTan
} //Actan

double Libtle::norm_angl(double A)
/*
"Нормализация" угла (в радианах), т.е.приведение его к диапазону (-пи, +пи)
*/
{
double R;

R = A;

while (!( (R<=const_PI) && (R>=-const_PI) ) )
{
  if(R>=0) {R = R - const_TWOPI;}
          else R= R + const_TWOPI;
 }
return R;
} // norm_angl


double Libtle::norm_angl_pos(double A)
/*
"Нормализация" угла (в радианах) положительная, т.е.приведение его к положительному диапазону (0, +2*пи)
*/
{
 double   R;
 R = A;
while ( !( (R<=const_TWOPI) && (R>=0) ) )
{
  if (R>=0) {R = R - const_TWOPI;} else { R= R + const_TWOPI;};
}
return R;
} // norm_angl_pos

double Libtle::rad_to_grad(double A)
// Перевод радианов в градусы
{
 return (A*180.0/const_PI);
} //rad_to_grad

double Libtle::grad_to_rad(double A)
// Перевод градусов в радианы
{
 return (A*const_PI/180.0);
} //grad_to_rad

double Libtle::TANGENS(double A)
// Дополнительная функция тангенса
{
  return (sin(A)/cos(A));
} // TANGENS}

double Libtle::ARCSIN(double A)
// Дополнительная функция арксинуса
{
    if (abs(A) >= 1) { return (Libbase::sign(A)*const_PI/2);} else {return atan(A/sqrt(1-A*A));}; // ARCTAN
} //ARCSIN

double Libtle::ARCCOS(double A)
// Дополнительная функция арккосинуса
{
  return (const_PI/2 - Libtle::ARCSIN(A));
} // ARCCOS

string Libtle::Chcksm(string line)
/*
Данная функция возвращает контрольную сумму для заданной строки,
вычисленную по правилам, установленным для двухстрочных элементов.}
*/
{
  int i,checksum;
  stringstream ss;
  string SBuf;

  checksum = 0;
  for (i=0; i<68; i++)
  {
    switch (line[i])
    {
        case '0':
            checksum = checksum + 0;
            break;
        case '1':
            checksum = checksum + 1;
            break;
        case '2':
            checksum = checksum + 2;
            break;
        case '3':
            checksum = checksum + 3;
            break;
        case '4':
            checksum = checksum + 4;
            break;
        case '5':
            checksum = checksum + 5;
            break;
        case '6':
            checksum = checksum + 6;
            break;
        case '7':
            checksum = checksum + 7;
            break;
        case '8':
            checksum = checksum + 8;
            break;
        case '9':
            checksum = checksum + 9;
            break;
        case '-':
            checksum = checksum + 1;
            break;
     }
   };
  checksum = checksum % 10;

  ss <<  setprecision(1) << checksum;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ss >> SBuf;
  ss.str("");

  return SBuf;
 } // Chcksm

string Libtle::Replace(string line, string A, string B)
/*
Данная функция возвращает строку, которая получается из строки
line путем замены всех символов A на символы B.
*/
{
  string BufLine;
  int   i;

  BufLine = line;
  for (i=0;i<int(BufLine.length());i++) { if (BufLine.substr(i,1)==A) {BufLine.insert(i,B); BufLine.erase((i+1),1); }; };

  return BufLine;

 } // Replace

string Libtle::Double_to_Str(double d_Var, int i_width, int i_prec)
/*
Данная функция возвращает строку, в которую преобразовано заданное действительное число.
Ширина представления - i_width, число знаков после точки - i_prec.
*/
{
    stringstream ss;
    string SBuf;

//STR(R_INCLIN:8:4,StrBuf);
  ss << fixed << right <<  setprecision(i_prec) << d_Var;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ss >> SBuf;
  if ((i_width - SBuf.length()) > 0) {SBuf.insert(0,(i_width - SBuf.length()),' ');};
  //while ((i_width - SBuf.length()) > 0) {SBuf = " " + SBuf;};
  ss.str("");  // ss.str(std::struing()); swap ???????
  return SBuf;
}

string Libtle::Int_to_Str(int i_Var, int i_width)
/*
Данная функция возвращает строку, в которую преобразовано заданное целое число.
Ширина представления - i_width.
*/

{
  stringstream ss;
  string SBuf;
  //  STR(ROUND(R_EXCENTR):7,StrBuf);
  ss << right <<  setprecision(i_width) << i_Var;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ss >> SBuf;
  if ((i_width - SBuf.length()) > 0) {SBuf.insert(0,(i_width - SBuf.length()),'0');};
  //while ((i_width - SBuf.length()) > 0) {SBuf = "0" + SBuf;};
  ss.str("");  // ss.str(std::struing()); swap ???????
  return SBuf;
}

void Libtle::CalcVect(string sat_data[2], double poss[3], double vell[3])
/*
Функция для расчета вектора (в инерциальной системе), исходя из двух строк TLE.
*/
{
      string two_line[2]; //array [1..2] of line_data; line_data = string[69];
      string catnr,elset;
      string obs_name; //obs_name             : string[25];
      string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
      //string sat_data[const_max_sats][2]; // array [1..max_sats] of two_line;
      string data_drive, data_dir, work_drive, work_dir;

      double julian_epoch;

      string eNRev;

      double RV[3],SS[3],A0[14];

      string abuf[2];


    //Подготовка данных для расчета векторов

      Libtle::PreparePrognSGP4(sat_data, &julian_epoch, A0);

    // Расчет векторов координат и скорости в инерциальной системе для момента эпохи TLE

      Libbase::PrognSGP4 (RV, SS, 0.0, A0);

        poss[0] = RV[0];
        poss[1] = RV[1];
        poss[2] = RV[2];
        vell[0] = SS[0];
        vell[1] = SS[1];
        vell[2] = SS[2];

} // CalcVect

void Libtle::PreparePrognSGP4(string sat_data[2], double* j_epoch, double A0[14])
/*
Данная процедура, исходя из двух строк TLE, заданных в массиве sat_data,
готовит в ячейках массива A0 параметры для последующей передачи в процедуру
баллистического прогноза PrognSGP4, а также заносит в переменную j_epoch
юлианскую дату эпохи двухстрочных элементов.
Часть ячеек массива A0 осталась зарезервированной.
*/
{    
      string two_line[2]; //array [1..2] of line_data; line_data = string[69];
      double epoch;
      string catnr,elset;
      string obs_name; //obs_name             : string[25];
      string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
      //string sat_data[const_max_sats][2]; // array [1..max_sats] of two_line;
      string data_drive, data_dir, work_drive, work_dir, eNRev;
      double xmo,xnodeo,omegao,eo,xincl,xno,xndt2o,xndd6o,bstar,julian_epoch,
             INCLIN, EXCENTR, MANOMAL, ANODE, ARGPERIG, MMOTION;
      double A001,A002,A003,A004,A005,A006,A007,A008,A009,A010,A011,A012,A013,A014;
      int iexp,ibexp;
      string abuf[2]; //two_line;

      abuf[0] = sat_data[0];
    // РАЗБОР ПЕРВОЙ СТРОКИ
      catnr    = abuf[0].substr(2,5); // номер спутника в каталоге НОРАД
      epoch    = Libtle::Real_Value(abuf[0],18,14); // эпоха
      //cout << "epoch: " << epoch << endl;
      epoch = stod(abuf[0].substr(18,14));
      julian_epoch = Libbase::Julian_Date_of_Epoch(epoch); // юлианская дата эпохи
      xndt2o   = Libtle::Real_Value(abuf[0],33,10); // первая производная среднего движения по времени
      xndd6o   = Libtle::Real_Value(abuf[0],44,6)*1E-5; // вторая производная среднего движения по времени
      iexp     = Libtle::Integer_Value(abuf[0],50,2); // "экспоненциальная" часть второй производной
      bstar    = Libtle::Real_Value(abuf[0],53,6)*1E-5; // коэффициент торможения в атмосфере
      ibexp    = Libtle::Integer_Value(abuf[0],59,2); // "экспоненциальная" часть коэффициента торможения
      elset    = abuf[0].substr(65,3); // номер элемента


    abuf[1] = sat_data[1];
    // РАЗБОР ВТОРОЙ СТРОКИ
      xincl    = Libtle::Real_Value(abuf[1],8,8); // наклонение орбиты в градусах
      xnodeo   = Libtle::Real_Value(abuf[1],17,8); // прямое восхождение восходящего узла в градусах
      eo       = Libtle::Real_Value(abuf[1],26,7)*1E-7; // эксцентриситет
      omegao   = Libtle::Real_Value(abuf[1],34,8); // аргумент перигея в градусах
      xmo      = Libtle::Real_Value(abuf[1],43,8); // средняя аномалия в градусах
      xno      = Libtle::Real_Value(abuf[1],52,11); // среднее движение (обороты в сутки)
    // period   := 1/xno;

    // ПЕРЕВОД В НУЖНЫЕ ЕДИНИЦЫ ИЗМЕРЕНИЯ
      xndd6o   = xndd6o*pow(10.0,iexp); //
      bstar    = bstar*pow(10.0,ibexp)/const_ae;  //
      //cout << "bstar: " << bstar << endl;
      xnodeo   = Libtle::grad_to_rad(xnodeo);  //
      omegao   = Libtle::grad_to_rad(omegao); //
      xmo      = Libtle::grad_to_rad(xmo); //
      xincl    = Libtle::grad_to_rad(xincl); //
      xno      = xno*const_TWOPI/const_xmnpda; //
      if (const_xmnpda < 0.0) Libbase::write_to_file("POW_const_xmnpda","const_xmnpda < 0.0");
      xndt2o   = xndt2o*const_TWOPI/std::pow(const_xmnpda, 2.0); // sqr(const_xmnpda)
      xndd6o   = xndd6o*const_TWOPI/std::pow(const_xmnpda,3.0); //


      *j_epoch = julian_epoch;

        INCLIN = xincl;
        ANODE = xnodeo;
        EXCENTR = eo;
        ARGPERIG = omegao;
        MANOMAL = xmo;
        //xno      := MMOTION*twopi/xmnpda;
        MMOTION = xno*const_xmnpda/const_TWOPI;

      //  t := (time_m - julian_epoch) * xmnpda;
      //  t := (time_m - julian_epoch) * 86400.0;
      // t := 0.0;
         A001 = 2000.0000000;

         //VAL(eEpoch,A002,code);

         A003 = 0.0003000;
         A004 = 0.0000000;
         A005 = bstar;
         A006 = 0.0000000;
         A007 = 900.0000000;
         A008 = INCLIN;
         A009 = ANODE;
         A010 = EXCENTR;
         A011 = ARGPERIG;
         A012 = MANOMAL;
         A013 = MMOTION;
      //   VAL(eNRev+'.0',A014,code);
         A002 = 1.0; //не используется
         A014 = 1.0; //не используется

      A0[0]=A001; A0[1]=A002; A0[2]=A003; A0[3]=A004; A0[4]=A005;
      A0[5]=A006; A0[6]=A007; A0[7]=A008; A0[8]=A009; A0[9]=A010;
      A0[10]=A011; A0[11]=A012; A0[12]=A013; A0[13]=A014;

} // PreparePrognSGP4

string Libtle::form_string_1(string NSAT, string INTDESIGN, string TEPOCHSTR,
           string EFIRST, string ESECOND, string STRBSTAR, string NELEM)
// Формирование первой строки TLE
{
    string eNofSat, eClsfct, eIntDes, eEpoch, eFirst, eSecond,
    eBstar, eTypeE, eNofEl, eCheck, SBuf;

    //ПОДГОТОВКА СТРОКОВЫХ ПЕРЕМЕННЫХ ДЛЯ ФОРМИРОВАНИЯ СТРОКИ 1

    if (NSAT.length() == 5) {eNofSat = NSAT;} else { return "(NSAT.length() != 5)"; };

    eClsfct = "U";
    if (INTDESIGN.length() == 8) { eIntDes = INTDESIGN; } else { return "(INTDESIGN.length() != 8)"; };
    // eIntDes - международное обозначение (8 символов)

    //STR(TEPOCH:14:8,StrBuf);
    //eEpoch := StrBuf;

    eEpoch = TEPOCHSTR;

    if (TEPOCHSTR.length() == 14) { eEpoch = TEPOCHSTR; } else { return "(TEPOCHSTR.length() != 14)"; };

    //ЗАДАЮТСЯ ФИКСИРОВАННЫЕ ЗНАЧЕНИЯ, ПРЕДСТАВЛЯЮЩИЕ ПЕРВУЮ И ВТОРУЮ ПРОИЗВОДНЫЕ,
    //КОТОРЫЕ ЗАНОСЯТСЯ В СТРОКУ, НО ПРИ ВЫЧИСЛЕНИЯХ НЕ ИСПОЛЬЗУЮТСЯ
    eFirst  = EFIRST;
    eSecond  = ESECOND;

    if (STRBSTAR.length() == 8) { eBstar = STRBSTAR; } else { return "(STRBSTAR.length() != 8)"; };

    eTypeE  = "0"; //"тип эфемериды" - всегда ноль

    if (NELEM.length() == 4) {eNofEl = NELEM; } else  { return "(NELEM.length() != 4)"; };

    //ФОРМИРОВАНИЕ СТРОКИ 1 (БЕЗ КОНТРОЛЬНОЙ СУММЫ)

    SBuf = "1 " +eNofSat+eClsfct+" "+eIntDes+" "+eEpoch+" "+
                eFirst+" "+eSecond+" "+eBstar+" "+eTypeE+" "+eNofEl;

    //ФОРМИРОВАНИЕ КОНТРОЛЬНОЙ СУММЫ
     eCheck = Libtle::Chcksm(SBuf);

     return (SBuf + eCheck);

} //form_string_1

string Libtle::form_string_2(string NSAT, double INCLIN, double ANODE,
       double EXCENTR, double ARGPERIG, double MANOMAL, double MMOTION,
       string NORBIT)
// Формирование второй строки TLE
{
    string eNofSat, eNRev, eCheck, SBuf;
    string eInclin, eAnode, eExc, eArgP, eMAnom, eMMotion;
    double R_INCLIN, R_ANODE, R_EXCENTR, R_ARGPERIG, R_MANOMAL,
           R_MMOTION;

    if (NSAT.length() == 5) {eNofSat = NSAT;} else { return "(NSAT.length() != 5)"; };

    //ПРИВЕДЕНИЕ УГЛОВ К ИНТЕРВАЛУ ОТ 0 ДО 360 ГРАДУСОВ

    ANODE = Libtle::norm_angl_pos(ANODE);
    ARGPERIG = Libtle::norm_angl_pos(ARGPERIG);
    MANOMAL = Libtle::norm_angl_pos(MANOMAL);

    //ПЕРЕСЧЕТ ЭЛЕМЕНТОВ ДЛЯ ВЫВОДА В СТРОКЕ
    R_EXCENTR = EXCENTR * 1.0E7; //эксцентриситет умножается на 10000000, чтобы целая часть
                                 //полученного числа была пригодна для переаода в текстовую строку
                                 //длиной семь символов (без указания десятичной точки, подразумеваемой слева)
    R_INCLIN = Libtle::rad_to_grad(INCLIN);
    R_ANODE = Libtle::rad_to_grad((ANODE));
    R_ARGPERIG = Libtle::rad_to_grad(ARGPERIG);
    R_MANOMAL = Libtle::rad_to_grad(MANOMAL);
    R_MMOTION = MMOTION;

    //ПРЕДСТАВЛЕНИЕ ЭЛЕМЕНТОВ В ВИДЕ СТРОКОВЫХ ПЕРЕМЕННЫХ ДЛЯ ФОРМИРОВАНИЯ СТРОКИ 2
    //STR(R_INCLIN:8:4,StrBuf);
    eInclin = Libtle::Double_to_Str(R_INCLIN,8,4);
//    cout << eInclin << endl;

    //STR(R_ANODE:8:4,StrBuf);
    eAnode = Libtle::Double_to_Str(R_ANODE,8,4);
//    cout << eAnode << endl;

  //  STR(ROUND(R_EXCENTR):7,StrBuf);
    eExc = Libtle::Int_to_Str(round(R_EXCENTR),7);
//    cout << eExc << endl;

    //STR(R_ARGPERIG:8:4,StrBuf);
    eArgP = Libtle::Double_to_Str(R_ARGPERIG,8,4);
//    cout << eArgP << endl;

    //STR(R_MANOMAL:8:4,StrBuf);
    eMAnom = Libtle::Double_to_Str(R_MANOMAL,8,4);
//    cout << eMAnom << endl;

    //STR(R_MMOTION:11:8,StrBuf);
    eMMotion = Libtle::Double_to_Str(R_MMOTION,11,8);
//    cout << eMMotion << endl;

  if (NORBIT.length() == 5) { eNRev = NORBIT; }   else { return "(NORBIT.length() != 5)"; };

    //ФОРМИРОВАНИЕ СТРОКИ 2 (БЕЗ КОНТРОЛЬНОЙ СУММЫ)}
    SBuf = "2 "+eNofSat+" "+eInclin+" "+eAnode+" "+
                eExc+" "+eArgP+" "+eMAnom+" "+eMMotion+eNRev;

    //ПОДСЧЕТ КОНТРОЛЬНОЙ СУММЫ ДЛЯ СТРОКИ 2
    //STR(Chcksm(StrElem2):1,StrBuf);
    eCheck = Libtle::Chcksm(SBuf);
    //ДОБАВЛЕНИЕ КОНТРОЛЬНОЙ СУММЫ К СТРОКЕ 2
    return (SBuf + eCheck);

} //form_string_2

double Libtle::BSTARcalc(double B)
/*
Расчет баллистического коэффициента  BSTAR для TLE,
исходя из стандартного баллистического коэффициента
*/
{
   string StrBuf;
   double R0, G;

R0 = 0.015696615; //Константа плотности воздуха, разделенная на 10
                  //(считаем, что деление на 10 вызвано переходом от СГС к СИ)
G = 9.81; //ускорение св. падения на Земле, с учетом высоты скорее 9.80

return (B * R0) / G;
} //BSTARcalc

double Libtle::fromBSTARcalc(double BSTAR)
/*
Расчет стандартного бал. коэффициента, исходя из бал.
коэффициента  BSTAR для TLE.
Функция, обратная BSTARcalc. Используется для целей тестирования
в программах, формирующих файлы LST, исходя из двухстрочных
элементов.
*/
{
   string StrBuf;
   double R0, G;

R0 = 0.015696615; //Константа плотности воздуха, разделенная на 10
                  //(считаем, что деление на 10 вызвано переходом от СГС к СИ)
G = 9.81; //ускорение св. падения на Земле, с учетом высоты скорее 9.80

return ((BSTAR*G)/R0);
} //fromBSTARcalc

string Libtle::BSTAR_to_str(double dBSTAR)
/*
Данная функция преобразует бал. коэффициент BSTAR в строку,
которая будет использоваться как представление BSTSAR
в первой строке двухстрочных элементов.
Функция рассчитана на то, что значение BSTAR положительно и меньше
единицы, причем не меньше десятичной дроби с девятью нулями после запятой.
Остальные случаи считаются нештатными и тогда в строку-результат
записывается не число, а сообщение.
*/
{
   string StrBuf;
   double dBUF;
   int i, ind;
   string stepen[10];

   stepen[0] = "+0";
   stepen[1] = "-1"; stepen[2] = "-2"; stepen[3] = "-3";
   stepen[4] = "-4"; stepen[5] = "-5"; stepen[6] = "-6";
   stepen[7] = "-7"; stepen[8] = "-8"; stepen[9] = "-9";


   i = 0;
   dBUF = dBSTAR;

   if (dBUF < 0.0) {return "<0.0";}

   if (dBUF >= 1) {return ">=1";}

    while (dBUF < 0.1)
    {
        i = i + 1;
        dBUF = dBUF*10.0;
    }

    if (i > 9) {return ">9";}

    StrBuf = Libtle::Double_to_Str(dBUF, 8, 5);

    ind = StrBuf.find(".");

    StrBuf.erase(0,(ind+1));

    return (StrBuf + stepen[i]);

} //BSTAR_to_str

int Libtle::formLSTshort(string strFSOURCE, string strFRESULT, double base_t, double delta_t, int NN, string strLat, string strLon, int argc, char *argv[])
/*
Формирование (обычно в тестовых целях) файла в упрощенном формате LST,
исходя из двухстрочных элементов.
Формируется последовательномсть строк, содержащих вектор состояния и бал. коэффициент
для моментов времени через равные интервалы.
strFSOURCE - название исходного файла двухстрочных элементов,
strFRESULT - имя файла (LST), в который записываются результаты,
base_t - начальный момент времени в секундах после эпохи двухстрочных элементов,
delta_t - интервал между моментами, для которых рассчитывааются вектора состояния
NN - количество интервалов (то есть рассчитанных векторов состояния)
Значение BSTAR из двухстрочных элементов пересчитывается в стандартный
баллистический коэффициент и записывается в файл.
Кроме того, значения географических широты и долготы записываются в текстовые файлы с именами,
которые задаются строковыми перемеными strLat и strLon.
*/
{
    ofstream fresult; // поток для записи в файл результатов

    //названия исходного файла и файла результатов по умолчанию
    strFSOURCE = "FOR_LST.TLE"; strFRESULT = "LST.REZ";

    //Определения переменных (некоторая их часть осталась зарезервированной и не используется)

      string two_line[2]; //array [1..2] of line_data; line_data = string[69];

      string catnr,elset, obs_name; //obs_name             : string[25];

      string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
      string sat_data[2]; // array [1..max_sats] of two_line;
      string data_drive, data_dir, work_drive, work_dir;

      double julian_epoch;

      string eNRev;

      double poss[3], vell[3], pos[3], vel[3];

      //double time_m;


      double RV[3],SS[3];

 string FileLST, FileDest, FileSample, StrReadBuf, StrB, strTMP;

 //double time;

 //int satnumber;
 double pposi[3], vvelo[3], RG[3], VG[3];

 double Rs, LAMBDA, PHI, H_ell, LAMBDA_ell, PHI_ell;

 int i;

 int int3Date[3], int4Time[4];

 string NAME_TLE;

 double A0[14];

    //Если консольное приложение запускается без параметров командной строки,
    //имена исходного файла и файла результатов задаются по умолчанию (см выше).
    //Если задан один параметр, он используется как имя исходного файла,
    //а имя файла результатов задается по умолчанию
    //Если заданы два параметра (или более), первый используется как имя исходного файла,
    //а второй - как имя файла результатов.

    if (argc > 2) { strFSOURCE = argv[1]; strFRESULT = argv[2];}
    else { if (argc > 1) { strFSOURCE = argv[1]; };  };
//    cout << strFSOURCE << endl; // контрольный вывод имени файла
//    cout << strFRESULT << endl; // контрольный вывод имени файла

    //Если не удалось обработать исходный файл, программа завершается
    if (!Libtle::read_tle(strFSOURCE, sat_data, &NAME_TLE))
    { //cout << " FALSE RETURNED!!!" << endl;
        return 1; };

    fresult.open(strFRESULT, ios::out); //открытие потока для записи результатов в файл

    //запись в файл результатов названия исходного файла и его содержимого

//    fresult << "TLE FILE: " << strFSOURCE << endl;
//    fresult  << endl;
//    fresult << NAME_TLE << endl;
//    fresult << sat_data[0] << endl;
//    fresult << sat_data[1] << endl;
//    fresult  << endl;

    // Подготовка данных для вычисления векторов состояния

     Libtle::PreparePrognSGP4(sat_data, &julian_epoch, A0);


       //Вычисление вектора состояния.
       //Параметр - время от эпохи элементов в секундах.

       int ii;

       ofstream fdolgota, fshirota; // потоки для записи координат подспутниковых точек

       std::locale().global(std::locale("C")); //на всякий случай - установка точки в качестве отделителя дробной части
       // setlocale(LC_ALL,"Russian"); //на вывод не влияло

       fdolgota.open(strLon, ios::out); //открытие потока для записи результатов в файл
       fshirota.open(strLat, ios::out); //открытие потока для записи результатов в файл

       //base_t = 0.0;
       //delta_t = 60.0;

       for (ii=0;ii<NN;ii++) // цикл расчета векторов через заданные интервалы
       {
       Libbase::PrognSGP4 (RV, SS, base_t, A0);

         poss[0] = RV[0];
         poss[1] = RV[1];
         poss[2] = RV[2];
         vell[0] = SS[0];
         vell[1] = SS[1];
         vell[2] = SS[2];

         //        close(f2);

       for (i = 0;i<3;i++) { pos[i] = poss[i]; vel[i] = vell[i];} //for i
       // Magnitude(pos); Magnitude(vel);

       //окончание CalculateSGDP4

       for (i = 0;i<3;i++) { pposi[i] = pos[i]; vvelo[i] = vel[i];} //for i

       // Libbase::Conv_Inert_Gr(pposi, vvelo, julian_epoch+ii*(delta_t/86400.0), RG, VG);
       Libbase::Conv_Inert_Gr(pposi, vvelo, julian_epoch+base_t/86400.0, RG, VG);

       Libbase::Jul_to_Date_Time_arr(julian_epoch + base_t/86400.0, int3Date, int4Time);
//       st = CSGP4_SDP4::CalendarDate (julian_epoch + base_t/86400.0);

               fresult << setw(4) << setfill(' ') << ii << "  ";
               fresult << setw(2) << setfill('0') << int3Date[0]-2000 << ":";
               fresult << setw(2) << setfill('0') << int3Date[1] << ":";
               fresult << setw(2) << setfill('0') << int3Date[2] << "  ";
               fresult << setw(2) << setfill('0') << int4Time[0] << ":";
               fresult << setw(2) << setfill('0') << int4Time[1] << ":";
               fresult << setw(2) << setfill('0') << int4Time[2] << ".";
              // fresult << setw(2) << setfill('0') << int4Time[3] << "0";
               fresult << setw(3) << setfill('0') << int4Time[3];

       //fresult << " " << setprecision(9) << fixed << right << setw(11) << setfill(' ') << A0[4];

       fresult << "  " << setprecision(9) << fixed << right << setw(11) << setfill(' ') << Libtle::fromBSTARcalc(A0[4]);

       //fresult.width(11);
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << RG[0];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << RG[1];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << RG[2];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << VG[0];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << VG[1];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << VG[2];

       fresult << endl;

       //fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << julian_epoch + base_t/86400.0;
       //doubleBUF = Libbase::Date_Time_to_Jul_arr(int3Date, int4Time, 1000.0);
       //fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << doubleBUF << endl;
       //fresult << " " << Libtle::Date_Time_to_epoch_arr(int3Date, int4Time) << endl;

   Libbase::decart_to_sph(RG, &Rs, &LAMBDA, &PHI);

    //RG[0] = RG[0]*1000;
    //RG[1] = RG[1]*1000;
    //RG[2] = RG[2]*1000;

    Libbase::decart_to_ell(RG, 1.0/298.257839303, 6378.136, &H_ell, &LAMBDA_ell, &PHI_ell);


//    fresult << "SPHERE " << endl;
//    fresult << "R:      " << Rs << endl;
//    fresult << "LAMBDA: " << Libtle::lambda_360_to_180(LAMBDA) << endl;
//    fresult << "PHI:    " << PHI << endl;
//    fresult  << endl;
//    fresult << "ELLIPSE " << endl;
//    fresult << "H: " << H_ell << endl;
//    fresult << "LAMBDA: " << Libtle::lambda_360_to_180(LAMBDA_ell) << endl;
//    fresult << "PHI: " << PHI_ell << endl;


    //Dolgota[ii] = Libtle::lambda_360_to_180(LAMBDA_ell);
    //Shirota[ii] = PHI_ell;

    fdolgota << Libtle::lambda_360_to_180(LAMBDA_ell) << endl;
    fshirota << PHI_ell << endl;

    base_t = base_t + delta_t;

       } //for (ii=0;ii<NN;ii++)   // окончание цикла расчета векторов через заданные интервалы

    fresult.close(); //закрытие потока для записи результатов в файл

    return 0;
} //formLSTshort


string Libtle::Date_Time_to_epoch_arr(int int3Date[3], int int4Time[4])
/*
Преобразование даты и времени, заданных в массивах, в строку
в формате эпохи двухстрочных элементов
*/
{

    int intBuf, YY;
    double DoubleBuf;
    int point;
    string StrB, StrB1, StrBufDate, StrBufTime;


//Дата

intBuf = Libbase::DOY(int3Date);
if (intBuf < 100) { if (intBuf < 10) {
                                      StrB = Libtle::Int_to_Str(intBuf,1);
                                      StrB = "00" + StrB;
                                    }
                                    else
                                    {
                                      StrB = Libtle::Int_to_Str(intBuf,2);
                                      StrB = "0" + StrB;
                                     }
                   }
                   else StrB = Libtle::Int_to_Str(intBuf,3);

StrBufDate = StrB;

//Время

DoubleBuf = Libbase::Frac_of_Day(int4Time,1000.0);
StrB1 = Libtle::Double_to_Str(DoubleBuf,12,8);
point = StrB1.find('.');
StrB1.erase(0,point);

StrBufTime = StrB1;

StrB = StrBufDate + StrBufTime;

YY = int3Date[0];

if (YY >= 2000) { YY = YY - 2000; }
              else {YY = YY - 1900;};

if (YY < 10)
           {
           StrB1 = Libtle::Int_to_Str(YY,1);
           StrB1 = " "+StrB1;
            }
           else
           StrB1 = Libtle::Int_to_Str(YY,2);

return (StrB1 + StrB);

} //Date_Time_to_epoch_arr

void Libtle::Date_Time_offset_arr(int int3dat[3], int int4time[4], double offset_sec, int int3dat_rez[3], int int4time_rez[4])
/*
"Смещение" даты и времени, заданных в массивах, на величину, задаваемую в секундах.
Функция предназначена в первую очередь для перехода между гринвичским и московским временами.
Исходные дата и время задаются в массивах int3dat[3] и int4time[4], смещение - в переменной offset_sec,
"смещенные" дата и время заносятся в массивы int3dat_rez[3] и int4time_rez[4].
Таким образом смещение в три часа между исходным декретным московским и результирующим гринвичским
временами задается величиной offset_sec = -60.0*60.0*3.0.
*/
{

double julBUF;

julBUF = Libbase::Date_Time_to_Jul_arr(int3dat, int4time, 1000.0);
julBUF = julBUF + offset_sec/86400.0;
Libbase::Jul_to_Date_Time_arr(julBUF, int3dat_rez, int4time_rez);

} // Date_Time_offset_arr



bool Libtle::copy_text_file(string strFile1, string strFile2)
/*
Копирование текстового файла построчно в другой файл
В целях отладки в данную функцию добавлялись дополнительные действия
*/
{

ifstream fread;
ofstream ferror, fwrite;
string strBUF;
//struct_time tim;

fread.open(strFile1, ios::in);

if (fread.fail())
{
    ferror.open("READ_ERROR", ios::out);
    ferror << "open file error" << endl;
    ferror.close();
    return false;
}

fwrite.open(strFile2, ios::out);

while(getline(fread, strBUF))
{
fwrite << strBUF << endl;

//Дальнейший фрагмент использовался в целях отладки:
/*
tim = read_time(strBUF, 23, 26, 29, 34, 3);
fwrite << fixed << setprecision(0) << right << setw(2) << setfill('0') << tim.hour << ":";
fwrite << fixed << setprecision(0) << right << setw(2) << setfill('0') <<  tim.min << ":";
fwrite << fixed << setprecision(3) << right << setw(6) << setfill('0')<< tim.sec << endl;
*/
}

fread.close();
fwrite.close();

return true;

} // copy_text_file

bool Libtle::convert_LST(string strFile1, string strFile2)
/*
Выборочное коппирование данных из файла типа LST в другой файл.
Функция предназначена для упрощения формата файлов данных.
*/
{
int i;

ifstream fread;
ofstream ferror, fwrite;
string strBUF, strBUF1;
struct_time tim;
struct_date dat;

istringstream iss;
vector<string> parts;
string part;

fread.open(strFile1, ios::in);

if (fread.fail())
{
    ferror.open("READ_ERROR", ios::out);
    ferror << "open file error" << endl;
    ferror.close();
    return false;
}

fwrite.open(strFile2, ios::out);

while(getline(fread, strBUF))
{
//fwrite << strBUF << endl;

strBUF1 = strBUF.substr(3,4);

fwrite << strBUF1 << "  ";

dat = read_date(strBUF, 18, 4, 15, 12);

tim = read_time(strBUF, 23, 26, 29, 34, 3);

fwrite << fixed << right << setw(2) << dat.year-2000 << ":";
fwrite << fixed << right << setw(2) << setfill('0') << dat.month << ":";
fwrite << fixed << right << setw(2) << setfill('0') << dat.day << "  ";

fwrite << fixed << setprecision(0) << right << setw(2) << setfill('0') << tim.hour << ":";
fwrite << fixed << setprecision(0) << right << setw(2) << setfill('0') <<  tim.min << ":";
fwrite << fixed << setprecision(3) << right << setw(6) << setfill('0')<< tim.sec;

//strBUF1 = strBUF.substr(139,70);
//fwrite << strBUF1 << endl;

strBUF1 = strBUF.substr(132,strBUF.length()- 133);
iss = istringstream(strBUF1);

while (iss >> part) { parts.push_back(part);}

   // for (const auto& w : parts) {cout << w << endl;}

for (i=0;i<7;i++) {fwrite << "  " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << parts[i]; }

fwrite << endl;

iss.clear();
parts.clear();
part.clear();

}

fread.close();
fwrite.close();

return true;

} // convert_LST

bool Libtle::convert_1_BNI_UTC(string strFile1, string strFile2)
/*
Выборочное коппирование данных из файла типа 1_BNI_UTC в другой файл
(чтобы упростить формат файла данных)
*/
{
int i;

ifstream fread;
ofstream ferror, fwrite;
string strBUF, strBUF1;
struct_time tim;
struct_date dat;

//istringstream iss;
//stringstream iss;
//vector<string> parts;
//string part;

fread.open(strFile1, ios::in);

if (fread.fail())
{
    ferror.open("READ_ERROR", ios::out);
    ferror << "open file error" << endl;
    ferror.close();
    return false;
}

fwrite.open(strFile2, ios::out);

while(getline(fread, strBUF))
{
//fwrite << strBUF << endl;

strBUF1 = strBUF.substr(0,4);

fwrite << strBUF1 << "  ";

dat = read_date(strBUF, 8, 4, 13, 16);

tim = read_time(strBUF, 19, 22, 25, 28, 3);

fwrite << fixed << right << setw(2) << dat.year-2000 << ":";
fwrite << fixed << right << setw(2) << setfill('0') << dat.month << ":";
fwrite << fixed << right << setw(2) << setfill('0') << dat.day << "  ";

fwrite << fixed << setprecision(0) << right << setw(2) << setfill('0') << tim.hour << ":";
fwrite << fixed << setprecision(0) << right << setw(2) << setfill('0') <<  tim.min << ":";
fwrite << fixed << setprecision(3) << right << setw(6) << setfill('0')<< tim.sec;

//strBUF1 = strBUF.substr(139,70);
//fwrite << strBUF1 << endl;

strBUF1 = strBUF.substr(32,strBUF.length()- 33);

istringstream iss(strBUF1);
//iss.str(strBUF1);

vector<string> parts;
string part;


while (iss >> part) { parts.push_back(part);}

   // for (const auto& w : parts) {cout << w << endl;}

fwrite << "  " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << parts[9];

for (i=0;i<6;i++) {fwrite << "  " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << stod(parts[i].substr(0,parts[i].length())); }

fwrite << endl;

//iss.str("");
//iss.clear();

}

fread.close();
fwrite.close();

return true;

} // convert_1_BNI_UTC


double Libtle::Date_Time_for_PROGN_arr(int int3dat[3], int int4time[4], double jul_epoch)
/*
Подготовка, исходя из заданного времени и ранее определенного (например, с помощью функции
PreparePrognSGP4) юлианского времени эпохи TLE, величины, которую надо подать на вход
функции расчета векторов координат и скорости по двухстрочным элементам (функции PrognSGP4
библиотеки libbase).
Результат - разность в секундах между заданным временем (дата и время в массивах int3dat[3]
и int int4time[4]) и юлианским временем эпохи TLE jul_epoch.
Если результат отрицательный, задан момент времени, более ранний по сравнению с эпохой.
*/
{

double julBUF;

julBUF = Libbase::Date_Time_to_Jul_arr(int3dat, int4time, 1000.0) - jul_epoch;
julBUF = julBUF * 86400.0;

return julBUF;

} // Date_Time_for_PROGN_arr

void Libtle::PrognSGP4VectCoord(double A0[14], double j_epoch, double time_after_epoch, double posInert[3], double velInert[3], double posGr[3], double velGr[3], double* high, double* longit, double* latit)
/*
"Расширенная" функция прогноза положения (и скорости) КА.
Данные в массиве A0 должны быть заранее подготовлены, исходя из TLE (например, с помощью функции PreparePrognSGP4).
Заранее должно быть определено и юлианское время эпохи TLE (например, с помощью той же функции).
Задается также момент времени, для которого нужно сделать расчеты, в виде секунд, прошедших от
времени эпохи TLE (можно найти с помощью функции Date_Time_for_PROGN_arr).
В результате в указанные массивы записываются векторы координат и скорости в инерциальной и гринвичской системах,
и в указанные переменные - высота КА, долгота и широта его подспутниковой точки.

*/
{
      // Libtle::PreparePrognSGP4(sat_data, &julian_epoch, A0);

      Libbase::PrognSGP4 (posInert, velInert, time_after_epoch, A0);

      Libbase::Conv_Inert_Gr(posInert, velInert, j_epoch+time_after_epoch/86400.0, posGr, velGr);
      //Libbase::Conv_Inert_Gr(pposi, vvelo, julian_epoch+ii*(delta_t/86400.0), RG, VG);

      //cout << "jul_ep 1: " << j_epoch+time_after_epoch/86400.0 << endl;

   Libbase::decart_to_ell(posGr, 1.0/298.257839303, 6378.136, high, longit, latit);


} // PrognSGP4VectCoord

int Libtle::calc4alt(string strFSOURCE, string strFRESULT, double base_t, double delta_t, int NN, string strLat, string strLon, int argc, char *argv[])
/*
Формирование (обычно в тестовых целях) текстовых файлов со значениями широты и долготы для подспутниковых
точек КА, которые он проходит через равные промежутки времени delta_t (сек), начиная с момента через base_t
секунд после эпохи двухстрочных элементов КА.
strFSOURCE - название исходного файла двухстрочных элементов,
strFRESULT - имя файла (LST), в который записываются дополнительные результаты (векторы)
base_t - начальный момент времени в секундах после эпохи двухстрочных элементов,
delta_t - интервал между моментами, для которых рассчитываются векторы состояния и географические координаты
NN - количество интервалов (то есть рассчитанных векторов состояния)
Значения географических широты и долготы записываются в текстовые файлы с именами,
которые задаются строковыми перемеными strLat и strLon.
*/
{
    ofstream fresult; // поток для записи в файл результатов

    //названия исходного файла и файла результатов по умолчанию
    strFSOURCE = "CALC4.TLE"; strFRESULT = "CALC4.REZ";

    //Определения переменных (некоторая их часть осталась зарезервированной и не используется)

       string two_line[2]; //array [1..2] of line_data; line_data = string[69];

      string catnr,elset;
      string obs_name; //obs_name             : string[25];
      string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
      string sat_data[2]; // array [1..max_sats] of two_line;
      string data_drive, data_dir, work_drive, work_dir;

      double julian_epoch;


        string eNRev;

        //double time_m;

 string FileLST, FileDest, FileSample, StrReadBuf, StrB, strTMP;
 //fi, f1, fresult, ff : text;

 //double time;

 //int satnumber;
 double pposi[3], vvelo[3], RG[3], VG[3];

 double H_ell, LAMBDA_ell, PHI_ell;

 int int3Date[3], int4Time[4];

 string NAME_TLE;

 double doubleBUF;

         double A0[14];

    //Если консольное приложение запускается без параметров командной строки,
    //имена исходного файла и файла результатов задаются по умолчанию (см выше).
    //Если задан один параметр, он используется как имя исходного файла,
    //а имя файла результатов задается по умолчанию
    //Если заданы два параметра (или более), первый используется как имя исходного файла,
    //а второй - как имя файла результатов.

    if (argc > 2) { strFSOURCE = argv[1]; strFRESULT = argv[2];}
    else { if (argc > 1) { strFSOURCE = argv[1]; };  };
//    cout << strFSOURCE << endl; // контрольный вывод имени файла
//    cout << strFRESULT << endl; // контрольный вывод имени файла

    //Если не удалось обработать исходный файл, программа завершается
    if (!Libtle::read_tle(strFSOURCE, sat_data, &NAME_TLE))
    { //cout << "READ_TLE FALSE RETURNED!!!" << endl;
      return 1; };

    fresult.open(strFRESULT, ios::out); //открытие потока для записи результатов в файл

    //запись в файл результатов названия исходного файла и его содержимого

//    fresult << "TLE FILE: " << strFSOURCE << endl;
//    fresult  << endl;
//    fresult << NAME_TLE << endl;
//    fresult << sat_data[0] << endl;
//    fresult << sat_data[1] << endl;
//    fresult  << endl;


    //satnumber = 1;


     Libtle::PreparePrognSGP4(sat_data, &julian_epoch, A0);


     //time = julian_epoch;


       //Procedure CalculateSGP4(time : double; var pos, vel: SGP_Math.vector);

       //double  t_calc;

       //time_m = time;

       //t_calc = (time_m - julian_epoch) * 86400.0;

       //double tpost;

       //tpost = t_calc;


       //Вычисление вектора состояния.
       //Выполняются те же действия, которые выполняла процедура CalculateVector,
       //написанная на паскале. Параметр - время от эпохи элементов в секундах.

       //     assign(f2,'VCOMBI.REZ'); rewrite(f2);
       int ii;
       //double Dolgota[48];
       //double Shirota[48];
       //double base_t, delta_t;
       ofstream fdolgota, fshirota; // потоки для записи координат подспутниковых точек

       std::locale().global(std::locale("C")); //на всякий случай - установка точки в качестве отделителя дробной части
       // setlocale(LC_ALL,"Russian"); //на вывод не влияло

       fdolgota.open(strLon, ios::out); //открытие потока для записи результатов в файл
       fshirota.open(strLat, ios::out); //открытие потока для записи результатов в файл

       //base_t = 0.0;
       //delta_t = 60.0;


       for (ii=0;ii<NN;ii++)
       {

//       Libbase::PrognSGP4 (RV, SS, base_t, A0);
       Libtle::PrognSGP4VectCoord(A0, julian_epoch, base_t, pposi, vvelo, RG, VG, &H_ell, &LAMBDA_ell, &PHI_ell);

       Libbase::Jul_to_Date_Time_arr(julian_epoch + base_t/86400.0, int3Date, int4Time);
//       st = CSGP4_SDP4::CalendarDate (julian_epoch + base_t/86400.0);


               fresult << setw(2) << setfill('0') << int3Date[0]-2000 << ":";
               fresult << setw(2) << setfill('0') << int3Date[1] << ":";
               fresult << setw(2) << setfill('0') << int3Date[2] << "  ";
               fresult << setw(2) << setfill('0') << int4Time[0] << ":";
               fresult << setw(2) << setfill('0') << int4Time[1] << ":";
               fresult << setw(2) << setfill('0') << int4Time[2] << ":";
               fresult << setw(2) << setfill('0') << int4Time[3];



       //fresult.width(11);
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << RG[0];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << RG[1];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << RG[2];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << VG[0];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << VG[1];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << VG[2];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << julian_epoch + base_t/86400.0;

       doubleBUF = Libbase::Date_Time_to_Jul_arr(int3Date, int4Time, 1000.0);

       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << doubleBUF << endl;

       fresult << " " << Libtle::Date_Time_to_epoch_arr(int3Date, int4Time) << endl;

   // Libbase::decart_to_sph(RG, &Rs, &LAMBDA, &PHI);

    //RG[0] = RG[0]*1000;
    //RG[1] = RG[1]*1000;
    //RG[2] = RG[2]*1000;

    Libbase::decart_to_ell(RG, 1.0/298.257839303, 6378.136, &H_ell, &LAMBDA_ell, &PHI_ell);
    LAMBDA_ell = Libtle::lambda_360_to_180(LAMBDA_ell);



//    fresult << "SPHERE " << endl;
//    fresult << "R:      " << Rs << endl;
//    fresult << "LAMBDA: " << Libtle::lambda_360_to_180(LAMBDA) << endl;
//    fresult << "PHI:    " << PHI << endl;
//    fresult  << endl;
//    fresult << "ELLIPSE " << endl;
//    fresult << "H: " << H_ell << endl;
//    fresult << "LAMBDA: " << Libtle::lambda_360_to_180(LAMBDA_ell) << endl;
//    fresult << "PHI: " << PHI_ell << endl;


    //Dolgota[ii] = Libtle::lambda_360_to_180(LAMBDA_ell);
    //Shirota[ii] = PHI_ell;

    fdolgota << LAMBDA_ell << endl;
    fshirota << PHI_ell << endl;

    base_t = base_t + delta_t;

       } //for (ii=0;ii<NN;ii++)

    fresult.close(); //закрытие потока для записи результатов в файл

    return 0;
} //calc4alt

int Libtle::compare_TLE_LST(string strFSOURCE, string strFLST, string strFRESULT, double offset_sec)
{
/*
 Сравнение прогноза по заданным TLE с данными, которые содержатся в упрощенном файле LST.
 strFSOURCE, strFLST, string strFRESULT - имена исходного файла с TLE, файла данных LST  и файла для результатов сравнения
*/
    ofstream fresult; // поток для записи в файл результатов
    ifstream fread;

    //названия исходного файла и файла результатов по умолчанию
    //strFSOURCE = "TLE.TLE"; strFLST = "LST.LST"; strFRESULT = "COMPARE.REZ";

      string two_line[2]; //array [1..2] of line_data; line_data = string[69];

      string catnr,elset, obs_name; //obs_name             : string[25];

      string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
      string sat_data[2]; // array [1..max_sats] of two_line;
      string data_drive, data_dir, work_drive, work_dir;

      double julian_epoch, jul_buf;
      //double  jul_lst;

      string eNRev;

      int i;



 string FileLST, FileDest, FileSample, StrReadBuf, StrB, strBUF, strBUF1, strTMP, strNumber;



double R_LST[3], V_LST[3], R_Inert[3], V_Inert[3], R_Gr[3], V_Gr[3];

 double H_ell, LAMBDA_ell, PHI_ell, H_ell_LST, LAMBDA_ell_LST, PHI_ell_LST;

 int int3Date[3], int4Time[4], int3Date1[3], int4Time1[4];

 string NAME_TLE;

 double A0[14];

 stringstream iss;
 vector<string> parts;
 string part;



 std::locale().global(std::locale("C")); //на всякий случай - установка точки в качестве отделителя дробной части
 // setlocale(LC_ALL,"Russian"); //на вывод не влияло


    fresult.open(strFRESULT, ios::out); //открытие потока для записи результатов в файл

    //Если не удалось обработать один из исходных файлов, программа завершается
    if (!Libtle::read_tle(strFSOURCE, sat_data, &NAME_TLE))
    { fresult << " open tle file FALSE RETURNED!!!" << endl; fresult.close(); return 1; };

    fread.open(strFLST, ios::in);
    if (fread.fail())
    {fresult << " opePrognSGP4VectCoord LST file FALSE RETURNED!!!" << endl; fresult.close(); return 1; }

     Libtle::PreparePrognSGP4(sat_data, &julian_epoch, A0);


       while(getline(fread, strBUF))
       {

           strNumber = strBUF.substr(0,4);

           read_date_arr_alt(strBUF, 6, 2, 9, 12, int3Date1);
           int3Date1[0] = int3Date1[0] + 2000;
           read_time_arr_alt(strBUF, 16, 19, 22, 25, 3, int4Time1);

           Libtle::Date_Time_offset_arr(int3Date1, int4Time1, offset_sec, int3Date, int4Time);

           //jul_lst = Libbase::Date_Time_to_Jul_arr(int3Date, int4Time, 1000.0);
           //cout << "jul_LST: " << jul_lst << endl;

           strBUF1 = strBUF.substr(29,strBUF.length()- 29);

           istringstream iss(strBUF1);

           while (iss >> part) { parts.push_back(part);}

              // for (const auto& w : parts) {cout << w << endl;}


           for (i=1;i<4;i++) R_LST[i-1] = stod(parts[i].substr(0,parts[i].length()));
           for (i=4;i<7;i++) V_LST[i-4] = stod(parts[i].substr(0,parts[i].length()));



       jul_buf = Libtle::Date_Time_for_PROGN_arr(int3Date, int4Time, julian_epoch);
       //cout << "jul_buf: " << jul_buf << endl;
       Libtle::PrognSGP4VectCoord(A0, julian_epoch, jul_buf, R_Inert, V_Inert, R_Gr, V_Gr, &H_ell, &LAMBDA_ell, &PHI_ell);
       //cout << "R:     " << R_Gr[0] << endl;
       //cout << "R_LST: " << R_LST[0] << endl;



       Libbase::Jul_to_Date_Time_arr(julian_epoch + jul_buf/86400.0, int3Date1, int4Time1);
       int3Date1[0] = int3Date1[0] - 2000;


       fresult << strNumber << " " << Date_to_Str_arr(int3Date1) << " " << Time_to_Str_arr(int4Time1);

       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << R_Gr[0];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << R_LST[0];

       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << Libbase::delt_vect_3(R_Gr,R_LST);
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << Libbase::delt_vect_3(V_Gr,V_LST);

    Libbase::decart_to_ell(R_LST, 1.0/298.257839303, 6378.136, &H_ell_LST, &LAMBDA_ell_LST, &PHI_ell_LST);

    fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << abs(H_ell_LST-H_ell);
    fresult << endl;


       iss.clear();
       parts.clear();
       part.clear();

       } //for (ii=0;ii<48;ii++)

    fresult.close(); //закрытие потока для записи результатов в файл

    return 0;
} //compare_TLE_LST

int Libtle::LST_to_TLE(string strFLST, string strFRESULT, double offset_sec)
/*
Функция построчно читает данные из файла LST (файл в упрощенном формате),
рассчитывает для каждой строки двухстрочные элементы, вычисляет по ним
векторы координат и скорости и находит разность между ними и теми векторами,
которые заданы в файле LST.
Кроме того функция для каждой строки вычисляет разность высот КА,
задаваемых двухстрочными элементами и теми векторами, которые
содержатся в файле LST.
Предусмотрен учет смещения (в секундах) oofset_sec между временем
данных в файле LST и гринвичским временем.
strFLST, string strFRESULT - имена исходного файла LST и файла с результатами расчетов.
*/

{
    ofstream fresult; // поток для записи в файл результатов
    ifstream fread;

    string tle_rez[2];

    //названия исходного файла и файла результатов по умолчанию
    //strFSOURCE = "TLE.TLE"; strFLST = "LST.LST"; strFRESULT = "COMPARE.REZ";

      string two_line[2]; //array [1..2] of line_data; line_data = string[69];

      string catnr,elset, obs_name; //obs_name             : string[25];

      string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
      string sat_data[2]; // array [1..max_sats] of two_line;
      string data_drive, data_dir, work_drive, work_dir;

      double jul_lst, jul_buf, julian_epoch;

      string eNRev;

      int i;

      string FileLST, FileDest, FileSample, StrReadBuf, StrB, strBUF, strBUF1, strTMP, strNumber;



double R_LST[3], V_LST[3], R_Inert[3], V_Inert[3], R_Gr[3], V_Gr[3];

 double H_ell, LAMBDA_ell, PHI_ell, H_ell_LST, LAMBDA_ell_LST, PHI_ell_LST, doubleBSTAR;

 int int3Date[3], int4Time[4], int3Date1[3], int4Time1[4];

 string NAME_TLE;


 double A0[14];

 stringstream iss;
 vector<string> parts;
 string part;

 str_data_tle sd;

 std::locale().global(std::locale("C")); //на всякий случай - установка точки в качестве отделителя дробной части
 // setlocale(LC_ALL,"Russian"); //на вывод не влияло


    fresult.open(strFRESULT, ios::out); //открытие потока для записи результатов в файл

    fread.open(strFLST, ios::in);  // открытие файла исходных данных (упрощенный формат LST)
    if (fread.fail())
    {fresult << " opePrognSGP4VectCoord LST file FALSE RETURNED!!!" << endl; fresult.close(); return 1; }

       while(getline(fread, strBUF))   // начало цикла чтения строк исходного файла (LST)
       {
           strNumber = strBUF.substr(0,4); // чтение номера витка (или порядкового номера)

           read_date_arr_alt(strBUF, 6, 2, 9, 12, int3Date1); // чтение даты
           int3Date1[0] = int3Date1[0] + 2000; // год в файле обозначается двумя знаками, поэтому прибавляется 2000
           read_time_arr_alt(strBUF, 16, 19, 22, 25, 3, int4Time1); // чтение времени

           Libtle::Date_Time_offset_arr(int3Date1, int4Time1, offset_sec, int3Date, int4Time);
                       //учет смещения (в секундах) относительно гринвичского времени

           jul_lst = Libbase::Date_Time_to_Jul_arr(int3Date, int4Time, 1000.0);

           //cout << "jul_LST: " << jul_lst << endl;

           strBUF1 = strBUF.substr(29,strBUF.length()- 29);  // чтение остатка строки, следующего за указанием времени

           istringstream iss(strBUF1);

           while (iss >> part) { parts.push_back(part);} // разбивка прочитанного остатка строки, разделителями считаются последовательности пробелов

              // for (const auto& w : parts) {cout << w << endl;}

           doubleBSTAR = stod(parts[0].substr(0,parts[0].length())); // чтение стандартного бал. коэффициента
           for (i=1;i<4;i++) R_LST[i-1] = stod(parts[i].substr(0,parts[i].length())); // чтение вектора координат
           for (i=4;i<7;i++) V_LST[i-4] = stod(parts[i].substr(0,parts[i].length())); // чтение вектора скорости

           doubleBSTAR = Libtle::BSTARcalc(doubleBSTAR); // преобразование стандартного бал. коэффициента в BSTAR


           sd.NSAT = "25544";
           sd.INTDESIGN = "98067A  ";
           sd.TEPOCHSTR = Libtle::Date_Time_to_epoch_arr(int3Date, int4Time); // преобразование даты и времени в формат эпохи TLE

           //cout << "TEPOCHSTR" << sd.TEPOCHSTR << endl << endl;

           sd.STRBSTAR = " " + Libtle::BSTAR_to_str(doubleBSTAR); // пробел добавляется, чтобы обеспечить нужную длину строки

           //cout << "STRBSTAR" << sd.STRBSTAR << endl;
           sd.NELEM = "0890";
           sd.NORBIT = "12345";

           //ЗАДАЮТСЯ ФИКСИРОВАННЫЕ ЗНАЧЕНИЯ, ПРЕДСТАВЛЯЮЩИЕ ПЕРВУЮ И ВТОРУЮ ПРОИЗВОДНЫЕ,
           //КОТОРЫЕ ЗАНОСЯТСЯ В ПЕРВУЮ СТРОКУ TLE, НО ПРИ ВЫЧИСЛЕНИЯХ НЕ ИСПОЛЬЗУЮТСЯ
           sd.E_FIRST  = " .00030924";
           sd.E_SECOND  = " 40826-8";

            //cout << "R_LST " << R_LST[0] << endl;

            if (Libtle::vec_to_tle(sd, R_LST, V_LST, tle_rez, true, true, "FALSE") == 1000)
           //if (Libtle::vec_to_tle4(sd, R_LST, V_LST, tle_rez, true, true, "FALSE") == 1000)
           {

               tle_rez[0] = "iterations limit";
               tle_rez[1] = "iterations limit";

               //cout << "tle " << tle_rez[0] << endl;
               //coat << "tle " << tle_rez[1] << endl;

               sat_data[0] = tle_rez[0];
               sat_data[1] = tle_rez[1];
               R_Gr[0] = 0.0; R_Gr[1] = 0.0; R_Gr[2] = 0.0;
               V_Gr[0] = 0.0; V_Gr[1] = 0.0; V_Gr[2] = 0.0;
               int3Date1[0] = 0; int3Date1[1] = 0; int3Date1[2] = 0;
               int4Time1[0] = 0; int4Time1[1] = 0; int4Time1[2] = 0; int4Time1[3] = 0;
           }
           else
           {
           //coat << "tle " << tle_rez[0] << endl;
           //coat << "tle " << tle_rez[1] << endl;

           sat_data[0] = tle_rez[0];
           sat_data[1] = tle_rez[1];

       Libtle::PreparePrognSGP4(sat_data, &julian_epoch, A0);

       //coat << "JUL LST " << jul_lst << "  JUL EPOCH " << julian_epoch << endl;

       jul_buf = Libtle::Date_Time_for_PROGN_arr(int3Date, int4Time, jul_lst);
       //cout << "jul_buf: " << jul_buf << endl;
       Libtle::PrognSGP4VectCoord(A0, jul_lst, jul_buf, R_Inert, V_Inert, R_Gr, V_Gr, &H_ell, &LAMBDA_ell, &PHI_ell);
       //cout << "R:     " << R_Gr[0] << endl;
       //cout << "R_LST: " << R_LST[0] << endl;



       Libbase::Jul_to_Date_Time_arr(jul_lst + jul_buf/86400.0, int3Date1, int4Time1);
       int3Date1[0] = int3Date1[0] - 2000;
           }


       fresult << strNumber << " " << Date_to_Str_arr(int3Date1) << " " << Time_to_Str_arr(int4Time1);

       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << R_Gr[0];
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << R_LST[0];

       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << Libbase::delt_vect_3(R_Gr,R_LST);
       fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << Libbase::delt_vect_3(V_Gr,V_LST);

    Libbase::decart_to_ell(R_LST, 1.0/298.257839303, 6378.136, &H_ell_LST, &LAMBDA_ell_LST, &PHI_ell_LST);

    fresult << " " << setprecision(5) << fixed << right << setw(12) << setfill(' ') << abs(H_ell_LST-H_ell);
    fresult << endl;


       iss.clear();
       parts.clear();
       part.clear();

       fresult << tle_rez[0] << endl;
       fresult << tle_rez[1] << endl;
       fresult << sd.TEPOCHSTR << endl;
       fresult << int4Time[2] << endl;
       fresult << int4Time[3] << endl;
       fresult << setprecision(12) << fixed << right << setw(12) << Libtle::Real_Value(sd.TEPOCHSTR,0,sd.TEPOCHSTR.length()) << endl;

       //fresult << Julian_Date_of_Epoch(double epoch)

       } //for (ii=0;ii<48;ii++)

    fresult.close(); //закрытие потока для записи результатов в файл

    return 0;
} //LST_to_TLE


int Libtle::vec_to_tle(str_data_tle sd, double R[3], double V[3], string tle_rez[2], bool bGreenw, bool bPrint, string strFile)
/*
Итеративный алгоритм (с использованием метода Ньютона), вычисляющий параметры двухстрочных элементов по заданным
для определенного момента времени векторам координат и скорости КА.
Вектора могут быть заданы как в инерциальной, так и в гринвичской системах координат. Систему указывает параметр bGreenw.
В процессе вычислений может формироваться справочный файл с именем strFile, если установлен параметр bPrint.
Дополнительные данные, необходимые для формирования TLE, задаются в структуре типа str_data_tle.
*/
{
int intReturn;

string sat_data[2]; // array [1..max_sats] of two_line;

int counter;

double pos[3],vel[3];  //на паскале был тип SGP_MATH.vector;
double V_COORD_GR[3], //ВЕКТОР КООРДИНАТ
       V_VELOC_GR[3], //ВЕКТОР СКОРОСТИ
       V_COORD_I[3], //ВЕКТОР КООРДИНАТ
       V_VELOC_I[3], //ВЕКТОР СКОРОСТИ

       V_COORD[3], //ВЕКТОР КООРДИНАТ
       V_VELOC[3], //ВЕКТОР СКОРОСТИ
       V_ORTOG[3], //ВЕКТОР, ОРТОГОНАЛЬНЫЙ ВЕКТОРАМ КООРДИНАТ И СКОРОСТИ
       V_ORTOG_N[3]; //НОРМИРОВАННЫЙ ВЕКТОР, ОРТОГОНАЛЬНЫЙ ВЕКТОРАМ КОРДИНАТ И СКОРОСТИ}
       // vector_type_3;
double VECT_IN[6],  //"ВХОДНОЙ" ВЕКТОР ПРИ РЕШЕНИИ СИСТЕМЫ ШЕСТИ УРАВНЕНИЙ
       VECT_OUT[6]; //"ВЫХОДНОЙ" ВЕКТОР ПРИ РЕШЕНИИ СИСТЕМЫ ШЕСТИ УРАВНЕНИЙ
       // vector_type_6;
double X1, Y1, Z1, //ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ ДЛЯ ПРЕДСТАВЛЕНИЯ КООРДИНАТ
       V1x, V1y, V1z; //ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ ДЛЯ ПРЕДСТАВЛЕНИЯ СКОРОСТИ

     //ПЕРЕМЕННЫЕ ДЛЯ ЭЛЕМЕНТОВ, КОТОРЫМ ДАЮТСЯ ПРИРАЩЕНИЯ:
double INCLIN_NEW, EXCENTR_NEW, MANOMAL_NEW, ANODE_NEW, ARGPERIG_NEW, MMOTION_NEW,
       Epsilon, DeltaParam;

double MATR[6][6]; //МАТРИЦА, ИСПОЛЬЗУЕМАЯ ПРИ РЕШЕНИИ СИСТЕМЫ ШЕСТИ УРАВНЕНИЙ (matrix_type_6)

     //ПОТОК ФАЙЛА ДЛЯ ОТЛАДОЧНОЙ ПЕЧАТИ (ЕСЛИ ОНА РАЗРЕШЕНА):
ofstream ff; // поток для записи в файл результатов


 //ПЕРЕМЕННЫЕ, КОТОРЫЕ ИСПОЛЬЗУЮТСЯ ПРИ ФОРМИРОВАНИИ СТРОК:
string eP,eNofSat,eClsfct,eIntDes,eEpoch,eFirst,eSecon,eBstar,
       eTypeE,eNofEl,eCheck,StrElem1,
       StrBuf,eInclin,eAnode,eExc,eArgP,eMAnom,eMMotion, /*{eNRev,}*/
       eInclin_new,eAnode_new,eExc_new,eArgP_new,eMAnom_new,eMMotion_new, StrElem2;

 //БУФЕРНАЯ СТРОКА:
string StrReadBuf;


double  EARTH_CONST, //ПЕРЕМЕННАЯ ДЛЯ ХРАНЕНИЯ ЗНАЧЕНИЯ ГЕОЦЕНТРИЧЕСКОЙ
                     //ГРАВИТАЦИОННОЙ ПОСТОЯННОЙ, КОТОРАЯ ИСПОЛЬЗУЕТСЯ
                     //ПРИ ВЫЧИСЛЕНИИ НАЧАЛЬНЫХ ЗНАЧЕНИЙ ЭЛЕМЕНТОВ

 VEL_CIRCLE_QUADR, //ПЕРЕМЕННАЯ ДЛЯ ХРАНЕНИЯ КВАДРАТА ПЕРВОЙ
                   //КОСМИЧЕСКОЙ ("КРУГОВОЙ") СКОРОСТИ

 RATIO_VELOC_QUADR, //ПЕРЕМЕННАЯ ДЛЯ ХРАНЕНИЯ ОТНОШЕНИЯ КВАДРАТА СКОРОСТИ
                    //К КВАДРАТУ КРУГОВОЙ СКОРОСТИ
 SEMI_AX,           //ПЕРЕМЕННАЯ ДЛЯ ХРАНЕНИЯ ВЕЛИЧИНЫ ГЛАВНОЙ ПОЛУОСИ
 COS_PHI, //SIN_PHI,
 U_0, //ПЕРЕМЕННАЯ ДЛЯ ХРАНЕНИЯ ЗНАЧЕНИЯ АРГУМЕНТА ШИРОТЫ
 V_0;  //ПЕРЕМЕННАЯ ДЛЯ ХРАНЕНИЯ ЗНАЧЕНИЯ ИСТИННОЙ АНОМАЛИИ

double ANOMAL_E, //e_SIN_E, e_COS_E, TAU, K_0, E_0, C_0,
      SIN_V, COS_V; //ЭТИ ВЕЛИЧИНЫ ДОЛЖНЫ ПРЕДСТАВЛЯТЬ СОБОЙ ЧИСЛИТЕЛЬ И
              //ЗНАМЕНАТЕЛЬ ДРОБИ, ЗНАЧЕНИЕ КОТОРОЙ РАВНО ТАНГЕНСУ ИСТИННОЙ АНОМАЛИИ

 //ПРИЗНАКИ, РАЗРЕШАЮЩИЕ ВЫДАЧУ ДОПОЛНИТЕЛЬНОЙ ИНФОРМАЦИИ
 //(ПРЕДНАЗНАЧЕНЫ ДЛЯ ОТЛАДКИ, А ПРИ ШТАТНОЙ РАБОТЕ ДЛЯ НИХ
 //УСТАНАВЛИВАЕТСЯ ЗНАЧЕНИЕ FALSE)
bool PRINT_MAIN, PRINT_MATR, PRINT_STATE_VECT, PRINT_ELEMENTS;

//Переменные, которые были глобальными в библиотеке for_tle_m.pas:

string HEAD_LINE; //СТРОКА - ЗАГОЛОВОК ЭЛЕМЕНТОВ
double X0,Y0,Z0; //КООРДИНАТЫ
double VX0,VY0,VZ0; //СКОРОСТЬ
//string TEPOCHSTR, TEPOCHSTR1; //ЭПОХА
/*
string NSAT; //НОМЕР СПУТНИКА, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 5 СИМВОЛОВ
string INTDESIGN; //МЕЖДУНАРОДНОЕ ОБОЗНАЧЕНИЕ, ДОЛЖНО СОДЕРЖАТЬ 8 СИМВОЛОВ
string STRBSTAR; //КОЭФФИЦИЕНТ ТОРМОЖЕНИЯ, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 8 СИМВОЛОВ
string NELEM; //НОМЕР ЭЛЕМЕНТОВ, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 4 СИМВОЛА
string NORBIT; //НОМЕР ВИТКА ДЛЯ ЭПОХИ, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 5 СИМВОЛОВ
string E_FIRST; //ПЕРВАЯ ПРОИЗВОДНАЯ, ДОЛЖНА СОДЕРЖАТЬ РОВНО 10 СИМВОЛОВ
string E_SECOND; //ВТОРАЯ ПРОИЗВОДНАЯ, ДОЛЖНА СОДЕРЖАТЬ РОВНО 8 СИМВОЛОВ
*/

//bool  HUNDERT; //ПРИЗНАК ТОГО, ЧТО ПЕРЕМЕННАЯ HU СОДЕРЖИТ СОТЫЕ (ЕСЛИ TRUE)
              //ИЛИ ТЫСЯЧНЫЕ (ЕСЛИ FALSE) ДОЛИ СЕКУНДЫ.
string StrPathFile; //ФАЙЛ, В КОТОРЫЙ ДОЛЖЕН БЫТЬ ЗАПИСАН РЕЗУЛЬТАТ
string StrFileLST; //ИМЯ ФАЙЛА ТИПА LST
string StrFileReport; //ИМЯ ФАЙЛА-ОТЧЕТА
string tlestr1, tlestr2; //ПЕРВАЯ И ВТОРАЯ СТРОКИ РЕЗУЛЬТАТА
int    LASTITERATION; //КОЛИЧЕСТВО ВЫПОЛНЕННЫХ ИТЕРАЦИЙ
//double COORD_GR[3], VELOC_GR[3], COORD_INERT[3], VELOC_INERT[3]; //vector_type_3
//int    intDEEP;
//double dblTIM, dblANGLE;

string StrDiagnos; //СТРОКА ДЛЯ ЗАПИСИ ДИАГНОСТИЧЕСКОГО СООБЩЕНИЯ
                   //ПРИ РАБОТЕ ФУНКЦИИ VECTOTLE

//"Инициализация" основных переменных, соответствующих тем, что задавались в Sgp_init.pas

   string two_line[2]; //array [1..2] of line_data; line_data = string[69];
  double epoch;
  string catnr,elset;
  string obs_name; //obs_name             : string[25];
  //bool selected[const_max_sats];
  string sat_name[const_max_sats]; //array [1..max_sats] of string[24];
  string data_drive, data_dir, work_drive, work_dir;

//"Инициализация" основных переменных, соответствующих тем, что задавались в SGP_INTF.pas

    //int iflag,ideep;
    double julian_epoch;

//Переменные, которые были общими для функций esetElements, VECTOTLE, ResetElemSGP4, VECTLESGP4

double INCLIN, EXCENTR, MANOMAL, ANODE, ARGPERIG, MMOTION;
double /* INCLIN0, ANODE0, ARGPERIG0, MANOMAL0, */ EXCENTR0, MMOTION0;


string eNRev;

double A001,A002,A003,A004,A005,A006,A007,A008,A009,A010,A011,A012,A013,A014;

double RV[3],SS[3];     //double RV[4],SS[4]; на паскале длина этих векторов была 4

stringstream ss_chsum;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//int iexp,ibexp;
//double a1,ao,del1,delo,xnodp,temp;
string abuf[2]; //two_line;

double A0[14];

intReturn = 1;

//ГЕОЦЕНТРИЧЕСКАЯ ГРАВИТАЦИОННАЯ ПОСТОЯНННАЯ
//(взята из списка на стр. 197 "Справочного руководства по небесной
//механике и астродинамике", часть I, глава 4, параграф 4.06)
EARTH_CONST = 398601.86;

Epsilon = 1.0E-8; //для итерационного процесса

  PRINT_MAIN = true;
  PRINT_MATR = true;
  PRINT_STATE_VECT = true;
  PRINT_ELEMENTS = true;

epoch = stod(sd.TEPOCHSTR);
julian_epoch = Libbase::Julian_Date_of_Epoch(epoch); // юлианская дата эпохи

if (bGreenw)
{
    V_COORD_GR[0] = R[0];
    V_COORD_GR[1] = R[1];
    V_COORD_GR[2] = R[2];

    V_VELOC_GR[0] = V[0];
    V_VELOC_GR[1] = V[1];
    V_VELOC_GR[2] = V[2];

    Libbase::Conv_Gr_Inert(V_COORD_GR, V_VELOC_GR, julian_epoch, V_COORD_I, V_VELOC_I);

}
else
{

    V_COORD_I[0] = R[0];
    V_COORD_I[1] = R[1];
    V_COORD_I[2] = R[2];

    V_VELOC_I[0] = V[0];
    V_VELOC_I[1] = V[1];
    V_VELOC_I[2] = V[2];

}

//ЗАПОМИНАЕМ ИСХОДНЫЙ ВЕКТОР КООРДИНАТ
X0 = V_COORD_I[0];
Y0 = V_COORD_I[1];
Z0 = V_COORD_I[2];;

//ЗАПОМИНАЕМ ИСХОДНЫЙ ВЕКТОР СКОРОСТИ
VX0 = V_VELOC_I[0];
VY0 = V_VELOC_I[1];
VZ0 = V_VELOC_I[2];

  //при преобразовании строки в действительное число считается, что
  //дробная часть отделена точкой, а не запятой:
  std::locale().global(std::locale("C"));

          //time_sup = 0.0; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  //ПРЕДВАРИТЕЛЬНАЯ УСТАНОВКА СТРОКИ ДИАГНОСТИКИ:
  StrDiagnos = "No Diagnosis";

  //ПРЕДВАРИТЕЛЬНАЯ УСТАНОВКА ПЕРЕМЕННОЙ, ЗНАЧЕНИЕ КОТОРОЙ В КОНЦЕ
  //РАБОТЫ БУДЕТ ИСПОЛЬЗОВАТЬСЯ КАК ЗНАЧЕНИЕ, ВОЗВРАЩАЕМОЕ ФУНКЦИЕЙ

//  bResult = false;  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  //УСТАНОВКА ПРИЗНАКОВ, РАЗРЕШАЮЩИХ ВЫДАЧУ ДОПОЛНИТЕЛЬНОЙ ИНФОРМАЦИИ
  //В ЦЕЛЯХ ОТЛАДКИ
  //(если выдача не нужна, устанавливается FALSE)

  PRINT_MAIN = true;
  PRINT_MATR = true;
  PRINT_STATE_VECT = true;
  PRINT_ELEMENTS = true;

  //КОПИРУЕМ ИСХОДНЫЙ ВЕКТОР КООРДИНАТ ДЛЯ ПЕРВОНАЧАЛЬНых РАСЧЕТОВ
  V_COORD[0] = X0;
  V_COORD[1] = Y0;
  V_COORD[2] = Z0;

  //КОПИРУЕМ ИСХОДНЫЙ ВЕКТОР СКОРОСТИ ДЛЯ ПЕРВОНАЧАЛЬНЫХ РАСЧЕТОВ
  V_VELOC[0] = VX0;
  V_VELOC[1] = VY0;
  V_VELOC[2] = VZ0;

  //КОПИРУЕМ ИСХОДНЫЙ ВЕКТОР КООРДИНАТ ДЛЯ ПЕРВОНАЧАЛЬНых РАСЧЕТОВ
  V_COORD[0] = V_COORD_I[0];
  V_COORD[1] = V_COORD_I[1];
  V_COORD[2] = V_COORD_I[2];

  //КОПИРУЕМ ИСХОДНЫЙ ВЕКТОР СКОРОСТИ ДЛЯ ПЕРВОНАЧАЛЬНЫХ РАСЧЕТОВ
  V_VELOC[0] = V_VELOC_I[0];
  V_VELOC[1] = V_VELOC_I[1];
  V_VELOC[2] = V_VELOC_I[2];


  //ВЫЧИСЛЕНИЕ ВЕКТОРА, ОРТОГОНАЛЬНОГО К ВЕКТОРАМ КООРДИНАТ И СКОРОСТИ
  Libbase::cross_mult_3(V_COORD,V_VELOC,V_ORTOG);

  //НОРМИРОВКА ОРТОГОНАЛЬНОГО ВЕКТОРА
  Libbase::norm_vect_3(V_ORTOG,V_ORTOG_N);

  //ЗАПОМНИЛИ НАЧАЛЬНОЕ НАКЛОНЕНИЕ
  //(Формула получена из геометрических соображений;
  //в "Справочном руководстве по небесной механике и астродинамике"
  //предлагается другая формула - см. часть III, глава 4, формулы 3.4.09, стр. 284.
  INCLIN = acos(V_ORTOG_N[2]); //ARCCOS

  //КВАДРАТ КРУГОВОЙ СКОРОСТИ
  VEL_CIRCLE_QUADR = EARTH_CONST/Libbase::length_vect_3(V_COORD);

  //ОТНОШЕНИЕ КВАДРАТА СКОРОСТИ К КВАДРАТУ КРУГОВОЙ СКОРОСТИ
  RATIO_VELOC_QUADR = (V_VELOC[0]*V_VELOC[0]+V_VELOC[1]*V_VELOC[1]+V_VELOC[2]*V_VELOC[2])/(VEL_CIRCLE_QUADR);

  //БОЛЬШАЯ ПОЛУОСЬ
  //(такая формула есть, например, в книге "Основы теории полета и
  //элементы проектирования искусственных спутников Земли", стр. 24, формула 1.31,
  //и в книге "Справочное руководство по небесной механике и астродинамике",
  //начало главы 4 части III, стр.283.)
  SEMI_AX = Libbase::length_vect_3(V_COORD)/(2-RATIO_VELOC_QUADR);

  //КОСИНУС УГЛА МЕЖДУ ВЕКТОРОМ КООРДИНАТ И ВЕКТОРОМ СКОРОСТИ
  //(вычисляется с помощью скалярного произведения.)
  COS_PHI=(V_COORD[0]*V_VELOC[0]+V_COORD[1]*V_VELOC[1]+V_COORD[2]*V_VELOC[2])/(Libbase::length_vect_3(V_COORD)*(sqrt(V_VELOC[0]*V_VELOC[0]+V_VELOC[1]*V_VELOC[1]+V_VELOC[2]*V_VELOC[2])));

  //ЗАПОМНИЛИ НАЧАЛЬНЫЙ ЭКСЦЕНТРИСИТЕТ
  //(Формула для эксцентриситета взята из "Справочного руководства
  //по небесной механике и астродинамике", часть III, глава 4, стр.284, формула 3.4.03.

  //EXCENTR := (RATIO_VELOC_QUADR - 1)*(RATIO_VELOC_QUADR - 1.0/RATIO_VELOC_QUADR);
  //закомментированная выше строка использовалась сначала
  EXCENTR = (RATIO_VELOC_QUADR - 1.0)*(RATIO_VELOC_QUADR - 1.0);
  EXCENTR = EXCENTR + Libbase::length_vect_3(V_COORD)*RATIO_VELOC_QUADR*COS_PHI*COS_PHI/SEMI_AX;
  EXCENTR = sqrt(EXCENTR);

  //cout << "EXCENTR:: " << EXCENTR << endl;

  //ЗАПОМНИЛИ НАЧАЛЬНУЮ ДОЛГОТУ ВОСХОДЯЩЕГО УЗЛА
  //(Эту формулу можно получить из формул 3.4.09 "Справочного руководства
  //по небесной механике и астродинамике", часть III, глава 4, стр. 284.)
  ANODE=Libtle::AcTan(V_ORTOG[0],(-V_ORTOG[1]));

  //ПОЛУЧАЕМ СРЕДНЕЕ ДВИЖЕНИЕ (НЕ В ВИДЕ КОЛИЧЕСТВА ОБОРОТОВ ЗА СУТКИ)
  //(Такая формула есть, например, в "Справочном руководстве по небесной
  //механике и астродинамике", часть II, глава 2, формулы 2.2.03, стр. 221.)
  MMOTION = sqrt(EARTH_CONST/(SEMI_AX*SEMI_AX*SEMI_AX));

  //ВЫЧИСЛЕНИЕ ИСТИННОЙ, ЭКСЦЕНТРИЧЕСКОЙ И СРЕДНЕЙ АНОМАЛИЙ
  //(С ВАРИАНТАМИ ВЫЧИСЛЕНИЯ ИСТИННОЙ АНОМАЛИИ)

  //Величины SIN_V и COS_V представляют собой числитель и знаменатель дроби,
  //значение которой равно тангенсу истинной аномалии.
  //Формулы взяты из "Справочного руководства по небесной механике и
  //астродинамике", часть III, глава 4, формулы 3.4.07, стр. 284.
  SIN_V = Libbase::dot_mult_3(V_COORD,V_VELOC)*sqrt(SEMI_AX*(1.0-EXCENTR*EXCENTR)/EARTH_CONST);
  COS_V = SEMI_AX*(1.0-EXCENTR*EXCENTR) - sqrt(X0*X0+Y0*Y0+Z0*Z0);

  //ВЫЧИСЛЯЕМ ИСТИННУЮ АНОМАЛИЮ}
  //Из "Справочного руководства по небесной механике и астродинамике",
  //часть III, глава 4, формулы 3.4.07, стр. 284; см. комментарий выше.
  V_0 = Libtle::AcTan(SIN_V,COS_V);

  //ВЫЧИСЛЯЕМ ЭКСЦЕНТРИЧЕСКУЮ АНОМАЛИЮ
  //Получено из формулы 2.2.08 главы 2 части II "Справочного руководства
  //по небесной механике и астродинамике", стр.222.
  ANOMAL_E = 2.0E0*atan(tan(V_0*0.5E0)*sqrt((1.0-EXCENTR)/(1.0+EXCENTR))); //ARCTAN, TAN

  //cout << "EXCENTR " << EXCENTR << endl;
  //cout << "ANOMAL_E " << ANOMAL_E << endl;

  //ЗАПОМИНАЕМ НАЧАЛЬНУЮ СРЕДНЮЮ АНОМАЛИЮ
  //(Эту формулу можно найти, например, на стр.222 "Справочного руководства}
  //по небесной механике и астродинамике", глава 2, часть II, формула 2.2.06.)}
  MANOMAL=Libtle::norm_angl_pos(ANOMAL_E-EXCENTR*sin(ANOMAL_E));

  //ВЫЧИСЛЯЕМ АРГУМЕНТ ШИРОТЫ
  //Формула получена из геометрических соображений.
  //В "Справочном руководстве по небесной механике и астродинамике"
  //приведена другая формула  - см. стр. 285, часть III, глава 4, формула 3.4.11.
  U_0 = Libtle::ARCSIN(V_COORD[2]/( Libbase::length_vect_3(V_COORD)*sin(INCLIN) ) );

  //ЗАПОМНИЛИ НАЧАЛЬНЫЙ АРГУМЕНТ ПЕРИГЕЯ
  //Формулу можно найти, например в "Справочном руководстве по небесной
  //механике и астродинамике", см. стр. 285, часть III, глава 4, формула 3.4.10.
  ARGPERIG = U_0 - V_0;
  ARGPERIG = Libtle::norm_angl_pos(ARGPERIG);

  //ЗАПОМНИЛИ НАЧАЛЬНОЕ СРЕДНЕЕ ДВИЖЕНИЕ В ВИДЕ КОЛИЧЕСТВА ОБОРОТОВ ЗА СУТКИ
  //MMOTION = (sqrt(EARTH_CONST/(SEMI_AX*SEMI_AX*SEMI_AX))*86400.0)/const_TWOPI;
  MMOTION = MMOTION*86400.0/const_TWOPI;

//  cout << "mark 0" << endl;
//  cout << "log: " << (PRINT_MAIN || (PRINT_ELEMENTS || (PRINT_STATE_VECT || PRINT_MATR))) << endl;

  if ((PRINT_MAIN || (PRINT_ELEMENTS || (PRINT_STATE_VECT || PRINT_MATR)))&&bPrint)
  {
  //    cout << "print 1" << endl;
   ff.open(strFile, ios::out); //открытие потока для записи результатов в файл
   ff << std::setprecision(8) << endl;
  };

  if ((PRINT_MAIN)&&bPrint)
  {
      ff << "IITIAL STATE VECTOR:" << endl;
      ff << "X: " << R[0] << " Y: " << R[1] << " Z: " << R[2] << endl;
      ff << "VX: " << V[0] << " VY: " << V[1] << " VZ: " << V[2] << endl;

      ff << "TRUE STATE VECTOR:" << endl;
      ff << "X: " << V_COORD[0] << " Y: " << V_COORD[1] << " Z: " << V_COORD[2] << endl;
      ff << "VX: " << V_VELOC[0] << " VY: " << V_VELOC[1] << " VZ: " << V_VELOC[2] << endl;
  };

  StrElem1 = Libtle::form_string_1(sd.NSAT,sd.INTDESIGN, sd.TEPOCHSTR, sd.E_FIRST, sd.E_SECOND, sd.STRBSTAR, sd.NELEM);
  StrElem2 = Libtle::form_string_2(sd.NSAT, INCLIN, ANODE, EXCENTR, ARGPERIG, MANOMAL, MMOTION, sd.NORBIT);

                                   //ОТЛАДОЧНАЯ ПЕЧАТЬ (ЕСЛИ РАЗРЕШЕНО) ПОЛУЧЕННЫХ СТРОК
  if ((PRINT_MAIN)&&bPrint)
  {
   ff << StrElem1 << endl;
   ff << StrElem2 << endl;
  };

  ANODE = Libtle::norm_angl_pos(ANODE);
  ARGPERIG = Libtle::norm_angl_pos(ARGPERIG);
  MANOMAL = Libtle::norm_angl_pos(MANOMAL);

  A001 = 2000.0000000;
  A003 = 0.0003000;
  A004 = 0.0000000;
  A005 = 0.0002800;
//   A005 = bstar;
  A006 = 0.0000000;
  A007 = 900.0000000;
  A008 = INCLIN;
  A009 = ANODE;
  A010 = EXCENTR;
  A011 = ARGPERIG;
  A012 = MANOMAL;
  A013 = MMOTION;
//   VAL(eNRev+'.0',A014,code);


  //INCLIN0 = INCLIN;
  //ANODE0 = ANODE;
  //ARGPERIG0 = ARGPERIG;
  //MANOMAL0 = MANOMAL;

  EXCENTR0 = EXCENTR; // запоминание первоначальных оценок
  MMOTION0 = MMOTION;

  counter = 1;        //первоначальное значение счетчика итераций

   while(counter<=(1000+1)) //будет сделано не более указанного числа итераций
 { //-------------НАЧАЛО ЦИКЛА ИТЕРАЦИЙ----------------

       if (counter == (1000+1)) { return 1000;};

       if ((PRINT_MAIN)&&bPrint) { ff << "ITERATION " << counter << endl;  };


//      ttime = julian_epoch;

/*
         std::cout  << "A001: " << A001 << std::endl;
         std::cout  << "A002: " << A002 << std::endl;
         std::cout  << "A003: " << A003 << std::endl;
         std::cout  << "A004: " << A004 << std::endl;
         std::cout  << "A005: " << A005 << std::endl;
         std::cout  << "A006: " << A006 << std::endl;
         std::cout  << "A007: " << A007 << std::endl;
         std::cout  << "A008: " << A008 << std::endl;
         std::cout  << "A009: " << A009 << std::endl;
         std::cout  << "A010: " << A010 << std::endl;
         std::cout  << "A011: " << A011 << std::endl;
         std::cout  << "A012: " << A012 << std::endl;
         std::cout  << "A013: " << A013 << std::endl;
         std::cout  << "A014: " << A014 << std::endl;
*/
         if ((PRINT_MAIN)&&bPrint)
         {
         ff  << "A001: " << A001 << std::endl;
         A002 = 1.0;
         ff  << "A002: " << A002 << std::endl;
         ff  << "A003: " << A003 << std::endl;
         ff  << "A004: " << A004 << std::endl;
         ff  << "A005: " << A005 << std::endl;
         ff  << "A006: " << A006 << std::endl;
         ff  << "A007: " << A007 << std::endl;
         ff  << "A008: " << A008 << std::endl;
         ff  << "A009: " << A009 << std::endl;
         ff  << "A010: " << A010 << std::endl;
         ff  << "A011: " << A011 << std::endl;
         ff  << "A012: " << A012 << std::endl;
         ff  << "A013: " << A013 << std::endl;
         if (A013 < 0.0) {A013 = - A013; ff  << "A013: " << A013 << std::endl;}
         //if (A013 >20.0) {A013 = 15.0;}
         //ff  << "A013: " << A013 << std::endl;
         A014 = 1.0;
         ff  << "A014: " << A014 << std::endl;

         if (isnan(A008)) { //cout << "A008 == NAN"; ff << "A008 == NAN";
                           return 1000; };
         };

      //Вычисления

      A0[0]=A001; A0[1]=A002; A0[2]=A003; A0[3]=A004; A0[4]=A005;
      A0[5]=A006; A0[6]=A007; A0[7]=A008; A0[8]=A009; A0[9]=A010;
      A0[10]=A011; A0[11]=A012; A0[12]=A013; A0[13]=A014;

Libbase::PrognSGP4 (RV, SS, 0.0, A0);

  pos[0] = RV[0];
  pos[1] = RV[1];
  pos[2] = RV[2];
  vel[0] = SS[0];
  vel[1] = SS[1];
  vel[2] = SS[2];

/*
  cout  << "CURRENT STATE VECTOR" << endl;
  cout << "X: " << pos[0] << endl;
  cout << "Y: " << pos[1] << endl;
  cout << "Z: " << pos[2] << endl;
  cout  << endl;
  cout << "VX: " << vel[0] << endl;
  cout << "VY: " << vel[1] << endl;
  cout << "VZ: " << vel[2] << endl;
*/

  //ПРИСВОЕНИЕ НОВЫХ ЗНАЧЕНИЙ ПЕРЕМЕННЫМ, ПРЕДСТАВЛЯЮЩИМ
  //КООРДИНАТЫ И СКОРОСТЬ

  X1 = pos[0];
  Y1 = pos[1];
  Z1 = pos[2];

  V1x = vel[0];
  V1y = vel[1];
  V1z = vel[2];


  //ФОРМИРОВАНИЕ "ВХОДНОГО" ВЕКТОРА ДЛЯ СИСТЕМЫ УРАВНЕНИЙ

  VECT_IN[0] = -(X0-X1);
  VECT_IN[1] = -(Y0-Y1);
  VECT_IN[2] = -(Z0-Z1);

  VECT_IN[3] = -(VX0-V1x);
  VECT_IN[4] = -(VY0-V1y);
  VECT_IN[5] = -(VZ0-V1z);


  //ПРОВЕРКА, НЕ ДОСТИГНУТЫ ЛИ ЗАДАННЫЕ ВЕЛИЧИНЫ ТОЧНОСТИ ПО КООРДИНАТАМ И ПО СКОРОСТИ
 if (((VECT_IN[0]*VECT_IN[0]+VECT_IN[1]*VECT_IN[1]+VECT_IN[2]*VECT_IN[2])<2.5E-9)
   && ((VECT_IN[3]*VECT_IN[3]+VECT_IN[4]*VECT_IN[4]+VECT_IN[5]*VECT_IN[5])<2.5E-17))

   //if (((VECT_IN[0]*VECT_IN[0]+VECT_IN[1]*VECT_IN[1]+VECT_IN[2]*VECT_IN[2])<2.5E-5)
   // && ((VECT_IN[3]*VECT_IN[3]+VECT_IN[4]*VECT_IN[4]+VECT_IN[5]*VECT_IN[5])<2.5E-12))

{
     //ЗАПОМИНАЕМ КОЛИЧЕСТВО ИТЕРАЦИЙ
     LASTITERATION = counter - 1;


     StrElem1 = Libtle::form_string_1(sd.NSAT,sd.INTDESIGN, sd.TEPOCHSTR, sd.E_FIRST, sd.E_SECOND, sd.STRBSTAR, sd.NELEM);

     StrElem2 = Libtle::form_string_2(sd.NSAT, INCLIN, ANODE, EXCENTR, ARGPERIG,
               MANOMAL, MMOTION, sd.NORBIT);

     tle_rez[0] = StrElem1;
     tle_rez[1] = StrElem2;

  //ПЕЧАТЬ (ЕСЛИ РАЗРЕШЕНО) СООБЩЕНИЯ ОБ ОКОНЧАНИИ РАБОТЫ}
  if ((PRINT_MAIN)&&bPrint)
  {
  ff << endl << "THE CALCULATION HAS BEEN FINISHED SUCCESSFULLY" << endl;
  ff << "THE NUMBER OF ITERATIONS IS: " << LASTITERATION << endl;
  ff << "THE TWO-LINE ELEMENTS ARE:" << endl;
  ff << StrElem1 << endl;
  ff << StrElem2 << endl;
  };


  //ЗАМЕНА ПРОБЕЛОВ НУЛЯМИ В СТРОКЕ, СОДЕРЖАЩЕЙ ЗНАЧЕНИЕ ЭКСЦЕНТРИСИТЕТА}
  //eExc = Libtle::Replace(eExc," ","0");

  //cout << Libtle::Replace("ACACAC", "A", "B") << endl; отладочная проверка функции
  //ПРИСВОЕНИЕ ЗНАЧЕНИЯ TRUE ВЕЛИЧИНЕ, КОТОРАЯ БУДЕТ ВОЗВРАЩЕНА
  //В КАЧЕСТВЕ ЗНАЧЕНИЯ ФУНКЦИИ И ВЫХОД ИЗ ЦИКЛА ИТЕРАЦИЙ

  //bResult := true;

  // cout << "end of iterations (condition)" << endl;


   break;

} // конец условия if (((VECT_IN[0]*VECT_IN[0]+VECT... и т.д.


  //ФОРМИРОВАНИЕ ПЕРВОЙ КОЛОНКИ МАТРИЦЫ СИСТЕМЫ УРАВНЕНИЙ (ДЛЯ НАКЛОНЕНИЯ)


  if (INCLIN < Epsilon) {INCLIN_NEW = Epsilon;}
  else
     { INCLIN_NEW = INCLIN + INCLIN * Epsilon; };

 //INCLIN_NEW = INCLIN + INCLIN * Epsilon;

 DeltaParam = INCLIN*Epsilon;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  A008 = INCLIN_NEW; A0[7]=A008;
  Libbase::PrognSGP4 (pos, vel, 0.0, A0);
  A008 = INCLIN; A0[7]=A008;

  MATR[0][0] = (X1-pos[0])/DeltaParam;
  MATR[1][0] = (Y1-pos[1])/DeltaParam;
  MATR[2][0] = (Z1-pos[2])/DeltaParam;
  MATR[3][0] = (V1x-vel[0])/DeltaParam;
  MATR[4][0] = (V1y-vel[1])/DeltaParam;
  MATR[5][0] = (V1z-vel[2])/DeltaParam;

  //ФОРМИРОВАНИЕ ВТОРОЙ КОЛОНКИ МАТРИЦЫ СИСТЕМЫ УРАВНЕНИЙ (ДЛЯ ДОЛГОТЫ ВОСХОДЯЩЕГО УЗЛА)

  if (ANODE < Epsilon) {ANODE_NEW = Epsilon;}
                  else { ANODE_NEW = ANODE + ANODE * Epsilon;};

  DeltaParam = ANODE*Epsilon;

  ANODE_NEW = Libtle::norm_angl_pos(ANODE_NEW);

  A009 = ANODE_NEW; A0[8]=A009;
  Libbase::PrognSGP4 (pos, vel, 0.0, A0);
  A009 = ANODE; A0[8]=A009;

  MATR[0][1] = (X1-pos[0])/DeltaParam;
  MATR[1][1] = (Y1-pos[1])/DeltaParam;
  MATR[2][1] = (Z1-pos[2])/DeltaParam;
  MATR[3][1] = (V1x-vel[0])/DeltaParam;
  MATR[4][1] = (V1y-vel[1])/DeltaParam;
  MATR[5][1] = (V1z-vel[2])/DeltaParam;

  //ФОРМИРОВАНИЕ ТРЕТЬЕЙ КОЛОНКИ МАТРИЦЫ СИСТЕМЫ УРАВНЕНИЙ (ДЛЯ ЭКСЦЕНТРИСИТЕТА)

  if (EXCENTR < Epsilon) {EXCENTR_NEW = Epsilon;}
                    else { EXCENTR_NEW = EXCENTR + EXCENTR * Epsilon; };

  DeltaParam = EXCENTR*Epsilon;

  A010 = EXCENTR_NEW; A0[9]=A010;
  Libbase::PrognSGP4 (pos, vel, 0.0, A0);
  A010 = EXCENTR; A0[9]=A010;

  MATR[0][2] = (X1-pos[0])/DeltaParam;
  MATR[1][2] = (Y1-pos[1])/DeltaParam;
  MATR[2][2] = (Z1-pos[2])/DeltaParam;
  MATR[3][2] = (V1x-vel[0])/DeltaParam;
  MATR[4][2] = (V1y-vel[1])/DeltaParam;
  MATR[5][2] = (V1z-vel[2])/DeltaParam;


  //ФОРМИРОВАНИЕ ЧЕТВЕРТОЙ КОЛОНКИ МАТРИЦЫ СИСТЕМЫ УРАВНЕНИЙ (ДЛЯ АРГУМЕНТА ПЕРИГЕЯ)

  if (ARGPERIG < Epsilon) {ARGPERIG_NEW = Epsilon;}
                     else { ARGPERIG_NEW = ARGPERIG + ARGPERIG * Epsilon;};

  DeltaParam = ARGPERIG*Epsilon;

  A011 = ARGPERIG_NEW; A0[10]=A011;
  Libbase::PrognSGP4 (pos, vel, 0.0, A0);
  A011 = ARGPERIG; A0[10]=A011;

  MATR[0][3] = (X1-pos[0])/DeltaParam;
  MATR[1][3] = (Y1-pos[1])/DeltaParam;
  MATR[2][3] = (Z1-pos[2])/DeltaParam;
  MATR[3][3] = (V1x-vel[0])/DeltaParam;
  MATR[4][3] = (V1y-vel[1])/DeltaParam;
  MATR[5][3] = (V1z-vel[2])/DeltaParam;

  //ФОРМИРОВАНИЕ ПЯТОЙ КОЛОНКИ МАТРИЦЫ СИСТЕМЫ УРАВНЕНИЙ (ДЛЯ СРЕДНЕЙ АНОМАЛИИ)

  if (MANOMAL < Epsilon) {MANOMAL_NEW = Epsilon;}
                    else {MANOMAL_NEW = MANOMAL + MANOMAL * Epsilon;};

  DeltaParam = MANOMAL*Epsilon;

  A012 = MANOMAL_NEW; A0[11]=A012;
  Libbase::PrognSGP4 (pos, vel, 0.0, A0);
  A012 = MANOMAL; A0[11]=A012;

  MATR[0][4] = (X1-pos[0])/DeltaParam;
  MATR[1][4] = (Y1-pos[1])/DeltaParam;
  MATR[2][4] = (Z1-pos[2])/DeltaParam;
  MATR[3][4] = (V1x-vel[0])/DeltaParam;
  MATR[4][4] = (V1y-vel[1])/DeltaParam;
  MATR[5][4] = (V1z-vel[2])/DeltaParam;

  //ФОРМИРОВАНИЕ ШЕСТОЙ КОЛОНКИ МАТРИЦЫ СИСТЕМЫ УРАВНЕНИЙ (ДЛЯ СРЕДНЕГО ДВИЖЕНИЯ)

  if (MMOTION < Epsilon) {MMOTION_NEW = Epsilon;}
                    else {MMOTION_NEW = MMOTION + MMOTION * Epsilon;};

  DeltaParam = MMOTION*Epsilon;

  A013 = MMOTION_NEW; A0[12]=A013;
  Libbase::PrognSGP4 (pos, vel, 0.0, A0);
  A013 = MMOTION; A0[12]=A013;

  MATR[0][5] = (X1-pos[0])/DeltaParam;
  MATR[1][5] = (Y1-pos[1])/DeltaParam;
  MATR[2][5] = (Z1-pos[2])/DeltaParam;
  MATR[3][5] = (V1x-vel[0])/DeltaParam;
  MATR[4][5] = (V1y-vel[1])/DeltaParam;
  MATR[5][5] = (V1z-vel[2])/DeltaParam;

  //ЕСЛИ СИСТЕМУ НЕ УДАЕТСЯ РЕШИТЬ, ВЫПОЛНЕНИЕ ИТЕРАЦИЙ ПРЕРЫВАЕТСЯ
  //И (ЕСЛИ РАЗРЕШЕНО) В ФАЙЛ ВЫДАЕТСЯ СООБЩЕНИЕ ОБ ОШИБКЕ
  if (Libbase::solv_syst_6(MATR,VECT_IN,VECT_OUT) == false)
  {
    StrDiagnos = "SOLV_SYST_6 FUNCTION RETURNS FALSE!!!";
    if (PRINT_MAIN) {ff << StrDiagnos << endl;};
     break; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  };
/*
  {ПЕЧАТЬ (ЕСЛИ РАЗРЕШЕНО) РЕШЕНИЯ СИСТЕМЫ УРАВНЕНИЙ}
  IF PRINT_MATR THEN
  BEGIN
  WRITELN(ff);
  WRITELN(ff,'VECT_OUT:');
  PRINT_VECT_6(VECT_OUT,ff);
  WRITELN(ff);
  END;
*/

    //ПРИРАЩЕНИЕ ЭЛЕМЕНТОВ
    INCLIN = INCLIN + VECT_OUT[0];
    ANODE  = ANODE +  VECT_OUT[1];
    EXCENTR = EXCENTR+ VECT_OUT[2];
    ARGPERIG = ARGPERIG + VECT_OUT[3];
    MANOMAL = MANOMAL+ VECT_OUT[4];
    MMOTION = MMOTION+ VECT_OUT[5];

  //МЕРА ПРОТИВ ПОЯВЛЕНИЯ ОТРИЦАТЕЛЬНОГО ЭКСЦЕНТРИСИТЕТА
  if (EXCENTR < 0) { EXCENTR = 0.000001; };

  ANODE = Libtle::norm_angl_pos(ANODE);
  ARGPERIG = Libtle::norm_angl_pos(ARGPERIG);
  MANOMAL = Libtle::norm_angl_pos(MANOMAL);

  //if (abs(INCLIN - INCLIN0) > abs(INCLIN0*0.5)) {INCLIN = INCLIN0;}
  //if (abs(ANODE - ANODE0) > abs(ANODE0*0.5)) {ANODE = ANODE0;}
  //if (abs(EXCENTR - EXCENTR0) > abs(EXCENTR0*0.5)) {EXCENTR = EXCENTR0;}
  //if (abs(ARGPERIG - ARGPERIG0) > abs(ARGPERIG0*0.5)) {ARGPERIG = ARGPERIG0;}
  //if (abs(MANOMAL - MANOMAL0) > abs(MANOMAL0*0.5)) {MANOMAL = MANOMAL0;}
  if (abs(MMOTION - MMOTION0) > abs(MMOTION0*0.5)) {MMOTION = MMOTION0;} // недопущение чрезмерного отклонения
                                                                         // ср. движения от первоначальной оценки

  if (abs(EXCENTR) >= 0.9) {EXCENTR = EXCENTR0;} // недопущение некорректной величины эксцентриситета
                                                 // и возвращение к ее первоначальной оценке
         A008 = INCLIN;
         A009 = ANODE;
         A010 = EXCENTR;
         A011 = ARGPERIG;
         A012 = MANOMAL;
         A013 = MMOTION;



//      A0[7]=A008; A0[8]=A009; A0[9]=A010; A0[10]=A011; A0[11]=A012; A0[12]=A013; A0[13]=A014;



    counter = counter + 1; //приращение счетчика итераций



}; //-------------ОКОНЧАНИЕ ЦИКЛА ИТЕРАЦИЙ----------------


  if ((PRINT_MAIN || PRINT_ELEMENTS || PRINT_STATE_VECT || PRINT_MATR)&&bPrint) { ff.close(); };

/*
  cout << "counter " << counter << endl;
  cout << "INCLIN " << Libtle::rad_to_grad(INCLIN) << endl;
  cout << "ANODE " << ANODE << endl;
  cout << "EXCENTR " << EXCENTR << endl;
  cout << "ARGPERIG " << ARGPERIG << endl;
  cout << "MANOMAL " << MANOMAL << endl;
  cout << "MMOTION " << MMOTION << endl << endl;

  cout << "mark end" << endl;
*/

intReturn = 0;
return intReturn;

} // vec_to_tle

void Libtle::Progn4(int int3Date[3], int int4Time[4], double A0[14], double j_epoch, double posInert[3], double velInert[3], double posGr[3], double velGr[3], double* high, double* longit, double* latit)
//Libtle::Progn4(int3Date, int4Time, A0, julian_epoch, posInert[3], velInert[3], posGr[3], velGr[3], &high, &longit, &latit);
/*
"Расширенная" функция прогноза положения, скорости и геодезических координат КА с более наглядными входными параметрами.
Данные в массиве A0 должны быть заранее подготовлены, исходя из TLE (например, с помощью функции PreparePrognSGP4).
Заранее должно быть определено и юлианское время эпохи TLE (например, с помощью той же функции).
Момент времени, для которого нужно сделать расчеты, задается в виде даты и времени - первые параметры функции.
В задданные массивы и переменные записываются результаты: векторы координат и скорости в в гринвичской и инерциальной системах,
высота КА, долгота и широта его подспутниковой точки.
*/
{
        double time_after_epoch;

        time_after_epoch = Libtle::Date_Time_for_PROGN_arr(int3Date, int4Time,j_epoch);

        Libbase::PrognSGP4 (posInert, velInert, time_after_epoch, A0);

        Libbase::Conv_Inert_Gr(posInert, velInert, j_epoch+time_after_epoch/86400.0, posGr, velGr);

        Libbase::decart_to_ell(posGr, 1.0/298.257839303, 6378.136, high, longit, latit);
        *longit = Libtle::lambda_360_to_180(*longit);

} // Progn4

double Libtle::Gr_Lat_to_Pix_d(double Lat, double AreaHight)
/*
Функция для преобразования значения географической широты (диапазон (-90.0)-(+90.00)
в "пиксельные" экранные координаты так, чтобы диапазон широты соответствовал заданной
высоте (диапазону) отображения AreaHight. Результат преобразования выдается в экранных
координатах, направление отсчета - сверху вниз.
*/
{
    double grad;

    grad = AreaHight/180.0;
    if (Lat>=0.0) {return (90.0*grad - Lat*grad);} else {return (90*grad + (-Lat)*grad);};

}

double Libtle::Gr_Long_to_Pix_d(double Long, double AreaWidth)
/*
Функция для преобразования значения географической долготы (диапазон (-180.0)-(+180.00)
в "пиксельные" экранные координаты так, чтобы диапазон долготы соответствовал заданной
ширине (диапазону) отображения AreaWidth. Результат преобразования выдается в экранных
координатах, направление отсчета - слева направо.
*/

{
    double grad;

    grad = AreaWidth/360.0;
    if (Long<=0.0) {return (180.0*grad - (-Long)*grad);} else {return (180*grad + (Long)*grad);};
}


