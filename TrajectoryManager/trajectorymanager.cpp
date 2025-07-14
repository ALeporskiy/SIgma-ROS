#include "trajectorymanager.h"

#include "libtle.h"
#include "libbase.h"
#include <cmath>

#include <QMetaType>
#include <QGeoCoordinate>



TrajectoryManager::TrajectoryManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &TrajectoryManager::startTracking);
    m_timer.setInterval(100); // обновлять каждые 100 мс

    connect(&m_refreshTrajectoryTimer, &QTimer::timeout, this, [this]() {
        //qDebug() << "[ТРАЕКТОРИЯ] Перерасчет траектории через 90 мин";
        loadTLE(m_lastLoadedFile); // будем хранить путь к TLE
    });
}

void TrajectoryManager::loadTLE(const QString &filename)
{
    std::string sat_data[2];
    std::string name;
    m_lastLoadedFile = filename;

    if (!Libtle::read_tle(filename.toStdString(), sat_data, &name)) {
        qWarning("Ошибка при чтении TLE файла");
        return;
    }

    // qDebug() << "sat_data[0]:" << QString::fromStdString(sat_data[0]);
    // qDebug() << "sat_data[1]:" << QString::fromStdString(sat_data[1]);

    // Подготовка SGP4 и эпоха
    Libtle::PreparePrognSGP4(sat_data, &m_julian_epoch, m_A0);
    m_trajectory.clear();

    // Получение текущего времени UTC
    int d[3], t[4];
    Libtle::Curr_Date_Time_UTC(d, t);

    // Смещение от эпохи TLE в секундах
    double dt_now = Libtle::Date_Time_for_PROGN_arr(d, t, m_julian_epoch);

    // Расчёт траектории от текущего момента до +90 мин (5400 сек)
    int t_start = static_cast<int>(dt_now);
    int t_end = t_start + 5400;

    for (int t = t_start; t <= t_end; t += 1) {
        double pos[3], vel[3], geo[3];

        Libbase::PrognSGP4(pos, vel, t, m_A0);
        Libbase::Conv_Inert_Gr(pos, vel, m_julian_epoch + t / 86400.0, geo, vel);

        double h, lon, lat;
        Libbase::decart_to_ell(geo, const_f, const_xkmper, &h, &lon, &lat);
        lon = Libtle::lambda_360_to_180(lon);

        m_trajectory.append(QVariant::fromValue(QGeoCoordinate(lat, lon)));
    }



    std::string line2 = sat_data[1];
    double meanMotion = std::stod(line2.substr(52, 11));
    int revAtEpoch = std::stoi(line2.substr(63, 5));
    m_meanMotion = meanMotion;
    m_revAtEpoch = revAtEpoch;



    // qDebug() << "meanMotion = " << meanMotion;

    emit trajectoryChanged();
    m_timer.start();

    // перезапускаем таймер пересчета траектории
    m_refreshTrajectoryTimer.stop();
    m_refreshTrajectoryTimer.start(0.1 * 60 * 1000); // 10 секунд
}

void TrajectoryManager::startTracking()
{

    int d[3], t[4];
    Libtle::Curr_Date_Time_UTC(d, t);

    // Получить смещение от эпохи в секундах
    double dt = Libtle::Date_Time_for_PROGN_arr(d, t, m_julian_epoch);

    double pos[3], vel[3], geo[3];
    Libbase::PrognSGP4(pos, vel, dt, m_A0);
    Libbase::Conv_Inert_Gr(pos, vel, m_julian_epoch + dt / 86400.0, geo, vel);

    double h, lon, lat;
    Libbase::decart_to_ell(geo, const_f, const_xkmper, &h, &lon, &lat);
    lon = Libtle::lambda_360_to_180(lon);

    m_currentPosition = QGeoCoordinate(lat, lon);

    m_currentAltitudeKm = h;



    double n0 = m_meanMotion; // обороты в сутки
    double days_since_epoch = dt / 86400.0;

    // qDebug() << "revAtEpoch:" << m_revAtEpoch;
    // qDebug() << "meanMotion:" << n0;
    // qDebug() << "daysSinceEpoch:" << days_since_epoch;
    // qDebug() << "resulting rev:" << m_currentRevNumber;


    m_currentRevNumber = static_cast<int>(m_revAtEpoch + std::floor(n0 * days_since_epoch));

    emit currentRevNumberChanged();

    emit currentAltitudeChanged();

    emit currentPositionChanged();
}

