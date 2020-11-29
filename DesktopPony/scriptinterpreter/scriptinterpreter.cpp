#include "scriptinterpreter.h"

ScriptInterpreter::ScriptInterpreter()
{

}

Value ScriptInterpreter::run(ScriptParserNode *node)
{
    QMap<QString, QVector<QPair<int, Value> >* > *data = new QMap<QString, QVector<QPair<int, Value>>*>;
    QVector<QVector<QString> *> *record = new QVector<QVector<QString> *>;
    try {
        if(node->m_p == parser_sl)
        {
            iStatementList(node, data, record, 0);
        }else if(node->m_p == parser_exp)
        {
            return iExpression(node, data, record, 0).first();
        }else if(node->m_p == parser_cond_head_exp)
        {
            return iConditionalHeadExpression(node, data, record, 0);
        }
    } catch (ScriptException& se) {
        ExceptionHandle(se);
    }
    Value v;
    v.type = type_error;
    return v;
}

Value ScriptInterpreter::run(QString str)
{

    ScriptParser *s = new ScriptParser();
    try {
        s->set(str);
    } catch (ScriptException& se) {
        ExceptionHandle(se);
        Value v;
        v.type = type_error;
    }
    return run(s->m_p_root);
}

void ScriptInterpreter::iStatementList(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    PARSER p = node->m_p_left_child->m_p;
    if(p == parser_s)
    {
        iStatement(node->m_p_left_child, data, record, layer);
        if(node->m_p_right_child != nullptr)
            iStatementList(node->m_p_right_child, data, record, layer);
    }else if(p == parser_cs)
    {
        iCompoundStatement(node->m_p_left_child, data, record, layer);
        if(node->m_p_right_child != nullptr)
            iStatementList(node->m_p_right_child, data, record, layer);
    }
}

void ScriptInterpreter::iStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    PARSER p = node->m_p_left_child->m_p;
    if(p == parser_declare)
    {
        iDeclare(node->m_p_left_child, data, record, layer);
    }else if(p == parser_while)
    {
        iWhile(node->m_p_left_child, data, record, layer);
    }else if(p == parser_if)
    {
        iIf(node->m_p_left_child, data, record, layer);
    }
}

void ScriptInterpreter::iCompoundStatement(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    PARSER p = node->m_p_left_child->m_p;
    if(p == parser_s)
    {
        record->append(new QVector<QString>);
        iStatement(node->m_p_left_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
    }else if(p == parser_sl)
    {
        record->append(new QVector<QString>);
        iStatementList(node->m_p_left_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
    }
}

void ScriptInterpreter::iJumpStatement(ScriptParserNode *node)
{
    /*******
     * 未完成
     *******/
}

Value ScriptInterpreter::iFunction(Value id, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    record->append(new QVector<QString>);
    QVector<Value> value = iExpression(node->m_p_right_child, data, record, layer + 1);
    clearVariable(data, record, layer + 1);
    /*
     * 未完成
     */
}

void ScriptInterpreter::iIf(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    record->append(new QVector<QString>);
    QVector<Value> value = iExpression(node->m_p_left_child, data, record, layer + 1);
    clearVariable(data, record, layer + 1);
    if(value.size() != 1)
    {
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
    }else if(value.first().type != type_bool && value.first().type != type_int)
    {
        /*
         * 异常
         * 数据类型
         */
    }
    iIfBody(node->m_p_right_child, data, record, layer, value.first());
}

void ScriptInterpreter::iIfBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer, Value flag)
{
    if(flag._bool || flag._int)
    {
        iStatement(node->m_p_left_child, data, record, layer);
    }else if(node->m_p_right_child != nullptr)
    {
        PARSER p = node->m_p_right_child->m_p;
        if(p == parser_if)
        {
            iIf(node->m_p_right_child, data, record, layer);
        }else{
            iStatement(node->m_p_right_child, data, record, layer);
        }
    }
}

void ScriptInterpreter::iWhile(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    record->append(new QVector<QString>);
    bool flag = iExpression(node->m_p_left_child, data, record, layer + 1).first()._bool;
    clearVariable(data, record, layer + 1);
    while(flag)
    {
        iStatement(node->m_p_right_child, data, record, layer);

        record->append(new QVector<QString>);
        flag = iExpression(node->m_p_left_child, data, record, layer + 1).first()._bool;
        clearVariable(data, record, layer + 1);
    }
}

void ScriptInterpreter::iDeclare(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    TokenData type = iType(node->m_p_left_child);
    iDeclares(node->m_p_right_child, data, record, layer, type);
}

void ScriptInterpreter::iDeclares(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer, TokenData t)
{
    iDeclareBody(node->m_p_left_child, data, record, layer, t);
    if(node->m_p_right_child != nullptr)
    {
        iDeclares(node->m_p_right_child, data, record, layer, t);
    }
}

void ScriptInterpreter::iDeclareBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer, TokenData t)
{
    Value value;
    if(node->m_p_right_child != nullptr)
    {
        value = iExpression(node->m_p_right_child, data, record, layer).first();
    }
    iDeclareBasic(node->m_p_left_child, data, record, layer, t, value);
}

