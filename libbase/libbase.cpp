/*
=============================================================================
РКК "ЭНЕРГИЯ"          БНОПО       БАЗОВАЯ БИБЛИОТЕКА ДЛЯ РАБОТЫ
                                   С ДВУХСТРОЧНЫМИ ЭЛЕМЕНТАМИ (TLE)

ОТДЕЛ 291               ИCПOЛHИTEЛЬ: БОРОВИХИН        ЯЗЫK: C++

-----------------------------------------------------------------------------
БИБЛИОТЕКА libbase                BEPCИЯ 1.0      ДATA COЗДAHИЯ 25.03.2025

БИБЛИОТЕКА СОДЕРЖИТ НЕКОТОРЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С
ДВУХСТРОЧНЫМИ ЭЛЕМЕНТАМИ И СОЗДАНА ПУТЕМ ПРЕОБРАЗОВАНИЯ
В КОД НА C++ РАНЕЕ НАПИСАННЫХ ТЕКСТОВ НА ЯЗЫКАХ
PASCAL И PL1.
ПОТРЕБОВАЛАСЬ СПЕЦИАЛЬНАЯ ДОРАБОТКА ФРАГМЕНТОВ КОДА
ОБЩИМ ОБЪЕМОМ НЕ БОЛЕЕ 20% ОТ ОБЪЕМА ВСЕЙ БИБЛИОТЕКИ.
ПРЕОБРАЗОВАННЫЕ ФУНКЦИИ ВЫДЕЛЕНЫ В ОТДЕЛЬНУЮ БИБЛИОТЕКУ,
ЧТОБЫ ОТЛИЧИТЬ ИХ ОТ НОВЫХ МОДУЛЕЙ ПРИ ПОДСЧЕТЕ СТРОК КОДА
И ОЦЕНКАХ ТРУДОЕМКОСТИ.
ГЛАВНАЯ ФУНКЦИЯ БИБЛИОТЕКИ ПОЗВОЛЯЕТ ПО ИСХОДНЫМ ДАННЫМ,
СОДЕРЖАЩИМСЯ В ДВУХСТРОЧНЫХ ЭЛЕМЕНТАХ (TLE), РАССЧИТАТЬ
ВЕКТОР СОСТОЯНИЯ ДЛЯ СПУТНИКОВ ЗЕМЛИ С ПЕРИОДОМ ОБРАЩЕНИЯ
ДО 225 МИНУТ.
ДОПОЛНИТЕЛЬНЫХ СПЕЦИАЛЬНЫХ ОГРАНИЧЕНИЙ НА ЭКСЦЕНТРИСИТЕТ
И НАКЛОНЕНИЕ ОРБИТЫ НЕ НАКЛАДЫВАТСЯ.

-----------------------------------------------------------------------------

*/

#include "libbase.h"

Libbase::Libbase()
{
}

double Libbase::ThetaG_JD(double jd)
/*
Вычисление среднего звездного гринвичского времени (Greenwich Mean Sidereal Time - GMST,
исходя из юлианской даты, стандартным способом.
Источник:  Астрономический альманах 1992, стр B6.
*/
{
double UT,TU,GMST;

UT   = fmod(jd + 0.5, 1.0);  // на паскале использовалось Frac(jd + 0.5)
jd   = jd - UT;
TU   = (jd - 2451545.0)/36525;
GMST = 24110.54841 + TU * (8640184.812866 + TU * (0.093104 - TU * 6.2E-6));
GMST = fmod(GMST + const_secday*const_omega_E*UT,const_secday);
  // на паскале использовалось Modulus(GMST + secday*omega_E*UT,secday)
//return (const_TWOPI * GMST/const_secday);
return (const_TWOPI * GMST/const_secday);

} //ThetaG_JD


void Libbase::Conv_Gr_Inert(double RG[3], double VG[3], double TIM, double RI[3], double VI[3])
/*
Процедура для пересчета координат и скорости из Гринвичской системы координат в инерциальную (ECI).
*/
{
double mfactor, theta;

    mfactor = const_TWOPI*const_omega_E/const_secday;
    theta = fmod(Libbase::ThetaG_JD(TIM),const_TWOPI);

    RI[0] =   RG[0]*cos(theta)-RG[1]*sin(theta);
    RI[1] =   RG[0]*sin(theta)+RG[1]*cos(theta);
    RI[2] =   RG[2];

    VI[0] = VG[0]*cos(theta)-VG[1]*sin(theta)-RI[1]*mfactor;
    VI[1] = VG[0]*sin(theta)+VG[1]*cos(theta)+RI[0]*mfactor;
    VI[2] = VG[2];
} //Conv_Gr_Inert

void Libbase::Conv_Inert_Gr(double RI[3], double VI[3], double TIM, double RG[3], double VG[3])
/*
Процедура для пересчета координат и скорости из инерциальной (ECI) системы координат
в Гринвичскую. Написана на основе процедуры Conv_Gr_Inert.
*/
{
double mfactor, theta;

    mfactor = const_TWOPI*const_omega_E/const_secday;
    theta = fmod(Libbase::ThetaG_JD(TIM),const_TWOPI);

    RG[0] =   RI[0]*cos(theta)+RI[1]*sin(theta);
    RG[1] =  -RI[0]*sin(theta)+RI[1]*cos(theta);
    RG[2] =   RI[2];

    VG[0] =  VI[0]*cos(theta)+VI[1]*sin(theta)+RG[1]*mfactor;
    VG[1] = -VI[0]*sin(theta)+VI[1]*cos(theta)-RG[0]*mfactor;
    VG[2] =  VI[2];

} //Conv_Inert_Gr



double Libbase::Julian_Date_of_Year(double year)
 // Вычисление юлианской даты для 0.0 января года
 // Астрономические формулы для калькуляторов, Jean Meeus, стр. 23-25
 {
  long int  A,B;
  year = year - 1;
  A = trunc(year/100);
  B = 2 - A + trunc(A/4);
  return (trunc(365.25 * year) + trunc(30.6001 * 14) + 1720994.5 + B);
  } //Function Julian_Date_of_Year


double Libbase::Julian_Date_of_Epoch(double epoch)
{
  double year,day;
  // Модификация для подержки Y2K, действительна с 1957 по 2056
  year = trunc(epoch*1E-3); // на паскале было Int(epoch*1E-3)
  if (year < 57) {    year = year + 2000;  }
  else { year = year + 1900;};
  // Завершение модификации
  day  = fmod(epoch*1E-3,1.0)*1E3; // соответствует функции паскаля Frac(epoch*1E-3)*1E3
  return (Julian_Date_of_Year(year) + day);
} // Julian_Date_of_Epoch


int Libbase::sign (double a)
// Функция возвращает знак действительного числа
{
    if (a < 0) { return -1; } else { if (a > 0) {return 1;} else { return 0;};};
}

double Libbase::arcc (double s, double c)
{

double rl;

   rl= std::sqrt(s*s+c*c);
   return std::acos(c/rl)*Libbase::sign(s);
}

double Libbase::Step(double a, double n)
 {
  return std::exp(n*std::log(a));  //!!!!!!!!!!!!!!!!!!!!!!!!! НАТУРАЛЬНЫЙ ЛОГАРИФМ !!!!!!!!!!!!!!!!!!!!!!!!
 }

double Libbase::Priv (double a)
{
double rl;
  rl=int(abs(a)/const_TWOPI+0.5);  // на паскале была константа DPi из supermin.pas
  //!!!!!!!!!!!!!!!!!! функция abs !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  return (a-const_TWOPI*rl*Libbase::sign(a));
}

