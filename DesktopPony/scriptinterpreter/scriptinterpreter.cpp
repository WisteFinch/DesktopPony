#include "scriptinterpreter.h"

ScriptInterpreter::ScriptInterpreter()
{

}

Value ScriptInterpreter::run(ScriptParserNode *node)
{
    QMap<QString, QVector<QPair<int, Value> >* > *data = new QMap<QString, QVector<QPair<int, Value>>*>;
    QVector<QVector<QString> *> *record = new QVector<QVector<QString> *>;
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
    return Value();
}

Value ScriptInterpreter::run(QString str)
{

    ScriptParser *s = new ScriptParser();
    s->set(str);
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
    }else if(p == parser_function)
    {
        iFunction(node->m_p_left_child, data, record, layer);
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

Value ScriptInterpreter::iFunction(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    TokenData id = iId(node->m_p_left_child);
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
    if(value.size() > 1)
    {
        /*
         * 错误
         * 过多参数
         */
    }else if(value.first().type != type_bool)
    {
        /*
         * 错误
         * 数据类型不匹配(期望bool)
        */
    }
    iIfBody(node->m_p_right_child, data, record, layer, value.first()._bool);
}

void ScriptInterpreter::iIfBody(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer, bool flag)
{
    if(flag)
    {
        iCompoundStatement(node->m_p_left_child, data, record, layer);
    }else if(node->m_p_right_child != nullptr)
    {
        PARSER p = node->m_p_right_child->m_p;
        if(p == parser_if)
        {
            iIf(node->m_p_right_child, data, record, layer);
        }else{
            iCompoundStatement(node->m_p_right_child, data, record, layer);
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
        iCompoundStatement(node->m_p_right_child, data, record, layer);

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
    TokenData idt = iId(node->m_p_left_child);
    id = idt.str;
    if(node->m_p_right_child != nullptr)
    {
        record->append(new QVector<QString>);
        QVector<Value> e = iExpression(node->m_p_right_child, data, record, layer + 1);
        clearVariable(data, record, layer + 1);
        if(e.size() > 1)
        {
            /*
             * 错误
             * 过多参数
             */
        }
        if(e.first().type == type_int)
        {
            id.append('[' + QString::number(e.first()._int) + ']');
        }else
        {
            /*
             * 错误
             * 数据类型不匹配(期望int)
            */
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
            * 错误
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
            * 错误
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
            * 错误
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
             * 错误
             * 数据类型不匹配
             */
        }
    }
}

TokenData ScriptInterpreter::iType(ScriptParserNode *node)
{
    return node->m_t;
}

TokenData ScriptInterpreter::iId(ScriptParserNode *node)
{
    return node->m_t;
}

QVector<Value> ScriptInterpreter::iExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    QVector<Value> value;
    value.append(iAssignmentExpression(node->m_p_left_child, data, record, layer));
    if(node->m_p_right_child != nullptr)
    {
        value.append(iExpression(node->m_p_right_child, data, record, layer));
    }
    return value;
}

Value ScriptInterpreter::iAssignmentExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    if(node->m_p_left_child->m_p == parser_cond_head_exp)
    {
        return iConditionalHeadExpression(node->m_p_left_child, data, record, layer);
    }
    /*
     * 赋值运算
     */
}

Value ScriptInterpreter::iConditionalHeadExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value value = iLogicalOrExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        if(value.type != type_bool)
        {
            /*
             * 错误
             * 数据类型不匹配（期望bool）
             */
        }
        return iConditionalBodyExpression(value._bool, node->m_p_left_child, data, record, layer);
    }
    return value;
}

Value ScriptInterpreter::iConditionalBodyExpression(bool flag, ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    if(flag)
    {
        QVector<Value> value = iExpression(node->m_p_left_child, data, record, layer);
        if(value.size() > 1)
        {
            /*
             * 错误
             * 过多参数
             */
        }
        return value.first();
    }else
    {
        return iConditionalHeadExpression(node->m_p_right_child, data, record, layer);
    }
}

