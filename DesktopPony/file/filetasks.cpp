#include "filetasks.h"

FileTasks::FileTasks()
{
    this->data = new QMap<QString, QMap<QString, STaskdata*>*>;
}

FileTasks::~FileTasks()
{
    removeData();
}

void FileTasks::clear()
{
    removeData();
    this->data = new QMap<QString, QMap<QString, STaskdata*>*>;
}

void FileTasks::removeData()
{
    QStringList rootKeys = this->data->keys();
    for(int i = 0; i < rootKeys.size(); i++)
    {
        QMap<QString, STaskdata*> *standardTasks = this->data->value(rootKeys.at(i));
        QStringList standardKeys = standardTasks->keys();
        for(int j = 0; j < standardKeys.size(); j++)
        {
            delete standardTasks->value(standardKeys.at(i));
            standardTasks->remove(standardKeys.at(i));
        }
        delete standardTasks;
        this->data->remove(rootKeys.at(i));
    }
    delete this->data;
}

void FileTasks::readAll()
{
    //读取文件夹
    QDir systemDir(STR_RULES_SYSTEM_ROOT_PATH);
    QDir userDir(STR_RULES_USER_ROOT_PATH);
    systemDir.setFilter(QDir::Dirs);
    systemDir.setSorting(QDir::Name);
    userDir.setFilter(QDir::Dirs);
    userDir.setSorting(QDir::Name);
    QFileInfoList systemList = systemDir.entryInfoList();
    QFileInfoList userList = userDir.entryInfoList();
    for(int i = 0; i < systemList.size(); i++)
    {
        read(systemList.at(i).fileName());
    }
    for(int i = 0; i < userList.size(); i++)
    {
        read(userList.at(i).fileName());
    }
}

