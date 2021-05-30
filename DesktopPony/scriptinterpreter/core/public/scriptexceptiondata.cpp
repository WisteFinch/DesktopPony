#include "scriptexceptiondata.h"

ScriptExceptionData::ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t)
{
    this->m_exception_type = e;
    this->m_token_data = t;
}

ScriptExceptionData::ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<ValueData> v)
{
    this->m_exception_type = e;
    this->m_token_data = t;
    this->m_value_data = v;
}

ScriptExceptionData::ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<int> i)
{
    this->m_exception_type = e;
    this->m_token_data = t;
    this->m_int_data = i;
}

ScriptExceptionData::ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<std::string> str)
{
    this->m_exception_type = e;
    this->m_token_data = t;
    this->m_str_data = str;
}

ScriptExceptionData::ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<int> i, std::vector<std::string> str)
{
    this->m_exception_type = e;
    this->m_token_data = t;
    this->m_int_data = i;
    this->m_str_data = str;
}
