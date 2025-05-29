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

public:
    explicit TrajectoryManager(QObject *parent = nullptr);

    Q_INVOKABLE void loadTLE(const QString &filename);
    Q_INVOKABLE void startTracking();

    QVariantList trajectory() const;
    QGeoCoordinate currentPosition() const;

signals:
    void trajectoryChanged();
    void currentPositionChanged();

private:
    QVariantList m_trajectory;
    QGeoCoordinate m_currentPosition;

    double m_A0[14];
    double m_julian_epoch;
    QTimer m_timer;
};

#endif // TRAJECTORYMANAGER_H
