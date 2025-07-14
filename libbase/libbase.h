#ifndef LIBBASE_H
#define LIBBASE_H

#include <iostream>
#include <fstream>
#include <sstream>  // для работы со строковыми потоками
#include <string>
#include <cmath>
#include <iostream>  //для вывода отладочной печати

#include <algorithm>  // чтобы для string можно было использовать replace
#include <iomanip>  // для управления выводом чисел в поток

using namespace std ;

//объявление констант

const double  const_PI = 3.141592653589;   //число pi
const double const_TWOPI = 2.0*3.141592653589;         // 2*pi
const double const_xmnpda = 1440.0;       //количество минут в сутках
const double const_secday = 86400.0;      //количество секунд в сутках
const double const_omega_E = 1.00273790934; //обороты Земли в звездные сутки (строго говоря, не константа)
const double const_omega_ER = const_omega_E*2.0*const_PI;  //вращение Земли, радианы в звездные сутки

const int const_max_sats = 250; // максимальное число спутников (на случай усложнения программ)

const double const_ae= 1;
const double const_tothrd = 2.0/3.0;
const double const_xkmper   = 6378.135;        //экваториальный радиус Земли - км (WGS '72)
const double const_f        = 1/298.26;        //сжатие Земли (WGS '72)
const double const_ge       = 398600.8;        //гравитационная константа Земли (WGS '72)
const double const_J2       = 1.0826158E-3;    //гармоника J2 (WGS '72)
const double const_J3       = -2.53881E-6;     //гармоника J3 (WGS '72)
const double const_J4       = -1.65597E-6;     //гармоника J4 (WGS '72)
const double const_ck2      = const_J2/2.0;
const double const_ck4      = -3.0*const_J4/8.0;
const double const_xj3      = const_J3;
const double const_qo       = const_ae + 120.0/const_xkmper;
const double const_s        = const_ae + 78.0/const_xkmper;
const double const_e6a      = 1E-6;
const double const_dpinit   = 1.0;               //код инициализации для дальнего космоса
const double const_dpsec    = 2.0;               //вековой код для дальнего космоса
const double const_dpper    = 3.0;               //периодический код для дальнего космоса



class Libbase
{
public:
    Libbase();
    static double ThetaG_JD(double jd); //Вычисление среднего звездного гринвичского времени по юлианской дате
    static void Conv_Gr_Inert(double RG[3], double VG[3], double TIM, double RI[3], double VI[3]); //из Гринвичской СК в инерциальную СК
    static void Conv_Inert_Gr(double RI[3], double VI[3], double TIM, double RG[3], double VG[3]); //из инерциальной СК в Гринвичскую
    static double Julian_Date_of_Year(double year); // юлианская дата года
    static double Julian_Date_of_Epoch(double epoch); // юлианская дата эпохи
    static int sign (double a); // знак действительного числа (-1, 1 или 0)
    static double arcc (double s, double c); //
    static double Step(double a, double n); // Альтернативная функция возведения в степень (для отладки)
    static double Priv (double a); //
    static void PrognSGP4 (double RV[3], double SS[3], double tpost, double A0[14]);
                // бал. прогноз согласно модели SGP4 для ближнего космоса
    static void decart_to_sph(double Vect[3], double* R, double* LAMBDA, double* PHI);
               // перевод из декартовой СК в географическую (сферическую) СК
    static void decart_to_ell(double Rgsk[3], double F, double Aekv, double* H_ell, double* LAMBDA_ell, double* PHI_ell);
               // перевод из декартовой СК в геодезическую (эллипсоидальную) СК
    static void sph_to_decart(double R, double LAMBDA, double PHI, double Rgsk[3]);
                // перевод из географической (сферической) СК в декартову СК
    static void ell_to_decart(double H, double LAMBDA, double PHI, double F, double Aekv, double Rgsk[3]);
               // перевод из геодезической (эллипсоидальной) СК в декартову СК

static bool inv_m_3(double mat[][3], double EPS); //упрощенное обращение массива (матрицы) 3x3
static bool inv_m_3_dyn(double** mat, double EPS); //упрощенное обращение динамического массива 3x3
static bool inv_m_dyn(double** mat, int N, double EPS); //упрощенное обращение динамического массива NxN
static bool inv_matr_3(double mat[][3], double EPS); //обращение массива (матрицы) 3х3
static bool inv_matr_6(double mat[][6], double EPS); //обращение массива (матрицы) 6х6
static bool inv_matr_3_dyn(double** mat, double EPS); //обращение динамического массива 3х3
static bool inv_matr_dyn(double** mat, int N, double EPS); //обращение динамического массива NxN
static void mult_matr_vect_3(double A[][3], double B[3], double C[3]); //умножение матрицы 3х3 на вектор
static void mult_matr_vect_6(double A[][6], double B[6], double C[6]); //умножение матрицы 6х6 на вектор
static void mult_matr_3(double matA[][3], double matB[][3], double matC[][3]); //умножение массивов 3x3
static void mult_matr_6(double matA[][6], double matB[][6], double matC[][6]); //умножение массивов 6x6
static void mult_matr_3_dyn(double** matA, double** matB, double** matC); //умножение дин. массивов 3x3
static void mult_matr_dyn(double** matA, double** matB, double** matC, int N); //умножение дин. массивов NxN
static bool solv_syst_3(double A[][3], double B[3], double C[3]); // решение системы 3х3
static bool solv_syst_6(double A[][6], double B[6], double C[6]); // решение системы 6х6
static void copy_matr_3(double matS[][3], double matD[][3]); //копирование массива 3х3
static void copy_matr_dyn(double** matS, double** matD, int N); //копирование динамического массива NхN
static void show_matr_3(double matS[][3], string strHead); //вывод массива (матрицы) 3x3 на экран
static void show_matr_dyn(double** matS, string strHead, int N); //вывод дин. массива NxN на экран
static void cross_mult_3(double A[3], double B[3], double C[3]);
static double dot_mult_3(double A[3], double B[3]);
static void norm_vect_3(double A[3], double B[3]);
static double length_vect_3(double A[3]);
static void Jul_to_Date_Time_arr(double dJulian, int int3Date[3], int int4Time[4]);
static double Date_Time_to_Jul_arr(int int3Date[3], int int4Time[4], double factor_s);
static int DOY(int int3Date[3]);
static double Frac_of_Day(int int4Time[4], double factor_s);
static double delt_vect_3(double A[3], double B[3]);
static bool write_to_file(string strFile, string strMessage);
static double Julian_Date(int year, int month, int day, int hour, int minute, double second);
};

#endif // LIBBASE_H
