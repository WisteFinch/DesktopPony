#include "scriptinterpreter.h"

ScriptInterpreter::ScriptInterpreter()
{

}

QVector<ValueData> ScriptInterpreter::run(ScriptParserNode *node)
{
    TokenData token;
    QVector<ValueData> value;
    //开始解释脚本
    QMap<QString, QVector<QPair<int, Variable *> > *> *data = new QMap<QString, QVector<QPair<int, Variable *> > *>;//局部变量数据
    QVector<QVector<QString> *> *record = new QVector<QVector<QString> *>;  //局部变量记录
    QVector<QString> *subRecord = new QVector<QString>;
    record->append(subRecord);
    try {
        //根节点为语句列表
        if(node->m_p == parser_sl) {
            token = iStatementList(node, data, record, 0);
            clearVariable(data, record, 0);
            if(token.syn == syn_continue) {
                /********************
                 * 异常
                 * 继续语句不在循环语句内
                 *******************/
                std::vector<TokenData> et;
                et.push_back(token);
                throw ScriptExceptionData(EXCEPTION_I015, et);
            } else if(token.syn == syn_break) {
                /********************
                 * 异常
                 * 中断语句不在循环语句内
                 *******************/
                std::vector<TokenData> et;
                et.push_back(token);
                throw ScriptExceptionData(EXCEPTION_I016, et);
            }
        }
        //根节点为表达式
        else if(node->m_p == parser_exp) {
            value.append(iExpression(node, data, record, 0).at(0));
        }
        //根节点为条件表达式
        else {
            value.append(iConditionalHeadExpression(node, data, record, 0));
        }
    } catch (ScriptExceptionData &se) {
        throw se;
    }
    //解释脚本结束
    delete data;
    delete record;
    //返回输出
    return value;
}

QVector<ValueData> ScriptInterpreter::run(QString str)
{
    //从脚本文本生成语法树
    ScriptParser *s = new ScriptParser();
    try {
        s->set(str);
        //解释脚本
        return run(s->m_p_root);
    } catch (ScriptExceptionData &se) {
        throw se;
    }
}

TokenData ScriptInterpreter::iStatementList(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    TokenData t;
    //获取子节点类型
    PARSER p = node->m_p_left_child->m_p;
    try {
        //子节点类型为语句
        if(p == parser_s) {
            t = iStatement(node->m_p_left_child, data, record, layer);
            if(node->m_p_right_child != nullptr && t.syn != syn_continue && t.syn != syn_break) {
                t = iStatementList(node->m_p_right_child, data, record, layer);
            }
        }
        //子节点类型为复合语句
        else if(p == parser_cs) {
            t = iCompoundStatement(node->m_p_left_child, data, record, layer);
            if(node->m_p_right_child != nullptr && t.syn != syn_continue && t.syn != syn_break) {
                t = iStatementList(node->m_p_right_child, data, record, layer);
            }
        }
    }  catch (ScriptExceptionData &se) {
        throw se;
    }

    return t;
}

TokenData ScriptInterpreter::iStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    TokenData t;
    //分析子节点类型
    PARSER p = node->m_p_left_child->m_p;
    try {
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
            t = iIf(node->m_p_left_child, data, record, layer);
        } else if(p == parser_js) {
            t = node->m_p_left_child->m_d;
        }
    }  catch (ScriptExceptionData &se) {
        throw se;
    }

    return t;
}

TokenData ScriptInterpreter::iCompoundStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    TokenData t;
    //添加新变量记录
    record->append(new QVector<QString>);
    //分析子节点类型
    PARSER p = node->m_p_left_child->m_p;
    try {
        //子节点类型为语句
        if(p == parser_s) {
            //解释子节点（变量层数加一）
            t = iStatement(node->m_p_left_child, data, record, layer + 1);
        }
        //子节点类型为语句列表
        else if(p == parser_sl) {
            //解释子节点（变量层数加一）
            t = iStatementList(node->m_p_left_child, data, record, layer + 1);
        }
    }  catch (ScriptExceptionData &se) {
        throw se;
    }
    //删除新变量记录
    clearVariable(data, record, layer + 1);

    return t;
}

ValueData ScriptInterpreter::iExpressionStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    if(node->m_p_left_child == nullptr) {
        ValueData v;
        return v;
    }
    QVector<ValueData> value;
    try {
        value = iExpression(node->m_p_left_child, data, record, layer);
    }  catch (ScriptExceptionData &se) {
        throw se;
    }
    return value.first();
}

