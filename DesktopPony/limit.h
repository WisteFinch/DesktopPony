#ifndef LIMIT_H
#define LIMIT_H

#include <QString>
#include <QDesktopWidget>
#include <QApplication>

class Limit
{
public:
    Limit();
    int widgetPosition(int x, int y);
    void getScreenSize();
    int screenSizeX,screenSizeY;

    struct spinBoxData
    {
        int min;
        int max;
    };

    struct tabWidgetData
    {
        QString title;
    };

    struct buttonData
    {
        int fixedHeight;
        int fixedWidth;
        QString icon;
    };

    struct formData
    {
        int minWidth;
        int minHeight;
        int maxWidth;
        int maxHeight;
        int fixedWidth;
        int fixedHeight;
    };

    struct LabelData
    {
        int fixedHeight;
        int fixedWidth;
    };

    //PonyWidget
    double widgetPositionVertical = 0.2;//位置竖直比0-1(暂时不用)
    double widgetPositionHorizontal = 0.12;//位置水平比0-1
    //uiSettingsWidget
    int widgetSettingsWidthMax = 280;
    int labelSettingsFilesPathTitleWidthMax = 50;
    int labelSettingsFilesPathWidthMax = 140;
    int labelSettingsFilesPathWidthMin = 140;
    int buttonSettingsFilesOpenWidthMax = 60;
    int buttonSettingsConfigButtonWidthMax = 100;
    int layoutSettingsVerticalSpacing = 2;
    int layoutSettingsHorizontalSpacing = 0;
    //Files
    int imagesStandSize = 1;
    int imagesTrotSize = 16;
    int imagesSitSize = 1;
    int imagesSittingSize = 10;
    int imagesUnsittingSize = 10;
    int imagesLieSize = 1;
    int imagesLyingSize = 8;
    int imagesUnlyingSize = 8;
    int imagesBoopSize = 18;
    int imagesFlySize = 10;
    QString pathConfig = "config.ini";
    QString groupConfigFilesPath = "FilesPath";
    QString groupConfigGeneral = "General";\
    int timeSitDefault = 60;
    int timeLieDefault = 180;
    QString ponyTypeEarthpony = "陆马";
    QString ponyTypePegasus = "飞马";
    QString ponyTypeUnicorn = "独角兽";
    QString ponyTypeAlicorn = "甜椒";
    //Message
    int valueMessageCardWordWrap = 30;
    int labelMessageCardWidth = 200;
    int labelMessageCardHeight = 60;
    int labelMessageCardHeightPerLine = 14;
    int layoutMessageBoxSpacing = 2;
    int layoutMessageBoxMargin = 20;
    int numberMoveDistance = 20;
    int numberMoveDeviation = 2;
    int buttonMessageCardSize = 30;
    int widgetPositionDeviation1X = -235;//位置偏移量
    int widgetPositionDeviation1Y = 0;
    int widgetPositionDeviation2X = 95;
    int widgetPositionDeviation2Y = 0;
    int widgetPositionDeviation3X = 95;
    int widgetPositionDeviation3Y = 0;
    int widgetPositionDeviation4X = -235;
    int widgetPositionDeviation4Y = 0;
//Widget
    //UISettings
    formData uiPony =
    {
        0,0,0,0,150,150,
    };
    formData uiSettings =
    {
        200,200,300,400,0,0,
    };
    tabWidgetData settingsTabWidget =
    {
        "",
    };
    //Condig
    buttonData uiSettingsConfigResetButton =
    {
        25, 0, "",
    };
    buttonData uiSettingsConfigSaveasButton =
    {
        25, 0, "",
    };
    buttonData uiSettingsConfigSaveButton =
    {
        25, 0 ,"",
    };
    buttonData uiSettingsConfigImportButton =
    {
        25, 0, "",
    };
    //第一页
    buttonData uiSettingsTabPonyCharacterButton =
    {
        25, 50, "",
    };
    spinBoxData uiSettingsTabPonyTimeSitSpinBox =
    {
        0, 99999999,
    };
    spinBoxData uiSettingsTabPonyTimeLieSpinBox =
    {
        0, 99999999,
    };
    //第二页
    spinBoxData uiSettingsTabGeneralTimeMessageSpinBox =
    {
        0, 99999999,
    };

    //UICharacter
    LabelData infoIcon =
    {
        100, 100,
    };
};

#endif // LIMIT_H
