#ifndef LIBTLE_H
#define LIBTLE_H

#include <QDate>
#include <QTime>
#include <QDateTime>

#include <iostream>
#include <fstream>
#include <sstream>  // для работы со строковыми потоками
#include <string>
#include <cmath>

#include <algorithm>  // чтобы для string можно было использовать replace
#include <iomanip>  // для управления выводом чисел в поток

#include <vector>

#include "libbase.h"

using namespace std ;


typedef struct struct_date {
int year;
int month;
int day;
} struct_date;

typedef struct struct_time {
double hour;
double min;
double sec;
} struct_time;

typedef struct str_data_tle {
    string NSAT; //НОМЕР СПУТНИКА, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 5 СИМВОЛОВ
    string INTDESIGN; //МЕЖДУНАРОДНОЕ ОБОЗНАЧЕНИЕ, ДОЛЖНО СОДЕРЖАТЬ 8 СИМВОЛОВ
    string STRBSTAR; //КОЭФФИЦИЕНТ ТОРМОЖЕНИЯ, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 8 СИМВОЛОВ
    string NELEM; //НОМЕР ЭЛЕМЕНТОВ, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 4 СИМВОЛА
    string NORBIT; //НОМЕР ВИТКА ДЛЯ ЭПОХИ, ДОЛЖЕН СОДЕРЖАТЬ РОВНО 5 СИМВОЛОВ
    string E_FIRST; //ПЕРВАЯ ПРОИЗВОДНАЯ, ДОЛЖНА СОДЕРЖАТЬ РОВНО 10 СИМВОЛОВ
    string E_SECOND; //ВТОРАЯ ПРОИЗВОДНАЯ, ДОЛЖНА СОДЕРЖАТЬ РОВНО 8 СИМВОЛОВ
    string TEPOCHSTR; //ЭПОХА ДЛЯ ДВУХСТРОЧНЫХ ЭЛЕМЕНТОВ, ДОЛЖНА СОДЕРЖАТЬ РОВНО 14 СИМВОЛОВ
} str_data_tle;