ValueData ScriptInterpreter::iFunction(ValueData id, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<ValueData> value;
    //解释参数
    try {
        value = iExpression(node->m_p_right_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    /*
     * 未完成
     */
}

TokenData ScriptInterpreter::iIf(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<ValueData> value;
    TokenData t;

    //文法：IF -> if ( EXP ) IFBODY
    record->append(new QVector<QString>);
    try {
        value = iExpression(node->m_p_left_child, data, record, layer + 1);
    }  catch (ScriptExceptionData &se) {
        throw se;
    }
    clearVariable(data, record, layer + 1);
    if(value.size() != 1) {
        /************
         * 异常
         * 参数数量错误
         ************/
        std::vector<TokenData> et;
        et.push_back(node->m_t);
        std::vector<int> ei;
        ei.push_back(1);
        ei.push_back(value.size());
        throw ScriptExceptionData(EXCEPTION_I001, et, ei);
    } else if(value.first().type != type_bool && value.first().type != type_int) {
        /***********
         * 异常
         * 无效的运算
         ***********/
        std::vector<ValueData> ev;
        ev.push_back(value.first());
        std::vector<TokenData> et;
        et.push_back(t);
        throw ScriptExceptionData(EXCEPTION_I002, et, ev);
    }
    try {
        t = iIfBody(node->m_p_right_child, data, record, layer, value.first());
    }  catch (ScriptExceptionData &se) {
        throw se;
    }
    return t;
}

TokenData ScriptInterpreter::iIfBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, ValueData flag)
{
    TokenData t;
    try {
        //真
        //文法：IFBODY -> S
        if(flag._bool || flag._int) {
            t = iStatement(node->m_p_left_child, data, record, layer);
        }
        // 假
        else if(node->m_p_right_child != nullptr) {
            PARSER p = node->m_p_right_child->m_p;
            //文法：IFBODY -> S else IF
            if(p == parser_if) {
                t =  iIf(node->m_p_right_child, data, record, layer);
            }
            //文法：IFBODY -> S else S
            else {
                t =  iStatement(node->m_p_right_child, data, record, layer);
            }
        }
    } catch (ScriptExceptionData &se) {
        throw se;
    }
    return t;
}

void ScriptInterpreter::iWhile(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    bool flag;
    TokenData t;
    //文法：WHILE -> while ( EXP ) CS
    //初始判断
    record->append(new QVector<QString>);
    try {
        flag = iExpression(node->m_p_left_child, data, record, layer + 1).at(0)._bool;
    } catch (ScriptExceptionData &se) {
        throw se;
    }
    clearVariable(data, record, layer + 1);
    //循环
    while(flag) {
        try {
            //运行
            t = iStatement(node->m_p_right_child, data, record, layer);
            if(t.syn == syn_continue) {
                continue;
            } else if(t.syn == syn_break) {
                break;
            }
            //判断
            record->append(new QVector<QString>);
            flag = iExpression(node->m_p_left_child, data, record, layer + 1).at(0)._bool;
            clearVariable(data, record, layer + 1);
        } catch (SYN &syn) {
            if(syn == syn_break) {
                break;
            } else if(syn == syn_continue) {
                continue;
            }
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
}

void ScriptInterpreter::iDeclaration(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：DECLARATION -> DECLARATIONTYPESPEC INITDECLARATORLIST
    try {
        //获取声明类型
        TokenData type = iDeclarationTypeSpecifier(node->m_p_left_child);
        //解释右节点
        iInitDeclaratorList(type, node->m_p_right_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }
}

void ScriptInterpreter::iInitDeclaratorList(TokenData type, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    try {
        //文法：NITDECLARATORLIST -> INITDECLARATOR
        iInitDeclarator(type, node->m_p_left_child, data, record, layer);
        //文法：NITDECLARATORLIST -> INITDECLARATOR, INITDECLARATORLIST
        if(node->m_p_right_child != nullptr) {
            iInitDeclaratorList(type, node->m_p_right_child, data, record, layer);
        }
    } catch (ScriptExceptionData &se) {
        throw se;
    }
}

void ScriptInterpreter::iInitDeclarator(TokenData t, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：INITDECLARATOR-> DECLARATOR
    //                   -> DECLARATOR = INITIALIZER
    try {
        //插入变量并获得指针
        Variable *var = iDeclarator(t, node->m_p_left_child, data, record, layer);
        //存在右节点
        if(node->m_p_right_child != nullptr) {
            //初始化值
            std::vector<int> subscripte;
            iInitializer(var, QVector<int>(subscripte.begin(), subscripte.end()), node->m_p_right_child, data, record, layer);
        }
    } catch (ScriptExceptionData &se) {
        throw se;
    }
}

Variable *ScriptInterpreter::iDeclarator(TokenData type, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：DECLARATOR -> ID
    bool isArray = false;
    //获取标识符
    QString id = QString::fromStdString(iId(node->m_p_left_child)._id);
    QVector<int> size;
    //判断是否为数组
    if(node->m_p_right_child != nullptr) {
        //文法：DECLARATOR -> ID DCLSIZE
        isArray = true;
        //解释下标
        try {
            size = iDeclaratorSize(node->m_p_right_child, data, record, layer + 1);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }

    //插入变量
    Variable *var;
    try {
        if(isArray) {
            var = insertVariable(id, size, type, data, record, layer);
        } else {
            var = insertVariable(id, type, data, record, layer);
        }
    } catch (SCRIPT_EXCEPTION_TYPE &se) {
        if(se == EXCEPTION_I010) {
            /************
             * 异常
             * 重复成员
             ************/
            std::vector<TokenData> et;
            et.push_back(node->m_t);
            std::vector<std::string> es;
            es.push_back(id.toStdString());
            throw ScriptExceptionData(EXCEPTION_I010, et, es);
        }
    }

    return var;
}

QVector<int> ScriptInterpreter::iDeclaratorSize(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<int> size;
    //文法：DCLSIZE -> [CONST]
    ValueData v = iConstant(node->m_p_left_child);
    if(v.isInt()) {
        size.append(v.castInt());
    } else {
        /******************
         * 异常
         * 数组大小不是整数类型
         ******************/
        std::vector<TokenData> et;
        et.push_back(node->m_t);
        std::vector<ValueData> ev;
        ev.push_back(v);
        throw ScriptExceptionData(EXCEPTION_I003, et, ev);
    }
    //文法：DCLSIZE -> [CONST] DCLSIZE
    if(node->m_p_right_child != nullptr) {
        try {
            size.append(iDeclaratorSize(node->m_p_right_child, data, record, layer));
        }  catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return size;
}

void ScriptInterpreter::iInitializer(Variable *var, QVector<int> subscripte, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：INITIALIZER -> AEXP
    if(node->m_p_left_child->m_p == parser_ass_exp) {
        if(var->isArray() && subscripte.last() >= var->getSize().at(subscripte.size() - 1)) {
            /******************
             * 异常
             * 设定初始值时元素多余
             ******************/
            std::vector<TokenData> et;
            et.push_back(node->m_t);
            throw ScriptExceptionData(EXCEPTION_I014, et);
        }
        ValueData v, *pv;
        try {
            v = iAssignmentExpression(node->m_p_left_child, data, record, layer);
            if(var->isArray()) {
                pv = var->getElementPointer(std::vector<int>(subscripte.begin(), subscripte.end()));
            } else {
                pv = var->getElementPointer();
            }
            iInitializerInitElement(pv, v);
        }  catch (SCRIPT_EXCEPTION_TYPE &e) {
            if(e == EXCEPTION_I009) {
                /**************
                 * 异常
                 * 无法初始化变量
                 **************/
                std::vector<TokenData> et;
                et.push_back(node->m_t);
                std::vector<ValueData> ev;
                ev.push_back(*pv);
                ev.push_back(v);
                throw ScriptExceptionData(EXCEPTION_I009, et, ev);
            }
        }
    } else
        //文法：INITIALIZER -> { INITIALIZERLIST } 或 { INITIALIZERLIST , }
    {
        //添加新变量记录
        record->append(new QVector<QString>);
        subscripte.append(0);
        if(static_cast<uint>(subscripte.size()) > var->getSize().size()) {
            /******************
             * 异常
             * 设定初始值时元素多余
             ******************/
            std::vector<TokenData> et;
            et.push_back(node->m_t);
            throw ScriptExceptionData(EXCEPTION_I014, et);
        }
        try {
            iInitializerList(var, subscripte, node->m_p_left_child, data, record, layer + 1);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
        //删除新变量记录
        clearVariable(data, record, layer + 1);
    }
}

void ScriptInterpreter::iInitializerList(Variable *var, QVector<int> subscripte, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：INITIALIZERLSIT -> INITIALIZER
    if(node->m_p_right_child == nullptr) {
        try {
            iInitializer(var, subscripte, node->m_p_left_child, data, record, layer + 1);
        }  catch (ScriptExceptionData &se) {
            throw se;
        }
    } else
        //文法：INITIALIZER -> INITIALIZER , INITIALIZERLIST
    {
        try {
            iInitializer(var, subscripte, node->m_p_left_child, data, record, layer + 1);
            subscripte.last()++;
            iInitializerList(var, subscripte, node->m_p_right_child, data, record, layer + 1);
        }  catch (ScriptExceptionData &se) {
            throw se;
        }
    }
}

void ScriptInterpreter::iInitializerInitElement(ValueData *left, ValueData right)
{
    //字符串型
    if(left->type == type_str) {
        if(right.type == type_str) {
            left->set(right._str);
        } else {
            throw EXCEPTION_I009;//异常：无法初始化变量
        }
    }
    //整型
    else if(left->type == type_int) {
        if(right.type == type_int) {
            left->set(static_cast<int>(right._int));
        } else if(right.type == type_float) {
            left->set(static_cast<int>(right._float));
        } else if(right.type == type_bool) {
            left->set(static_cast<int>(right._bool));
        } else {
            throw EXCEPTION_I009;//异常：无法初始化变量
        }
    }
    //浮点型
    else if(left->type == type_float) {
        if(right.type == type_int) {
            right.set(static_cast<float>(right._int));
        } else if(right.type == type_float) {
            right.set(static_cast<float>(right._float));
        } else if(right.type == type_bool) {
            right.set(static_cast<float>(right._bool));
        } else {
            throw EXCEPTION_I009;//异常：无法初始化变量
        }
    }
    //布尔型
    else if(left->type == type_bool) {
        if(right.type == type_int) {
            right.set(static_cast<bool>(right._int));
        } else if(right.type == type_float) {
            right.set(static_cast<bool>(right._float));
        } else if(right.type == type_bool) {
            right.set(static_cast<bool>(right._bool));
        } else {
            throw EXCEPTION_I009;//异常：无法初始化变量
        }
    } else {
        throw EXCEPTION_I002;//异常：无效的运算
    }
}

TokenData ScriptInterpreter::iDeclarationTypeSpecifier(ScriptParserNode *node)
{
    return node->m_d;
}

ValueData ScriptInterpreter::iId(ScriptParserNode *node)
{
    ValueData v;
    v.type = type_id;
    v._id = node->m_d.str;
    return v;
}

QVector<ValueData> ScriptInterpreter::iExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<ValueData> value;

    //文法：EXP -> AEXP
    try {
        //解释左节点
        value.append(iAssignmentExpression(node->m_p_left_child, data, record, layer));
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：EXP -> AEXP, EXP
    if(node->m_p_right_child != nullptr) {
        try {
            //解释右节点
            value.append(iExpression(node->m_p_right_child, data, record, layer));
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    //返回运算结果
    return value;
}

ValueData ScriptInterpreter::iAssignmentExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：AEXP -> CHEXP
    if(node->m_p_left_child->m_p == parser_cond_head_exp) {
        try {
            //返回运算结果
            return iConditionalHeadExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
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
        ValueData left;
        ValueData right;
        try {
            //解释左节点
            left = iUnaryExpression(node->m_p_left_child, data, record, layer);
            //解释右节点
            right = iAssignmentExpression(node->m_p_right_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
            throw se;
        }

        if(left.type != type_var) {
            /*************
             * 异常
             * 表达式不可赋值
             *************/
            std::vector<ValueData> ev;
            ev.push_back(left);
            ev.push_back(right);
            std::vector<TokenData> et;
            et.push_back(node->m_t);
            throw ScriptExceptionData(EXCEPTION_I008, et, ev);
        }
//===========================存在问题=================================

        try {
            //运算
            *left._var = operation(*left._var, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
        //返回运算结果
        return *left._var;
    }
//==================================================================
}

ValueData ScriptInterpreter::iConditionalHeadExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：CHEXP	 -> LOREXP
    try {
        //解释左节点
        left = iLogicalOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：CHEXP -> LOREXP ? CBEXP
    if(node->m_p_right_child != nullptr) {
        if(left.type != type_bool && left.type != type_int) {
            /***********
             * 异常
             * 无效的运算
             ***********/
            std::vector<ValueData> ev;
            ev.push_back(left);
            std::vector<TokenData> et;
            et.push_back(node->m_t);
            throw ScriptExceptionData(EXCEPTION_I002, et, ev);
        }

        try {
            //返回运算结果
            return iConditionalBodyExpression(node->m_p_right_child, data, record, layer, left._bool || left._int);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iConditionalBodyExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer, bool flag)
{
    //文法：CBEXP	 -> AEXP : CHEXP
    //标示为真
    if(flag) {
        try {
            //返回左节点
            return iAssignmentExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    //标示为假
    else {
        try {
            //返回右节点
            return iConditionalHeadExpression(node->m_p_right_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
}

ValueData ScriptInterpreter::iLogicalOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：LOREXP -> LANDEXP
    try {
        //解释左节点
        left = iLogicalAndExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：LOREXP -> LANDEXP || LOREXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iLogicalOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iLogicalAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：LANDEXP -> IOREXP
    try {
        //解释左节点
        left = iInclusiveOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：LANDEXP -> IOREXP && LANDEXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iLogicalAndExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iInclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：LANDEXP -> IOREXP
    try {
        //解释左节点
        left = iExclusiveOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：LANDEXP -> IOREXP | IOREXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iInclusiveOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iExclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：EOREXP -> ANDEXP
    try {
        //解释左节点
        left = iAndExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：EOREXP -> ANDEXP ^ EOREXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iExclusiveOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：ANDEXP -> EQUALEXP
    try {
        //解释左节点
        left = iEqualityExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：ANDEXP -> EQUALEXP & ANDEXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iAndExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iEqualityExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：EQUALEXP -> RELAEXP
    try {
        //解释左节点
        left = iRelationalExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：EQUALEXP -> RELAEXP == EQUALEXP
    //              -> RELAEXP != EQUALEXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iEqualityExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iRelationalExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：RELAEXP -> SHIFTEXP
    try {
        //解释左节点
        left = iShiftExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：RELAEXP -> SHIFTEXP < RELAEXP
    //             -> SHIFTEXP > RELAEXP
    //             -> SHIFTEXP <= RELAEXP
    //             -> SHIFTEXP >= RELAEXP

    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iRelationalExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iShiftExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：SHIFTEXP -> ADDEXP
    try {
        //解释左节点
        left = iAdditiveExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：SHIFTEXP -> ADDEXP << SHIFTEXP
    //              -> ADDEXP >> SHIFTEXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iShiftExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iAdditiveExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：ADDEXP -> MULTEXP
    try {
        //解释左节点
        left = iMultiplicativeExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    //文法：ADDEXP -> MULTEXP + ADDEXP
    //            -> MULTEXP - ADDEXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iAdditiveExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iMultiplicativeExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    //文法：MULTEXP -> UEXP
    try {
        //解释左节点
        left = iUnaryExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }

    if(left.type == type_var) {
        left = *left._var;
    }

    //文法：MULTEXP -> UEXP * MULTEXP
    //             -> UEXP / MULTEXP
    //             -> UEXP % MULTEXP
    if(node->m_p_right_child != nullptr) {
        ValueData right;
        try {
            //解释右节点
            right = iMultiplicativeExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_d);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return left;
}

ValueData ScriptInterpreter::iUnaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：UEXP	-> ++UEXP
    //          -> --UEXP
    if(node->m_d.syn == syn_inc_op || node->m_d.syn == syn_dec_op) {
        ValueData var;
        //解释节点
        try {
            var = iUnaryExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
        try {
            *var._var = operation(*var._var, node->m_d);
        }  catch (ScriptExceptionData &se) {
            throw se;
        }
        return var;
    }
    //文法：UEXP -> PEXP
    //解释节点
    try {
        return iPostfixExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptExceptionData &se) {
        throw se;
    }
}

ValueData ScriptInterpreter::iPostfixExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    ValueData left;

    if(node->m_p_left_child->m_p == parser_prim_exp) {
        try {
            left = iPrimaryExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
            throw se;
        }

        if(left.type == type_id) {
            //文法：PEXP	-> PEXP(EXP)
            if(node->m_d.syn == syn_front_bracket) {
                try {
                    return iFunction(left, node->m_p_right_child, data, record, layer);
                } catch (ScriptExceptionData &se) {
                    throw se;
                }
            }

            //文法：PEXP	-> PEXP<id> | PEXP[EXP]
            QString id = QString::fromStdString(left._id);
            QVector<int> subscripte;
            bool isArray = false;
            if(node->m_d.syn == syn_front_array || node->m_d.syn == syn_left) {
                isArray = true;
                try {
                    subscripte = iVariableSubscripte(node->m_p_right_child, data, record, layer + 1);
                } catch (ScriptExceptionData &se) {
                    throw se;
                }
            }
            try {
                if(isArray) {
                    return getVariablePointer(id, subscripte, data);
                } else {
                    return getVariablePointer(id, data);
                }
            } catch (SCRIPT_EXCEPTION_TYPE &se) {
                if(se == EXCEPTION_I002) {
                    /***********
                     * 异常
                     * 无效的运算
                     ***********/
                    std::vector<ValueData> ev;
                    ev.push_back(left);
                    std::vector<TokenData> et;
                    et.push_back(node->m_t);
                    throw ScriptExceptionData(EXCEPTION_I002, et, ev);
                } else if(se == EXCEPTION_I011) {
                    /******************
                     * 异常
                     * 数组索引超出数组结尾
                     ******************/
                    std::vector<ValueData> ev;
                    ev.push_back(left);
                    std::vector<TokenData> et;
                    et.push_back(node->m_t);
                    throw ScriptExceptionData(EXCEPTION_I011, et, ev);
                } else if(se == EXCEPTION_I012) {
                    /**************
                     * 异常
                     * 下标值不是数组
                     **************/
                    std::vector<ValueData> ev;
                    ev.push_back(left);
                    std::vector<TokenData> et;
                    et.push_back(node->m_t);
                    throw ScriptExceptionData(EXCEPTION_I012, et, ev);
                }
            }
        }
        return left;
    } else {
        //文法：PEXP	-> PEXP++
        //          -> PEXP--
        try {
            left = iPostfixExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
        if(left.type != type_var) {
            if(node->m_d.syn == syn_inc_op) {
                /******************
                 * 异常
                 * 无法增加指定类型的值
                 ******************/
                std::vector<TokenData> et;
                et.push_back(node->m_t);
                std::vector<ValueData> ev;
                ev.push_back(left);
                throw ScriptExceptionData(EXCEPTION_I006, et, ev);
            } else {
                /******************
                 * 异常
                 * 无法减少指定类型的值
                 ******************/
                std::vector<TokenData> et;
                et.push_back(node->m_t);
                std::vector<ValueData> ev;
                ev.push_back(left);
                throw ScriptExceptionData(EXCEPTION_I007, et, ev);
            }
        }
        ValueData v = *left._var;
        try {
            *left._var = operation(*left._var, node->m_d);
        }  catch (ScriptExceptionData &se) {
            throw se;
        }
        left._var = nullptr;
        return v;
    }
}

ValueData ScriptInterpreter::iPrimaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：PRIMEXP	-> CONST
    SYN syn = node->m_p_left_child->m_d.syn;
    if(syn == syn_const_int || syn == syn_const_float || syn == syn_const_bool || syn == syn_const_char) {
        try {
            return iConstant(node->m_p_left_child);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> ID
    else if(syn == syn_id) {
        try {
            return iId(node->m_p_left_child);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> ID
    else if(syn == syn_str_lit) {
        try {
            return iStringLiteral(node->m_p_left_child);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> (EXP)
    else {
        QVector<ValueData> v;
        try {
            v = iExpression(node->m_p_left_child, data, record, layer + 1);
        } catch (ScriptExceptionData &se) {
            throw se;
        }

        if(v.size() > 1) {
            /************
             * 异常
             * 参数数量错误
             ************/
            std::vector<TokenData> et;
            et.push_back(node->m_t);
            std::vector<int> ei;
            ei.push_back(1);
            ei.push_back(v.size());
            throw ScriptExceptionData(EXCEPTION_I001, et, ei);
        }
        return v.first();
    }
}

ValueData ScriptInterpreter::iConstant(ScriptParserNode *node)
{
    ValueData v;
    TokenData t = node->m_d;
    //文法：CONST -> int
    if(node->m_p == parser_constant_int) {
        v.set(QString::fromStdString(t.str).toInt());
    }
    //文法：CONST -> float
    else if(node->m_p == parser_constant_float) {
        v.set(QString::fromStdString(t.str).toFloat());
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

ValueData ScriptInterpreter::iStringLiteral(ScriptParserNode *node)
{
    ValueData v;
    TokenData t = node->m_d;
    //文法：STR -> string
    v.set(t.str);
    return v;
}

Variable *ScriptInterpreter::insertVariable(QString id, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Variable *var = new Variable;
    //初始化变量
    if(type.syn == syn_int) {
        var->init(id, type_int);
    } else if(type.syn == syn_float) {
        var->init(id, type_float);
    } else if(type.syn == syn_bool) {
        var->init(id, type_bool);
    } else if(type.syn == syn_string) {
        var->init(id, type_str);
    }
    //插入变量
    if(data->contains(id)) {
        QVector<QPair<int, Variable *> > *vector = data->value(id);
        if(vector->last().first == layer) {
            throw EXCEPTION_I010;//异常:重复成员
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
    //记录变量
    QVector<QString> *subRecord = record->last();
    subRecord->append(id);

    return var;
}

Variable *ScriptInterpreter::insertVariable(QString id, QVector<int> size, TokenData type, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Variable *var = new Variable;
    //初始化变量
    if(type.syn == syn_int) {
        var->init(id, std::vector<int>(size.begin(), size.end()), type_int);
    } else if(type.syn == syn_float) {
        var->init(id, std::vector<int>(size.begin(), size.end()), type_float);
    } else if(type.syn == syn_bool) {
        var->init(id, std::vector<int>(size.begin(), size.end()), type_bool);
    } else if(type.syn == syn_string) {
        var->init(id, std::vector<int>(size.begin(), size.end()), type_str);
    }
    //插入变量
    if(data->contains(id)) {
        QVector<QPair<int, Variable *> > *vector = data->value(id);
        if(vector->last().first == layer) {
            throw EXCEPTION_I010;//异常:重复成员
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
    //记录变量
    QVector<QString> *subRecord = record->last();
    subRecord->append(id);

    return var;
}

ValueData ScriptInterpreter::getVariable(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量
    Variable *v = data->value(id)->last().second;
    if(v->isArray() == true) {
        throw EXCEPTION_I002;//异常：无效的运算
    }
    return v->getValue();
}

ValueData ScriptInterpreter::getVariable(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量
    Variable *var = data->value(id)->last().second;

    //判断是否非数组
    if(var->isArray() == false) {
        throw EXCEPTION_I012;//异常：下标值不是数组
    }
    //判断下标是否越界
    if(static_cast<uint>(subscripte.size()) >= var->getSize().size()) {
        throw QPair<int, SCRIPT_EXCEPTION_TYPE>(var->getSize().size(), EXCEPTION_I011);//异常：数组下标超出数组结尾
    }

    return var->getValue(std::vector<int>(subscripte.begin(), subscripte.end()));
}

ValueData ScriptInterpreter::getVariablePointer(QString id, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量指针
    Variable *var = data->value(id)->last().second;

    if(var->isArray()) {
        throw EXCEPTION_I002;//异常：无效的运算
    }

    return var->getValuePointer();
}

ValueData ScriptInterpreter::getVariablePointer(QString id, QVector<int> subscripte, QMap<QString, QVector<QPair<int, Variable *> > *> *data)
{
    //获取变量
    Variable *var = data->value(id)->last().second;

    //判断是否非数组
    if(var->isArray() == false) {
        throw EXCEPTION_I012;//异常：下标值不是数组
    }
    //判断下标是否越界
    if(static_cast<uint>(subscripte.size()) > var->getSize().size()) {
        throw QPair<int, SCRIPT_EXCEPTION_TYPE>(var->getSize().size(), EXCEPTION_I011);//异常：数组下标超出数组结尾
    }

    return var->getValuePointer(std::vector<int>(subscripte.begin(), subscripte.end()));
}

void ScriptInterpreter::clearVariable(QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    while(record->size() >= layer && !record->isEmpty()) {
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

ValueData ScriptInterpreter::operation(ValueData left, ValueData right, TokenData t)
{
    ValueData v;
    try {
        v = _operation(left, right, t);
    } catch (SCRIPT_EXCEPTION_TYPE &e) {
        if(e == EXCEPTION_I002) {
            /***********
             * 异常
             * 无效的运算
             ***********/
            std::vector<ValueData> ev;
            ev.push_back(left);
            ev.push_back(right);
            std::vector<TokenData> et;
            et.push_back(t);
            throw ScriptExceptionData(EXCEPTION_I002, et, ev);
        }
    }
    return v;
}

ValueData ScriptInterpreter::operation(ValueData var, TokenData t)
{
    //加
    if(t.syn == syn_inc_op) {
        //加->整型
        if(var.type == type_int) {
            var._int++;
        }
        //加->浮点型
        else if(var.type == type_float) {
            var._float++;
        } else {
            /******************
             * 异常
             * 无法增加指定类型的值
             ******************/
            std::vector<TokenData> et;
            et.push_back(t);
            std::vector<ValueData> ev;
            ev.push_back(var);
            throw ScriptExceptionData(EXCEPTION_I006, et, ev);
        }
    }
    //减
    else if(t.syn == syn_dec_op) {
        //减->整型
        if(var.type == type_int) {
            var._int--;
        }
        //减->浮点型
        else if(var.type == type_float) {
            var._float--;
        } else {
            /******************
             * 异常
             * 无法减少指定类型的值
             ******************/
            std::vector<TokenData> et;
            et.push_back(t);
            std::vector<ValueData> ev;
            ev.push_back(var);
            throw ScriptExceptionData(EXCEPTION_I007, et, ev);
        }
    }
    return var;
}

ValueData ScriptInterpreter::_operation(ValueData left, ValueData right, TokenData t)
{
    ValueData value;

    //赋值
    if(t.syn == syn_equal) {
        //赋值->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(right._str);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //取模->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool * right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool * right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
        }
    }
    //加法
    else if(t.syn == syn_add || t.syn == syn_add_assign) {
        //加法->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(left._str + right._str);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //退位->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool << right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool << right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //进位->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool >> right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool >> right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
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
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
        }
    }
    //不等于
    else if(t.syn == syn_eq_op) {
        //等于->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(left._str == right._str);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //等于->整型
        else if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int == right._int);
            } else if(right.type == type_bool) {
                value.set(left._int == right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //等于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool == right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool == right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
        }
    }
    //不等于
    else if(t.syn == syn_eq_op) {
        //不等于->字符串型
        if(left.type == type_str) {
            if(right.type == type_str) {
                value.set(left._str != right._str);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //不等于->整型
        else if(left.type == type_int) {
            if(right.type == type_int) {
                value.set(left._int != right._int);
            } else if(right.type == type_bool) {
                value.set(left._int != right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        }
        //不等于->布尔型
        else if(left.type == type_bool) {
            if(right.type == type_int) {
                value.set(left._bool != right._int);
            } else if(right.type == type_bool) {
                value.set(left._bool != right._bool);
            } else {
                throw EXCEPTION_I002;//异常：无效的运算
            }
        } else {
            throw EXCEPTION_I002;//异常：无效的运算
        }
    }
    //按位与
    else if(t.syn == syn_bitand || t.syn == syn_and_assign) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw EXCEPTION_I002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) & (right.type == type_bool ? right._bool : right._int));
    }
    //按位异或
    else if(t.syn == syn_bitxor || t.syn == syn_xor_assign) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw EXCEPTION_I002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) ^ (right.type == type_bool ? right._bool : right._int));
    }
    //按位或
    else if(t.syn == syn_bitor || t.syn == syn_or_assign) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw EXCEPTION_I002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) | (right.type == type_bool ? right._bool : right._int));
    }
    //逻辑与
    else if(t.syn == syn_and_op) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw EXCEPTION_I002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) && (right.type == type_bool ? right._bool : right._int));
    }
    //逻辑或
    else if(t.syn == syn_or_op) {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int)) {
            throw EXCEPTION_I002;//异常：无效的运算
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
    //文法：VARSUB -> [EXP]
    QVector<ValueData> v;
    try {
        record->append(new QVector<QString>);
        v = iExpression(node->m_p_left_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
    }  catch (ScriptExceptionData &se) {
        throw se;
    }
    if(v.size() > 1) {
        /************
         * 异常
         * 参数数量错误
         ************/
        std::vector<TokenData> et;
        et.push_back(node->m_t);
        std::vector<int> ei;
        ei.push_back(1);
        ei.push_back(v.size());
        throw ScriptExceptionData(EXCEPTION_I001, et, ei);
    }
    if(v.first().isInt()) {
        subscripte.append(v.first().castInt());
    } else {
        /******************
         * 异常
         * 数组下标不是整数类型
         ******************/
        std::vector<TokenData> et;
        et.push_back(node->m_t);
        std::vector<ValueData> ev;
        ev.push_back(v.first());
        throw ScriptExceptionData(EXCEPTION_I005, et, ev);
    }
    //文法：VARSUB -> [EXP] VARSUB
    if(node->m_p_right_child != nullptr) {
        try {
            subscripte.append(iVariableSubscripte(node->m_p_right_child, data, record, layer));
        }  catch (ScriptExceptionData &se) {
            throw se;
        }
    }
    return subscripte;
}
