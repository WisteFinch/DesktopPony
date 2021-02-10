#include "scriptinterpreter.h"

ScriptInterpreter::ScriptInterpreter()
{

}

QStringList ScriptInterpreter::run(ScriptParserNode *node)
{
    //清空输出
    this->m_output.clear();
    //开始解释脚本
    QMap<QString, QVector<QPair<int, Variable *> > *> *data = new QMap<QString, QVector<QPair<int, Variable *> > *>;//局部变量数据
    QVector<QVector<QString> *> *record = new QVector<QVector<QString> *>;  //局部变量记录
    QVector<QString> *subRecord = new QVector<QString>;
    record->append(subRecord);
    try {
        //根节点为语句列表
        if(node->m_p == parser_sl) {
            iStatementList(node, data, record, 0);
        }
        //根节点为表达式
        else if(node->m_p == parser_exp) {
            this->m_output.append(valueToString(iExpression(node, data, record, 0).at(0)));
        }
        //根节点为条件表达式
        else {
            this->m_output.append(valueToString(iConditionalHeadExpression(node, data, record, 0)));
        }
    } catch (ScriptException &se) {
        ExceptionHandle(se);
    }
    //解释脚本结束
    //返回输出
    return this->m_output;
}

QStringList ScriptInterpreter::run(QString str)
{
    //清空输出
    this->m_output.clear();
    //从脚本文本生成语法树
    ScriptParser *s = new ScriptParser();
    try {
        s->set(str);
    } catch (ScriptException &se) {
        ExceptionHandle(se);
    }
    //解释脚本
    return run(s->m_p_root);
}

QString ScriptInterpreter::valueToString(Value v)
{
    if(v.type == type_int) {
        return QString(v._int);
    } else if(v.type == type_float) {
        return QString("%1").arg(static_cast<double>(v._float));
    } else if(v.type == type_bool) {
        if(v._bool) {
            return "true";
        } else {
            return "false";
        }
    } else if(v.type == type_str) {
        return "\"" + v._str + "\"";
    } else if(v.type == type_id) {
        return "id:" + v._id;
    } else if(v.type == type_var) {
        return "var:" + valueToString(*v._var);
    } else {
        return nullptr;
    }
}

Value ScriptInterpreter::ExceptionHandle(ScriptException &se)
{

}

void ScriptInterpreter::iStatementList(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //获取子节点类型
    PARSER p = node->m_p_left_child->m_p;
    //子节点类型为语句
    if(p == parser_s) {
        iStatement(node->m_p_left_child, data, record, layer);
        if(node->m_p_right_child != nullptr) {
            iStatementList(node->m_p_right_child, data, record, layer);
        }
    }
    //子节点类型为复合语句
    else if(p == parser_cs) {
        iCompoundStatement(node->m_p_left_child, data, record, layer);
        if(node->m_p_right_child != nullptr) {
            iStatementList(node->m_p_right_child, data, record, layer);
        }
    }
}

void ScriptInterpreter::iStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //分析子节点类型
    PARSER p = node->m_p_left_child->m_p;
    //子节点类型为声明
    if(p == parser_declaration) {
        iDeclaration(node->m_p_left_child, data, record, layer);
    }
    //子节点类型为表达式
    else if (p == parser_es) {
        iExpressionStatement(node->m_p_left_child, data, record, layer);
    }
    //子节点类型为while
    else if(p == parser_while) {
        iWhile(node->m_p_left_child, data, record, layer);
    }
    //子节点类型为if
    else if(p == parser_if) {
        iIf(node->m_p_left_child, data, record, layer);
    }
}

void ScriptInterpreter::iCompoundStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //添加新变量记录
    record->append(new QVector<QString>);
    //分析子节点类型
    PARSER p = node->m_p_left_child->m_p;
    //子节点类型为语句
    if(p == parser_s) {
        //解释子节点（变量层数加一）
        iStatement(node->m_p_left_child, data, record, layer + 1);
    }
    //子节点类型为语句列表
    else if(p == parser_sl) {
        //解释子节点（变量层数加一）
        iStatementList(node->m_p_left_child, data, record, layer + 1);
    }
    //删除新变量记录
    clearVariable(data, record, layer + 1);
}

void ScriptInterpreter::iJumpStatement(ScriptParserNode *node)
{
    /*******
     * 未完成
     *******/
}

Value ScriptInterpreter::iExpressionStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    if(node->m_p_left_child == nullptr) {
        Value v;
        return v;
    }
    QVector<Value> v;
    try {
        v = iExpression(node->m_p_left_child, data, record, layer);
    }  catch (ScriptException &se) {
        throw se;
    }
    return v.first();
}

