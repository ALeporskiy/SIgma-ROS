#include "savehelperlib.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QUrl>

SaveHelper::SaveHelper(QObject *parent)
    : QObject(parent) {}

void SaveHelper::saveJsonToFile(const QString &path, const QString &jsonContent) {
    QString cleanPath = QUrl(path).toLocalFile();
    qDebug() << "Сохраняем в:" << cleanPath;
    QFile file(cleanPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Не удалось сохранить файл:" << cleanPath;
        return;
    }

    QTextStream out(&file);
    out << jsonContent;
    file.close();
}

QString SaveHelper::loadJsonFromFile(const QString &path) {
    QString cleanPath = QUrl(path).toLocalFile();
    QFile file(cleanPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для чтения:" << cleanPath;
        return "";
    }

    QTextStream in(&file);
    QString json = in.readAll();
    file.close();
    return json;
}
