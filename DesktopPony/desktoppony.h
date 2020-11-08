#ifndef DESKTOPPONY_H
#define DESKTOPPONY_H

#include "limit.h"
#include "imageprocessing/imageprocessing.h"
#include "ui/uisettings.h"
#include "file/config.h"
#include "file/filetasks.h"
#include "metadata.h"
#include "file/filecharacter.h"
#include "qss.h"
#include "ui/uipony.h"
#include "file/localisation.h"
#include <QObject>

class DesktopPony : QObject
{
    Q_OBJECT
public:
    DesktopPony();
    void start();
    void initConnect();

private:
    FileTasks *fileTasks;
    Config *config;
    Localisation *localisation;
    FileCharacter *fileCharacter;
    Limit *limit;
    QSS *qss;
    UISettings *uiSettings;
    UIPony *uiPony;
    ImageProcessing *imageProcessing;

private slots:
    void slotSettings(int);
};

#endif // DESKTOPPONY_H
