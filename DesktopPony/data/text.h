/**
 * @file data/text.h
 * @brief 数据-文本
 * @author WisteFinch
 * @date 2021.7.27
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

#ifndef TEXT_H
#define TEXT_H

#include "data/localisation.h"
#include "metadata.h"
#include <QRegularExpression>
#include <QVariant>

/**
 * @brief 文本类
 * @details 对程序中文本进行变换操作
 */
class Text
{
public:
    Text();
    ~Text();

    /**
     * @brief 初始化
     * @param 本地化指针
     */
    void init(Localisation *ptrLoc);

    /**
     * @brief 获取文本
     * @param 文本
     * @param 附加变量
     * @return 处理后文本
     */
    QString get(QString str, QVariant var);

    /**
     * @brief 获取文本
     * @param 文本
     * @return 处理后文本
     */
    QString get(QString str)
    {
        return get(str, QVariant());
    }

    /**
     * @brief 获取公有本地化文本
     * @param 键
     * @param 附加变量
     * @return 值
     */
    QString getLoc(QString key, QVariant var);

    /**
     * @brief 获取公有本地化文本
     * @param 键
     * @return 值
     */
    QString getLoc(QString key)
    {
        return getLoc(key, QVariant());
    }

    /**
     * @brief 获取私有本地化文本
     * @param 键
     * @param 附加变量
     * @return 值
     */
    QString getLocPriv(QString key, QVariant var);

    /**
     * @brief 获取私有本地化文本
     * @param 键
     * @return 值
     */
    QString getLocPriv(QString key)
    {
        return getLocPriv(key, QVariant());
    }

private:
    Localisation *m_p_loc = nullptr;///< 本地化数据


    /**
     * @brief 替换文本中的参数
     * @param 原文
     * @param 附加变量
     * @return 替换后文本
     */
    QString replacePara(QString str, QVariant var);

    /**
     * @brief 获取参数值
     * @param 参数
     * @param 附加变量
     * @return 值
     */
    QString getPara(QString para, QVariant var);

    /**
     * @brief 解析附加值
     * @param 附加值
     * @param 下标
     * @return 结果
     */
    QVariant getVar(QVariant &var, qint32 s = 0);

    /**
     * @brief 变量转字符串
     * @param 变量
     * @return 字符串
     */
    QString varToString(QVariant var);
};

#endif // TEXT_H
