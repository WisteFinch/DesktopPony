/**
 * @file scriptinterpreter/parser/parsernode.h
 * @brief 语法分析节点
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

#ifndef PARSERNODE_H
#define PARSERNODE_H

#include "scriptinterpreter/limit.h"
#include <QPair>
#include <QString>

class ScriptParserNode
{
public:

    ScriptParserNode();
    ~ScriptParserNode();

    void set(QString d, PARSER p, ScriptParserNode *l, ScriptParserNode *r);
    void set(QString d, PARSER p, ScriptParserNode *l, ScriptParserNode *r, SYN syn);
    ScriptParserNode *m_p_left_child = nullptr;
    ScriptParserNode *m_p_right_child = nullptr;
    PARSER m_p = parser_null;
    QString m_d = nullptr;
    SYN m_syn = syn_null;

};

#endif // PARSERNODE_H