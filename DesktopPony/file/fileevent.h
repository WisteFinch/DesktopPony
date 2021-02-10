/**
 * @file file/fileevent.h
 * @brief 文件-事件
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

#ifndef FILEEVENT_H
#define FILEEVENT_H

#include "metadata.h"
#include <QString>
#include <QFile>
#include <QDir>
#include <QMap>
#include <QMapIterator>


class FileTasks
{
public:
    FileTasks();
    ~FileTasks();

    /**
     * @brief 任务数据结构体
     */
    struct STaskdata{
        /**
         * @brief 事件数据结构体
         */
        struct STaskEvent{
            QString name; ///< 事件名称
            QString path; ///< 事件文件位置
            QString trigger;///< 触发条件
            QString option;///< 选项
        };

        QString groupName;///< 组名称
        QString groupPath;///< 组文件位置
        QString author;///< 作者
        QString desc;///< 介绍
        QString standard;///< 标准
        QString groupTrigger;///< 组触发条件
        QString groupOption;///< 组选项
        QMap<QString, STaskEvent*> events;///< 事件数据
    };

    QMap<QString, QMap<QString, STaskdata*>*> *data;

    /**
     * @brief 读取指定标准的数据
     */
    void read(QString standard);

    /**
     * @brief 读取所有数据
     */
    void readAll();

    /**
     * @brief 清空数据
     */
    void clear();

    /**
     * @brief 获得任务数据
     * @param 标准
     * @return 数据
     */
    QMap<QString, STaskdata*> *get(QString standard);

    /**
     * @brief 划分命令
     * @param cmds 命令
     * @return 命令列表
     */

    inline QStringList doDivCMD(QString cmds) const;

private:
    /**
     * @brief 清除数据
     */
    void removeData();
};

#endif // FILEEVENT_H
