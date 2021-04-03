/**
 * @file scriptinterpreter/core/lexer/lexer.h
 * @brief 脚本-词法划分器-主类
 * @author WisteFinch
 * @date 2020.10.29
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

#ifndef LEXER_H
#define LEXER_H
#include "../scriptlimit.h"
#include <QString>
#include <QChar>
#include <QPair>
#include <QVector>

class ScriptLexer
{
public:
    ScriptLexer();

    QVector<TokenData> *m_tokens; ///< 词列表

    /**
     * @brief 划分词块
     * @param 脚本文本
     */
    QVector<TokenData> *divToken(QString str);

private:
    QChar m_ch; ///< 当前字符
    QString m_token_num; ///< 当前词块文本
    QString m_str; ///< 脚本文本
    int m_point; ///< 位置
    int m_line; ///< 行
    int m_row; ///< 列

    SYN judge(); ///< 判断词类型
    inline QChar getNext(); ///< 获取下一个词
    inline QChar get(int n); ///< 获取词
};

#endif // LEXER_H
