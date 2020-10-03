#include "imageprocessing\imageprocessing.h"


ImageProcessing::ImageProcessing()
{
}

void ImageProcessing::__doReadLayerRules()
{
    //读取文件
    QFile *loadFile = new QFile;
    loadFile->setFileName(":data/rules/" + this->m_struct_characterdata.standard + "/layer.json");
    loadFile->open(QIODevice::ReadOnly);
    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    //>>>>>抛出错误<<<<<

    //读取图层顺序
    QJsonObject obj = jsonDoc.object();//获得主数据
    QJsonArray array = obj.value("layer_order").toArray();//获得图层顺序数据
    for(int i = 0; i< array.size(); i++)//获取图层顺序写入数组layerOrger
    {
        this->m_p_strl_layer_order->push_back(array[i].toString());
    }
    //获取图层组合
    for(int i = 0; i < this->m_p_strl_layer_order->size(); i++)
    {
        QString layer = this->m_p_strl_layer_order->at(i);

        if(!this->m_p_map_layer_comb->contains(""))
        {
            this->m_p_map_layer_comb->insert("", new QStringList);
        }
        this->m_p_map_layer_comb->value("")->push_back(layer);
        QStringList list = layer.split('.');
        QString str;
        for(int j = 0; j < list.size(); j++)
        {
            if(str.isEmpty())
                str = list.at(j).simplified();
            else
                str = str + '.' + list.at(j).simplified();
            if(!this->m_p_map_layer_comb->contains(str))
            {
                this->m_p_map_layer_comb->insert(str, new QStringList);
            }
            this->m_p_map_layer_comb->value(str)->push_back(layer);
        }
    }
}

void ImageProcessing::__doReadActionRules()
{
    //读取文件
    QFile *loadFile = new QFile;
    loadFile->setFileName(":/data/rules/" + this->m_struct_characterdata.standard + "/action.json");
    loadFile->open(QIODevice::ReadOnly);
    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    //>>>>>抛出错误<<<<<
    //处理数据
    QJsonObject rootObj = jsonDoc.object();//获得主数据
    QStringList actionNames = rootObj.keys();//获得动作列表
    for(int i = 0; i < actionNames.size(); ++i)
    {//获取动作数据写入数组actionData
        QString actionName = actionNames.at(i);
        QJsonObject subAction = rootObj.value(actionName).toObject();
        if(!this->m_p_map_actiondata->contains(actionName))
            this->m_p_map_actiondata->insert(actionName, new ImageStruct::Sactiondata);
        ImageStruct::Sactiondata *tmpAction = this->m_p_map_actiondata->value(actionName);
        tmpAction->frame = subAction.value("frame").toInt();
        tmpAction->repeat = subAction.value("repeat").toInt();

        QJsonArray posArray = subAction.value("pos").toArray();
        QMap<QString, QMap<int, ImageStruct::Sactiondata::SactiondataPos*>*> posMap;
        for(int k = 0; k < posArray.size(); ++k)
        {
            QJsonObject subPos = posArray.at(k).toObject();
            QString id = subPos.value("id").toString();
            QJsonArray frames = subPos.value("frame").toArray();
            for(int j = 0; j < frames.size(); ++j)
            {
                QJsonObject subFrame = frames[j].toObject();
                ImageStruct::Sactiondata::SactiondataPos *pos = new ImageStruct::Sactiondata::SactiondataPos;
                pos->left = subPos["left"].toDouble();
                pos->top = subPos["top"].toDouble();
                pos->rotate = subPos["rotate"].toInt();
                pos->mirror = subPos["mirror"].toInt();
                if(!posMap.contains(id))
                    posMap.insert(id, new QMap<int, ImageStruct::Sactiondata::SactiondataPos*>);
                posMap[id]->insert(j, pos);
            }
            tmpAction->pos = posMap;
        }

        QJsonArray layerArray = subAction.value("layer").toArray();
        for(int j = 0; j < layerArray.size(); j++)
        {
            tmpAction->layer.push_back(layerArray.at(j).toString());
        }
        tmpAction = nullptr;
    }
}