QVariantList TrajectoryManager::trajectory() const
{
    return m_trajectory;
}

QGeoCoordinate TrajectoryManager::currentPosition() const
{
    return m_currentPosition;
}

double TrajectoryManager::currentAltitudeKm() const {
    return m_currentAltitudeKm;
}

int TrajectoryManager::currentRevNumber() const {
    return m_currentRevNumber;
}

QGeoCoordinate TrajectoryManager::calculateSubsolarPoint() const {
    int d[3], t[4];
    Libtle::Curr_Date_Time_UTC(d, t);

    double jd = Libbase::Julian_Date(d[0], d[1], d[2], t[0], t[1], static_cast<double>(t[2]));
    double dSinceJ2000 = jd - 2451545.0;

    double meanLongitude = fmod(280.460 + 0.9856474 * dSinceJ2000, 360.0);
    double meanAnomaly = fmod(357.528 + 0.9856003 * dSinceJ2000, 360.0);
    double lambda = meanLongitude + 1.915 * sin(qDegreesToRadians(meanAnomaly)) +
                    0.020 * sin(qDegreesToRadians(2 * meanAnomaly));
    double epsilon = 23.439 - 0.0000004 * dSinceJ2000;

    double declination = qRadiansToDegrees(asin(sin(qDegreesToRadians(epsilon)) * sin(qDegreesToRadians(lambda))));
    double eqTime = -1.915 * sin(qDegreesToRadians(meanAnomaly)) - 0.020 * sin(qDegreesToRadians(2 * meanAnomaly));
    double solarTime = fmod(t[0] * 60.0 + t[1] + eqTime, 1440.0); // в минутах
    double subsolarLon = fmod(180.0 - solarTime * 0.25, 360.0);

    if (subsolarLon > 180.0) subsolarLon -= 360.0;


    return QGeoCoordinate(declination, subsolarLon);
}

QGeoCoordinate TrajectoryManager::calculateSublunarPoint() const
{
    int d[3], t[4];
    Libtle::Curr_Date_Time_UTC(d, t);

    double jd = Libbase::Julian_Date(d[0], d[1], d[2], t[0], t[1], static_cast<double>(t[2]));
    double dSinceJ2000 = jd - 2451545.0;

    // Средняя долгота Луны
    double L = fmod(218.316 + 13.176396 * dSinceJ2000, 360.0);

    // Средное расстояние Луны от восходящего узла
    double F = fmod(93.272 + 13.229350 * dSinceJ2000, 360.0);

    // Средная аномалия Луны
    double M_moon = fmod(134.963 + 13.064993 * dSinceJ2000, 360.0);

    // Средная аномалия Солнца
    // double M_sun = fmod(357.529 + 0.9856003 * dSinceJ2000, 360.0);

    // Прямое восхождение и склонение Луны (приближённо)
    double lambda = L + 6.289 * sin(qDegreesToRadians(M_moon)); // эклиптическая долгота
    double beta = 5.128 * sin(qDegreesToRadians(F));            // эклиптическая широта

    double epsilon = 23.439 - 0.0000004 * dSinceJ2000;

    // Перевод в экваториальные координаты
    double sinLambda = sin(qDegreesToRadians(lambda));
    double cosLambda = cos(qDegreesToRadians(lambda));
    double sinBeta = sin(qDegreesToRadians(beta));
    double cosBeta = cos(qDegreesToRadians(beta));
    double sinEpsilon = sin(qDegreesToRadians(epsilon));
    double cosEpsilon = cos(qDegreesToRadians(epsilon));

    double x = cosBeta * cosLambda;
    double y = cosBeta * sinLambda * cosEpsilon - sinBeta * sinEpsilon;
    double z = cosBeta * sinLambda * sinEpsilon + sinBeta * cosEpsilon;

    double declination = qRadiansToDegrees(asin(z));
    double rightAscension = qRadiansToDegrees(atan2(y, x));

    // Перевод во время и географическую долготу
    double GMST = fmod(280.46061837 + 360.98564736629 * dSinceJ2000, 360.0);
    double sublon = fmod(rightAscension - GMST + 360.0, 360.0);
    if (sublon > 180.0)
        sublon -= 360.0;

    return QGeoCoordinate(declination, sublon);
}




void TrajectoryManager::updateTerminator()
{
    m_terminatorPoints.clear();
    QVector<QGeoCoordinate> path = computeTerminatorPath();
    for (const QGeoCoordinate &coord : path)
        m_terminatorPoints.append(QVariant::fromValue(coord));
    updateTwilights();
    emit terminatorUpdated();
}