void Libbase::PrognSGP4 (double RV[3], double SS[3], double tpost, double A0[14])
/*
Процедура подготовки баллистического прогноза согласно модели SGP4 для ближнего космоса.
Получена на основе вспомогательных процедур, которые использовались для "Сигмы"
и были написаны на языках PL1 и Pascal.
Рассчитанные в результате векторы координат и скорости КА записываются соответственно
в массивы RV и SS.
Через массив A0 в процедуру передаются исходные параметры. Некоторые  ячейки этого массива,
а также значительная часть переменных, объявленных в процедуре, остались зарезервированными.
Параметр tpost задает время, для которого должны быть рассчитаны координаты и скорость,
в секундах, прошедших после момента эпохи, значение которой взято из двухстрочных
элементов и передается в процедуру с помощью массива A0.
*/
{
    //type mas=array[1..7] of integer;
const double Ae = 6378.135;
//      var f,f1,f3,f4,f5,f6: text;  НЕ ИСПОЛЬЗУЮТСЯ
//      XScr,YScr: byte;  НЕ ИСПОЛЬЗУЮТСЯ

double A001,A002,A003,A004,A005,A006,A007,
A008,A009,A010,A011,A012,A013,A014,

K001,K002,K003,K004,K005,K006,K007,K008,K009,K010,K011,K012,K013,K014,K015,
K016,K017,K018,K019,K020,K021,K022,K023,K024,K025,K026,K027,K028,K029,K030,
K031,K032,K033,K034,K035,K036,K037,K038,K039,K040,K041,K042,K043,K044,K045,
K046,K047,K048,K049,K050,K051,K052,K053,K054,K055,K056,K057,K058,K059,K060,
K061,K062,K063,K064,K065,K066,K067,K068,K069,K070,K071,K072,K073,K074,K075,
K076,K077,K078,K079,K080,K081,K082,K083,K084,K085,K086,K087,K088,K089,K090,
K091,K092,K093,K094,K095,K096,K097,K098,K099,K100,K101,K102,K103,K104,K105,
K106,K107,K108,K109,K110,K111,K112,K113,K114,K115,K116,K117,K118,K119,K120,
K121,K122,K123,K124,K125,K126,
Ux,Uy,Uz, Vx,Vy,Vz, Om,e,w,a,L,
Xw,Yw,aaa,
Dt_e,Om0,Om1,
c35,s35,swe,swe_K080,cwe,k28,

Ma,Ma0,Dt_Ma,w0,w1,
i_beg,Om_beg,e_beg,w_beg,
i_end,Om_end,e_end,w_end,a_end,u_end,
time_sup,time2,time3,time4,Ci,Ci2,Ci4,e2,X6F40,
      dx,
        Si; // вводилась в библиотеке supermin

//      xg,yg,zg             : vector; НЕ ИСПОЛЬЗУЮТСЯ

int    i,ii,M,N,k,nm,ny,nd;
//double RV[3], SS[3]; //   RV,SS   : vector;

int dbg; // признак для вывода параметров в отладочных целях.
ofstream fdbg;

dbg = 0;

 if (dbg==1) {fdbg.open("DBG", ios::out); fdbg << setprecision(8) << endl;}; //открытие потока для записи параметров в файл

//Присваивание переменным значений, переданных через массив:
A001 = A0[0]; A002 = A0[1]; A003 = A0[2]; A004 = A0[3]; A005 = A0[4];
A006 = A0[5];A007 = A0[6];A008 = A0[7];A009 = A0[8];A010 = A0[9];
A011 = A0[10];A012 = A0[11];A013 = A0[12];A014 = A0[13];

// СЛЕДУЮШИЙ ФРАГМЕНТ В ПРОГРАММЕ НА ПАСКАЛЕ ВЫЗЫВАЛСЯ ПРОЦЕДУРОЙ CalculateVect ПЕРЕД
// ВЫЗОВОМ ПРОЦЕДУРЫ РАСЧЕТА ВЕКТОРА СОСТОЯНИЯ (PrognSGP4)/
// ФРАГМЕНТ ПЕРЕНЕСЕН СЮДА ИЗ-ЗА ОТКАЗА ОТ ГЛОБАЛЬНОСТИ ПЕРЕМЕННЫХ Om_beg, w_beg, e_beg, time_sup

       //---- ПЕРЕВОД MEAN ANOMALY (12) В РАДИАНЫ ----
       //A012:=A012*1.7453292519943E-2;
       //---- ПЕРЕВОД RIGHT ASCENSION OF THE ASCENDING NODE (9) В РАДИАНЫ ----
       //Om_beg:=A009*1.7453292519943E-2;
       Om_beg=A009; if (dbg==1) {fdbg  << "Om_beg: " << Om_beg << endl;};
       //---- ПЕРЕВОД ARGUMENT OF PERIGEE (11) В РАДИАНЫ ----
       w_beg=A011; if (dbg==1) {fdbg  << "w_beg: " << w_beg << endl;};

       //---- ПЕРЕПИСЬ ECCENTRICITY (10) ----
       e_beg=A010; if (dbg==1) {fdbg  << "e_beg: " << e_beg << endl;};

       //5---- ПЕРЕВОД INCLINATION (8) В РАДИАНЫ ----
       i_beg=A008; if (dbg==1) {fdbg  << "A008: " << A008 << endl;};

       //---- ПЕРЕВОД MEAN MOTION (13) ----
       A013=A013*4.3633231299858E-3; if (dbg==1) {fdbg << "A013: " << A013 << endl;};
       //---- ПЕРЕВОД FIRST TIME DERIVATIVE (3) ----
       A003=A003*3.0300855069346E-6; if (dbg==1) {fdbg << "A003: " << A003 << endl;};
       //---- ПЕРЕВОД SECOND TIME DERIVATIVE (4) ----
       A004=A004*2.1042260464823E-9; if (dbg==1) {fdbg << "A004: " << A004 << endl;};
       //---- ПЕРЕПИСЬ BSTAR DRAG TERM (5) ----
       //A005=A005;
       if (abs(A005) >= 0.020) { A005=0.0015; }; if (dbg==1) {fdbg << "A005: " << A005 << endl;};
       time_sup=tpost/60;

       // TextBackground(7); TextColor(1); ClrScr;

       //------------------------------------------------------------------

   if (A013 < 0.0) Libbase::write_to_file("POW","A013 < 0.0");
   K003=pow(0.0743669161/A013,2.0/3.0); if (dbg==1) {fdbg << "K003: " << K003 << endl;};


   Ci=cos(i_beg); if (dbg==1) {fdbg << "Ci: " << Ci << endl;};
   Si=sin(i_beg); if (dbg==1) {fdbg << "Si: " << Si << endl;};

   Ci2=Ci*Ci; if (dbg==1) {fdbg << "Ci2: " << Ci2 << endl;};
   if (Ci2 < 0.0) Libbase::write_to_file("POW","Ci2 < 0.0");
   Ci4=pow(Ci2, 2.0);  if (dbg==1) {fdbg << "Ci4: " << Ci4 << endl;}; //sqr(Ci2)

   e2=e_beg*e_beg; if (dbg==1) {fdbg << "e2: " << e2 << endl;};
   K006=3*Ci2-1;  if (dbg==1) {fdbg << "K006: " << K006 << endl;};
   K009=K006/((1-e2)*sqrt(1-e2)*K003*K003)*0.000811962; if (dbg==1) {fdbg << "K009: " << K009 << endl;};
   K010=(1-((((K009*134/81+1.0)*K009)+1/3E0)*K009))*K003; if (dbg==1) {fdbg << "K010: " << K010 << endl;};
   K011=K006/((1-e2)*sqrt(1-e2)*K010*K010)*0.000811962; if (dbg==1) {fdbg << "K011: " << K011 << endl;};
   K012=A013/(K011+1); if (dbg==1) {fdbg << "K012: " << K012 << endl;};
   K013=K010/(1-K011); if (dbg==1) {fdbg << "K013: " << K013 << endl;};

   if ( ((1-e_beg)*K013)/1 < 1.0344928415594 ) { X6F40=1; } else { X6F40=0; };
                                                                 //X6F40:=1;
   if (dbg==1) {fdbg << "X6F40: " << X6F40 << endl;};

   K016=(((1-e_beg)*K013)-1)*6378.135; if (dbg==1) {fdbg << "K016: " << K016 << endl;};
//   IF K016 >= 156.0 THEN  ?????}
   if ((1-e2)*K013 < 0.0) Libbase::write_to_file("POW","(1-e2)*K013 < 0.0");
   K017=1/pow( ((1-e2)*K013),2.0 );   if (dbg==1) {fdbg << "K017: " << K017 << endl;}; //   K017:=1/sqr((1-e2)*K013);   
   K018=1/(K013-1.0122292801892); if (dbg==1) {fdbg << "K018: " << K018 << endl;};

  // K018 = 20.47067391;   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   K019=K013*e_beg*K018; if (dbg==1) {fdbg << "K019: " << K019 << endl;};
   K020=K019*K019; if (dbg==1) {fdbg << "K020: " << K020 << endl;};
   K021=e_beg*K019; if (dbg==1) {fdbg << "K021: " << K021 << endl;};
   K022=abs(1-K020);  if (dbg==1) {fdbg << "K022: " << K022 << endl;};
   //K023=Step(K018,4)*Step(6.5849970249924E-3,4);
   if (K018 < 0.0) Libbase::write_to_file("POW","K018 < 0.0");
   K023=pow(K018,4)*pow(6.5849970249924E-3,4.0); if (dbg==1) {fdbg << "K023: " << K023 << endl;};
   if (K022 < 0.0) Libbase::write_to_file("POW","K022 < 0.0");
   K024=K023/pow(K022,3.5); if (dbg==1) {fdbg << "K024: " << K024 << endl;};
   K025=((((((((((K020+8)*K020)*3)+8)*K018)*K006)/K022)*0.000405981)+
        ((((K020+4)*K021)+(K020*3/2+1))*K013))*K024)*K012; if (dbg==1) {fdbg << "K025: " << K025 << endl;};
   K026=A005*K025; if (dbg==1) {fdbg << "K026: " << K026 << endl;};
   K028=4.690139440023E-3; if (dbg==1) {fdbg << "K028: " << K028 << endl;};
   K029=(K023*K018*K028*K012*Si)/e_beg; if (dbg==1) {fdbg << "K029: " << K029 << endl;};
//   if (Ci < 0.0) Libbase::write_to_file("POW_Ci","Ci < 0.0");  //!!!отладка
   K030=1-pow(Ci,2.0); if (dbg==1) {fdbg << "K030: " << K030 << endl;};

      //---- ВЫЧИСЛЕНИЕ ПЕРВОЙ ПРОИЗВОДНОЙ ЭКСЦЕНТРИСИТЕТА ----}
   Dt_e=K012*K024*K013*(1-e2)*2; if (dbg==1) {fdbg << "Dt_e: " << Dt_e << endl;};
   Dt_e=((((K020*2+1/2E0)*e_beg)-((((((((std::cos(w_beg*2))*(K020*2-
        ((K020+1)*K021)))*K030)*3/4)+(((((1.5-(K021/2))*K020)+
        (1-(K021*2.0)))*K006)*(-3)))/(K022*K013))*K018)*0.001082616))+
        (((K020/2)+2)*K019))*Dt_e; if (dbg==1) {fdbg << "Dt_e: " << Dt_e << endl;};
   K032=(((((K021*K020)+(((K020+K021)*11/4)+1))*K024)*K013)*(1-e2))*2; if (dbg==1) {fdbg << "K032: " << K032 << endl;};

   K034=K017*K012*0.001623924; if (dbg==1) {fdbg << "K034: " << K034 << endl;};
   K035=K034*K017*0.000541308; if (dbg==1) {fdbg << "K035: " << K035 << endl;};
   K036=K017*K017*K012*7.762359375E-7; if (dbg==1) {fdbg << "K036: " << K036 << endl;};

   //---- ВЫЧИСЛЕНИЕ ПЕРВОЙ ПРОИЗВОДНОЙ СРЕДНЕЙ АНОМАЛИИ ----}
   Dt_Ma=((((13-78*Ci2+137*Ci4)*K035)*std::sqrt((1-e2)))/16)
           +(K034*std::sqrt((1-e2))*K006/2+K012);

   if (dbg==1) {fdbg << "Dt_Ma: " << Dt_Ma << endl;};

   //---- ВЫЧИСЛЕНИЕ ПЕРВОЙ ПРОИЗВОДНОЙ АРГУМЕНТА ПЕРИГЕЯ ----}
   K039=  (( 3 -36*Ci2+ 49*Ci4)*K036)
        +((((7-114*Ci2+395*Ci4)*K035)/16)
           -(K034*(1-5*Ci2)/2)); if (dbg==1) {fdbg << "K039: " << K039 << endl;};

   //---- ВЫЧИСЛЕНИЕ ПЕРВОЙ ПРОИЗВОДНОЙ ДОЛГОТЫ ВОСХОДЯЩЕГО УЗЛА ----}
   K041=(((((3  -7*Ci2)*K036)*2)+(((4-19*Ci2)*K035)/2))
         *Ci)-K034*Ci; if (dbg==1) {fdbg << "K041: " << K041 << endl;};

   K042=cos(w_beg)*A005*K029; if (dbg==1) {fdbg << "K042: " << K042 << endl;};
   K043=((K023*A005)/K021)*(-2/3E0); if (dbg==1) {fdbg << "K043: " << K043 << endl;};

   //---- ВЫЧИСЛЕНИЕ ВТОРОЙ ПРОИЗВОДНОЙ ДОЛГОТЫ ВОСХОДЯЩЕГО УЗЛА ----}
   K044=-(1-e2)*K034*Ci*K026*3.5; if (dbg==1) {fdbg << "K044: " << K044 << endl;};

   K045=K026*3.0/2.0; if (dbg==1) {fdbg << "K045: " << K045 << endl;};

//   std::cout  << "K045: " << (K045) << std::endl;

if (dbg==1) {fdbg << "K045: " << K045 << endl;};

   K046=((((Ci*5+3)/(Ci+1))*K028)*Si)/8; if (dbg==1) {fdbg << "K046: " << K046 << endl;};
   K047=K028*Si/4; if (dbg==1) {fdbg << "K047: " << K047 << endl;};
   if ((std::cos(A012)*K019+1.0) < 0.0) Libbase::write_to_file("POW","(std::cos(A012)*K019+1.0) < 0.0");
   K048=pow((std::cos(A012)*K019+1.0),3.0); if (dbg==1) {fdbg << "K048: " << K048 << endl;};
   K050=7*Ci2-1; if (dbg==1) {fdbg << "K050: " << K050 << endl;};

   if (X6F40==1)
    {
     K052=4*K013*K018*K026*K026; if (dbg==1) {fdbg << "K052: " << K052 << endl;};
     K053=(K052*K018*K026)/3; if (dbg==1) {fdbg << "K053: " << K053 << endl;};
     K054=(K013*17+1.0122292801892)*K053; if (dbg==1) {fdbg << "K054: " << K054 << endl;};
     K055=(((((1.0122292801892*31+K013*221)*K053)*K013)*K018)*K026)/2; if (dbg==1) {fdbg << "K055: " << K055 << endl;};
     K056=2*K026*K026+K052; if (dbg==1) {fdbg << "K056: " << K056 << endl;};
     K057=(((K026*K026*10+K052*12)*K026)+(K054*3))/4; if (dbg==1) {fdbg << "K057: " << K057 << endl;};
     K058=((((K052*2+K026*K026)*K026*K026)*15)+(K052*K052*6+(K026*K054*12+K055*3)))/5; if (dbg==1) {fdbg << "K058: " << K058 << endl;};
     };

   time2=time_sup*time_sup;    time3=time2*time_sup;    time4=time2*time2;

   if (dbg==1) {fdbg << "time2: " << time2 << endl;};
   if (dbg==1) {fdbg << "time3: " << time3 << endl;};
   if (dbg==1) {fdbg << "time4: " << time4 << endl;};

   //---- УЧЕТ ПЕРВОЙ ПРОИЗВОДНОЙ СРЕДНЕЙ АНОМАЛИИ ----}
   Ma0=time_sup*Dt_Ma+A012; if (dbg==1) {fdbg << "Ma0: " << Ma0 << endl;};
   //---- УЧЕТ ПЕРВОЙ ПРОИЗВОДНОЙ АРГУМЕНТА ПЕРИГЕЯ ----}
   w0=time_sup*K039+w_beg; if (dbg==1) {fdbg << "w0 " << w0 << endl;};
   //---- УЧЕТ ПЕРВОЙ ПРОИЗВОДНОЙ ДОЛГОТЫ ВОСХОДЯЩЕГО УЗЛА ----}
   Om0=time_sup*K041+Om_beg; if (dbg==1) {fdbg << "Om0: " << Om0 << endl;};

   //---- УЧЕТ ВТОРОЙ ПРОИЗВОДНОЙ СРЕДНЕЙ АНОМАЛИИ ----}
   Ma=Ma0; if (dbg==1) {fdbg << "Ma: " << Ma << endl;};
   //---- УЧЕТ ВТОРОЙ ПРОИЗВОДНОЙ АРГУМЕНТА ПЕРИГЕЯ ----}
   w1=w0; if (dbg==1) {fdbg << "w1: " << w1 << endl;};
   //---- УЧЕТ ВТОРОЙ ПРОИЗВОДНОЙ ДОЛГОТЫ ВОСХОДЯЩЕГО УЗЛА ----}
   Om1=Om0+K044*time2; if (dbg==1) {fdbg << "Om1: " << Om1 << endl;};

   K066=1-time_sup*K026; if (dbg==1) {fdbg << "K066: " << K066 << endl;};

   //---- УЧЕТ ПЕРВОЙ ПРОИЗВОДНОЙ ЭКСЦЕНТРИСИТЕТА ----}
   K067=time_sup*A005*Dt_e; if (dbg==1) {fdbg << "K067: " << K067 << endl;};

   K068=K045*time2; if (dbg==1) {fdbg << "K068: " << K068 << endl;};

   //---- УЧЕТ ПОПРАВКИ ----}
   if (X6F40==1)
   {
     //---- ВЫЧИСЛЕНИЕ ПОПРАВКИ ----}
     if ((std::cos(Ma0)*K019+1) < 0.0) Libbase::write_to_file("POW","(std::cos(Ma0)*K019+1)");
     K053=time_sup*K042+(pow((std::cos(Ma0)*K019+1),3)-K048)*K043; if (dbg==1) {fdbg << "K053: " << K053 << endl;};
     //---- ПОПРАВКА К СРЕДНЕЙ АНОМАЛИИ ----}
     Ma=Ma+K053; if (dbg==1) {fdbg << "Ma: " << Ma << endl;};
     //---- ПОПРАВКА К АРГУМЕНТУ ПЕРИГЕЯ ----}
     w1=w1-K053; if (dbg==1) {fdbg << "w1: " << w1 << endl;};
     //---- ПОПРАВКА К БОЛЬШОЙ ПОЛУОСИ ----}
     K066=K066-K052*time2-K054*time3-K055*time4; if (dbg==1) {fdbg << "K066: " << K066 << endl;};
     //---- ПОПРАВКА К ПЕРВОЙ ПРОИЗВОДНОЙ ЭКСЦЕНТРИСИТЕТА ----}
     K067=K067+(((std::sin(Ma)-std::sin(A012))*A005)*K032); if (dbg==1) {fdbg << "K067: " << K067 << endl;};

     K068=K068+K056*time3+(time_sup*K058+K057)*time4; if (dbg==1) {fdbg << "K068: " << K068 << endl;};
   };

   //---- ОПРЕДЕЛЕНИЕ БОЛЬШОЙ ПОЛУОСИ ----}
   a=K013*K066*K066; if (dbg==1) {fdbg << "a: " << a << endl;};

      //---- УЧЕТ ПЕРВОЙ ПРОИЗВОДНОЙ ЭКСЦЕНТРИСИТЕТА ПО ВРЕМЕНИ ----}
   e=e_beg-K067; if (dbg==1) {fdbg << "e: " << e << endl;};

   K075=K012*K068+Ma+w1+Om1; if (dbg==1) {fdbg << "K075: " << K075 << endl;};

   //---- СРЕДНЯЯ УГЛОВАЯ СКОРОСТЬ ДВИЖЕНИЯ ----}
   if (a < 0.0) Libbase::write_to_file("POW","a < 0.0");
   L=0.0743669161/pow(a,3.0/2.0); if (dbg==1) {fdbg << "L: " << L << endl;};

   K079=K046*std::cos(w1)*e/(a*std::sqrt(1-e*e)); if (dbg==1) {fdbg << "K079: " << K079 << endl;};
   K080=K028*Si/4/(a*std::sqrt(1-e*e)); if (dbg==1) {fdbg << "K080: " << K080 << endl;};
   swe_K080=std::sin(w1)*e+K080; if (dbg==1) {fdbg << "swe_K080: " << swe_K080 << endl;};
   cwe=std::cos(w1)*e; if (dbg==1) {fdbg << "cwe: " << cwe << endl;};

   K084=Priv(K075+K079-Om1); if (dbg==1) {fdbg << "K084: " << K084 << endl;};

   if (K084 < 0) {K084=K084+2*const_PI; };

   if (dbg==1) {fdbg << "K084: " << K084 << endl;};

   K035=K084;

   if (dbg==1) {fdbg << "K035: " << K035 << endl;};

   //---- РЕШЕНИЕ УРАВНЕНИЯ КЕПЛЕРА МЕТОДОМ ИТЕРАЦИЙ (10) ----}

   for (i=1;i<=10;i++)
     {
      K036=cwe*std::sin(K035); if (dbg==1) {fdbg << "K036: " << K036 << endl;};
      K088=cwe*std::cos(K035); if (dbg==1) {fdbg << "K088: " << K088 << endl;};
      K087=swe_K080*std::cos(K035); if (dbg==1) {fdbg << "K087: " << K087 << endl;};
      K089=swe_K080*std::sin(K035); if (dbg==1) {fdbg << "K089: " << K089 << endl;};
      K090=(((K036-K035)+(K084-K087))/(1-K088-K089))+K035; if (dbg==1) {fdbg << "K090: " << K090 << endl;};

//    IF ABS(K090-K035) <= 1.0E-020 THEN goto 77;
      K035=K090;  if (dbg==1) {fdbg << "K035: " << K035 << endl;};
      };

//{77:}
    K091=K088+K089; if (dbg==1) {fdbg << "K091: " << K091 << endl;};

//   {K053:=1-K080+(1+K080)*SIN(w1)*e+sqr((COS(w1)*e));}
   K092=K036-K087; if (dbg==1) {fdbg << "K092: " << K092 << endl;};
   if (swe_K080 < 0.0) Libbase::write_to_file("POW","swe_K080 < 0.0");
//   if (cwe < 0.0) Libbase::write_to_file("POW_cwe","cwe < 0.0"); // !!!!! отладка !!!!
   K093=std::pow(swe_K080,2.0)+std::pow(cwe,2.0); if (dbg==1) {fdbg << "K093: " << K093 << endl;};
   K053=1-K093; if (dbg==1) {fdbg << "K053: " << K053 << endl;};

   K096=0.0743669161*std::sqrt(a)*(K036-K087)/((1-K091)*a); if (dbg==1) {fdbg << "K096: " << K096 << endl;};
   K097=0.0743669161*std::sqrt(a*K053)/((1-K091)*a); if (dbg==1) {fdbg << "K097: " << K097 << endl;};
   K035=a/((1-K091)*a); if (dbg==1) {fdbg << "K035: " << K035 << endl;};
   K036=1/(std::sqrt(K053)+1); if (dbg==1) {fdbg << "K036: " << K036 << endl;};

   K099=(swe_K080*K092*K036+(std::cos(K090)-cwe))*K035; if (dbg==1) {fdbg << "K099: " << K099 << endl;};
   K100=((std::sin(K090)-(swe_K080))-cwe*K092*K036)*K035; if (dbg==1) {fdbg << "K100: " << K100 << endl;};
   K101=Libbase::arcc(K100,K099); if (dbg==1) {fdbg << "K101: " << K101 << endl;};
   if (K101 < 0) { K101=K101+2*const_PI; };

   if (dbg==1) {fdbg << "K101: " << K101 << endl;};

   K102=K100*K099*2; if (dbg==1) {fdbg << "K102: " << K102 << endl;};
   K103=2*K099*K099-1; if (dbg==1) {fdbg << "K103: " << K103 << endl;};
   K053=1/(a*K053); if (dbg==1) {fdbg << "K053: " << K053 << endl;};
   K034=K053*0.000541308; if (dbg==1) {fdbg << "K034: " << K034 << endl;};
   K035=K034*K053; if (dbg==1) {fdbg << "K035: " << K035 << endl;};

   //---- МГНОВЕННЫЙ МОДУЛЬ РАДИУСА-ВЕКТОРА ----}

   a_end=(K034*K030*K103/2.0)+((1-(K035*std::sqrt(K053)*K006*3.0/2.0))*(1-K091)*a);

   if (dbg==1) {fdbg << "a_end: " << a_end << endl;};

   //---- МГНОВЕННЫЙ АРГУМЕНТ ШИРОТЫ ----}

   u_end=K101-K035*K050*K102/4; if (dbg==1) {fdbg << "u_end: " << u_end << endl;};

   //---- МГНОВЕННАЯ ДОЛГОТА ВОСХОДЯЩЕГО УЗLА ----}

   Om_end=K035*Ci*K102*3/2+Om1; if (dbg==1) {fdbg << "Om_end: " << Om_end << endl;};

   //---- МГНОВЕННОЕ НАКЛОНЕНИЕ ОРБИТЫ ----}

   i_end=K035*Ci*Si*K103*3/2+i_beg; if (dbg==1) {fdbg << "i_end: " << i_end << endl;};

   //---- ДВЕ КООРДИНАТЫ РАДИУСА-ВЕКТОРА В ОСК ----}

   Xw=K096-L*K034*K030*K102;  if (dbg==1) {fdbg << "Xw: " << Xw << endl;};
   Yw=((3*Ci2-1)*3/2+K030*K103)*L*K034+K097; if (dbg==1) {fdbg << "Yw: " << Yw << endl;};

   //---- ЕДИНИЧНЫЙ ВЕКТОР ТРИЕДРА U В ИСК (НА КА)----}

   Ux= std::cos(Om_end)*std::cos(u_end)-std::sin(Om_end)*std::cos(i_end)*std::sin(u_end);
   Uy= std::sin(Om_end)*std::cos(u_end)+std::cos(Om_end)*std::cos(i_end)*std::sin(u_end);
   Uz= std::sin(i_end) *std::sin(u_end);

   if (dbg==1) {fdbg << "Ux: " << Ux << endl;};
   if (dbg==1) {fdbg << "Uy: " << Uy << endl;};
   if (dbg==1) {fdbg << "Uz: " << Uz << endl;};

   //---- ЕДИНИЧНЫЙ ВЕКТОР ТРИЕДРА V В ИСК ----}

   Vx=-std::sin(Om_end)*std::cos(i_end)*std::cos(u_end)-std::cos(Om_end)*std::sin(u_end);
   Vy= std::cos(Om_end)*std::cos(i_end)*std::cos(u_end)-std::sin(Om_end)*std::sin(u_end);
   Vz= std::sin(i_end) *std::cos(u_end);

   if (dbg==1) {fdbg << "Vx: " << Vx << endl;};
   if (dbg==1) {fdbg << "Vy: " << Vy << endl;};
   if (dbg==1) {fdbg << "Vz: " << Vz << endl;};

   //---- РАДИУС-ВЕКТОР В ИСК ----}

   RV[0]=a_end*Ux*6378.135;
   RV[1]=a_end*Uy*6378.135;
   RV[2]=a_end*Uz*6378.135;

   //---- ВЕКТОР СКОРОСТИ В ИСК ----}

   SS[0]=(Yw*Vx+Xw*Ux)*6378.135/60;  // {/ 106.30225; ПЕРЕВОД ИЗ Э.Р. /МИН}
   SS[1]=(Yw*Vy+Xw*Uy)*6378.135/60;
   SS[2]=(Yw*Vz+Xw*Uz)*6378.135/60;

   if (dbg==1) {fdbg.close();};

}//PrognSGP4

   void Libbase::decart_to_sph(double Vect[3], double* R, double* LAMBDA, double* PHI)
   /*
    Функция для определения сферических координат точки по декартовым координатам.
   Исходные данные - вектор Vect.
   Результат - длина радиус-вектора R, долгота LAMBDA (град.), широта PHI (град).
   */
   {
   double eps, X, Y, Z, R1, PHI1, LAMBDA1;

   eps = 0.00000001;
   X = Vect[0];
   Y = Vect[1];
   Z = Vect[2];
   R1 = sqrt(X*X+Y*Y+Z*Z);

   if (abs(Z) < eps ) {   PHI1 = 0.0; } else { PHI1 = asin(Z/R1); };

   if ((abs(X) < eps) && (abs(Y) < eps)) { LAMBDA1 = 0.0; PHI1 = 0.0; }
   else { if (abs(X) < eps) { if (Y > 0.0) {LAMBDA1 = const_PI/2.0;} else {LAMBDA1 = 3.0*const_PI/2.0;} }
          if (abs(Y) < eps) { if (X > 0.0) {LAMBDA1 = 0.0;   } else {LAMBDA1 = const_PI; }        }
        };
   if ((abs(X) >= eps) && (abs(Y) >= eps)) { LAMBDA1 = atan(Y/X); };
      //Проверить диапазоны углов!!!!!!!!!!!!!!!!!!!!!}
   *R = R1;
   *LAMBDA = LAMBDA1*180.0/const_PI; // перевод радианов в градусы
   *PHI = PHI1*180.0/const_PI; // перевод радианов в градусыPHI = rad2deg(PHI1);

   } // decart_to_sph

   void Libbase::decart_to_ell(double Rgsk[3], double F, double Aekv, double* H_ell, double* LAMBDA_ell, double* PHI_ell)
   /*
   % Функция для определения геодезических координат точки по ее координатам в декартовой СК.
   % Исходные данные - вектор Rgsk.
   % F - сжатие.
   % Aekv - радиус экватора.
   %Для Земли:
   %F = 1.0/298.257839303;
   %Aekv = 6378136.0;

   % Результат - высота над поверхностью планеты H, долгота LAMBDA (град.), широта PHI (град).
   */
   {

   double LAMBDA, H, PHI, EPSILON, RX, RY, RZ, Esqr, D, La, R, C, P, S1, S2;

   EPSILON = 0.0001*const_PI/(180.0*60.0*60.0);

   RX = Rgsk[0];
   RY = Rgsk[1];
   RZ = Rgsk[2];

   Esqr = 2.0*F - F*F;

   D = sqrt(RX*RX+RY*RY);



   if (D == 0)
   {
              PHI = (const_PI/2)*(RZ/abs(RZ));
              LAMBDA = 0.0;
     //         if (sin(PHI) < 0.0) Libbase::write_to_file("POW_sin1","sin(PHI) < 0.0"); //!!! отладка !!!
              H = RZ*sin(PHI)-Aekv*sqrt(1.0-Esqr*pow(sin(PHI),2.0));

//              cout << "1: " << H << endl;
   }
   else
   {
              La = abs(asin(RY/D));
              if ( (RY<0.0) && (RX>0.0) ) { LAMBDA = 2*const_PI - La; };
              if ( (RY<0.0) && (RX<0.0) ) { LAMBDA = const_PI + La; };
              if ( (RY>0.0) && (RX<0.0) ) { LAMBDA = const_PI - La; };
              if ( (RY>0.0) && (RX>0.0) ) { LAMBDA = La; };
              if ( (RY==0.0) && (RX>0.0) ) { LAMBDA = 0.0; };
              if ( (RY==0.0) && (RX<0.0) ) { LAMBDA = const_PI; };
              if (RZ == 0.0)
              {
                            PHI = 0.0;
                            H = D - Aekv;

//                            cout << "2: " << H << endl;
                            if (RX==0.0)  //НЕ БЫЛО В ОПИСАНИЯХ!
                            {
                                      if (RY<0.0) { LAMBDA = -La; } else { LAMBDA = La; };
                            }
              }
              else
              {
                            R = sqrt(RX*RX+RY*RY+RZ*RZ);
                            C = asin(RZ/R);
                            P = Esqr*Aekv/(2.0*R);
                            S2 = 0;
                            S1 = S2;
                            PHI = C + S1;
      //                      if (sin(PHI) < 0.0) Libbase::write_to_file("POW_sin2","sin(PHI) < 0.0");  //!!! отладка !!!
                            S2 = asin(P*sin(2.0*PHI)/sqrt(1.0-Esqr*pow(sin(PHI),2.0)));
                            while (abs(S2-S1) >= EPSILON)
                            {
                            S1 = S2;
                            PHI = C + S1;
                   //         if (sin(PHI) < 0.0) Libbase::write_to_file("POW_sin3","sin(PHI) < 0.0"); //!!! отладка !!!
                            S2 = asin(P*sin(2.0*PHI)/sqrt(1.0-Esqr*pow(sin(PHI),2.0)));
                            };
                   //         if (sin(PHI) < 0.0) Libbase::write_to_file("POW_sin4","sin(PHI) < 0.0"); //!!! отладка !!!
                            H = D*cos(PHI)+RZ*sin(PHI)-Aekv*sqrt(1.0-Esqr*pow(sin(PHI),2));
//                            cout << "3: " << H << endl;
              }

   }

   *LAMBDA_ell = (LAMBDA/const_PI)* 180.0; //перевод радианов в градусы
   *PHI_ell = (PHI/const_PI)* 180.0; //перевод радианов в градусы
   *H_ell = H;

   } // decart_ell

   void Libbase::sph_to_decart(double R, double LAMBDA, double PHI, double Rgsk[3])
   /*
   Процедура для определения координат точки в декартовой СК по ее сферическим координатам.
   Исходные данные - длина радиус-вектора R, долгота LAMBDA (град.), широта PHI (град).
   Результат - координаты RX, RY, RZ.
   */
   {

LAMBDA = (LAMBDA/180.0)*const_PI; //перевод из градусов в радианы
PHI= (PHI/180.0)*const_PI; //перевод из градусов в радианы

Rgsk[0] = R*cos(PHI)*cos(LAMBDA);

Rgsk[1] = R*cos(PHI)*sin(LAMBDA);

Rgsk[2] = R*sin(PHI);
   } //sph_decart

   void Libbase::ell_to_decart(double H, double LAMBDA, double PHI, double F, double Aekv, double Rgsk[3])
   /*
   Процедура для определения декартовых координат точки по ее эллипсоидным угловым координатам.
   Исходные данные -
   высота над поверхностью планеты H, долгота LAMBDA (град.), широта PHI (град).
   Результат - координаты RX, RY, RZ.
   F - сжатие.
   Aekv - радиус экватора.
   Для Земли:
   F = 1.0/298.257839303;
   Aekv = 6378136.0;
   */
   {
       double Esqr, N;

   LAMBDA = (LAMBDA/180.0)*const_PI; // перевод градусов в радианы
   PHI = (PHI/180.0)*const_PI; // перевод градусов в радианы

   Esqr = 2.0*F - F*F;
//   if (sin(PHI) < 0.0) Libbase::write_to_file("POW_sin5","sin(PHI) < 0.0");  //!!! отладка !!!
   N = Aekv/sqrt(1.0-Esqr*pow(sin(PHI),2.0));


   Rgsk[0] = (N+H)*cos(PHI)*cos(LAMBDA);
   Rgsk[1] = (N+H)*cos(PHI)*sin(LAMBDA);
   Rgsk[2] = ((1.0- Esqr)*N+H)*sin(PHI);


   } // ell_decart



