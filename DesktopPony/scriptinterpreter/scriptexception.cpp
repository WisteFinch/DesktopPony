#include "scriptexception.h"

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t)
{
    this->m_exception_data.e = e;
    this->m_exception_data.token = t;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<Value> v)
{
    this->m_exception_data.e = e;
    this->m_exception_data.token = t;
    this->m_exception_data.value = v;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i)
{
    this->m_exception_data.e = e;
    this->m_exception_data.token = t;
    this->m_exception_data.i = i;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<QString> str)
{
    this->m_exception_data.e = e;
    this->m_exception_data.token = t;
    this->m_exception_data.str = str;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i, QVector<QString> str)
{
    this->m_exception_data.e = e;
    this->m_exception_data.token = t;
    this->m_exception_data.i = i;
    this->m_exception_data.str = str;
}

ScriptException::ScriptException(ExceptionData e)
{
    this->m_exception_data = e;
}
