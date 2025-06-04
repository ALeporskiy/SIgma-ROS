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

signals:
    void objectsChanged();

private:
    QVariantList m_objects;
};
