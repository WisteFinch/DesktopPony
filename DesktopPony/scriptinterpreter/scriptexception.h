/**
 * @file scriptinterpreter/scriptexception.h
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

#include <QString>
#include <QVector>
#include "scriptlimit.h"

class ScriptException
{
public:
    enum EXCEPTION_TYPE {
        I001,   ///< 参数数量错误
        I002,   ///< 无效的运算
        I003,   ///< 数组大小不是整数类型
        I004,   ///< 重复定义
        I005,   ///< 数组下标不是整数
        I006,   ///< 无法增加指定类型的值
        I007,   ///< 无法减少指定类型的值
        I008,   ///< 表达式不可赋值
        I009,   ///< 无法初始化变量
        I010,   ///< 重复成员
        I011,   ///< 数组索引超出数组结尾
        I012,   ///< 下标值不是数组
        I013,   ///< 数组初始值必须是初始值列表
        I014,   ///< 设定初始值时元素多余
        I015,   ///< 继续语句不在循环语句中
        I016,   ///< 中断语句不在循环语句中
    };
    struct ExceptionData {
        EXCEPTION_TYPE e; /// 异常类型
        QVector<TokenData> token; ///< 位置和内容信息
        QVector<Value> value;
        QVector<int> i;
        QVector<QString> str;
        VALUE_TYPE value_type;
    } m_exception_data;

    ScriptException(ExceptionData e);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<Value> v);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<QString> str);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i, QVector<QString> str);
};

#endif // SCRIPTEXCEPTION_H
