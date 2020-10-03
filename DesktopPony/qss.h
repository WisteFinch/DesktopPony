#ifndef QSS_H
#define QSS_H

#include <QString>
#include <QFile>
#include <QTextStream>
class QSS
{
public:
    QSS();
    void setQSS();

    QString UISettings;
private:
    QString readQSS(QString path);
    const QString pathUISettings = ":/QSS/uisettings.css";
};

#endif // QSS_H