Value ScriptInterpreter::iLogicalOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iLogicalAndExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        if(left.type != type_bool)
        {
            /*
             * 错误
             * 数据类型不匹配（期望bool）
             */
        }
        Value right = iLogicalOrExpression(node->m_p_right_child, data, record, layer);
        if(right.type != type_bool)
        {
            /*
             * 错误
             * 数据类型不匹配（期望bool）
             */
        }
        Value value;
        value.set(left._bool || right._bool);
        return value;
    }
    return left;
}

Value ScriptInterpreter::iLogicalAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iInclusiveOrExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        if(left.type != type_bool && left.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（期望int,bool）
             */
        }
        Value right = iLogicalAndExpression(node->m_p_right_child, data, record, layer);
        if(right.type != type_bool && right.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（期望int,bool）
             */
        }
        Value value;
        value.set((left.type == type_bool ? left._bool : left._int) && (right.type == type_bool ? right._bool : right._int));
        return value;
    }
    return left;
}

Value ScriptInterpreter::iInclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iExclusiveOrExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        if(left.type != type_bool && left.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（期望int,bool）
             */
        }
        Value right = iInclusiveOrExpression(node->m_p_right_child, data, record, layer);
        if(right.type != type_bool && right.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（期望int,bool）
             */
        }
        Value value;
        value.set((left.type == type_bool ? left._bool : left._int) | (right.type == type_bool ? right._bool : right._int));
        return value;
    }
    return left;
}

Value ScriptInterpreter::iExclusiveOrExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iAndExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        if(left.type != type_bool && left.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（int,bool）
             */
        }
        Value right = iExclusiveOrExpression(node->m_p_right_child, data, record, layer);
        if(right.type != type_bool && right.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（int,bool）
             */
        }
        Value value;
        value.set((left.type == type_bool ? left._bool : left._int) ^ (right.type == type_bool ? right._bool : right._int));
        return value;
    }
    return left;
}

Value ScriptInterpreter::iAndExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iEqualityExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        if(left.type != type_bool && left.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（int,bool）
             */
        }
        Value right = iAndExpression(node->m_p_right_child, data, record, layer);
        if(right.type != type_bool && right.type != type_int)
        {
            /*
             * 错误
             * 数据类型不匹配（bool,int）
             */
        }
        Value value;
        value.set((left.type == type_bool ? left._bool : left._int) & (right.type == type_bool ? right._bool : right._int));
        return value;
    }
    return left;
}

