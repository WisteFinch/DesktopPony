#ifndef FILETASKS_H
#define FILETASKS_H

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

#endif // FILETASKS_H