class Libtle
{

public:
    Libtle();
static bool read_tle(string strDataFile, string sat_data[2], string* NAME_TLE);
    //чтение двухстрочных элементов (TLE) из заданного файла в строковаые переменные
static int  read_date_arr(string StrDate, string StrMark, int int3Date[3]); //чтение даты из строки
static struct_date read_date(string StrDate, int p_y, int l_y, int p_m, int p_d); //чтение даты из позиций строки
static void read_date_arr_alt(string StrDate, int p_y, int l_y, int p_m, int p_d, int int3Date[3]); //чтение даты из позиций строки
static int  read_time_arr(string StrTime, string StrMark, int int4Time[4]); //чтение времени из строки
static struct_time read_time(string StrTime, int p_h, int p_m, int p_s, int p_ss, int l_pss); //чтение времени из позиций строки
static void read_time_arr_alt(string StrTime, int p_h, int p_m, int p_s, int p_ss, int l_ss, int int4Time[4]); //чтение времени из позиций строки
static int Date_to_Arr(int Y, int M, int D, int int3Date[3]); //для ввода даты в массив
static int Time_to_Arr(int H, int M, int S, int SS, int int4Time[4]); //для ввода времени в массив
static int Arr_to_Date(int int3Date[3], int Y, int M, int D); //для ввода даты в массив
static int Arr_to_Time(int int4Time[4], int H, int M, int S, int SS); //для ввода времени в массив
static string Date_to_Str_arr(int int3Date[3]); //преобразование даты из массива в строку
static string Time_to_Str_arr(int int4Time[4]); //преобразование времени из массива в строку
static void Curr_Date_Time_Loc(int int3Date[3], int int4Time[4]); //занесение текущих местных даты и времени в массивы
static void Curr_Date_Time_UTC(int int3Date[3], int int4Time[4]); //занесение текущих гринвичских даты и времени в массивы
static void Convert_Blanks(string* field); //замена символов пробела символами нуля
static int  Integer_Value(string buffer, int start, int leng); //преобразование строки в целое число
static double Real_Value(string buffer, int start, int leng); //преобразование строки в действительное число
static double lambda_360_to_180(double lam360); //преобразование в диапазоне -180 до +180 градусов (для долготы)
static double AcTan(double sinx, double cosx); //дополнительная функция для расчета арктангенса
static double norm_angl(double A); //приведение угла (в радианах) к диапазону (-пи, +пи)
static double norm_angl_pos(double A); //приведение угла (в радианах) к диапазону (0 , +2*пи)
static double rad_to_grad(double A); //перевод радианов в градусы
static double grad_to_rad(double A); //перевод градусов в радианы
static double TANGENS(double A); //дополнительная функция тангенса
static double ARCSIN(double A); //дополнительная функция арксинуса
static double ARCCOS(double A); //дополнительная функция арккосинуса
static string Chcksm(string line); //контрольная сумма строки TLE
static string Replace(string line, string A, string B); //замена подстрок (обычно из одного символа) в заданной строке
static string Double_to_Str(double d_Var, int i_width, int i_prec); //преобразование действительного числа в строку указанной ширины,
                                                                    //с указанным числом знаков после десятичной точки
static string Int_to_Str(int i_Var, int i_width); //преобразование целого числа в строку указанной ширины
static void CalcVect(string sat_data[2], double poss[3], double vell[3]); //расчет вектора, исходя из TLE
static void PreparePrognSGP4(string sat_data[2], double* j_epoch, double A0[14]); //подготовка к вызову функции бал. прогноза
static string form_string_1(string NSAT, string INTDESIGN, string TEPOCHSTR,
     string EFIRST, string ESECOND, string STRBSTAR, string NELEM); //формирование строки 1 TLE
static string form_string_2(string NSAT, double INCLIN, double ANODE,
       double EXCENTR, double ARGPERIG, double MANOMAL, double MMOTION,
       string NORBIT);  //формирование строки 2 TLE
static double BSTARcalc(double B); //расчет бал. коэффициента BSTAR для TLE
static double fromBSTARcalc(double BSTAR); //расчет "обычного" бал. коэффициента по BTSAR
static string BSTAR_to_str(double dBSTAR); //представление BSTAR в строковом виде
static int formLSTshort(string strFSOURCE, string strFRESULT, double base_t, double delta_t, int NN, string strLat, string strLon, int argc, char *argv[]);
           //формирование файла в упрощенном формате LST, исходя из двухстрочных элементов.
static string Date_Time_to_epoch_arr(int int3Date[3], int int4Time[4]);
           //преобразование даты и времени, заданных в массивах, в строку
           //в формате эпохи двухстрочных элементов
static void Date_Time_offset_arr(int int3dat[3], int int4time[4], double offset_sec, int int3dat_rez[3], int int4time_rez[4]);
           //"смещение" даты и времени, заданных в массивах, на заданную величину
static bool copy_text_file(string strFile1, string strFile2); //копирование текстового файла
static bool convert_LST(string strFile1, string strFile2); //преобразование файла LST в упрощенный формат
static bool convert_1_BNI_UTC(string strFile1, string strFile2); //преобразование файла 1_BNI_UTC в упрощенный формат LST
static double Date_Time_for_PROGN_arr(int int3dat[3], int int4time[4], double jul_epoch); //предварительное преобразование времени перед вызовом функции прогноза
static void PrognSGP4VectCoord(double A0[14], double j_epoch, double time_after_epoch, double posInert[3], double velInert[3], double posGr[3], double velGr[3], double* high, double* longit, double* latit);
           //"расширенная" функция прогноза
static int calc4alt(string strFSOURCE, string strFRESULT, double base_t, double delta_t, int NN, string strLat, string strLon, int argc, char *argv[]);
         // формирование файлов, содержащих последовательности значений широты и долготы для подспутниковых точек КА
static int compare_TLE_LST(string strFSOURCE, string strFLST, string strFRESULT, double offset_sec); //сравнение прогноза по TLE с данными файла LST
static int LST_to_TLE(string strFLST, string strFRESULT, double offset_sec); //расчет TLE для каждой строки файла данных LST
static int vec_to_tle(str_data_tle sd, double R[3], double V[3], string tle_rez[2], bool bGreenw, bool bPrint, string strFile); //расчет TLE по векторам координат и скорости
static void Progn4(int int3Date[3], int int4Time[4], double A0[14], double j_epoch, double posInert[3], double velInert[3], double posGr[3], double velGr[3], double* high, double* longit, double* latit);
               //"расширенная" функция прогноза с альтернативным интерфейсом
static double Gr_Lat_to_Pix_d(double Lat, double AreaHight);
static double Gr_Long_to_Pix_d(double Long, double AreaWidth);
};

#endif // LIBTLE_H