Value ScriptInterpreter::iEqualityExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iRelationalExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        Value value;
        Value right = iEqualityExpression(node->m_p_left_child, data, record, layer);
        if(node->m_t.syn == syn_eq_op)
        {
            if(left.type == type_str || left.type == type_null)
            {
                if(right.type == type_str)
                {
                    value.set(left._str == right._str);
                }else if(right.type == type_null)
                {
                    value.set(left._str == nullptr);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
                return value;
            }else if(left.type == type_int)
            {
                if(right.type == type_int)
                {
                    value.set(left._int == right._int);
                }else if(right.type == type_bool)
                {
                    value.set(left._int == right._bool);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
            {
                if(right.type == type_int)
                {
                    value.set(left._bool == right._int);
                }else if(right.type == type_bool)
                {
                    value.set(left._bool == right._bool);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else{
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }else
        {
            if(left.type == type_str || left.type == type_null)
            {
                if(right.type == type_str)
                {
                    value.set(left._str != right._str);
                }else if(right.type == type_null)
                {
                    value.set(left._str != nullptr);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
                return value;
            }else if(left.type == type_int)
            {
                if(right.type == type_int)
                {
                    value.set(left._int != right._int);
                }else if(right.type == type_bool)
                {
                    value.set(left._int != right._bool);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
            {
                if(right.type == type_int)
                {
                    value.set(left._bool != right._int);
                }else if(right.type == type_bool)
                {
                    value.set(left._bool != right._bool);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else{
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }
    }
    return left;
}

Value ScriptInterpreter::iRelationalExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iShiftExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        Value value;
        Value right = iEqualityExpression(node->m_p_left_child, data, record, layer);
        if(node->m_t.syn == syn_left)
        {
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_float)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else
            {
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }else if(node->m_t.syn == syn_right)
        {
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_float)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else
            {
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }else if(node->m_t.syn == syn_le_op)
        {
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_float)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else
            {
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }else
        {
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_float)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else
            {
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }
    }
    return left;
}

Value ScriptInterpreter::iShiftExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iAdditiveExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        Value value;
        Value right = iShiftExpression(node->m_p_left_child, data, record, layer);
        if(node->m_t.syn == syn_left_op){
            if(left.type == type_int)
            {
                if(right.type == type_int)
                {
                    value.set(left._int << right._int);
                }else if(right.type == type_bool)
                {
                    value.set(left._int << right._bool);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
            {
                if(right.type == type_int)
                {
                    value.set(left._bool << right._int);
                }else if(right.type == type_bool)
                {
                    value.set(left._bool << right._bool);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else
            {
                /*
                 * 错误
                 * 数据类型不匹配
                 */
            }
            return value;
        }else{
            Value value;
            Value right = iShiftExpression(node->m_p_left_child, data, record, layer);
            if(node->m_t.syn == syn_left_op){
                if(left.type == type_int)
                {
                    if(right.type == type_int)
                    {
                        value.set(left._int >> right._int);
                    }else if(right.type == type_bool)
                    {
                        value.set(left._int >> right._bool);
                    }else{
                        /*
                         * 错误
                         * 数据类型不匹配
                         */
                    }
                }else if(left.type == type_bool)
                {
                    if(right.type == type_int)
                    {
                        value.set(left._bool >> right._int);
                    }else if(right.type == type_bool)
                    {
                        value.set(left._bool >> right._bool);
                    }else{
                        /*
                         * 错误
                         * 数据类型不匹配
                         */
                    }
                }else
                {
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
                return value;
            }
        }
    }
    return left;
}

Value ScriptInterpreter::iAdditiveExpression(ScriptParserNode *node, QMap<QString, QVector<QPair<int, Value> > *> *data, QVector<QVector<QString> *> *record, int layer)
{
    Value left = iMultiplicativeExpression(node->m_p_left_child, data, record, layer);
    if(node->m_p_right_child != nullptr)
    {
        Value value;
        Value right = iAdditiveExpression(node->m_p_left_child, data, record, layer);
        if(node->m_t.syn == syn_add)
        {
            if(left.type == type_str  || node->m_t.syn == syn_null)
            {
                if(right.type == type_str)
                {
                    value.set(left._str + right._str);
                }else if(right.type == type_null)
                {
                    value.set(left._str);
                }else{
                    /*
                     * 错误
                     * 数据类型不匹配(期望str)
                     */
                }
                return value;
            }else if(left.type == type_int)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_float)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }
        }else if(node->m_t.syn == syn_sub)
        {
            if(left.type == type_str  || node->m_t.syn == syn_null)
            {
                /*
                 * 错误
                 * 无法运算
                 */
            }else if(left.type == type_int)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_float)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }else if(left.type == type_bool)
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
                    /*
                     * 错误
                     * 数据类型不匹配
                     */
                }
            }
        }
    }
    return left;
}

void ScriptInterpreter::insertVariable(QString id, Value v, QMap<QString, QVector<QPair<int, Value> >* > *data, QVector<QVector<QString> *> *record, int layer)
{
    if(data->contains(id))
    {
        QVector<QPair<int, Value> >* vector = data->value(id);
        if(vector->last().first == layer)
        {
            /*
             * 错误
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
         * 错误
         * 数据类型不匹配
         */
    }
    return v;
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