void ScriptInterpreter::iDeclareBasic(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer, TokenData t, Value v)
{
    QString id;
    id = iId(node->m_p_left_child)._id;
    if(node->m_p_right_child != nullptr)
    {
        record->append(new QVector<QString>);
        QVector<Value> e = iExpression(node->m_p_right_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
        if(e.size() > 1)
        {
            /************
             * 异常
             * 参数数量错误
             ************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<int> i;
            i.append(1);
            i.append(e.size());
            throw new ScriptException(ScriptException::C001, t, i);
        }
        if(e.first().type == type_int)
        {
            id.append('[' + QString::number(e.first()._int) + ']');
        }else
        {
            /***************
             * 异常
             * 数组下标不是整数
             ***************/
            QVector<TokenData> t;
            t.append(node->m_t);
            QVector<Value> v;
            v.append(e.first());
            throw new ScriptException(ScriptException::C005, t, v);
        }
    }
    if(t.syn == syn_int)
    {
        if(v.type == type_int || v.type == type_empty)
        {
            insertVariable(id, v, data, record, layer);
        }else
        {
            /*
            * 异常
            * 数据类型不匹配
            */
        }
    }else if(t.syn == syn_float)
    {
        if(v.type == type_float || v.type == type_empty)
        {
            insertVariable(id, v, data, record, layer);
        }else
        {
            /*
            * 异常
            * 数据类型不匹配
            */
        }
    }else if(t.syn == syn_bool)
    {
        if(v.type == type_bool || v.type == type_empty)
        {
            insertVariable(id, v, data, record, layer);
        }else
        {
            /*
            * 异常
            * 数据类型不匹配
            */
        }
    }else if(t.syn == syn_string)
    {
        if(v.type == type_str || v.type == type_empty)
        {
            insertVariable(id, v, data, record, layer);
        }else
        {
            /*
             * 异常
             * 数据类型不匹配
             */
        }
    }
}

TokenData ScriptInterpreter::iType(ScriptParserNode *node)
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

QVector<Value> ScriptInterpreter::iExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<Value> value;

    //文法：EXP -> AEXP
    try {
        //解释左节点
        value.append(iAssignmentExpression(node->m_p_left_child, data, record, layer));
    } catch (ScriptException&se) {
        throw se;
    }

    //文法：EXP -> AEXP, EXP
    if(node->m_p_right_child != nullptr)
    {
        try {
            //解释右节点
            value.append(iExpression(node->m_p_right_child, data, record, layer));
        } catch (ScriptException& se) {
            throw se;
        }
    }
    //返回运算结果
    return value;
}

