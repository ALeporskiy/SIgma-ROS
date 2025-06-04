#pragma once

#include <QObject>

class SaveHelper : public QObject {
    Q_OBJECT
public:
    explicit SaveHelper(QObject *parent = nullptr);

    Q_INVOKABLE void saveJsonToFile(const QString &path, const QString &jsonContent);
    Q_INVOKABLE QString loadJsonFromFile(const QString &path);
};
