/**
 * @file imageprocessing/accessories.h
 * @brief 图像处理-配件
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

#ifndef IMAGEPROCESSINGACCESSORIES_H
#define IMAGEPROCESSINGACCESSORIES_H

#include "imageprocessing/imagestruct.h"
#include <QString>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QPainter>

/**
 * @brief 部件处理类
 * 负责存放部件数据，并生成图像
 */
class Accessories
{
public:
    /**
     * @brief Accessories
     * @param ID ID
     * @param name 名称
     * @param tempActionData 动作数据指针
     * @param tempLayerComb 图层组合数据指针
     * @param tempCharacterColorReplace 颜色数据指针
     */
    Accessories(QString ID, QString name, QString rootPath, QMap<QString, ImageStruct::Sactiondata*> *actionData, QMap<QString, QStringList*> *layerComb, QMap<QString, ImageStruct::ScolorReplace*> *characterColorReplace);

    QString m_str_name;///< 名称
    QString m_str_accessoriesID;///< ID
    QString m_str_path;///< 地址
    int m_i_version;///< 版本
    QString m_str_support_verison;///< 支持的版本
    QString m_str_author;///< 配件作者
    QString m_str_desc;///< 介绍
    QMap<QString, QMap<int, ImageStruct::SaccessoriesSVG*>*> m_map_SVG_image;///< 存放SVG图像
    QMap<QString, ImageStruct::ScolorReplace*> m_map_color_replace;///< 颜色数据
    QStringList m_strl_layer;///< 图层列表
    QMap<QString, ImageStruct::SaccessoriesFrame*> m_map_action;///< 动作数据
    QMap<QString, QMap<int, QImage*>*> m_map_image;///< 存放位图
    QString m_str_index_path;///< 索引地址
    QString m_str_root_path;///< 根地址

    /**
     * @brief 读取配置文件
     */
    void doReadConfig();
    /**
     * @brief QColor转16进制颜色
     * @param color 原QColor
     * @return 16进制颜色
     */
    QString toHex(QColor color) const;
    /**
     * @brief SVG图像转位图
     * @param width 限定宽度
     * @param height 限定高度
     */
    void toImage(int width, int height);
    /**
     * @brief 读取索引
     * @return 文件地址
     * @param index 索引地址
     * @param name 名称
     */
    QString readAccessoriesIndex(QString index, QString name) const;
    /**
     * @brief 生成索引
     * @param index 索引地址
     * @param ID ID
     */
    void initAccessoriesIndex(QString index, QString ID);

private:
    QMap<QString, ImageStruct::Sactiondata*> *m_p_map_characterdata_actiondata;///< 传入的动作数据指针
    QMap<QString, QStringList*> *m_p_map_characterdata_layer_comb;///< 传入的图层数据指针
    QMap<QString, ImageStruct::ScolorReplace*> *m_p_map_characterdata_color_replace;///< 传入的颜色数据指针

    /**
     * @brief 读取图像
     * @param layer 图层
     * @param actionID 动作
     * @param path 地址
     */
    void __doReadImage(QString layer, QString actionID, QString path);
};

#endif // IMAGEPROCESSINGACCESSORIES_H
