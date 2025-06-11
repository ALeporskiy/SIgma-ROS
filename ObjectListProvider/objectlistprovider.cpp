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

void ObjectListProvider::addObject(const QVariantMap &obj) {
    m_objects.append(obj);
    emit objectsChanged();
}

void ObjectListProvider::saveToJson(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Не удалось открыть файл для записи:" << path;
        return;
    }

    QJsonArray array;
    for (const QVariant &v : m_objects) {
        array.append(QJsonObject::fromVariantMap(v.toMap()));
    }

    QJsonObject root;
    root["objects"] = array;

    QJsonDocument doc(root);
    file.write(doc.toJson());
}

void ObjectListProvider::removeAt(int index) {
    if (index < 0 || index >= m_objects.size())
        return;

    m_objects.removeAt(index);
    emit objectsChanged();
    saveToJson("predefined_objects.json");
}
