/**
 * @file scriptinterpreter/scriptinterpreter.h
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
#include "scriptexception.h"
#include "scriptvariable.h"
#include <QMap>
#include <QVector>
#include <QString>

class ScriptInterpreter
{
public:
    ScriptInterpreter();

    QMap<QString, Value> m_global_data;

    /**
     * @brief 运行
     * @details 解释运行脚本
     * @param 语法树
     */
    QStringList run(ScriptParserNode *node);

    /**
     * @brief 运行
     * @details 解释运行脚本
     * @param 脚本文本
     */
    QStringList run(QString str);

private:

    QStringList m_output;

    QString valueToString(Value v);

    /**
     * @brief 异常处理
     * @param 异常
     */
    Value ExceptionHandle(ScriptException &se);

    Value setVariable(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data);
    Value getVariable(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 获取局部变量
    Value getVariable(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 从数组获取局部变量
    Value getVariablePointer(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 获取局部变量指针
    Value getVariablePointer(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data); ///< 从数组获取局部变量指针
    void insertVariable(QString id, Value rValue, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 插入局部变量
    void insertVariable(QString id, QVector<int> size, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 插入局部变量数组
    void clearVariable(QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 清除局部变量
    Value operation(Value left, Value right, TokenData t);
    Value _operation(Value left, Value right, TokenData t);

    void iStatementList(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 语句列表
    void iStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 语句
    void iCompoundStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 复合语句
    void iJumpStatement(ScriptParserNode *node);
    Value iExpressionStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 表达式语句
    Value iFunction(Value id, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 函数
    void iIf(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< if
    void iIfBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, Value flag); ///< if主体
    void iWhile(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< while
    void iDeclaration(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 声明
    void iInitDeclaratorList(TokenData type, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化声明列表
    void iInitDeclarator(TokenData t, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化声明
    void iDeclarator(TokenData type, Value rValue, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 声明符
    QVector<int> iDeclaratorSize(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 声明数组大小
    Value iInitializer(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化
    //暂时禁用数组初始化void iInitializerList(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 初始化列表
    TokenData iDeclarationTypeSpecifier(ScriptParserNode *node); ///< 类型说明符
    Value iId(ScriptParserNode *node); ///< id
    QVector<Value> iExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 表达式
    Value iAssignmentExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 赋值表达式
    //Value iAssignmentOperator(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 赋值符号
    Value iConditionalHeadExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 条件表达式
    Value iConditionalBodyExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, bool flag); ///< 条件表达式
    Value iLogicalOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 逻辑或表达式
    Value iLogicalAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 逻辑与表达式
    Value iInclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 或运算表达式
    Value iExclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 异或表达式
    Value iAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 与表达式
    Value iEqualityExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 相等表达式
    Value iRelationalExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 关系表达式
    Value iShiftExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 替换表达式
    Value iAdditiveExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 加法表达式
    Value iMultiplicativeExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 乘法表达式
    Value iUnaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 一元表达式
    Value iPostfixExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 后缀表达式
    Value iPrimaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 基本表达式
    Value iConstant(ScriptParserNode *node); ///< 常量
    Value iStringLiteral(ScriptParserNode *node); ///< 字符串常量
    QVector<int> iVariableSubscripte(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 变量下标
};

#endif // SCRIPTINTERPRETER_H
