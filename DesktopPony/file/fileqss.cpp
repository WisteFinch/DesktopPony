#include "file/fileqss.h"

QSS::QSS()
{
    UISettings = readQSS(pathUISettings);
}

QString QSS::readQSS(QString path)
{
    QString temp;
    QFile *f = new QFile(path);
    f->open(QFile::ReadOnly);
    temp = f->readAll();
    return temp;
}