void ImageProcessing::init(QString name, Config *tmpConfig, FileCharacter *tmpFileCharacter, FileTasks *tmpFileTasks)
{
    this->m_p_config = tmpConfig;
    this->m_p_file_tasks = tmpFileTasks;
    this->m_p_file_character = tmpFileCharacter;
    tmpConfig = nullptr;
    tmpFileTasks = nullptr;
    tmpFileCharacter = nullptr;

    this->m_struct_characterdata.name = name;//获得角色名，写入数组m_struct_characterdata

    __doReadCharacter();//读取角色数据(包含获取配件数据)
    __initDatapool();
}

void ImageProcessing::__doReadCharacter()
{
    //读取角色索引
    if(!QFileInfo(STR_CHARACTER_ROOT_PATH + "\\index.json").isFile())
        this->m_p_file_character->initCharacterIndex();
    this->m_struct_characterdata.path = this->m_p_file_character->getCharacterIndex(this->m_struct_characterdata.name);
    if(this->m_struct_characterdata.path == nullptr)
    {
        this->m_p_file_character->initCharacterIndex();
        this->m_struct_characterdata.path = this->m_p_file_character->getCharacterIndex(this->m_struct_characterdata.name);
    }
    if(this->m_struct_characterdata.path == nullptr)
    {
        throw QString("wrong");
    }

    //读取文件
    QFile *loadFile = new QFile;
    loadFile->setFileName(this->m_struct_characterdata.path);
    loadFile->open(QIODevice::ReadOnly);
    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    //>>>>>抛出错误<<<<<

    //处理数据
    QJsonObject rootObj = jsonDoc.object();//获得主数据
    QJsonObject metadata = rootObj.value("metadata").toObject();//获得子数据metadata
    this->m_struct_characterdata.author = metadata["author"].toString();//获取角色.作者
    this->m_struct_characterdata.desc = metadata["desc"].toString();//获取角色.介绍
    this->m_struct_characterdata.standard = metadata["standard"].toString();

    __doReadLayerRules();//读图层分布规则
    __doReadActionRules();//读动作规则
    this->m_p_map_taskdata = m_p_file_tasks->get(this->m_struct_characterdata.standard);    //读任务规则

    QJsonArray data = rootObj.value("accessories").toArray();//获得配件列表
    for(int i = 0; i < data.size(); i++)//获取配件数据写入角色.配件
    {
        QJsonObject subData = data.at(i).toObject();//获得配件数据
        QString accessoriesID = subData["id"].toString();
        if(!this->m_struct_characterdata.accessories.contains(accessoriesID))
            this->m_struct_characterdata.accessories.insert(accessoriesID , new Scharacterdata::Saccessoriesdata);
        Scharacterdata::Saccessoriesdata *tmpAccessories = this->m_struct_characterdata.accessories.value(accessoriesID);
        tmpAccessories->name = subData["name"].toString();//获取角色.配件.名称
        QJsonArray color = subData["color"].toArray();//获得颜色列表
        for(int j = 0; j < color.size(); j++)
        {//获取颜色替换列表
            QJsonObject subColor = color.at(j).toObject();//获得颜色替换数据
            if(!tmpAccessories->colorReplace->contains(subColor["target_color"].toString()))
                tmpAccessories->colorReplace->insert(subColor["target_color"].toString(), new ImageStruct::ScolorReplace);
            ImageStruct::ScolorReplace *tmpColor = tmpAccessories->colorReplace->value(subColor["target_color"].toString());
            tmpColor->replace = subColor["replace_color"].toString();
            tmpColor->target = subColor["target_color"].toString();
            tmpColor = nullptr;
        }
        if(this->m_map_accessoriesdata.contains(accessoriesID))
            delete this->m_map_accessoriesdata.value(accessoriesID);
        this->m_map_accessoriesdata[accessoriesID] = new Accessories(subData["id"].toString(),
                                                                    tmpAccessories->name,
                                                                    STR_ACCESSORIES_ROOT_PATH,
                                                                    this->m_p_map_actiondata,
                                                                    this->m_p_map_layer_comb,
                                                                    tmpAccessories->colorReplace);
        tmpAccessories = nullptr;
    }
}
/*
void ImageProcessing::__doHandleTasks()
{
    QMapIterator<QString, FileTasks::STaskdata*> tasks(*this->m_p_map_taskdata);
    while(tasks.hasNext())
    {
        tasks.next();
        if(this->m_p_pool->getBool(tasks.value()->groupTrigger))//如果组条件成立
        {
            int totalRunningTaskGroup = this->m_p_pool->m_p_map_intdata->value("total_running_task_group");
            bool hasInserted = false;
            //判断是否已加入
            for(int i = 1; i <= this->m_p_pool->m_p_map_intdata->value("total_running_task_group"); i++)
            {
                if(m_p_pool->m_p_map_stringdata->value("running_task_group_list_" + QString::number(i)) == tasks.key())
                {
                    hasInserted = true;
                    break;
                }
            }
            if(!hasInserted)//如果未加入
            {
                //加入
                totalRunningTaskGroup++;
                this->m_p_pool->m_p_map_intdata->insert("total_running_task_group", totalRunningTaskGroup);
                this->m_p_pool->m_p_map_stringdata->insert("running_task_group_list_" + QString::number(totalRunningTaskGroup), tasks.key());
                this->m_p_pool->runCMDList(tasks.value()->groupOption);//执行组命令
            }
        }
    }

    //遍历所有运行的组
    for(int i = 1; i <= this->m_p_pool->m_p_map_intdata->value("total_running_task_group"); i++)
    {
        QString groupName = this->m_p_pool->m_p_map_stringdata->value("running_task_group_list_" + QString::number(i));
        if(!groupName.isNull())
        {
            FileTasks::STaskdata *group = this->m_p_map_taskdata->value(groupName);
            QMapIterator<QString, FileTasks::STaskdata::STaskEvent*> events(group->events);
            while(events.hasNext())//遍历组中事件
            {
                events.next();
                if(this->m_p_pool->getBool(events.value()->trigger))//如果事件条件成立
                    this->m_p_pool->runCMDList(events.value()->option);//执行事件命令
            }
        }
    }
}

void ImageProcessing::__doHandleFrames()
{
    QStringList *list = this->m_p_map_layer_comb->value("");
    for(int i = 0; i < list->size(); i++)
    {
        //获取图层的相关信息
        int frame = this->m_p_pool->m_p_map_intdata->value(list->at(i)+"_frame");
        QString present = this->m_p_pool->m_p_map_stringdata->value(list->at(i) + "_action_present");
        QString target = this->m_p_pool->m_p_map_stringdata->value(list->at(i) + "_action_target");
        int repeat = this->m_p_pool->m_p_map_intdata->value(list->at(i) + "_repeat");
        ImageStruct::Sactiondata *data = m_p_map_actiondata->value(present);
        int maxFrame = data->frame;
        int maxRepeat = data->repeat;

        if(frame + 1 > maxFrame)//如果帧数大于限定帧数
        {
            if(repeat + 1 > maxRepeat)//如果重复次数大于最大重复次数
            {
                if(target.isNull())//如果无目标，重复次数+1，重置帧数
                {
                    this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_frame", 1);
                    this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_repeat", repeat + 1);
                }else{
                    QString vector = this->m_p_pool->m_p_map_stringdata->value("vector_" + present + "_to_" + target);//获取引导
                    if(vector.isNull())//无引导，当前动作改为目标
                    {
                        this->m_p_pool->m_p_map_stringdata->insert(list->at(i)+"_action_present", target);
                    }else{//否则当前动作改为引导
                        this->m_p_pool->m_p_map_stringdata->insert(list->at(i)+"_action_present", vector);
                    }
                    this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_frame", 1);
                    this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_repeat", 0);
                }
            }else//否则重复次数+1，帧数重置
            {
                this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_frame", 1);
                this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_repeat", repeat + 1);
            }
        }else//否则帧数+1
        {
            this->m_p_pool->m_p_map_intdata->insert(m_p_strl_layer_order->at(i)+"_frame", frame + 1);
        }
    }
}

void ImageProcessing::__doHandleImages(QString currentID, QString parent,  ImageStruct::Scombdata rootPos)
{
    if(this->tmp_p_layer_pos->contains(currentID))
        return;
    QString actionID = this->m_p_pool->m_p_map_stringdata->value(currentID + "_action_present");
    int frame = this->m_p_pool->m_p_map_intdata->value(currentID + "_frame");
    ImageStruct::Sactiondata::SactiondataPos *currentPos = m_p_map_actiondata->value(actionID)->pos.value(currentID)->value(frame);
    ImageStruct::Scombdata pos;
    QStringList *comb = this->m_p_map_layer_comb->value(currentID);

    pos.top = rootPos.top + currentPos->top;
    pos.left = rootPos.left + currentPos->left;
    pos.mirror = currentPos->mirror ? !rootPos.mirror : rootPos.mirror;
    pos.rotate = rootPos.rotate + currentPos->rotate;
    pos.frame = frame;
    pos.actionID = actionID;
    pos.parent = parent;

    this->tmp_p_layer_pos->insert(currentID, pos);
    if(comb->size() == 1)
        return;

    int size = currentID.size();
    for(int i = 0; i < comb->size(); i++)
    {
        int p = comb->at(i).indexOf('.', size + 1);
        QString newID = comb->at(i).left(p);
        __doHandleImages(newID, currentID, pos);
    }

}
*/
void ImageProcessing::__doCompositeImage()
{
    delete this->image;
    this->image = new QImage(this->width, this->height, QImage::Format_ARGB32);
    QPainter rootPainter(this->image);

    for(int i = 0; i < this->m_p_strl_layer_order->size(); i++)
    {
        QString layerID = m_p_strl_layer_order->at(i);
        ImageStruct::Scombdata combdata = this->tmp_p_layer_pos->value(layerID);
        QImage image = *this->m_map_accessoriesdata[combdata.parent]->m_map_image.value(layerID)->value(combdata.frame);
        QRect rect(static_cast<int>(combdata.left*this->width), static_cast<int>(combdata.top*this->height), this->width, this->height);
        if(combdata.mirror)
            image.mirrored(true, false);
        rootPainter.drawImage(rect, image, QRect(0,0,this->width, this->height));
        //=============================here=================================
    }
}

