/**
 * @file scriptinterpreter/core/scriptexception.h
 * @brief 脚本-异常
 * @author WisteFinch
 * @date 2020.12.13
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

#ifndef SCRIPTEXCEPTION_H
#define SCRIPTEXCEPTION_H

#include "scriptlimit.h"
#include "scriptvariable.h"
#include "file/localisation.h"
#include <QString>
#include <QVector>
#include <QRegularExpression>

struct Value;
class VariableDimension;
class Variable;

enum SCRIPT_EXCEPTION_TYPE {
    EXCEPTION_I001,   ///< 参数数量错误
    EXCEPTION_I002,   ///< 无效的运算
    EXCEPTION_I003,   ///< 数组大小不是整数类型
    //EXCEPTION_I004,   ///<
    EXCEPTION_I005,   ///< 数组下标不是整数
    EXCEPTION_I006,   ///< 无法增加指定类型的值
    EXCEPTION_I007,   ///< 无法减少指定类型的值
    EXCEPTION_I008,   ///< 表达式不可赋值
    EXCEPTION_I009,   ///< 无法初始化变量
    EXCEPTION_I010,   ///< 重复定义
    EXCEPTION_I011,   ///< 数组索引超出数组结尾
    EXCEPTION_I012,   ///< 下标值不是数组
    EXCEPTION_I013,   ///< 数组初始值必须是初始值列表
    EXCEPTION_I014,   ///< 设定初始值时元素多余
    EXCEPTION_I015,   ///< 继续语句不在循环语句内
    EXCEPTION_I016,   ///< 中断语句不在循环语句内
};

struct ScriptExceptionTypeName {
    QMap<SCRIPT_EXCEPTION_TYPE, QString> m_names;
    ScriptExceptionTypeName()
    {
        this->m_names.insert(EXCEPTION_I001, "EXCEPTION_I001");
        this->m_names.insert(EXCEPTION_I002, "EXCEPTION_I002");
        this->m_names.insert(EXCEPTION_I003, "EXCEPTION_I003");
        //this->m_names.insert(EXCEPTION_I004, "EXCEPTION_I004");
        this->m_names.insert(EXCEPTION_I005, "EXCEPTION_I005");
        this->m_names.insert(EXCEPTION_I006, "EXCEPTION_I006");
        this->m_names.insert(EXCEPTION_I007, "EXCEPTION_I007");
        this->m_names.insert(EXCEPTION_I008, "EXCEPTION_I008");
        this->m_names.insert(EXCEPTION_I009, "EXCEPTION_I009");
        this->m_names.insert(EXCEPTION_I010, "EXCEPTION_I010");
        this->m_names.insert(EXCEPTION_I011, "EXCEPTION_I011");
        this->m_names.insert(EXCEPTION_I012, "EXCEPTION_I012");
        this->m_names.insert(EXCEPTION_I013, "EXCEPTION_I013");
        this->m_names.insert(EXCEPTION_I014, "EXCEPTION_I014");
        this->m_names.insert(EXCEPTION_I015, "EXCEPTION_I015");
        this->m_names.insert(EXCEPTION_I016, "EXCEPTION_I016");
    }
    QString getName(SCRIPT_EXCEPTION_TYPE e)
    {
        return this->m_names.value(e);
    }
};

struct ScriptExceptionData {
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, QVector<TokenData> t);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, QVector<TokenData> t, QVector<Value> v);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, QVector<TokenData> t, QStringList str);
    ScriptExceptionData(SCRIPT_EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i, QStringList str);
    SCRIPT_EXCEPTION_TYPE m_exception_type; /// 异常类型
    QVector<TokenData> m_token_data; ///< 位置和内容信息
    QVector<Value> m_value_data;
    QVector<int> m_int_data;
    QStringList m_str_data;
    VALUE_TYPE m_value_type;
};

class ScriptException
{
public:
    ~ScriptException();
    void init(ScriptExceptionData data, Localisation *l);
    void init(Localisation *l);
    void initInfo();
    QString toStr();
    ScriptExceptionData *m_exception_data = nullptr;
    QString m_exception_info;
private:
    Localisation *m_p_localisation = nullptr;
    QString m_regexp = "(§\\[.*?\\])";
    ScriptExceptionTypeName m_exception_type_name;
    QString getPara(QString str);
    QVector<int> strRegExp(QString &str, QString regExp);
};

#endif // SCRIPTEXCEPTION_H