bool Libbase::inv_m_3(double mat[][3], double EPS)
/*
Функция обращения матрицы по методу Гаусса
ДЛЯ НЕДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ РАЗМЕРОМ 3х3.
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.5, стр. 36).
Из-за особенностей алгоритма могут возникать значительные ошибки
при обращении плохо обусловленных матриц.
Параметр EPS обычно задавался равным нулю.
*/

{
    int i,j,k,N;
    double DET,Q;

N = 3;
DET = 1;

for (i=0;i<N;i++)
{
 Q = mat[i][i]; DET=DET*Q;
 if (std::abs(Q)<EPS) {return false;};
 mat[i][i]=1;
 for (j=0;j<N;j++) { mat[i][j]=mat[i][j]/Q; };
 for (j=0;j<i;j++)
   {
   Q = mat[j][i]; mat[j][i]=0;
   for (k=0;k<N;k++) { mat[j][k] = mat[j][k]-mat[i][k]*Q; };
   };
 for (j=i+1;j<N;j++)
   {
   Q=mat[j][i]; mat[j][i]=0;
   for (k=0;k<N;k++) mat[j][k]=mat[j][k]-mat[i][k]*Q;
   };
};
return true;
} //inv_m_3


bool Libbase::inv_m_3_dyn(double** mat, double EPS)
/*
Процедура обращения матрицы размером 3х3 по методу Гаусса.
ДЛЯ ДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ С ИЗВЕСТНЫМ РАЗМЕРОМ 3x3
Эта функция использовалась для отладки.
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.5, стр. 36).
Из-за особенностей алгоритма могут возникать значительные ошибки
при обращении плохо обусловленных матриц.
*/

