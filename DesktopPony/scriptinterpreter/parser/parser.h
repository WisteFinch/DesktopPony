/**
 * @file scriptinterpreter/parser/parser.h
 * @brief 脚本-语法分析器-主类
 * @author WisteFinch
 * @date 2020.11.29
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

#ifndef PARSER_H
#define PARSER_H

#include "scriptinterpreter/lexer/lexer.h"
#include "scriptinterpreter/parser/parsernode.h"
#include "scriptinterpreter/scriptlimit.h"
#include <QString>
#include <QPair>
#include <QDebug>
#include "iostream"

class ScriptParser
{
public:
    ScriptParser();

    enum MODE{
        mode_normal,
        mode_cond
    };

    ScriptParserNode *m_p_root; ///< 根节点
    ScriptLexer *m_p_lexer; ///< 词法划分器

    int m_index; ///< 序号

/*test
    void ergodic(ScriptParserNode *n);
    void test(QString s);
*/

    /**
     * @brief 设置语法树
     * @param 脚本文本
     */
    void set(QString s);

    /**
     * @brief 设置语法树
     * @param 脚本文本, 类型
     */
    void set(QString s, MODE m);

    /**
     * @brief 设置语法树
     * @param 词法划分器
     */
    void set(ScriptLexer *l);

    /**
     * @brief 设置语法树
     * @param 词法划分器, 类型
     */
    void set(ScriptLexer *l, MODE m);


    /**
     * @brief 取词块
     * @param 词块文本
     */
    void eat(QString s);

    /**
     * @brief 取词块
     * @param 词块类型
     */
    void eat(SYN s);
    TokenData getToken(); ///< 获取词块
    TokenData curToken(); ///< 当前词块
    TokenData lookAhead(); ///< 前一词块
    TokenData lookAhead(int i); ///< 前第n个词块
    void errorExit();

    void pStatementList(ScriptParserNode *node); ///< 语句列表
    void pStatement(ScriptParserNode *node); ///< 语句
    void pCompoundStatement(ScriptParserNode *node); ///< 复合语句
    void pExpressionStatement(ScriptParserNode *node); ///< 表达式语句
    void pJumpStatement(ScriptParserNode *node); ///< 跳转语句
    void pIf(ScriptParserNode *node); ///< if
    void pIfBody(ScriptParserNode *node); ///< if主体
    void pWhile(ScriptParserNode *node); ///< while
    void pDeclare(ScriptParserNode *node); ///< 声明
    void pDeclareBody(ScriptParserNode *node); ///< 声明主体
    void pDeclares(ScriptParserNode *node); ///< 声明列表
    void pDeclareBasic(ScriptParserNode *node); ///< 基本声明
    void pType(ScriptParserNode *node); ///< 类型
    void pId(ScriptParserNode *node); ///< id
    void pExpression(ScriptParserNode *node); ///< 表达式
    void pAssignmentExpression(ScriptParserNode *node); ///< 赋值表达式
    //void pAssignmentOperator(ScriptParserNode *node); ///< 赋值符号
    void pConditionalHeadExpression(ScriptParserNode *node); ///< 条件表达式
    void pConditionalBodyExpression(ScriptParserNode *node); ///< 条件表达式
    void pLogicalOrExpression(ScriptParserNode *node); ///< 逻辑或表达式
    void pLogicalAndExpression(ScriptParserNode *node); ///< 逻辑与表达式
    void pInclusiveOrExpression(ScriptParserNode *node); ///< 或运算表达式
    void pExclusiveOrExpression(ScriptParserNode *node); ///< 异或表达式
    void pAndExpression(ScriptParserNode *node); ///< 与表达式
    void pEqualityExpression(ScriptParserNode *node); ///< 相等表达式
    void pRelationalExpression(ScriptParserNode *node); ///< 关系表达式
    void pShiftExpression(ScriptParserNode *node); ///< 替换表达式
    void pAdditiveExpression(ScriptParserNode *node); ///< 加法表达式
    void pMultiplicativeExpression(ScriptParserNode *node); ///< 乘法表达式
    void pUnaryExpression(ScriptParserNode *node); ///< 一元表达式
    void pPostfixExpression(ScriptParserNode *node); ///< 后缀表达式
    void pPrimaryExpression(ScriptParserNode *node); ///< 基本表达式
    void pConstant(ScriptParserNode *node); ///< 常量
    void pStringLiteral(ScriptParserNode *node); ///< 字符串常量
};

#endif // PARSER_H
