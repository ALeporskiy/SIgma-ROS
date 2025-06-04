#include "ObjectListProvider.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QDebug>

ObjectListProvider::ObjectListProvider(QObject *parent)
    : QObject(parent) {}

QVariantList ObjectListProvider::objects() const {
    return m_objects;
}

void ObjectListProvider::loadFromJson(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть JSON:" << path;
        return;
    }

    const auto data = file.readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    const QJsonArray array = doc.object()["objects"].toArray();

    QVariantList result;
    for (const QJsonValue &v : array) {
        result.append(v.toObject().toVariantMap());
    }

    m_objects = result;
    emit objectsChanged();
}