{
    int N,i,j,k;
    double DET,Q;

EPS = 0;
N   = 3;
DET = 1;

for (i=0;i<N;i++)
{
 Q = mat[i][i]; DET=DET*Q;
 if (std::abs(Q)<EPS) {return false;};
 mat[i][i]=1;
 for (j=0;j<N;j++) { mat[i][j]=mat[i][j]/Q; };
 for (j=0;j<i;j++)
   {
   Q = mat[j][i]; mat[j][i]=0;
   for (k=0;k<N;k++) { mat[j][k] = mat[j][k]-mat[i][k]*Q; };
   };
 for (j=i+1;j<N;j++)
   {
   Q=mat[j][i]; mat[j][i]=0;
   for (k=0;k<N;k++) mat[j][k]=mat[j][k]-mat[i][k]*Q;
   };
};
return true;
} //inv_m_3_dyn

bool Libbase::inv_m_dyn(double** mat, int N, double EPS)
/*
Функция обращения матрицы по методу Гаусса.
ДЛЯ ДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ С РАЗМЕРОМ NxN
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.5, стр. 36).
Из-за особенностей алгоритма могут возникать значительные ошибки
при обращении плохо обусловленных матриц.
Параметр EPS обычно задавался равным нулю.
*/

{
    int i,j,k;
    double DET,Q;

DET = 1;

for (i=0;i<N;i++)
{
 Q = mat[i][i]; DET=DET*Q;
 if (std::abs(Q)<EPS) {return false;};
 mat[i][i]=1;
 for (j=0;j<N;j++) { mat[i][j]=mat[i][j]/Q; };
 for (j=0;j<i;j++)
   {
   Q = mat[j][i]; mat[j][i]=0;
   for (k=0;k<N;k++) { mat[j][k] = mat[j][k]-mat[i][k]*Q; };
   };
 for (j=i+1;j<N;j++)
   {
   Q=mat[j][i]; mat[j][i]=0;
   for (k=0;k<N;k++) mat[j][k]=mat[j][k]-mat[i][k]*Q;
   };
};
return true;
} //inv_m_dyn

