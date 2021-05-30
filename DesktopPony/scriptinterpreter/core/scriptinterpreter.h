/**
 * @file scriptinterpreter/core/scriptinterpreter.h
 * @brief 脚本-解释器
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

#ifndef SCRIPTINTERPRETER_H
#define SCRIPTINTERPRETER_H

#include "scriptlimit.h"
#include "parser/parser.h"
#include "scriptvariable.h"
#include "scriptexception.h"
#include <QMap>
#include <QVector>
#include <QString>

class ScriptInterpreter
{
public:
    ScriptInterpreter();

    QMap<QString, ValueData> m_global_data;

    /**
     * @brief 运行
     * @details 解释运行脚本
     * @param 语法树
     */
    QVector<ValueData> run(ScriptParserNode *node);

    /**
     * @brief 运行
     * @details 解释运行脚本
     * @param 脚本文本
     */
    QVector<ValueData> run(QString str);

private:
    ValueData setVariable(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data);
    ValueData getVariable(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 获取局部变量
    ValueData getVariable(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 从数组获取局部变量
    ValueData getVariablePointer(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 获取局部变量指针
    ValueData getVariablePointer(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 从数组获取局部变量指针
    Variable *insertVariable(QString id, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 插入局部变量
    Variable *insertVariable(QString id, QVector<int> size, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 插入局部变量数组
    void clearVariable(QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 清除局部变量
    ValueData operation(ValueData left, ValueData right, TokenData t);
    ValueData operation(ValueData var, TokenData t);
    ValueData _operation(ValueData left, ValueData right, TokenData t);

    TokenData iStatementList(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 语句列表
    TokenData iStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 语句
    TokenData iCompoundStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 复合语句
    TokenData iJumpStatement(ScriptParserNode *node);
    ValueData iExpressionStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 表达式语句
    ValueData iFunction(ValueData id, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 函数
    TokenData iIf(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< if
    TokenData iIfBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, ValueData flag); ///< if主体
    void iWhile(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< while
    void iDeclaration(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 声明
    void iInitDeclaratorList(TokenData type, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化声明列表
    void iInitDeclarator(TokenData t, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化声明
    Variable *iDeclarator(TokenData type, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 声明符
    QVector<int> iDeclaratorSize(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 声明数组大小
    void iInitializer(Variable *var, QVector<int> subscripte, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化
    void iInitializerList(Variable *var, QVector<int> subscripte, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化列表
    void iInitializerInitElement(ValueData *left, ValueData right); ///< 初始化变量-初始化元素
    TokenData iDeclarationTypeSpecifier(ScriptParserNode *node); ///< 类型说明符
    ValueData iId(ScriptParserNode *node); ///< id
    QVector<ValueData> iExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 表达式
    ValueData iAssignmentExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 赋值表达式
    ValueData iConditionalHeadExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 条件表达式
    ValueData iConditionalBodyExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, bool flag); ///< 条件表达式
    ValueData iLogicalOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 逻辑或表达式
    ValueData iLogicalAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 逻辑与表达式
    ValueData iInclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 或运算表达式
    ValueData iExclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 异或表达式
    ValueData iAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 与表达式
    ValueData iEqualityExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 相等表达式
    ValueData iRelationalExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 关系表达式
    ValueData iShiftExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 替换表达式
    ValueData iAdditiveExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 加法表达式
    ValueData iMultiplicativeExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 乘法表达式
    ValueData iUnaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 一元表达式
    ValueData iPostfixExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 后缀表达式
    ValueData iPrimaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 基本表达式
    ValueData iConstant(ScriptParserNode *node); ///< 常量
    ValueData iStringLiteral(ScriptParserNode *node); ///< 字符串常量
    QVector<int> iVariableSubscripte(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 变量下标
};

#endif // SCRIPTINTERPRETER_H