Value ScriptInterpreter::iAssignmentExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：AEXP -> CHEXP
    if(node->m_p_left_child->m_p == parser_cond_head_exp)
    {
        try {
            //返回运算结果
            return iConditionalHeadExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    //文法：AEXP -> UEXP += AEXP
    //          -> UEXP -= AEXP
    //          -> UEXP *= AEXP
    //          -> UEXP /= AEXP
    //          -> UEXP %= AEXP
    //          -> UEXP &= AEXP
    //          -> UEXP ^= AEXP
    //          -> UEXP |= AEXP
    else{
        Value left;
        Value right;
        try {
            //解释左节点
            left = iUnaryExpression(node->m_p_left_child, data, record, layer);
            //解释右节点
            right = iAssignmentExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException& se) {
            throw se;
        }

        if(left.type != type_var)
        {
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
        try {
            //运算
            *left._var = operation(*left._var, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
        //返回运算结果
        return *left._var;
    }
}

Value ScriptInterpreter::iConditionalHeadExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：CHEXP	 -> LOREXP
    try {
        //解释左节点
        left = iLogicalOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：CHEXP -> LOREXP ? CBEXP
    if(node->m_p_right_child != nullptr)
    {
        if(left.type != type_bool && left.type != type_int)
        {
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
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iConditionalBodyExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer, bool flag)
{
    //文法：CBEXP	 -> AEXP : CHEXP
    //标示为真
    if(flag)
    {
        try {   
            //返回左节点
            return iAssignmentExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    //标示为假
    else{
        try {
            //返回右节点
            return iConditionalHeadExpression(node->m_p_right_child, data, record, layer);
        } catch (ScriptException& se) {
            throw se;
        }
    }
}

Value ScriptInterpreter::iLogicalOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：LOREXP -> LANDEXP
    try {
        //解释左节点
        left = iLogicalAndExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：LOREXP -> LANDEXP || LOREXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iLogicalOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iLogicalAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：LANDEXP -> IOREXP
    try {
        //解释左节点
        left = iInclusiveOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：LANDEXP -> IOREXP && LANDEXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iLogicalAndExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iInclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：LANDEXP -> IOREXP
    try {
        //解释左节点
        left = iExclusiveOrExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：LANDEXP -> IOREXP | IOREXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iInclusiveOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iExclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：EOREXP -> ANDEXP
    try {
        //解释左节点
        left = iAndExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：EOREXP -> ANDEXP ^ EOREXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iExclusiveOrExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：ANDEXP -> EQUALEXP
    try {
        //解释左节点
        left = iEqualityExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：ANDEXP -> EQUALEXP & ANDEXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iAndExpression(node->m_p_right_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iEqualityExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：EQUALEXP -> RELAEXP
    try {
        //解释左节点
        left = iRelationalExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：EQUALEXP -> RELAEXP == EQUALEXP
    //              -> RELAEXP != EQUALEXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iEqualityExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iRelationalExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：RELAEXP -> SHIFTEXP
    try {
        //解释左节点
        left = iShiftExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：RELAEXP -> SHIFTEXP
    //             -> SHIFTEXP < RELAEXP
    //             -> SHIFTEXP > RELAEXP
    //             -> SHIFTEXP <= RELAEXP
    //             -> SHIFTEXP >= RELAEXP

    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iRelationalExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iShiftExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法:SHIFTEXP -> ADDEXP
    try {
        //解释左节点
        left = iAdditiveExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法:SHIFTEXP -> ADDEXP << SHIFTEXP
    //             -> ADDEXP >> SHIFTEXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iShiftExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iAdditiveExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：ADDEXP -> MULTEXP
    try {
        //解释左节点
        left = iMultiplicativeExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    //文法：ADDEXP -> MULTEXP + ADDEXP
    //            -> MULTEXP - ADDEXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iAdditiveExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iMultiplicativeExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：MULTEXP -> UEXP
    try {
        //解释左节点
        left = iUnaryExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    if(left.type == type_var)
    {
        left = *left._var;
    }

    //文法：MULTEXP -> UEXP * MULTEXP
    //             -> UEXP / MULTEXP
    //             -> UEXP % MULTEXP
    if(node->m_p_right_child != nullptr)
    {
        Value right;
        try {
            //解释右节点
            right = iMultiplicativeExpression(node->m_p_left_child, data, record, layer);
            //返回运算结果
            return operation(left, right, node->m_t);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iUnaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    //文法：UEXP -> ++UEXP
    if(node->m_t.syn == syn_inc_op)
    {
        Value value;
        //解释节点
        try {
            value = iUnaryExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException& se) {
            throw se;
        }

        if(value.type != type_var)
        {
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
        if(var->type == type_int)
        {
            var->_int++;
        }else if(var->type == type_float)
        {
            var->_float++;
        }else
        {
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
    else if(node->m_t.syn == syn_dec_op)
    {
        Value value;
        //解释节点
        try {
            value = iUnaryExpression(node->m_p_left_child, data, record, layer);
        } catch (ScriptException& se) {
            throw se;
        }
        QString a;
        if(value.type != type_var)
        {
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
        if(var->type == type_int)
        {
            var->_int--;
        }else if(var->type == type_float)
        {
            var->_float--;
        }else
        {
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
    } catch (ScriptException& se) {
        throw se;
    }
}

Value ScriptInterpreter::iPostfixExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：PEXP	-> PRIMEXP
    try {
        left = iPrimaryExpression(node->m_p_left_child, data, record, layer);
    } catch (ScriptException& se) {
        throw se;
    }

    if(left.type == type_id)
    {
        //文法：PEXP	-> PRIMEXP(EXP)
        if(node->m_t.syn == syn_front_bracket)
        {
            try {
                return iFunction(left, node->m_p_right_child, data, record, layer);
            } catch (ScriptException& se) {
                throw se;
            }
        }

        //文法：PEXP	-> PRIMEXP<id> | PRIMEXP[EXP]
        QString id = left._id;
        if(node->m_p_right_child != nullptr)
        {
            QVector<Value> right;
            try {
                right = iExpression(node->m_p_right_child, data, record, layer + 1);
            } catch (ScriptException& se) {
                throw se;
            }

            if(right.size() > 1)
            {
                /************
                 * 异常
                 * 参数数量错误
                 ************/
                QVector<TokenData> t;
                t.append(node->m_t);
                QVector<int> i;
                i.append(1);
                i.append(right.size());
                throw new ScriptException(ScriptException::C001, t, i);
            }
            if(right.first().type == type_int)
            {
                id.append('[' + QString::number(right.first()._int) + ']');
            }else
            {
                /***************
                 * 异常
                 * 数组下标不是整数
                 ***************/
                QVector<TokenData> t;
                t.append(node->m_t);
                QVector<Value> v;
                v.append(right.first());
                throw new ScriptException(ScriptException::C005, t, v);
            }
        }
        try {
            return getVariablePointer(id, data);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    return left;
}

Value ScriptInterpreter::iPrimaryExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left;

    //文法：PRIMEXP	-> CONST
    if(node->m_t.syn == syn_const_int || node->m_t.syn == syn_const_float || node->m_t.syn == syn_const_bool || node->m_t.syn == syn_const_char)
    {
        try {
            return iConstant(node->m_p_left_child);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> ID
    else if(node->m_t.syn == syn_id)
    {
        try {
            return iId(node->m_p_left_child);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> ID
    else if(node->m_t.syn == syn_str_lit)
    {
        try {
            return iStringLiteral(node->m_p_left_child);
        } catch (ScriptException& se) {
            throw se;
        }
    }
    //文法：PRIMEXP -> (EXP)
    else
    {
        QVector<Value> v;
        try {
            v = iExpression(node->m_p_left_child, data, record, layer + 1);
        } catch (ScriptException& se) {
            throw se;
        }

        if(v.size() > 1)
        {
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
    if(node->m_p == parser_constant_int)
    {
        v.set(t.str.toInt());
    }
    //文法：CONST -> float
    else if(node->m_p == parser_constant_float)
    {
        v.set(t.str.toFloat());
    }
    //文法：CONST -> bool
    else if(node->m_p == parser_constant_bool)
    {
        v.set(t.str == "false" ? false : true);
    }
    //文法：CONST -> char
    else
    {
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

void ScriptInterpreter::insertVariable(QString id, Value v, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    if(data->contains(id))
    {
        QVector<QPair<int, Value> >* vector = data->value(id);
        if(vector->last().first == layer)
        {
            /*
             * 异常
             * 重复定义
             */

        }
        vector->append(QPair<int, Value>(layer, v));
    }else{
        QVector<QPair<int, Value> >* vector = new QVector<QPair<int, Value> >;
        vector->append(QPair<int, Value>(layer, v));
    }

    QVector<QString> *subRecord = record->last();
    subRecord->append(id);
}

Value ScriptInterpreter::getVariable(QString id, VALUE_TYPE t, QMap<QString, QVector<QPair<int, Value> > *> *data)
{
    Value v = data->value(id)->last().second;
    if(v.type != t)
    {
        /*
         * 异常
         * 数据类型不匹配
         */
    }
}

Value ScriptInterpreter::getVariablePointer(QString id, QMap<QString, QVector<QPair<int, Value> > *> *data)
{
    Value v = data->value(id)->last().second;

}

void ScriptInterpreter::clearVariable(QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    while(record->size() >= layer)
    {
        QVector<QString> *subRecord = record->last();
        for(int i = 0; i < subRecord->size(); i++)
        {
            QString id = subRecord->at(i);
            if(data->contains(id))
            {
                QVector<QPair<int, Value> > *vector = data->value(id);
                if(!vector->isEmpty())
                {
                    while(vector->last().first > layer)
                    {
                        vector->pop_back();
                    }
                    if(!vector->isEmpty())
                    {
                        if(vector->last().first == layer)
                            vector->pop_back();
                    }
                    if(vector->isEmpty())
                    {
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
    try {
        _operation(left, right, t);
    } catch (ScriptException::EXCEPTION_TYPE &e) {
        if(e == ScriptException::C002)
        {
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
}

Value ScriptInterpreter::_operation(Value left, Value right, TokenData t)
{
    Value value;

    //乘法
    if(t.syn == syn_mul || t.syn == syn_mul_assign)
    {
        //乘法->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int * right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int * right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int * right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //乘法->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float * right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float * right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float * right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //乘法->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool * right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool * right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool * right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //除法
    else if(t.syn == syn_div || t.syn == syn_div_assign)
    {
        //除法->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int / right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int / right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int / right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //除法->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float / right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float / right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float / right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //除法->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool / right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool / right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool / right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //取模
    else if(t.syn == syn_mod || t.syn == syn_mod_assign)
    {
        //取模->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int % right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._int % right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //取模->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool * right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._bool * right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //加法
    else if(t.syn == syn_add || t.syn == syn_add_assign)
    {
        //加法->字符串型
        if(left.type == type_str)
        {
            if(right.type == type_str)
            {
                value.set(left._str + right._str);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //加法->整型
        else if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int + right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int + right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int + right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //加法->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float + right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float + right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float + right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //加法->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool + right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool + right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool + right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //减法
    else if(t.syn == syn_sub || t.syn == syn_sub_assign)
    {
        //减法->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int - right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int - right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int - right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //减法->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float - right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float - right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float - right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //减法->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool - right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool - right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool - right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //退位
    else if(t.syn == syn_left_op || t.syn == syn_left_assign){
        //退位->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int << right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._int << right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //退位->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool << right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._bool << right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //进位
    else if(t.syn == syn_right_op || t.syn == syn_right_assign){
        //进位->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int >> right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._int >> right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //进位->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool >> right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._bool >> right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //小于
    else if(t.syn == syn_left)
    {
        //小于->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int < right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int < right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int < right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float < right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float < right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float < right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool < right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool < right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool < right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //大于
    else if(t.syn == syn_right)
    {
        //大于->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int > right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int > right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int > right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float > right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float > right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float > right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool > right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool > right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool > right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //小于等于
    else if(t.syn == syn_le_op)
    {
        //小于等于->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int <= right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int <= right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int <= right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于等于->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float <= right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float <= right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float <= right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //小于等于->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool <= right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool <= right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool <= right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //大于等于
    else if(t.syn == syn_ge_op)
    {
        //大于等于->整型
        if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int >= right._int);
            }else if(right.type == type_float)
            {
                value.set(left._int >= right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._int >= right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于等于->浮点型
        else if(left.type == type_float)
        {
            if(right.type == type_int)
            {
                value.set(left._float >= right._int);
            }else if(right.type == type_float)
            {
                value.set(left._float >= right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._float >= right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //大于等于->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool >= right._int);
            }else if(right.type == type_float)
            {
                value.set(left._bool >= right._float);
            }else if(right.type == type_bool)
            {
                value.set(left._bool >= right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //不等于
    else if(t.syn == syn_eq_op)
    {
        //等于->字符串型
        if(left.type == type_str)
        {
            if(right.type == type_str)
            {
                value.set(left._str == right._str);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //等于->整型
        else if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int == right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._int == right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //等于->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool == right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._bool == right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //不等于
    else if(t.syn == syn_eq_op)
    {
        //不等于->字符串型
        if(left.type == type_str)
        {
            if(right.type == type_str)
            {
                value.set(left._str != right._str);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //不等于->整型
        else if(left.type == type_int)
        {
            if(right.type == type_int)
            {
                value.set(left._int != right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._int != right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }
        //不等于->布尔型
        else if(left.type == type_bool)
        {
            if(right.type == type_int)
            {
                value.set(left._bool != right._int);
            }else if(right.type == type_bool)
            {
                value.set(left._bool != right._bool);
            }else{
                throw ScriptException::C002;//异常：无效的运算
            }
        }else{
            throw ScriptException::C002;//异常：无效的运算
        }
    }
    //按位与
    else if(t.syn == syn_bitand || t.syn == syn_and_assign)
    {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int))
        {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) & (right.type == type_bool ? right._bool : right._int));
    }
    //按位异或
    else if(t.syn == syn_bitxor || t.syn == syn_xor_assign)
    {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int))
        {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) ^ (right.type == type_bool ? right._bool : right._int));
    }
    //按位或
    else if(t.syn == syn_bitor || t.syn == syn_or_assign)
    {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int))
        {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) | (right.type == type_bool ? right._bool : right._int));
    }
    //逻辑与
    else if(t.syn == syn_and_op)
    {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int))
        {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) && (right.type == type_bool ? right._bool : right._int));
    }
    //逻辑或
    else if(t.syn == syn_or_op)
    {
        if((left.type != type_bool && left.type != type_int) || (right.type != type_bool && right.type != type_int))
        {
            throw ScriptException::C002;//异常：无效的运算
        }
        value.set((left.type == type_bool ? left._bool : left._int) || (right.type == type_bool ? right._bool : right._int));
    }

    //返回运算结果
    return value;
}