bool Libbase::inv_matr_3(double mat[][3], double EPS)
/*
Функция обращения матрицы размером 3х3 по методу Гаусса-Жордана
с выбором максимального элемента в строке.
ДЛЯ НЕДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ С РАЗМЕРОМ 3x3
Эта функция использовалась для отладки.
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.6, стр. 36-37).
Параметр EPS обычно задавался равным нулю.
*/
{
    int Z[3];
    int N,i,j,k;
    double DET,Q,R;
EPS=0; N=3; DET = 1;
for (k=0;k<N;k++) Z[k]=k;
for (i=0;i<N;i++) {
 k=i; R=mat[i][i];
 for(j=i+1;j<N;j++) { if (std::abs(R)<std::abs(mat[i][j])) { k=j; R=mat[i][j]; }; };
 if (std::abs(R)<EPS) {return false;};
 DET=DET*R;
 for (j=0;j<N;j++) {Q=mat[j][i]; mat[j][i]=mat[j][k]; mat[j][k]=Q; mat[i][j]=mat[i][j]/R; };
 mat[i][i]=1/R; j=Z[i]; Z[i]=Z[k]; Z[k]=j;
 for(j=0;j<i;j++)
  { Q=mat[j][i]; mat[j][i]=0; for(k=0;k<N;k++){ mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
 for (j=i+1;j<N;j++)
  { Q=mat[j][i]; mat[j][i]=0; for (k=0;k<N;k++) { mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
};
for (i=0;i<N;i++) {
 L1:
 k=Z[i];
 if (k!=i) {
 for (j=0;j<N;j++) { Q=mat[i][j]; mat[i][j]=mat[k][j]; mat[k][j]=Q; };
 Z[i]=Z[k]; Z[k]=k; DET=-DET; goto L1;
 };
};
return true;
} //inv_matr_3

bool Libbase::inv_matr_6(double mat[][6], double EPS)
/*
Функция обращения матрицы размером 3х3 по методу Гаусса-Жордана
с выбором максимального элемента в строке.
ДЛЯ НЕДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ С РАЗМЕРОМ 3x3
Эта функция использовалась для отладки.
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.6, стр. 36-37).
Параметр EPS обычно задавался равным нулю.
*/
{
    int Z[6];
    int N,i,j,k;
    double DET,Q,R;
EPS=0; N=6; DET = 1;
for (k=0;k<N;k++) Z[k]=k;
for (i=0;i<N;i++) {
 k=i; R=mat[i][i];
 for(j=i+1;j<N;j++) { if (std::abs(R)<std::abs(mat[i][j])) { k=j; R=mat[i][j]; }; };
 if (std::abs(R)<EPS) {return false;};
 DET=DET*R;
 for (j=0;j<N;j++) {Q=mat[j][i]; mat[j][i]=mat[j][k]; mat[j][k]=Q; mat[i][j]=mat[i][j]/R; };
 mat[i][i]=1/R; j=Z[i]; Z[i]=Z[k]; Z[k]=j;
 for(j=0;j<i;j++)
  { Q=mat[j][i]; mat[j][i]=0; for(k=0;k<N;k++){ mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
 for (j=i+1;j<N;j++)
  { Q=mat[j][i]; mat[j][i]=0; for (k=0;k<N;k++) { mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
};
for (i=0;i<N;i++) {
 L1:
 k=Z[i];
 if (k!=i) {
 for (j=0;j<N;j++) { Q=mat[i][j]; mat[i][j]=mat[k][j]; mat[k][j]=Q; };
 Z[i]=Z[k]; Z[k]=k; DET=-DET; goto L1;
 };
};
return true;
} //inv_matr_6

bool Libbase::inv_matr_3_dyn(double** mat, double EPS)
/*
Функция обращения матрицы по методу Гаусса-Жордана
с выбором максимального элемента в строке.
ДЛЯ ДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ С ИЗВЕСТНЫМ РАЗМЕРОМ 3x3
Эта функция использовалась для отладки.
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.6, стр. 36-37).
Параметр EPS обычно задавался равным нулю.
*/
{
    int Z[3];
    int N,i,j,k;
    double DET,Q,R;
EPS=0; N=3; DET = 1;
for (k=0;k<N;k++) Z[k]=k;
for (i=0;i<N;i++) {
 k=i; R=mat[i][i];
 for(j=i+1;j<N;j++) { if (std::abs(R)<std::abs(mat[i][j])) { k=j; R=mat[i][j]; }; };
 if (std::abs(R)<EPS) {return false;};
 DET=DET*R;
 for (j=0;j<N;j++) {Q=mat[j][i]; mat[j][i]=mat[j][k]; mat[j][k]=Q; mat[i][j]=mat[i][j]/R; };
 mat[i][i]=1/R; j=Z[i]; Z[i]=Z[k]; Z[k]=j;
 for(j=0;j<i;j++)
  { Q=mat[j][i]; mat[j][i]=0; for(k=0;k<N;k++){ mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
 for (j=i+1;j<N;j++)
  { Q=mat[j][i]; mat[j][i]=0; for (k=0;k<N;k++) { mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
};
for (i=0;i<N;i++) {
 L1:
 k=Z[i];
 if (k!=i) {
 for (j=0;j<N;j++) { Q=mat[i][j]; mat[i][j]=mat[k][j]; mat[k][j]=Q; };
 Z[i]=Z[k]; Z[k]=k; DET=-DET; goto L1;
 };
};
return true;
} //inv_matr_3_dyn

bool Libbase::inv_matr_dyn(double** mat, int N, double EPS)
/*
Функция обращения матрицы по методу Гаусса-Жордана
с выбором максимального элемента в строке.
См. сравочник
"БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ.
Для системы программирования на языке PL/1 для Windows-XP",
(п. 4.6, стр. 36-37).
Матрица квадратная, размер задается параметром N.
Параметр EPS обычно задавался равным нулю.
Данная функция применялась для обращения матриц в качестве основной.
*/
{
    int Z[3];
    int i,j,k;
    double DET,Q,R;
EPS=0; DET = 1;
for (k=0;k<N;k++) Z[k]=k;
for (i=0;i<N;i++) {
 k=i; R=mat[i][i];
 for(j=i+1;j<N;j++) { if (std::abs(R)<std::abs(mat[i][j])) { k=j; R=mat[i][j]; }; };
 if (std::abs(R)<EPS) {return false;};
 DET=DET*R;
 for (j=0;j<N;j++) {Q=mat[j][i]; mat[j][i]=mat[j][k]; mat[j][k]=Q; mat[i][j]=mat[i][j]/R; };
 mat[i][i]=1/R; j=Z[i]; Z[i]=Z[k]; Z[k]=j;
 for(j=0;j<i;j++)
  { Q=mat[j][i]; mat[j][i]=0; for(k=0;k<N;k++){ mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
 for (j=i+1;j<N;j++)
  { Q=mat[j][i]; mat[j][i]=0; for (k=0;k<N;k++) { mat[j][k]=mat[j][k]-mat[i][k]*Q; }; };
};
for (i=0;i<N;i++) {
 L1:
 k=Z[i];
 if (k!=i) {
 for (j=0;j<N;j++) { Q=mat[i][j]; mat[i][j]=mat[k][j]; mat[k][j]=Q; };
 Z[i]=Z[k]; Z[k]=k; DET=-DET; goto L1;
 };
};
return true;
} //inv_matr_dyn

void Libbase::mult_matr_vect_3(double A[][3], double B[3], double C[3])
{
    int I,J,N,M;
    double R;

M= 3;
N=M;

for (I=0; I<M; I++)
 {
 R=0;
 for (J=0; J<N; J++) { R=R+A[I][J]*B[J]; };
 C[I]=R;
 };

} // mult_matr_vect_3

void Libbase::mult_matr_vect_6(double A[][6], double B[6], double C[6])
{
    int I,J,N,M;
    double R;

M= 6;
N=M;

for (I=0; I<M; I++)
 {
 R=0;
 for (J=0; J<N; J++) { R=R+A[I][J]*B[J]; };
 C[I]=R;
 };

} // mult_matr_vect_6

void Libbase::mult_matr_3(double matA[][3], double matB[][3], double matC[][3])
/*
Умножение двух массивов (квадратных матриц) размером 3x3.
ДЛЯ НЕДИНАМИЧЕСКИХ МАССИВОВ.
Содержимое преремножаемых матриц matA и matB не изменяется,
результат записывается в матрицу matC.
*/

{
    int i,j,k,Q,P,N;
    double R;

N = 3, Q = N; P = N;

for(i=0;i<N;i++)
{
 for (j=0;j<Q;j++)
 {
  R=0;
  for (k=0;k<P;k++) { R=R+matA[i][k]* matB[k][j]; };
  matC[i][j]=R;
 };
};
} //mult_matr_3

void Libbase::mult_matr_6(double matA[][6], double matB[][6], double matC[][6])
/*
Умножение двух массивов (квадратных матриц) размером 6x6.
ДЛЯ НЕДИНАМИЧЕСКИХ МАССИВОВ.
Содержимое преремножаемых матриц matA и matB не изменяется,
результат записывается в матрицу matC.
*/

{
    int i,j,k,Q,P,N;
    double R;

N = 6, Q = N; P = N;

for(i=0;i<N;i++)
{
 for (j=0;j<Q;j++)
 {
  R=0;
  for (k=0;k<P;k++) { R=R+matA[i][k]* matB[k][j]; };
  matC[i][j]=R;
 };
};
} //mult_matr_6

void Libbase::mult_matr_3_dyn(double** matA, double** matB, double** matC)
/*
Умножение двух ДИНАМИЧЕСКИХ МАССИВОВ (квадратных матриц) размером NxN.
Содержимое преремножаемых матриц matA и matB не изменяется,
результат записывается в матрицу matC.
*/

{
    int i,j,k,Q,P,N;
    double R;

N = 3; Q = N; P = N;

for(i=0;i<N;i++)
{
 for (j=0;j<Q;j++)
 {
  R=0;
  for (k=0;k<P;k++) { R=R+matA[i][k]* matB[k][j]; };
  matC[i][j]=R;
 };
};
} //mult_matr_3_dyn

void Libbase::mult_matr_dyn(double** matA, double** matB, double** matC, int N)
/*
Умножение двух ДИНАМИЧЕСКИХ МАССИВОВ (квадратных матриц) размером NxN.
Содержимое преремножаемых матриц matA и matB не изменяется,
результат записывается в матрицу matC.
*/

{
    int i,j,k,Q,P;
    double R;

Q = N; P = N;

for(i=0;i<N;i++)
{
 for (j=0;j<Q;j++)
 {
  R=0;
  for (k=0;k<P;k++) { R=R+matA[i][k]* matB[k][j]; };
  matC[i][j]=R;
 };
};
} //mult_matr_dyn

bool Libbase::solv_syst_3(double A[][3], double B[3], double C[3])
{
if (Libbase::inv_matr_3(A,0.0) == false) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   {return false;}
   else
   {Libbase::mult_matr_vect_3(A,B,C); return true;};
} // solv_syst_3

bool Libbase::solv_syst_6(double A[][6], double B[6], double C[6])
{
if (Libbase::inv_matr_6(A,0.0) == false) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   {return false;}
   else
   {Libbase::mult_matr_vect_6(A,B,C); return true;};
} // solv_syst_6

void Libbase::copy_matr_3(double matS[][3], double matD[][3])
/*
Копирование содержимого квадратной матрицы в другую матрицу.
ДЛЯ НЕДИНАМИЧЕСКИХ ДВУМЕРНЫХ МАССИВОВ РАЗМЕРОМ 3х3.
*/
{
    int i,j,N;

    N = 3;
    for (i=0;i<N;i++) for (j=0;j<N;j++) matD[i][j]=matS[i][j];
} //copy_matr_3


void Libbase::copy_matr_dyn(double** matS, double** matD, int N)
/*
Копирование содержимого квадратной матрицы в другую матрицу.
Размер матриц - NxN.
*/
{
    int i,j;

    for (i=0;i<N;i++) for (j=0;j<N;j++) matD[i][j]=matS[i][j];
} //copy_matr_dyn

void Libbase::show_matr_3(double matS[][3], string strHead)
/*
Вывод содержимого квадратной матрицы на экран.
ДЛЯ НЕДИНАМИЧЕСКИХ МАССИВОВ РАЗМЕРОМ 3x3.
Функция использовалась для отладки.
*/
{
    int i,j,N;

    N = 3;
//    cout << endl << strHead << endl;

    for (i=0;i<N;i++)
    {for (j=0;j<N;j++) cout << setiosflags(ios::fixed) << setiosflags(ios::left) \
                            << setiosflags(ios::showpos) << matS[i][j] << "  ";
//     cout << endl;
    }

} //show_matr_3


void Libbase::show_matr_dyn(double** matS, string strHead, int N)
/*
Вывод содержимого квадратной матрицы на экран.
ДЛЯ ДИНАМИЧЕСКИХ МАССИВОВ РАЗМЕРОМ NxN.
Функция использовалась для отладки.
*/
{
    int i,j;

    cout << endl << strHead << endl;

    for (i=0;i<N;i++)
    {for (j=0;j<N;j++) cout << setiosflags(ios::fixed) << setiosflags(ios::left) \
                            << setiosflags(ios::showpos) << matS[i][j] << "  ";
//     cout << endl;
    }

} //show_matr_dyn

void Libbase::cross_mult_3(double A[3], double B[3], double C[3])
{

C[0] =     A[1]*B[2] - A[2]*B[1];

C[1] = - ( A[0]*B[2] - A[2]*B[0] ) ;

C[2] =     A[0]*B[1] - A[1]*B[0];

} //cross_mult_3

double Libbase::dot_mult_3(double A[3], double B[3])
{
  int I,M;
  double R;

M= 2;
R = 0;

for (I=0; I<=M;I++){ R = R + A[I]*B[I];};

return R;

} //dot_mult_3

void Libbase::norm_vect_3(double A[3], double B[3])
{
    int I,M;
    double R;

M= 2;
R = 0;

R = sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2]);

for(I = 0;I<=M;I++) {B[I] = A[I]/R;};

} //norm_vect_3

double Libbase::length_vect_3(double A[3])
{
    int I,M;
    double R;

M= 2;
R = 0;

for (I=0; I<=M;I++) {R = R + A[I]*A[I];};
return (sqrt(R));

} //length_vect_3

void Libbase::Jul_to_Date_Time_arr(double dJulian, int int3Date[3], int int4Time[4])
/*
Процедура для перевода юлианской даты в "обычные" дату и время GMT
Текст поцедуры заимствован.
Восходит к Astronomical Formulae for Calculators, Jean Meeus, pages 26-27
*/
{
    int		month, alpha;
    double	Z,A,B,C,D,E,F;
    double	day,year;
    double	hour, minute, second;
//    double hseconds; // для сотых долей секунд
    double mseconds; // для тысячных долей секунд

    F = modf(dJulian + 0.5,&Z);
    if (Z < 2299161)
        A = Z;
    else	{
        alpha = (int)((Z - 1867216.25)/36524.25);
        A = Z + 1.0 + alpha - (int)(alpha/4);
    }
    B   = A + 1524.0;
    modf((B - 122.1)/365.25, &C);
    modf(365.25 * C, &D);
    modf((B - D)/30.6001, &E);
    day = B - D - (int)(30.6001 * E) + F;

    if (E < 13.5)	month = round(E - 1.0);
    else			month = round(E - 13.0);
    if (month > 2.5)year  = C - 4716.0;
    else			year  = C - 4715.0;

    A = modf(day, &Z);
    A = modf(A*24.0, &hour);  //т.е. A/(1/24)
    A = modf(A*60.0, &minute);
    A = modf(A*60.0, &second);
//    A = modf(A*100,&hseconds);
    A = modf(A*1000,&mseconds);

    int3Date[0]			= (int)year;
    int3Date[1]			= (int)month;
    int3Date[2]			= (int)day;
    int4Time[0]			= (int)hour;
    int4Time[1]			= (int)minute;
    int4Time[2]			= (int)second; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //int4Time[3]	        = (int)hseconds;
    int4Time[3]	        = (int)mseconds;

} //Jul_to_Date_Time_arr

double Libbase::Date_Time_to_Jul_arr(int int3Date[3], int int4Time[4], double factor_s)
/*
Заимствованная функция для перевода даты и времени GMT в юлианскую дату
*/
{
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int i;
    double arg, arg2, fFraction;
    int iYear = (int)int3Date[0]-1;
    int iDay  = (int)int3Date[2];
    double A,B;

    for (i=0;i < int3Date[1]-1;i++) iDay += days[i];	// addiere die Tage des Jahres ...
    if	(((int3Date[0] %4) == 0) &&
        (((int3Date[0] %100) != 0) || ((int3Date[0] %400) == 0)) &&
        (int3Date[1] > 2)) iDay += 1;		// Schaltjahr ???

    modf(iYear/100.0,&A);
    modf(A/4.0,&B);
    B += 2.0-A;

    modf(365.25 * iYear, &arg);
//	modf(30.6001 * 14, &arg2);
    arg2 = 428;
    arg += arg2 + 1720994.5 + B;
    fFraction = (double) (int4Time[0]*60.0*60.0	+
                          int4Time[1]*60.0		+
                          //int4Time[2] + int4Time[3]/100.0  )/(24.0*60.0*60.0);
                          int4Time[2] + int4Time[3]/factor_s  )/(24.0*60.0*60.0);
    return arg+iDay+fFraction;
} //Date_Time_to_Jul_arr


int Libbase::DOY(int int3Date[3])

{
  int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};

  int  i,day,yr,mo,dy;

  yr = int3Date[0];
  mo = int3Date[1];
  dy = int3Date[2];

  day = 0;
  for (i=0; i < mo-1; i++) { day = day + days[i]; };
  day = day + dy;
  if (((yr % 4) == 0) && (((yr % 100) != 0) || ((yr % 400) == 0)) && (mo > 2))
    day = day + 1;
  return day;
} // DOY

double Libbase::Frac_of_Day(int int4Time[4], double factor_s)

{
    double hr,mi,se,sse;

    hr = (double)int4Time[0];
    mi = (double)int4Time[1];
    se = (double)int4Time[2];
    sse = (double)int4Time[3];

  return (hr + (mi + (se + sse/factor_s)/60.0)/60.0)/24.0;
} //Frac_of_Day}

double Libbase::delt_vect_3(double A[3], double B[3])
{
    double R;

R = sqrt((A[0]-B[0])*(A[0]-B[0]) + (A[1]-B[1])*(A[1]-B[1]) + (A[2]-B[2])*(A[2]-B[2]));

return R;

} //delt_vect_3

bool Libbase::write_to_file(string strFile, string strMessage)
/*
Вывод сообщения в файл.
*/
{

   ofstream ffout;

   ffout.open(strFile, ios::out);
   ffout << strMessage << endl;

   ffout.close();

   return true;

} // write_to_file
