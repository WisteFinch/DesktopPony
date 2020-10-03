/**
 * @file scriptinterpreter/lexer/lexer.h
 * @brief 词法划分器
 * @author WisteFinch
 * @date 2020.10.3
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

#ifndef LEXER_H
#define LEXER_H
#include "scriptinterpreter/limit.h"
#include <QString>
#include <QChar>
#include <QPair>
#include <QVector>

class ScriptLexer
{
public:
    ScriptLexer();

    QVector<QPair<QString, SYN>> *m_token;

    QVector<QPair<QString, SYN>>* divToken(QString str);

private:
    QChar m_ch;
    QString m_token_num, m_str;
    int m_point;

    SYN judge();
    inline QChar getNext();
    inline QChar get(int n);
};

#endif // LEXER_H