void FileTasks::read(QString standard)
{
    QMap<QString, STaskdata*> *newTasksList = new QMap<QString, STaskdata*>;
    if(this->data->contains(standard))//清除已有数据
    {
        delete this->data->value(standard);
    }

    //读取文件夹
    QDir systemDir(STR_RULES_SYSTEM_ROOT_PATH + standard + "\\tasks");
    QDir userDir(STR_RULES_USER_ROOT_PATH + standard + "\\tasks");
    systemDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    systemDir.setSorting(QDir::Name);
    userDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    userDir.setSorting(QDir::Name);
    QFileInfoList systemList = systemDir.entryInfoList();
    QFileInfoList userList = userDir.entryInfoList();
    QFileInfoList list;
    for(int i = 0; i < systemList.size(); i++)
    {
        list.push_back(systemList.at(i));
    }
    for(int i = 0; i < userList.size(); i++)
    {
        list.push_back(userList.at(i));
    }
    QFile *loadFile = nullptr;
    //遍历文件
    for (int i = 0; i < list.size(); i++)
    {
        if(list.at(i).isFile())
        {
            //读取文件
            loadFile = new QFile();
            loadFile->setFileName(list.at(i).filePath());
            loadFile->open(QIODevice::ReadOnly);

            QString allData = loadFile->readAll().simplified();
            loadFile->close();
            delete loadFile;
            loadFile = nullptr;

            //读取数据
            QStringList root = doDivCMD(allData);
            if(root.at(root.size() - 1) == "ERROR")//判断内容是否出错
            {
                root.pop_back();
                //============================报错==================================
            }
            for(int j = 0; j < root.size(); j++)//遍历组列表
            {
                QString groupName = nullptr;
                STaskdata newTask;
                QStringList subTask = doDivCMD(root.at(j).mid(1, root.at(j).size() - 2));//划分内容
                if(subTask.at(root.size() - 1) == "ERROR")//判断内容是否出错
                {
                    subTask.pop_back();
                    //============================报错==================================
                }
                for(int k = 0; k < subTask.size(); k++)//遍历内容
                {
                    int groupPos = subTask.at(k).indexOf('=');
                    QString groupKey = subTask.at(k).left(groupPos - 1).simplified();
                    QString groupValue = subTask.at(k).right(subTask.at(k).size() - groupPos - 1).simplified();
                    if(groupKey == "name")
                    {
                        groupName = groupValue;
                    }else if(groupKey == "author")
                    {
                        newTask.author = groupValue;
                    }else if(groupKey == "desc")
                    {
                        newTask.desc = groupValue;
                    }else if(groupKey == "standard")
                    {
                        newTask.standard = groupValue;
                    }else if(groupKey == "group_trigger")
                    {
                        newTask.groupTrigger = groupValue.mid(1, groupValue.size() - 2).simplified();
                    }else if(groupKey == "group_option")
                    {
                        newTask.groupOption = groupValue.mid(1, groupValue.size() - 2).simplified();
                    }else if(groupKey == "events")
                    {
                        QStringList rootEvent = doDivCMD(groupValue.mid(1, groupValue.size() - 2).simplified());
                        if(subTask.at(root.size() - 1) == "ERROR")
                        {
                            subTask.pop_back();
                            //============================报错==================================
                        }
                        for(int l = 0; l < rootEvent.size(); l++)
                        {
                            STaskdata::STaskEvent *newEvent = new STaskdata::STaskEvent;
                            QString eventName;
                            QStringList subEvent = doDivCMD(rootEvent.at(l).mid(1, rootEvent.at(l).size() - 2).simplified());
                            if(subEvent.at(root.size() - 1) == "ERROR")
                            {
                                subEvent.pop_back();
                                //============================报错==================================
                            }
                            for(int m = 0; m < subEvent.size(); m++)
                            {
                                int eventPos = subEvent.at(m).indexOf('=');
                                QString eventKey = subEvent.at(m).left(eventPos - 1).simplified();
                                QString eventValue = subEvent.at(m).right(subEvent.at(m).size() - eventPos - 1).simplified();
                                if(eventKey == "name")
                                {
                                    eventName = eventValue;
                                }else if(eventKey == "trigger")
                                {
                                    newEvent->trigger = eventValue.mid(1, eventValue.size() - 2).simplified();
                                }else if(eventKey == "option")
                                {
                                    newEvent->option = eventValue.mid(1, eventValue.size() - 2).simplified();
                                }
                            }
                            newEvent->name = eventName;
                            newEvent->path = list.at(i).filePath();
                            newTask.events[eventName] = newEvent;
                        }
                    }
                }
                if(!newTasksList->contains(groupName))
                    newTasksList->insert(groupName, new STaskdata);
                newTasksList->value(groupName)->author = newTask.author;
                newTasksList->value(groupName)->desc = newTask.desc;
                newTasksList->value(groupName)->standard = newTask.standard;
                newTasksList->value(groupName)->groupTrigger = newTask.groupTrigger;
                newTasksList->value(groupName)->groupOption = newTask.groupOption;
                QStringList keys = newTask.events.keys();
                for(int k = 0; k < keys.size(); k++)
                    newTasksList->value(groupName)->events.insert(keys.at(k), newTask.events.value(keys.at(k)));
                newTasksList->value(groupName)->groupPath = list.at(i).filePath();
                newTasksList->value(groupName)->groupName = groupName;
            }
        }
    }
    if(newTasksList->size() != 0)
        this->data->insert(standard, newTasksList);
}

QMap<QString, FileTasks::STaskdata*> *FileTasks::get(QString standard)
{
    if(!this->data->contains(standard))
        read(standard);
    return this->data->value(standard);
}

QStringList FileTasks::doDivCMD(QString cmds) const
{
    cmds = cmds.simplified();
    QStringList cmdList;
    int begin = 0, end, layer = 0;
    bool isStr = false;
    QChar c;
    for(end = begin; end< cmds.size(); end++)//以";"切割命令
    {
        c = cmds.at(end);
        if(c == '\"' && cmds[end - 1 <0 ? 0 : end - 1] != '\\')
        {
            isStr = isStr ? false : true;
            continue;
        }
        if(!isStr)
        {
            if(cmds[end] == '(' || cmds[end] == '{')
            {
                layer++;
            }else if(cmds[end] == ')' || cmds[end] == '}')
            {
                layer--;
            }else if(layer == 0)
            {
                if(cmds[end] == ';')
                {
                    cmdList.push_back(cmds.mid(begin, end - begin).simplified());
                    begin = end + 1;
                }
            }
        }
    }
    if(end != cmds.size())
    {
        cmdList.push_back("ERROR");
    }
    return cmdList;
}