QVariantList TrajectoryManager::terminatorPath() const
{
    return m_terminatorPoints;
}

QVector<QGeoCoordinate> TrajectoryManager::computeTerminatorPath()
{
    QVector<QGeoCoordinate> points;
    QGeoCoordinate subsolar = calculateSubsolarPoint();

    double sunLat = subsolar.latitude();
    double sunLon = subsolar.longitude();

    // Верхняя граница — линия терминатора
    for (int lon = -180; lon <= 180; lon += 2) {
        double deltaLon = lon - sunLon;
        double sunLatRad = qDegreesToRadians(sunLat);
        double latRad = atan(-cos(qDegreesToRadians(deltaLon)) / tan(sunLatRad));
        double lat = qRadiansToDegrees(latRad);
        points.append(QGeoCoordinate(lat, lon));
    }

    // Нижняя граница — юг карты, в обратную сторону
    // for (int lon = 180; lon >= -180; lon -= 2) {
    //     points.append(QGeoCoordinate(-90.0, lon)); // или -90, если ты используешь полный полюс
    // }

    // // Замыкаем фигуру
    points.append(points.first());

    return points;
}



QVariantList TrajectoryManager::terminatorShadowPolygon()
{
    QVector<QGeoCoordinate> terminator;

    QGeoCoordinate subsolar = calculateSubsolarPoint();
    double sunLat = subsolar.latitude();
    double sunLon = subsolar.longitude();

    double sunLatRad = qDegreesToRadians(sunLat);

    // Строим линию терминатора
    for (int lon = -180; lon <= 180; lon += 1) {
        double deltaLon = lon - sunLon;
        double latRad = atan(-cos(qDegreesToRadians(deltaLon)) / tan(sunLatRad));
        double lat = qRadiansToDegrees(latRad);
        terminator.append(QGeoCoordinate(lat, lon));
    }

    QVector<QGeoCoordinate> path;

    // 1. Терминатор (в прямом порядке)
    for (const QGeoCoordinate &coord : terminator)
        path.append(coord);

    // 2. Добавим южное полушарие (нижнюю границу закраски)
    for (int i = terminator.size() - 1; i >= 0; --i)
        path.append(QGeoCoordinate(-85.0, terminator[i].longitude()));

    // 3. Замыкаем
    path.append(path.first());

    QVariantList result;
    for (const QGeoCoordinate &coord : path)
        result.append(QVariant::fromValue(coord));

    return result;
}







double TrajectoryManager::latitudeForSolarAltitude(double hDeg,
                                                   double declRad,
                                                   double hourAngleRad) const
{
    const double hRad = qDegreesToRadians(hDeg);

    for (double lat = -90.0; lat <= 90.0; lat += 0.1) {
        const double latRad = qDegreesToRadians(lat);
        const double sinH  = sin(latRad) * sin(declRad)
                            + cos(latRad) * cos(declRad) * cos(hourAngleRad);
        if (qAbs(asin(sinH) - hRad) < 0.002)   // ≈0.1°
            return lat;
    }
    return 999.0; // не найдено
}

QVariantList TrajectoryManager::computeTwilightBand(double hMinDeg,
                                                    double hMaxDeg) const
{
    QVector<QGeoCoordinate> upper, lower;

    const QGeoCoordinate subsolar = calculateSubsolarPoint();
    const double declRad = qDegreesToRadians(subsolar.latitude());

    for (int lon = -180; lon <= 180; lon += 2) {
        const double Hrad = qDegreesToRadians(lon - subsolar.longitude());
        const double latTop = latitudeForSolarAltitude(hMinDeg, declRad, Hrad);
        const double latBot = latitudeForSolarAltitude(hMaxDeg, declRad, Hrad);
        if (qAbs(latTop) < 90.0 && qAbs(latBot) < 90.0) {
            upper.append(QGeoCoordinate(latTop, lon));
            lower.prepend(QGeoCoordinate(latBot, lon));
        }
    }

    QVector<QGeoCoordinate> poly;
    poly << upper << lower;
    if (!upper.isEmpty())
        poly << upper.first();

    QVariantList res;
    for (const auto &c : poly)
        res.append(QVariant::fromValue(c));
    return res;
}

void TrajectoryManager::updateTwilights()
{
    m_civil  = computeTwilightBand(-6.0,   0.0);
    m_nautic = computeTwilightBand(-12.0, -6.0);
    m_astro  = computeTwilightBand(-18.0, -12.0);

}




