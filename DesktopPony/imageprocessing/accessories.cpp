#include "imageprocessing\accessories.h"

Accessories::Accessories(QString ID, QString name, QString rootPath, QMap<QString, ImageStruct::Sactiondata*> *actionData, QMap<QString, QStringList*> *layerComb, QMap<QString, ImageStruct::ScolorReplace*> *characterColorReplace)
{
    this->m_p_map_characterdata_actiondata = actionData;
    this->m_p_map_characterdata_layer_comb = layerComb;
    this->m_p_map_characterdata_color_replace = characterColorReplace;
    this->m_str_accessoriesID = ID;
    this->m_str_name = name;
    this->m_str_root_path = rootPath;
    this->m_str_index_path = rootPath + "\\" + this->m_str_accessoriesID;//获取地址

    //读取配件索引
    if(!QFileInfo(m_str_index_path + "\\index.json").isFile())
        initAccessoriesIndex(this->m_str_index_path, this->m_str_accessoriesID);
    this->m_str_path = readAccessoriesIndex(this->m_str_index_path, this->m_str_name);
    if(this->m_str_path == nullptr || !QFileInfo(m_str_path+"\\metadata.json").exists()){
        initAccessoriesIndex(this->m_str_index_path, this->m_str_accessoriesID);
        this->m_str_path = readAccessoriesIndex(this->m_str_index_path, this->m_str_name);
    }
    if(this->m_str_path == nullptr)
    {

        return;
    }


    doReadConfig();

    //获取拥有的图层名
    m_strl_layer = *m_p_map_characterdata_layer_comb->value(this->m_str_accessoriesID);

    //处理图像
    for(int i = 0; i < this->m_strl_layer.size(); ++i)
    {
        QMapIterator<QString, ImageStruct::SaccessoriesFrame*> actionIterator(this->m_map_action);//设置配件.动作迭代器
        while(actionIterator.hasNext())
        {
            actionIterator.next();
            QString layer = m_strl_layer.at(i).right(m_strl_layer.at(i).size() - m_str_accessoriesID.size() - 1);
            __doReadImage(layer, actionIterator.key(), this->m_str_path);//读取图像
        }
    }
}

void Accessories::doReadConfig()
{
    //读取文件
    QFile *loadFile = new QFile;
    loadFile->setFileName(this->m_str_path + "\\metadata.json");
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
    QJsonObject metadataObj = rootObj.value("metadata").toObject();//获得信息
    this->m_str_name = metadataObj.value("name").toString();//获取名称
    this->m_str_author = metadataObj.value("author").toString();//获取作者
    this->m_str_desc = metadataObj.value("desc").toString();//获取介绍
    this->m_str_support_verison = metadataObj.value("support_version").toString();//获取支持版本
    this->m_i_version = metadataObj.value("version").toString().toInt();//获取版本
    //获取动作数据
    QJsonArray actionArray = rootObj.value("action").toArray();//获得动作列表
    for(int i = 0; i < actionArray.size(); ++i){//获取动作
        QJsonObject subAction = actionArray.at(i).toObject();//获得动作列表
        QString name = subAction.value("name").toString();
        if(this->m_p_map_characterdata_actiondata->contains(name)){//判断是否有此动作
            if(!this->m_map_action.contains(name))
                this->m_map_action.insert(name, new ImageStruct::SaccessoriesFrame);
            this->m_map_action.value(name)->frame = subAction.value("frame").toInt();//获取动作.帧数
        }
    }
    //获取颜色替换数据
    QJsonArray colorArray = rootObj.value("color").toArray();//获得颜色列表
    for(int i = 0; i < colorArray.size(); ++i){//获取颜色替换
        QJsonObject colorSubObj = colorArray.at(i).toObject();//获得颜色替换列表
        QString targetColor = colorSubObj.value("target_color").toString();//获取颜色替换
        if(!this->m_map_color_replace.contains(targetColor))
            this->m_map_color_replace.insert(targetColor, new ImageStruct::ScolorReplace);
        this->m_map_color_replace.value(targetColor)->target = targetColor;//获取颜色替换.目标
        this->m_map_color_replace.value(targetColor)->defaultColor = colorSubObj["default_color"].toString();//获取颜色替换.默认
        //this->m_map_color_replace[targetColor].depend = colorSubObj["depend_color"].toString();//获取颜色替换.依赖
        //this->m_map_color_replace[targetColor].offset = colorSubObj["offset"].toString();//获取颜色替换.偏移
    }
    //处理角色.替换色彩中的无效数据
    QMapIterator<QString, ImageStruct::ScolorReplace*> iterator(*this->m_p_map_characterdata_color_replace);//设置角色颜色替换迭代器
    while(iterator.hasNext()){//是否有下一个数据
        iterator.next();//下一个数据
        if(!this->m_map_color_replace.contains(iterator.key()))//角色.颜色替换中是否包含当前颜色
        {
            delete this->m_map_color_replace.value(iterator.key());
            this->m_map_color_replace.remove(iterator.key());//不包含->删除
        }
    }
}

