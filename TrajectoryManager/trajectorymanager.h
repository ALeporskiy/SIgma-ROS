#ifndef TRAJECTORYMANAGER_H
#define TRAJECTORYMANAGER_H

#include <QObject>
#include <QGeoCoordinate>
#include <QVariantList>
#include <QTimer>

class TrajectoryManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList trajectory READ trajectory NOTIFY trajectoryChanged)
    Q_PROPERTY(QGeoCoordinate currentPosition READ currentPosition NOTIFY currentPositionChanged)
    Q_PROPERTY(double currentAltitudeKm READ currentAltitudeKm NOTIFY currentAltitudeChanged)
    Q_PROPERTY(int currentRevNumber READ currentRevNumber NOTIFY currentRevNumberChanged)
    Q_PROPERTY(QVariantList terminatorPath READ terminatorPath NOTIFY terminatorUpdated)
    Q_PROPERTY(QVariantList shadowPolygon READ terminatorShadowPolygon NOTIFY terminatorUpdated)
    Q_PROPERTY(QVariantList civilTwilight   READ civilTwilight   NOTIFY terminatorUpdated)
    Q_PROPERTY(QVariantList nauticalTwilight READ nauticalTwilight NOTIFY terminatorUpdated)
    Q_PROPERTY(QVariantList astroTwilight   READ astroTwilight   NOTIFY terminatorUpdated)
    Q_PROPERTY(QVariantList nightBand READ nightBand NOTIFY terminatorUpdated)


public:
    explicit TrajectoryManager(QObject *parent = nullptr);

    Q_INVOKABLE void loadTLE(const QString &filename);
    Q_INVOKABLE void startTracking();
    Q_INVOKABLE QGeoCoordinate calculateSubsolarPoint() const;
    Q_INVOKABLE QGeoCoordinate calculateSublunarPoint() const;
    Q_INVOKABLE void updateTerminator();
    Q_INVOKABLE QVariantList terminatorShadowPolygon();
    Q_INVOKABLE QVariantList civilTwilight()   const { return m_civil;   }
    Q_INVOKABLE QVariantList nauticalTwilight()const { return m_nautic; }
    Q_INVOKABLE QVariantList astroTwilight()   const { return m_astro;  }
    Q_INVOKABLE QVariantList nightBand() const { return m_night; }
    //Q_INVOKABLE QVariantList computeNightPolygon() const;

    QVariantList terminatorPath() const;



    QVariantList trajectory() const;

    QGeoCoordinate currentPosition() const;
    double currentAltitudeKm() const;
    int currentRevNumber() const;




signals:
    void trajectoryChanged();       // траектория (трасса полета)
    void currentPositionChanged();  // координаты
    void currentAltitudeChanged();  // высота
    void currentRevNumberChanged(); // виток
    void terminatorUpdated();



private:
    QVariantList m_trajectory;
    QGeoCoordinate m_currentPosition;
    QVariantList m_terminatorPoints;
    QVector<QGeoCoordinate> computeTerminatorPath();
    QVariantList computeNightPolygon() const;
    void updateTwilights();
    double m_A0[14];
    double m_julian_epoch;
    QTimer m_timer;
    QTimer m_refreshTrajectoryTimer;
    QString m_lastLoadedFile;
    double m_currentLat = 0.0;
    double m_currentLon = 0.0;
    double m_currentAltitudeKm = 0.0;
    int m_currentRevNumber = 0;
    double m_meanMotion = 0.0;
    double m_revAtEpoch = 0.0;
    QVariantList m_civil, m_nautic, m_astro, m_night;
    QVariantList computeTwilightBand(double hMinDeg, double hMaxDeg) const;
    double latitudeForSolarAltitude(double hDeg,
                                    double declRad,
                                    double hourAngleRad) const;
};

#endif // TRAJECTORYMANAGER_H