Value ScriptInterpreter::iFunction(Value id, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    record->append(new QVector<QString>);
    QVector<Value> value;
    try {
        value = iExpression(node->m_p_right_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
    } catch (ScriptException &se) {
        throw se;
    }

    /*
     * 未完成
     */
}

void ScriptInterpreter::iIf(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    record->append(new QVector<QString>);
    QVector<Value> value = iExpression(node->m_p_left_child, data, record, layer + 1);
    clearVariable(data, record, layer + 1);
    if(value.size() != 1) {
        /************
         * 异常
         * 参数数量错误
         ************/
        QVector<TokenData> t;
        t.append(node->m_t);
        QVector<int> i;
        i.append(1);
        i.append(value.size());
        throw new ScriptException(ScriptException::C001, t, i);
    } else if(value.first().type != type_bool && value.first().type != type_int) {
        /***********
         * 异常
         * 无效的运算
         ***********/
        QVector<Value> v;
        v.append(value.first());
        QVector<TokenData> t;
        t.append(t);
        throw new ScriptException(ScriptException::C002, t, v);
    }
    iIfBody(node->m_p_right_child, data, record, layer, value.first());
}

void ScriptInterpreter::iIfBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, Value flag)
{
    try {
        //真
        if(flag._bool || flag._int) {
            iStatement(node->m_p_left_child, data, record, layer);
        }
        // 假
        else if(node->m_p_right_child != nullptr) {
            PARSER p = node->m_p_right_child->m_p;
            if(p == parser_if) {
                iIf(node->m_p_right_child, data, record, layer);
            } else {
                iStatement(node->m_p_right_child, data, record, layer);
            }
        }
    } catch (ScriptException &se) {
        throw se;
    }
}

void ScriptInterpreter::iWhile(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    record->append(new QVector<QString>);
    bool flag;
    try {
        flag = iExpression(node->m_p_left_child, data, record, layer + 1).at(0)._bool;
        clearVariable(data, record, layer + 1);
    } catch (ScriptException &se) {
        throw se;
    }
    while(flag) {
        try {
            iStatement(node->m_p_right_child, data, record, layer);
            record->append(new QVector<QString>);
            flag = iExpression(node->m_p_left_child, data, record, layer + 1).at(0)._bool;
            clearVariable(data, record, layer + 1);
        } catch (SYN &syn) {
            if(syn == syn_break) {
                break;
            } else if(syn == syn_continue) {
                continue;
            }
        } catch (ScriptException &se) {
            throw se;
        }
    }
}

