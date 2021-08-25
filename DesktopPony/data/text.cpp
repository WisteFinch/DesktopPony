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

QString Text::replacePara(QString str, QVariant var)
{
    //正则匹配
    QRegularExpression re(STR_PARA_REPLACE_REGEXP);
    QRegularExpressionMatchIterator i = re.globalMatch(str);
    if(i.hasNext()) {
        QString replacedStr;
        qint32 start = 0, end = 0;
        while(i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            start = match.capturedStart();
            replacedStr.append(str.midRef(end, start - end));
            end = match.capturedEnd();
            replacedStr.append(getPara(match.captured(), var));
        }
        replacedStr.append(str.midRef(end));
        return replacedStr;
    } else {
        return str;
    }
}

QString Text::getPara(QString para, QVariant var)
{
    qint32 index = para.indexOf(QStringLiteral(":"));
    QString key, value;
    if(index == -1) {
        key = para.mid(2, para.length() - 3);
    } else {
        key = para.mid(2, index - 2).toLower();
        value = para.midRef(index + 1, para.length() - index - 2).toString();
    }
    if(key == QStringLiteral("loc_pub")) {
        return getLoc(value);
    } else if(key == QStringLiteral("loc_priv")) {
        return getLocPriv(value, var);
    } else if(key == QStringLiteral("var")) {
        qint32 s = value.toInt();
        s = s < 1 ? 1 : s;
        return varToString(getVar(var, s));
    } else {
        return nullptr;
    }
}

QString Text::get(QString str, QVariant var)
{
    return replacePara(str, var);
}

QString Text::getLoc(QString key, QVariant var)
{
    return replacePara(this->m_p_loc->get(key), var);
}

QString Text::getLocPriv(QString key, QVariant var)
{
    if(getVar(var).type() != QVariant::String) {
        return nullptr;
    }
    return replacePara(this->m_p_loc->getPriv(getVar(var).toString(), key), var);
}

QVariant Text::getVar(QVariant &var, qint32 s)
{
    if(s == 0) {
        if(var.type() == QVariant::List) {
            return var.toList().at(0);
        } else {
            return var;
        }
    } else if(s > 0) {
        if(var.type() == QVariant::List) {
            if(var.toList().count() <= s) {
                return QVariant();
            } else {
                return var.toList().at(s);
            }
        } else {
            return QVariant();
        }
    } else {
        return QVariant();
    }
}

QString Text::varToString(QVariant var)
{
    if(var.type() == QVariant::String) {
        return var.toString();
    } else if(var.type() == QVariant::Int) {
        return QString::number(var.toInt());
    } else if(var.type() == QVariant::Double) {
        return QString::number(var.toDouble());
    } else if(var.type() == QVariant::Bool) {
        return var.toBool() ? QStringLiteral("true") : QStringLiteral("false");
    } else if(var.type() == QVariant::UInt) {
        return QString::number(var.toUInt());
    } else if(var.type() == QVariant::Url) {
        return var.toUrl().toString();
    } else if(var.type() == QVariant::LongLong) {
        return QString::number(var.toLongLong());
    } else if(var.type() == QVariant::ULongLong) {
        return QString::number(var.toULongLong());
    } else {
        return nullptr;
    }
}
