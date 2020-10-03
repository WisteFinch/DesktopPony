/**
 * @file imageprocessing/imagestruct.h
 * @brief 图像公用结构体
 * @author WisteFinch
 * @date 2020.10.3
 *
 * MIT License
 * Copyright (c) 2019-2020 WisteFinch
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

#ifndef IMAGESTRUCT_H
#define IMAGESTRUCT_H

#include <QMap>
#include <QString>
#include <QSvgRenderer>
#include <QColor>

/**
 * @brief 图像公用结构体类
 */
class ImageStruct
{
public:

    /**
     * @brief 动作数据结构体
     */
    struct Sactiondata
    {
        /**
         * @brief 图层位置数据结构体
         */
        struct SactiondataPos
        {
            double left;
            double top;
            double rotate;//暂时不用
            bool mirror;
        };

        int frame;//帧数
        bool isMain;
        int repeat;//重复次数
        QMap<QString, QString*> goTo;
        QMap<QString, QMap<int, SactiondataPos*>*> pos;//控制位置
        QStringList layer;//控制图层
    };

    /**
     * @brief 图像数据结构体
     */
    struct SaccessoriesSVG{
        QSvgRenderer *originalImage = new QSvgRenderer();//原图像
        QSvgRenderer *replacedImage = new QSvgRenderer();//替换完图像
    };

    /**
     * @brief 配件帧数据结构体
     */
    struct SaccessoriesFrame
    {
        int frame;//帧数
        int repeat;//循环次数
        int delay;//等待
    };

    /**
     * @brief 颜色数据结构体
     */
    struct ScolorReplace{
        QColor target;//目标颜色
        QColor replace;//替换颜色
        QColor defaultColor;//默认颜色
        //QColor depend;//依赖
        //QColor offset;//偏移
    };

    struct Scombdata{
        double left;
        double top;
        double rotate;//暂时不用
        bool mirror;
        int frame;
        QString actionID;
        QString parent;
    };
};

#endif // IMAGESTRUCT_H