void Accessories::__doReadImage(QString layer, QString actionID, QString path)
{
    QString ID = this->m_str_accessoriesID + "." + layer + "." + actionID;//设置配件ID
        QString rootImagePath = path +"/" + layer + "/" + actionID + "/";//设置图像主路径
    for(int i = 1; i <= this->m_p_map_characterdata_actiondata->value(actionID)->frame; ++i)//循环读 配件数据.图像
    {
        ImageStruct::SaccessoriesSVG *rootSVG = new ImageStruct::SaccessoriesSVG;//设置配件数据.图像结构体
        rootSVG->originalImage->load(QString(rootImagePath + QString().number(i) +".svg"));//读取原图像

        //替换颜色
        QFile imageFile(QString(rootImagePath + "\\" + QString().number(i) +".svg"));
        imageFile.open(QIODevice::ReadOnly);
        QString imageData = imageFile.readAll();
        QMapIterator<QString, ImageStruct::ScolorReplace*> iterator(*this->m_p_map_characterdata_color_replace);
        while(iterator.hasNext())
        {
            iterator.next();
            imageData.replace(QRegExp(toHex(iterator.value()->target)), toHex(iterator.value()->replace));
        }
        rootSVG->replacedImage->load(imageData.toUtf8());

        QString frameID = this->m_str_accessoriesID + "." + layer + "." + actionID;
        if(!this->m_map_SVG_image.contains(frameID))
            this->m_map_SVG_image.insert(frameID, new QMap<int, ImageStruct::SaccessoriesSVG*>);
        QMap<int, ImageStruct::SaccessoriesSVG*> *tmpSVG = this->m_map_SVG_image.value(frameID);
        if(tmpSVG->contains(i))
        {
            delete tmpSVG->value(i);
            tmpSVG->remove(i);
        }
        tmpSVG->insert(i, rootSVG);
    }
}

void Accessories::toImage(int width, int height)
{
    QMapIterator<QString, QMap<int, ImageStruct::SaccessoriesSVG*>*> layerIterator(this->m_map_SVG_image);
    while(layerIterator.hasNext())
    {
        layerIterator.next();
        for(int i = 0; i < layerIterator.value()->size(); i++)
        {
            QPixmap *pixel = new QPixmap(width, height);
            pixel->fill(Qt::transparent);//设置背景透明
            QPainter painter(pixel);
            layerIterator.value()->value(i)->replacedImage->render(&painter);
            QImage image = pixel->toImage();
            QImage *ponintImage = new QImage;
            ponintImage = &image;
            this->m_map_image[layerIterator.key()]->insert(i, ponintImage);
            delete pixel;
        }

    }
}

QString Accessories::toHex(QColor color) const
{
    QString str =
            (color.red()>15?(QString().number(color.red(), 16)):("0" + QString().number(color.red()))) +
            (color.green()>15?(QString().number(color.green(), 16)):("0" + QString().number(color.green()))) +
            (color.blue()>15?(QString().number(color.blue(), 16)):("0" + QString().number(color.blue())));
    return str.toUpper();
}

QString Accessories::readAccessoriesIndex(QString index, QString name) const
{
    QFile *loadFile = new QFile;
    loadFile->setFileName(index + "\\index.json");
    if(!loadFile->open(QIODevice::ReadOnly))
        {
            return nullptr;
        }

    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        return nullptr;
    }

    QJsonObject obj = jsonDoc.object();
    QStringList keys = obj.keys();

    for(int i = 0; i < keys.size(); i++)
        {
        if(keys.at(i) == name)
            return obj[keys.at((i))].toString();
        }

    return nullptr;
}

void Accessories::initAccessoriesIndex(QString index, QString ID)
{
    QFile file(index + "\\index.json");
    if(file.exists())
        file.remove();
    QJsonObject indexJson;
    QDir dir(index);
    dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        if(list.at(i).isDir())
        {
            QString name;
            if(QFileInfo(list.at(i).filePath() + "/metadata.json").isFile()){
                QFile *loadMetadata = new QFile;
                loadMetadata->setFileName(list.at(i).filePath() + "/metadata.json");
                loadMetadata->open(QIODevice::ReadOnly);
                QByteArray allData = loadMetadata->readAll();
                loadMetadata->close();
                delete loadMetadata;
                loadMetadata = nullptr;

                QJsonParseError json_error;
                QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
                if(json_error.error != QJsonParseError::NoError)
                {
                    break;
                }

                QJsonObject rootObj = jsonDoc.object();
                QJsonObject obj = rootObj.value("metadata").toObject();
                if(obj["id"] != ID)
                    break;
                name = obj["name"].toString();
            }else{
                continue;
            }
            indexJson.insert(name, list.at(i).filePath());
        }

    }
    QJsonDocument indexJsonDoc;
    indexJsonDoc.setObject(indexJson);
    file.open(QFile::ReadWrite);
    file.write(indexJsonDoc.toJson());
    file.close();
}
