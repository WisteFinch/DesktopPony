#include "scriptexception.h"

ScriptException::ScriptException()
{

}

ScriptException::~ScriptException()
{
    this->m_p_localisation = nullptr;
}

void ScriptException::init(ScriptExceptionData data, Localisation *l)
{
    this->m_exception_data = new ScriptExceptionData(data);
    this->m_p_localisation = l;
}

void ScriptException::init(Localisation *l)
{
    this->m_p_localisation = l;
}

void ScriptException::initInfo()
{
    ScriptExceptionTypeName *n = new ScriptExceptionTypeName;
    QString str = this->m_p_localisation->get(QString::fromStdString(n->getName(this->m_exception_data->m_exception_type)));
    QString replacedStr;
    int start = 0, end = 0;
    //正则匹配
    QRegularExpression re(this->m_regexp);
    QRegularExpressionMatchIterator i = re.globalMatch(str);
    while(i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        start = match.capturedStart();
        replacedStr.append(str.midRef(end, start - end));
        end = match.capturedEnd();
        replacedStr.append(getPara(match.captured()));
    }
    replacedStr.append(str.midRef(end));

    this->m_exception_info = replacedStr;
}

QString ScriptException::getPara(QString str)
{
    int index = str.indexOf(":");
    QString key;
    int value = 0;
    if(index == -1) {
        key = str.mid(2, str.length() - 3);
    } else {

        key = str.mid(2, index - 2).toLower();
        value = str.midRef(index + 1, str.length() - index - 2).toInt();
    }
    if(key == "type") {
        return QString::fromStdString(this->m_exception_type_name.getName(this->m_exception_data->m_exception_type));
    } else if(key == "num") {
        if(this->m_exception_data->m_int_data.empty() || this->m_exception_data->m_int_data.size() < static_cast<uint>(value)) {
            return nullptr;
        }
        return QString::number(this->m_exception_data->m_int_data.at(value));
    } else if(key == "str") {
        if(this->m_exception_data->m_str_data.empty() || this->m_exception_data->m_str_data.size() < static_cast<uint>(value)) {
            return nullptr;
        }
        return QString::fromStdString(this->m_exception_data->m_str_data.at(value));
    } else if(key == "value") {
        if(this->m_exception_data->m_value_data.empty() || this->m_exception_data->m_value_data.size() < static_cast<uint>(value)) {
            return nullptr;
        }
        return QString::fromStdString(this->m_exception_data->m_value_data[value].castStr());
    } else if(key == "value_type") {
        if(this->m_exception_data->m_value_data.empty() || this->m_exception_data->m_value_data.size() < static_cast<uint>(value)) {
            return nullptr;
        }
        return QString::fromStdString(this->m_exception_data->m_value_data[value].getType());
    }
    return nullptr;
}

QString ScriptException::toStr()
{
    QString str;
    str.append("[ERROR/" +
               QString::fromStdString(this->m_exception_type_name.getName(this->m_exception_data->m_exception_type)) +
               "]:");
    str.append(this->m_exception_info);
    return str;
}
