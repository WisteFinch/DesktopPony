/**
 * @file scriptinterpreter/scriptexception.h
 * @brief 脚本-异常
 * @author WisteFinch
 * @date 2020.12.13
 *
 * MIT License
 * Copyright (c) 2019-2020 WisteFinch
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
#include "scriptinterpreter/scriptlimit.h"

class ScriptException
{
public:
    enum EXCEPTION_TYPE{
        C001,   ///< 参数数量错误
        C002,   ///< 无效的运算
        C003,   ///< 数组大小不是整数类型
        C004,   ///< 重复定义
        C005,   ///< 数组下标不是整数
        C006,   ///< 无法增加指定类型的值
        C007,   ///< 无法减少指定类型的值
        C008,   ///< 表达式不可赋值
        C009,   ///< 无法初始化变量
        C010,   ///< 重复成员
        C011,   ///< 数组索引超出数组结尾
        C012,   ///< 下标值不是数组
    };

    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<Value> v);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<QString> str);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i, QVector<QString> str);


    EXCEPTION_TYPE e;
    QVector<TokenData> t;
    QVector<int> i;
    QVector<QString> str;
    QVector<Value> v;
};

#endif // SCRIPTEXCEPTION_H