void ScriptInterpreter::iDeclaration(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法:DECLARATION -> DECLARATIONTYPESPEC INITDECLARATORLIST
    try {
        //获取声明类型
        TokenData type = iDeclarationTypeSpecifier(node->m_p_left_child);
        //解释右节点
        iInitDeclaratorList(type, node->m_p_right_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }
}

void ScriptInterpreter::iInitDeclaratorList(TokenData type, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    try {
        //文法:NITDECLARATORLIST -> INITDECLARATOR
        iInitDeclarator(type, node->m_p_left_child, data, record, layer);
        //文法:NITDECLARATORLIST -> INITDECLARATOR, INITDECLARATORLIST
        if(node->m_p_right_child != nullptr) {
            iInitDeclaratorList(type, node->m_p_right_child, data, record, layer);
        }
    } catch (ScriptException &se) {
        throw se;
    }
}

void ScriptInterpreter::iInitDeclarator(TokenData t, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法:INITDECLARATOR-> DECLARATOR
    //                  -> DECLARATOR = INITIALIZER
    try {
        Value value;
        //存在右节点
        if(node->m_p_right_child != nullptr) {
            //获取初始化值
            value = iInitializer(node->m_p_right_child, data, record, layer);
        }
        iDeclarator(t, value, node->m_p_left_child, data, record, layer);
    } catch (ScriptInterpreter &se) {
        throw se;
    }
}

void ScriptInterpreter::iDeclarator(TokenData type, Value rValue, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法:DECLARATOR -> ID
    bool isArray = false;
    //获取标识符
    QString id = iId(node->m_p_left_child)._id;
    QVector<int> size;
    //判断是否为数组
    if(node->m_p_right_child != nullptr) {
        //文法:DECLARATOR -> ID DCLSIZE
        isArray = true;
        //解释下标
        try {
            size = iDeclaratorSize(node->m_p_right_child, data, record, layer + 1);
        } catch (ScriptException &se) {
            throw se;
        }
    }

    //插入变量
    try {
        if(isArray) {
            if(rValue.type != type_empty) {
                /************
                 * 异常
                 * 无法初始化变量
                 ************/
                QVector<TokenData> t;
                t.append(node->m_t);
                QVector<Value> v;
                v.append(rValue);
                throw new ScriptException(ScriptException::C009, t, v);
            }
            insertVariable(id, size, type, data, record, layer);
        } else {
            insertVariable(id, rValue, type, data, record, layer);
        }
    } catch (ScriptException::EXCEPTION_TYPE &se) {
        if(se == ScriptException::C009) {
            /************
             * 异常
             * 无法初始化变量
             ************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(rValue);
            throw new ScriptException(ScriptException::C009, t, v);
        } else if(se == ScriptException::C010) {
            /************
             * 异常
             * 重复成员
             ************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(rValue);
            throw new ScriptException(ScriptException::C009, t, v);
        }
    }
}

QVector<int> ScriptInterpreter::iDeclaratorSize(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<int> size;
    //文法:DCLSIZE -> [CONST]
    Value v = iConstant(node->m_p_left_child);
    if(v.isInt()) {
        size.append(v.toInt());
    } else {
        /******************
         * 异常
         * 数组大小不是整数类型
         ******************/
        QVector<TokenData> t;
        t.append(node->m_t);
        QVector<Value> v;
        v.append(v);
        throw new ScriptException(ScriptException::C003, t, v);
    }
    //文法:DCLSIZE -> [CONST] DCLSIZE
    if(node->m_p_right_child != nullptr) {
        try {
            size.append(iDeclaratorSize(node->m_p_right_child, data, record, layer));
        }  catch (ScriptException &se) {
            throw se;
        }
    }
    return size;
}

Value ScriptInterpreter::iInitializer(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法:INITIALIZER -> AEXP
    try {
        return iAssignmentExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptInterpreter &se) {
        throw se;
    }
}


TokenData ScriptInterpreter::iDeclarationTypeSpecifier(ScriptParserNode *node)
{
    return node->m_t;
}

Value ScriptInterpreter::iId(ScriptParserNode *node)
{
    Value v;
    v.type = type_id;
    v._id = node->m_t.str;
    return v;
}

QVector<Value> ScriptInterpreter::iExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<Value> value;

    //文法：EXP -> AEXP
    try {
        //解释左节点
        value.append(iAssignmentExpression(node->m_p_left_child, data, record, layer));
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：EXP -> AEXP, EXP
    if(node->m_p_right_child != nullptr) {
        try {
            //解释右节点
            value.append(iExpression(node->m_p_right_child, data, record, layer));
        } catch (ScriptException &se) {
            throw se;
        }
    }
    //返回运算结果
    return value;
}

Value ScriptInterpreter::iAssignmentExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：AEXP -> CHEXP
    if(node->m_p_left_child->m_p == parser_cond_head_exp) {
        try {
            //返回运算结果
            return iConditionalHeadExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    //文法：AEXP -> UEXP = AEXP
    //          -> UEXP += AEXP
    //          -> UEXP -= AEXP
    //          -> UEXP *= AEXP
    //          -> UEXP /= AEXP
    //          -> UEXP %= AEXP
    //          -> UEXP &= AEXP
    //          -> UEXP ^= AEXP
    //          -> UEXP |= AEXP
    else {
        Value left;
        Value right;
        try {
            //解释左节点
            left = iUnaryExpression(node->m_p_left_child, data, record, layer);
            //解释右节点
            right = iAssignmentExpression(node->m_p_right_child, data, record, layer);
        } catch (ScriptException &se) {
            throw se;
        }

        if(left.type != type_var) {
            /*************
             * 异常
             * 表达式不可赋值
             *************/
            QVector<Value> v;
            v.append(left);
            v.append(right);
            QVector<TokenData> t;
            t.append(node->m_t);
            throw new ScriptException(ScriptException::C008, t, v);
        }

//===========================存在问题=================================

        try {
            //运算
            *left._var = operation(*left._var, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
        //返回运算结果
        return *left._var;
    }
//==================================================================
}

Value ScriptInterpreter::iConditionalHeadExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：CHEXP	 -> LOREXP
    try {
        //解释左节点
        left = iLogicalOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：CHEXP -> LOREXP ? CBEXP
    if(node->m_p_right_child != nullptr) {
        if(left.type != type_bool && left.type != type_int) {
            /***********
             * 异常
             * 无效的运算
             ***********/
            QVector<Value> v;
            v.append(left);
            QVector<TokenData> t;
            t.append(t);
            throw new ScriptException(ScriptException::C002, t, v);
        }

        try {
            //返回运算结果
            return iConditionalBodyExpression(node->m_p_right_child, data, record, layer, left._bool || left._int);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iConditionalBodyExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, bool flag)
{
    //文法：CBEXP	 -> AEXP : CHEXP
    //标示为真
    if(flag) {
        try {
            //返回左节点
            return iAssignmentExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    //标示为假
    else {
        try {
            //返回右节点
            return iConditionalHeadExpression(node->m_p_right_child, data, record, layer);
        } catch (ScriptException &se) {
            throw se;
        }
    }
}

Value ScriptInterpreter::iLogicalOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：LOREXP -> LANDEXP
    try {
        //解释左节点
        left = iLogicalAndExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：LOREXP -> LANDEXP || LOREXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iLogicalOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iLogicalAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：LANDEXP -> IOREXP
    try {
        //解释左节点
        left = iInclusiveOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：LANDEXP -> IOREXP && LANDEXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iLogicalAndExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iInclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：LANDEXP -> IOREXP
    try {
        //解释左节点
        left = iExclusiveOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：LANDEXP -> IOREXP | IOREXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iInclusiveOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iExclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：EOREXP -> ANDEXP
    try {
        //解释左节点
        left = iAndExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：EOREXP -> ANDEXP ^ EOREXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iExclusiveOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：ANDEXP -> EQUALEXP
    try {
        //解释左节点
        left = iEqualityExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：ANDEXP -> EQUALEXP & ANDEXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iAndExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iEqualityExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：EQUALEXP -> RELAEXP
    try {
        //解释左节点
        left = iRelationalExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：EQUALEXP -> RELAEXP == EQUALEXP
    //              -> RELAEXP != EQUALEXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iEqualityExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iRelationalExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：RELAEXP -> SHIFTEXP
    try {
        //解释左节点
        left = iShiftExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：RELAEXP -> SHIFTEXP
    //             -> SHIFTEXP < RELAEXP
    //             -> SHIFTEXP > RELAEXP
    //             -> SHIFTEXP <= RELAEXP
    //             -> SHIFTEXP >= RELAEXP

    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iRelationalExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iShiftExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法:SHIFTEXP -> ADDEXP
    try {
        //解释左节点
        left = iAdditiveExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法:SHIFTEXP -> ADDEXP << SHIFTEXP
    //             -> ADDEXP >> SHIFTEXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iShiftExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iAdditiveExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：ADDEXP -> MULTEXP
    try {
        //解释左节点
        left = iMultiplicativeExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    //文法：ADDEXP -> MULTEXP + ADDEXP
    //            -> MULTEXP - ADDEXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iAdditiveExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iMultiplicativeExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：MULTEXP -> UEXP
    try {
        //解释左节点
        left = iUnaryExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    if(left.type == type_var) {
        left = *left._var;
    }

    //文法：MULTEXP -> UEXP * MULTEXP
    //             -> UEXP / MULTEXP
    //             -> UEXP % MULTEXP
    if(node->m_p_right_child != nullptr) {
        Value right;
        try {
            //解释右节点
            right = iMultiplicativeExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iUnaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：UEXP -> ++UEXP
    if(node->m_t.syn == syn_inc_op) {
        Value value;
        //解释节点
        try {
            value = iUnaryExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException &se) {
            throw se;
        }

        if(value.type != type_var) {
            /**************
             * 异常
             * 表达式不可赋值
             **************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(value);
            throw new ScriptException(ScriptException::C008, t, v);
        }
        Value *var = value._var;
        if(var->type == type_int) {
            var->_int++;
        } else if(var->type == type_float) {
            var->_float++;
        } else {
            /******************
             * 异常
             * 无法增加指定类型的值
             ******************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(*var);
            throw new ScriptException(ScriptException::C006, t, v);
        }
        return value;
    }
    //文法：UEXP -> --UEXP
    else if(node->m_t.syn == syn_dec_op) {
        Value value;
        //解释节点
        try {
            value = iUnaryExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException &se) {
            throw se;
        }
        if(value.type != type_var) {
            /**************
             * 异常
             * 表达式不可赋值
             **************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(value);
            throw new ScriptException(ScriptException::C008, t, v);
        }
        Value *var = value._var;
        if(var->type == type_int) {
            var->_int--;
        } else if(var->type == type_float) {
            var->_float--;
        } else {
            /******************
             * 异常
             * 无法减少指定类型的值
             ******************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(*var);
            throw new ScriptException(ScriptException::C007, t, v);
        }
        return value;
    }

    //文法：UEXP -> PEXP
    //解释节点
    try {
        return iPostfixExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }
}

Value ScriptInterpreter::iPostfixExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：PEXP	-> PRIMEXP
    try {
        left = iPrimaryExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException &se) {
        throw se;
    }

    if(left.type == type_id) {
        //文法：PEXP	-> PRIMEXP(EXP)
        if(node->m_t.syn == syn_front_bracket) {
            try {
                return iFunction(left, node->m_p_right_child, data, record, layer);
            } catch (ScriptException &se) {
                throw se;
            }
        }

        //文法：PEXP	-> PRIMEXP<id> | PRIMEXP[EXP]
        QString id = left._id;
        QVector<int> subscripte;
        bool isArray = false;
        if(node->m_p_right_child != nullptr) {
            isArray = true;
            try {
                subscripte = iVariableSubscripte(node->m_p_right_child, data, record, layer + 1);
            } catch (ScriptException &se) {
                throw se;
            }
        }
        try {
            if(isArray) {
                return getVariablePointer(id, subscripte, data);
            } else {
                return getVariablePointer(id, data);
            }
        } catch (ScriptException::EXCEPTION_TYPE &se) {
            if(se == ScriptException::C002) {
                /***********
                 * 异常
                 * 无效的运算
                 ***********/
                QVector<Value> v;
                v.append(left);
                QVector<TokenData> t;
                t.append(t);
                throw new ScriptException(ScriptException::C002, t, v);
            } else if(se == ScriptException::C011) {
                /***********
                 * 异常
                 * 数组索引超出数组结尾
                 ***********/
                QVector<Value> v;
                v.append(left);
                QVector<TokenData> t;
                t.append(t);
                throw new ScriptException(ScriptException::C011, t, v);
            } else if(se == ScriptException::C012) {
                /***********
                 * 异常
                 * 下标值不是数组
                 ***********/
                QVector<Value> v;
                v.append(left);
                QVector<TokenData> t;
                t.append(t);
                throw new ScriptException(ScriptException::C012, t, v);
            }
        }
    }
    return left;
}

Value ScriptInterpreter::iPrimaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：PRIMEXP	-> CONST
    SYN syn = node->m_p_left_child->m_t.syn;
    if(syn == syn_const_int || syn == syn_const_float || syn == syn_const_bool || syn == syn_const_char) {
        try {
            return iConstant(node->m_p_left_child);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> ID
    else if(syn == syn_id) {
        try {
            return iId(node->m_p_left_child);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> ID
    else if(node->m_t.syn == syn_str_lit) {
        try {
            return iStringLiteral(node->m_p_left_child);
        } catch (ScriptException &se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> (EXP)
    else {
        QVector<Value> v;
        try {
            v = iExpression(node->m_p_left_child, data, record, layer + 1);
        } catch (ScriptException &se) {
            throw se;
        }

        if(v.size() > 1) {
            /************
             * 异常
             * 参数数量错误
             ************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<int> i;
            i.append(1);
            i.append(v.size());
            throw new ScriptException(ScriptException::C001, t, i);
        }
        return v.first();
    }
}

Value ScriptInterpreter::iConstant(ScriptParserNode *node)
{
    Value v;
    TokenData t = node->m_t;
    //文法：CONST -> int
    if(node->m_p == parser_constant_int) {
        v.set(t.str.toInt());
    }
    //文法：CONST -> float
    else if(node->m_p == parser_constant_float) {
        v.set(t.str.toFloat());
    }
    //文法：CONST -> bool
    else if(node->m_p == parser_constant_bool) {
        v.set(t.str == "false" ? false : true);
    }
    //文法：CONST -> char
    else {
        v.set(t.str);
    }
    return v;
}

Value ScriptInterpreter::iStringLiteral(ScriptParserNode *node)
{
    Value v;
    TokenData t = node->m_t;
    //文法：STR -> string
    v.set(t.str);
    return v;
}

void ScriptInterpreter::insertVariable(QString id, Value rValue, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //判断类型
    if(rValue.type != type_empty) {
        if(type.syn == syn_int || type.syn == syn_bool) {
            if(rValue.type != type_int && rValue.type != type_bool) {
                throw ScriptException::C009;//异常：无法初始化变量
            }
        } else if(type.syn == syn_string) {
            if(rValue.type != type_str) {
                throw ScriptException::C009;//异常：无法初始化变量
            }
        } else if(type.syn == syn_float) {
            if(rValue.type != type_float) {
                throw ScriptException::C009;//异常：无法初始化变量
            }
        }
    } else {
        if(type.syn == syn_int) {
            rValue.type = type_int;
        } else if(type.syn == syn_float) {
            rValue.type = type_float;
        } else if(type.syn == syn_bool) {
            rValue.type = type_bool;
        } else if(type.syn == syn_string) {
            rValue.type = type_str;
        }
    }

    Variable *var = new Variable;
    var->init(id, &rValue);
    if(data->contains(id)) {
        QVector<QPair<int, Variable *> > *vector = data->value(id);
        if(vector->last().first == layer) {
            throw ScriptException::C010;//异常:重复成员
        } else {
            QPair<int, Variable *> v;
            v.first = layer;
            v.second = var;
            data->last()->append(v);
        }
    } else {
        QVector<QPair<int, Variable *> > *vector = new QVector<QPair<int, Variable *> >;
        QPair<int, Variable *> v;
        v.first = layer;
        v.second = var;
        vector->append(v);
        data->insert(id, vector);
    }

    QVector<QString> *subRecord = record->last();
    subRecord->append(id);
}

void ScriptInterpreter::insertVariable(QString id, QVector<int> size, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Variable *var = new Variable;

    if(type.syn == syn_int) {
        var->init(id, &size, type_int);
    } else if(type.syn == syn_float) {
        var->init(id, &size, type_float);
    } else if(type.syn == syn_bool) {
        var->init(id, &size, type_bool);
    } else if(type.syn == syn_string) {
        var->init(id, &size, type_str);
    }

    if(data->contains(id)) {
        QVector<QPair<int, Variable *> > *vector = data->value(id);
        if(vector->last().first == layer) {
            throw ScriptException::C010;//异常:重复成员
        } else {
            QPair<int, Variable *> v;
            v.first = layer;
            v.second = var;
            data->last()->append(v);
        }
    } else {
        QVector<QPair<int, Variable *> > *vector = new QVector<QPair<int, Variable *> >;
        QPair<int, Variable *> v;
        v.first = layer;
        v.second = var;
        vector->append(v);
        data->insert(id, vector);
    }

    QVector<QString> *subRecord = record->last();
    subRecord->append(id);
}

Value ScriptInterpreter::getVariable(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量
    Variable *v = data->value(id)->last().second;
    if(v->isArray() == true) {
        throw ScriptException::C002;//异常：无效的运算
    }
    return v->getValue();
}

Value ScriptInterpreter::getVariable(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量
    Variable *var = data->value(id)->last().second;

    //判断是否非数组
    if(var->isArray() == false) {
        throw ScriptException::C012;//异常：下标值不是数组
    }
    //判断下标是否越界
    if(subscripte.size() >= var->getSize().size()) {
        throw QPair<int, ScriptException::EXCEPTION_TYPE>(var->getSize().size(), ScriptException::C011);//异常：数组下标超出数组结尾
    }

    return var->getValue(&subscripte);
}

Value ScriptInterpreter::getVariablePointer(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量指针
    Variable *var = data->value(id)->last().second;

    if(var->isArray()) {
        throw ScriptException::C002;//异常：无效的运算
    }

    return var->getValuePointer();
}

Value ScriptInterpreter::getVariablePointer(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量
    Variable *var = data->value(id)->last().second;

    //判断是否非数组
    if(var->isArray() == false) {
        throw ScriptException::C012;//异常：下标值不是数组
    }
    //判断下标是否越界
    if(subscripte.size() >= var->getSize().size()) {
        throw QPair<int, ScriptException::EXCEPTION_TYPE>(var->getSize().size(), ScriptException::C011);//异常：数组下标超出数组结尾
    }

    return var->getValuePointer(&subscripte);
}

void ScriptInterpreter::clearVariable(QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    while(record->size() >= layer) {
        QVector<QString> *subRecord = record->last();
        for(int i = 0; i < subRecord->size(); i++) {
            QString id = subRecord->at(i);
            if(data->contains(id)) {
                QVector<QPair<int, Variable *> > *vector = data->value(id);
                if(!vector->isEmpty()) {
                    while(vector->last().first > layer) {
                        vector->pop_back();
                    }
                    if(!vector->isEmpty()) {
                        if(vector->last().first == layer) {
                            vector->pop_back();
                        }
                    }
                    if(vector->isEmpty()) {
                        delete vector;
                        data->remove(id);
                    }
                }
            }
        }
        subRecord->clear();
        delete subRecord;
        record->pop_back();
    }
}

Value ScriptInterpreter::operation(Value left, Value right, TokenData t)
{
    Value v;
    try {
        v = _operation(left, right, t);
    } catch (ScriptException::EXCEPTION_TYPE &e) {
        if(e == ScriptException::C002) {
            /***********
             * 异常
             * 无效的运算
             ***********/
            QVector<Value> v;
            v.append(left);
            v.append(right);
            QVector<TokenData> t;
            t.append(t);
            throw new ScriptException(ScriptException::C002, t, v);
        }
    }
    return v;
}

Value ScriptInterpreter::_operation(Value left, Value right, TokenData t)
{
    Value value;

    //赋值
    if(t.syn == syn_equal) {
        //赋值->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(right._str);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //赋值->整型
        else if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(static_cast<int>(right._int));
            } else if(right.type == type_float) {
                value.set(static_cast<int>(right._float));
            } else if(right.type == type_bool) {
                value.set(static_cast<int>(right._bool));
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //赋值->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(static_cast<float>(right._int));
            } else if(right.type == type_float) {
                value.set(static_cast<float>(right._float));
            } else if(right.type == type_bool) {
                value.set(static_cast<float>(right._bool));
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //赋值->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(static_cast<bool>(right._int));
            } else if(right.type == type_float) {
                value.set(static_cast<bool>(right._float));
            } else if(right.type == type_bool) {
                value.set(static_cast<bool>(right._bool));
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //乘法
    else if(t.syn == syn_mul || t.syn == syn_mul_assign) {
        //乘法->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int * right._int);
            } else if(right.type == type_float) {
                value.set(left._int * right._float);
            } else if(right.type == type_bool) {
                value.set(left._int * right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //乘法->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float * right._int);
            } else if(right.type == type_float) {
                value.set(left._float * right._float);
            } else if(right.type == type_bool) {
                value.set(left._float * right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //乘法->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool * right._int);
            } else if(right.type == type_float) {
                value.set(left._bool * right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool * right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //除法
    else if(t.syn == syn_div || t.syn == syn_div_assign) {
        //除法->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int / right._int);
            } else if(right.type == type_float) {
                value.set(left._int / right._float);
            } else if(right.type == type_bool) {
                value.set(left._int / right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //除法->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float / right._int);
            } else if(right.type == type_float) {
                value.set(left._float / right._float);
            } else if(right.type == type_bool) {
                value.set(left._float / right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //除法->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool / right._int);
            } else if(right.type == type_float) {
                value.set(left._bool / right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool / right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //取模
    else if(t.syn == syn_mod || t.syn == syn_mod_assign) {
        //取模->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int % right._int);
            } else if(right.type == type_bool) {
                value.set(left._int % right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //取模->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool * right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool * right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //加法
    else if(t.syn == syn_add || t.syn == syn_add_assign) {
        //加法->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(left._str + right._str);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //加法->整型
        else if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int + right._int);
            } else if(right.type == type_float) {
                value.set(left._int + right._float);
            } else if(right.type == type_bool) {
                value.set(left._int + right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //加法->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float + right._int);
            } else if(right.type == type_float) {
                value.set(left._float + right._float);
            } else if(right.type == type_bool) {
                value.set(left._float + right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //加法->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool + right._int);
            } else if(right.type == type_float) {
                value.set(left._bool + right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool + right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //减法
    else if(t.syn == syn_sub || t.syn == syn_sub_assign) {
        //减法->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int - right._int);
            } else if(right.type == type_float) {
                value.set(left._int - right._float);
            } else if(right.type == type_bool) {
                value.set(left._int - right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //减法->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float - right._int);
            } else if(right.type == type_float) {
                value.set(left._float - right._float);
            } else if(right.type == type_bool) {
                value.set(left._float - right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //减法->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool - right._int);
            } else if(right.type == type_float) {
                value.set(left._bool - right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool - right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //退位
    else if(t.syn == syn_left_op || t.syn == syn_left_assign) {
        //退位->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int << right._int);
            } else if(right.type == type_bool) {
                value.set(left._int << right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //退位->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool << right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool << right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //进位
    else if(t.syn == syn_right_op || t.syn == syn_right_assign) {
        //进位->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int >> right._int);
            } else if(right.type == type_bool) {
                value.set(left._int >> right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //进位->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool >> right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool >> right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //小于
    else if(t.syn == syn_left) {
        //小于->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int < right._int);
            } else if(right.type == type_float) {
                value.set(left._int < right._float);
            } else if(right.type == type_bool) {
                value.set(left._int < right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float < right._int);
            } else if(right.type == type_float) {
                value.set(left._float < right._float);
            } else if(right.type == type_bool) {
                value.set(left._float < right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool < right._int);
            } else if(right.type == type_float) {
                value.set(left._bool < right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool < right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //大于
    else if(t.syn == syn_right) {
        //大于->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int > right._int);
            } else if(right.type == type_float) {
                value.set(left._int > right._float);
            } else if(right.type == type_bool) {
                value.set(left._int > right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float > right._int);
            } else if(right.type == type_float) {
                value.set(left._float > right._float);
            } else if(right.type == type_bool) {
                value.set(left._float > right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool > right._int);
            } else if(right.type == type_float) {
                value.set(left._bool > right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool > right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //小于等于
    else if(t.syn == syn_le_op) {
        //小于等于->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int <= right._int);
            } else if(right.type == type_float) {
                value.set(left._int <= right._float);
            } else if(right.type == type_bool) {
                value.set(left._int <= right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于等于->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float <= right._int);
            } else if(right.type == type_float) {
                value.set(left._float <= right._float);
            } else if(right.type == type_bool) {
                value.set(left._float <= right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于等于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool <= right._int);
            } else if(right.type == type_float) {
                value.set(left._bool <= right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool <= right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //大于等于
    else if(t.syn == syn_ge_op) {
        //大于等于->整型
        if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int >= right._int);
            } else if(right.type == type_float) {
                value.set(left._int >= right._float);
            } else if(right.type == type_bool) {
                value.set(left._int >= right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于等于->浮点型
        else if(left.type == type_float) {
            if(right.type == type_int) {
                value.set(left._float >= right._int);
            } else if(right.type == type_float) {
                value.set(left._float >= right._float);
            } else if(right.type == type_bool) {
                value.set(left._float >= right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于等于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool >= right._int);
            } else if(right.type == type_float) {
                value.set(left._bool >= right._float);
            } else if(right.type == type_bool) {
                value.set(left._bool >= right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //不等于
    else if(t.syn == syn_eq_op) {
        //等于->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(left._str == right._str);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //等于->整型
        else if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int == right._int);
            } else if(right.type == type_bool) {
                value.set(left._int == right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //等于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool == right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool == right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //不等于
    else if(t.syn == syn_eq_op) {
        //不等于->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(left._str != right._str);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //不等于->整型
        else if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int != right._int);
            } else if(right.type == type_bool) {
                value.set(left._int != right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //不等于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool != right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool != right._bool);
            } else {
                throw ScriptException::C002;//异常：无效的运算
            }
        } else {
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //按位与
    else if(t.syn == syn_bitand || t.syn == syn_and_assign) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) & (right.type == type_bool ? right._bool : right._int));
    }
    //按位异或
    else if(t.syn == syn_bitxor || t.syn == syn_xor_assign) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) ^ (right.type == type_bool ? right._bool : right._int));
    }
    //按位或
    else if(t.syn == syn_bitor || t.syn == syn_or_assign) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) | (right.type == type_bool ? right._bool : right._int));
    }
    //逻辑与
    else if(t.syn == syn_and_op) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) && (right.type == type_bool ? right._bool : right._int));
    }
    //逻辑或
    else if(t.syn == syn_or_op) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) || (right.type == type_bool ? right._bool : right._int));
    }

    //返回运算结果
    value._id = left._id;
    return value;
}

QVector<int> ScriptInterpreter::iVariableSubscripte(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<int> subscripte;
    //文法:VARSUB -> [EXP]
    QVector<Value> v;
    try {
        record->append(new QVector<QString>);
        v = iExpression(node->m_p_left_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
    }  catch (ScriptException &se) {
        throw se;
    }
    if(v.size() > 1) {
        /************
         * 异常
         * 参数数量错误
         ************/
        QVector<TokenData> t;
        t.append(node->m_t);
        QVector<int> i;
        i.append(1);
        i.append(v.size());
        throw new ScriptException(ScriptException::C001, t, i);
    }
    if(v.first().isInt()) {
        subscripte.append(v.first().toInt());
    } else {
        /******************
         * 异常
         * 数组大小不是整数类型
         ******************/
        QVector<TokenData> t;
        t.append(node->m_t);
        QVector<Value> v;
        v.append(v.first());
        throw new ScriptException(ScriptException::C003, t, v);
    }
    //文法:VARSUB -> [EXP] VARSUB
    if(node->m_p_right_child != nullptr) {
        try {
            subscripte.append(iVariableSubscripte(node->m_p_right_child, data, record, layer));
        }  catch (ScriptException &se) {
            throw se;
        }
    }
    return subscripte;
}
