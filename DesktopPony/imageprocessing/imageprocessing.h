/**
 * @file imageprocessing/imageprocessing.h
 * @brief 图像处理-主类
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2021 WisteFinch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef IMAGEPROCESSINGIMAGEPROCESSING_H
#define IMAGEPROCESSINGIMAGEPROCESSING_H

#include "data/config.h"
#include "imageprocessing/imagestruct.h"
#include "imageprocessing/accessories.h"
#include "imagestruct.h"
#include "file/fileevent.h"
#include "file/filecharacter.h"
#include "scriptinterpreter/scriptinterpretershell.h"
#include "metadata.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMap>
#include <QMapIterator>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QDir>
#include <QVector>
#include <QVectorIterator>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QRgb>
#include <QtSvg/QtSvg>
#include <QtSvg/QSvgRenderer>
#include <QFile>

/**
 * @brief 图像处理类
 * 处理事件并根据配置动态生成图像
 */
class ImageProcessing: QObject
{
    Q_OBJECT

public:

    int height;
    int width;
    QImage *image = nullptr;

    /**
     * @brief ImageProcessing
     */
    ImageProcessing();

    /**
     * @brief 初始化类
     * @param name 名称
     */
    void init(QString name, Config *tmpConfig, FileCharacter *tmpFileCharacter,  FileTasks *tmpFileTasks);

    /**
     * @brief 获取位图
     * @return 位图
     */
    const QPixmap getFrame();
private:

    /**
     * @brief 角色数据结构体
     */
    struct Scharacterdata {
        /**
         * @brief 配件数据结构体
         */
        struct Saccessoriesdata {
            QString name;///< 配件名
            QMap<QString, QMap<int, QImage *>*> image; ///< 图像数据
            QMap<QString, ImageStruct::ScolorReplace *> *colorReplace = new QMap<QString, ImageStruct::ScolorReplace *>; ///< 颜色数据
            bool isWrong = false;
        };

        QString name;///< 名称
        QString path;///< 地址
        QString author;///< 作者
        QString desc;///< 介绍
        QMap<QString, Saccessoriesdata *> accessories; ///< 配件数据
        QString standard;///< 标准
    };

    /**
     * @brief 图像数据结构体
     */
    struct SimageReplace {
        QSvgRenderer *originalImage = new QSvgRenderer();///< 原图像
        QSvgRenderer *replacedImage = new QSvgRenderer();///< 替换后图像
    };

    /**
     * @brief 图层位置结构体
     */

    //变量
    Scharacterdata m_struct_characterdata;///< 角色数据
    QStringList *m_p_strl_layer_order = new QStringList();///< 图层顺序
    QMap<QString, QStringList *> *m_p_map_layer_comb = new QMap<QString, QStringList *>; ///< 图层组合
    QMap<QString, Accessories *> m_map_accessoriesdata; ///< 配件数据
    QMap<QString, ImageStruct::Sactiondata *> *m_p_map_actiondata = new QMap<QString, ImageStruct::Sactiondata *>; ///< 动作数据
    QMap<QString, FileTasks::STaskdata *> *m_p_map_taskdata; ///< 任务数据
    ScriptInterpreter *m_p_script = new ScriptInterpreter();///< 数据池

    QMap<QString, ImageStruct::Scombdata> *tmp_p_layer_pos;///< 暂时储存图层位置
    //QMap<QString, QImage> *tmp_p_image;///< 暂时储存组合图像

    /**
     * @brief 读取图层顺序
     */
    void __doReadLayerRules();

    /**
     * @brief 读取角色数据
     */
    void __doReadCharacter();

    /**
     * @brief 读取动作数据
     */
    void __doReadActionRules();

    /**
     * @brief 处理任务关系
     */
    void __doHandleTasks();

    /**
     * @brief 处理帧数关系
     */
    void __doHandleFrames();

    /**
     * @brief 处理图像关系
     */
    void __doHandleImages(QString currentID, QString parent, ImageStruct::Scombdata rootPos);

    /**
     * @brief 组合图像
     */
    void __doCompositeImage();

    /**
     * @brief 初始化数据
     */
    void __initDatapool();

    Config *m_p_config;
    FileTasks *m_p_file_tasks;
    FileCharacter *m_p_file_character;

};

#endif // IMAGEPROCESSINGIMAGEPROCESSING_H
