#include "trajectorymanager.h"

#include "libtle.h"
#include "libbase.h"

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
