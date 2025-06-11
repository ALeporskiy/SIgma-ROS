#pragma once
#include <QObject>
#include <QVariantList>

class ObjectListProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList objects READ objects NOTIFY objectsChanged)

public:
    explicit ObjectListProvider(QObject *parent = nullptr);
    QVariantList objects() const;

    Q_INVOKABLE void loadFromJson(const QString &path);

    Q_INVOKABLE void addObject(const QVariantMap &obj);
    Q_INVOKABLE void saveToJson(const QString &path);
    Q_INVOKABLE void removeAt(int index);

signals:
    void objectsChanged();

private:
    QVariantList m_objects;
};
