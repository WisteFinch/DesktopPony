/**
 * @file scriptinterpreter/core/public/scriptexceptiondata.h
 * @brief 脚本-异常信息
 * @author WisteFinch
 * @date 2021.5.3
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

#ifndef SCRIPTEXCEPTIONDATA_H
#define SCRIPTEXCEPTIONDATA_H

#include "scriptvariabledata.h"
#include "sharedefinition.h"

class ValueData;

struct ScriptExceptionTypeName {
    std::map<SCRIPT_EXCEPTION_TYPE, std::string> m_names;
    ScriptExceptionTypeName()
    {
        this->m_names.insert(std::make_pair(EXCEPTION_I001, "EXCEPTION_I001"));
        this->m_names.insert(std::make_pair(EXCEPTION_I002, "EXCEPTION_I002"));
        this->m_names.insert(std::make_pair(EXCEPTION_I003, "EXCEPTION_I003"));
        //this->m_names.insert(EXCEPTION_I004, "EXCEPTION_I004");
        this->m_names.insert(std::make_pair(EXCEPTION_I005, "EXCEPTION_I005"));
        this->m_names.insert(std::make_pair(EXCEPTION_I006, "EXCEPTION_I006"));
        this->m_names.insert(std::make_pair(EXCEPTION_I007, "EXCEPTION_I007"));
        this->m_names.insert(std::make_pair(EXCEPTION_I008, "EXCEPTION_I008"));
        this->m_names.insert(std::make_pair(EXCEPTION_I009, "EXCEPTION_I009"));
        this->m_names.insert(std::make_pair(EXCEPTION_I010, "EXCEPTION_I010"));
        this->m_names.insert(std::make_pair(EXCEPTION_I011, "EXCEPTION_I011"));
        this->m_names.insert(std::make_pair(EXCEPTION_I012, "EXCEPTION_I012"));
        this->m_names.insert(std::make_pair(EXCEPTION_I013, "EXCEPTION_I013"));
        this->m_names.insert(std::make_pair(EXCEPTION_I014, "EXCEPTION_I014"));
        this->m_names.insert(std::make_pair(EXCEPTION_I015, "EXCEPTION_I015"));
        this->m_names.insert(std::make_pair(EXCEPTION_I016, "EXCEPTION_I016"));
    }
    std::string getName(SCRIPT_EXCEPTION_TYPE e)
    {
        return this->m_names.at(e);
    }
};

struct ScriptExceptionData {
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<ValueData> v);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<int> i);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<std::string> str);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, std::vector<TokenData> t, std::vector<int> i, std::vector<std::string> str);
    SCRIPT_EXCEPTION_TYPE m_exception_type; ///< 异常类型
    std::vector<TokenData> m_token_data;
    std::vector<ValueData> m_value_data;
    std::vector<int> m_int_data;
    std::vector<std::string> m_str_data;
    VALUE_TYPE m_value_type;
};

#endif // SCRIPTEXCEPTIONDATA_H