const QPixmap ImageProcessing::getFrame()
{
    //缺少：获取系统变量
    __doHandleTasks();//处理任务
    __doHandleFrames();//修改帧属性
    __doHandleImages("", "", ImageStruct::Scombdata{0, 0, 0, 0, 0, "", ""});//获得图像
    __doCompositeImage();

    return QPixmap::fromImage(*this->image);

    /*

    QPixmap* pixmap = new QPixmap(300,300);
    pixmap->fill(Qt::transparent);//设置背景透明
    QPainter p(pixmap);
    this->m_map_accessoriesdata["body.head.eyes"]->m_map_SVG_image["body.head.eyes.front.blink"]->value(1)->replacedImage->render(&p);
    QMatrix rotate;
    rotate.rotate(90.0);
    QPixmap a = pixmap->transformed(rotate,Qt::SmoothTransformation);
    return a.copy(125,125,50,50);
*/
    //return m_map_accessoriesdata["body.head.eyes"]->m_map_pixel_image["body.head.eyes.front.blink"].at(0).copy(125,125,50,50);
}
/*
void ImageProcessing::__initDatapool()
{
    QStringList *list = this->m_p_map_layer_comb->value("");
    for(int i = 0; i < list->size(); i++)
    {
        this->m_p_pool->m_p_map_intdata->insert(list->at(i)+"_frame", 0);
        this->m_p_pool->m_p_map_stringdata->insert(list->at(i) + "_action_present", "normal");
        this->m_p_pool->m_p_map_stringdata->value(list->at(i) + "_action_target", "normal");
        this->m_p_pool->m_p_map_intdata->value(list->at(i) + "_repeat", 0);
    }
    list = nullptr;
}
*/
