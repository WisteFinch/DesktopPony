#include "text.h"

Text::Text()
{

}

Text::~Text()
{
    m_p_loc = nullptr;
}

void Text::init(Localisation *ptrLoc)
{
    this->m_p_loc = ptrLoc;
    ptrLoc = nullptr;
}

QString Text::replacePara(QString str, QString objUuid)
{
    //正则匹配
    QRegularExpression re(STR_PARA_REPLACE_REGEXP);
    QRegularExpressionMatchIterator i = re.globalMatch(str);
    if(i.hasNext()) {
        QString replacedStr;
        int start = 0, end = 0;
        while(i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            start = match.capturedStart();
            replacedStr.append(str.midRef(end, start - end));
            end = match.capturedEnd();
            replacedStr.append(getPara(match.captured(), objUuid));
        }
        replacedStr.append(str.midRef(end));
        return replacedStr;
    } else {
        return str;
    }
}

QString Text::getPara(QString para, QString objUuid)
{
    int index = para.indexOf(":");
    QString key, value;
    if(index == -1) {
        key = para.mid(2, para.length() - 3);
    } else {
        key = para.mid(2, index - 2).toLower();
        value = para.midRef(index + 1, para.length() - index - 2).toString();
    }
    if(key == "loc_pub") {
        return getLoc(value);
    } else if(key == "loc_priv") {
        return getLocPriv(value, objUuid);
    } else {
        return nullptr;
    }
}

QString Text::get(QString str, QString objUuid)
{
    return replacePara(str, objUuid);
}

QString Text::getLoc(QString key)
{
    return replacePara(this->m_p_loc->get(key));
}

QString Text::getLocPriv(QString key, QString objUuid)
{
    return replacePara(this->m_p_loc->getPriv(objUuid, key));
}
