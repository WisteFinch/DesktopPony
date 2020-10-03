#include "scriptinterpreter/parser/parser.h"

ScriptParser::ScriptParser(ScriptLexer *l)
{
    this->m_p_lexer = l;
    l = nullptr;

    this->m_index = 0;
}

/*Test_Begin*
void ScriptParser::test()
{
    QString s = "";
    this->m_p_lexer->divToken(s);
    this->m_p_root = new ScriptParserNode();
    this->pStatementList(this->m_p_root);
    ergodic(this->m_p_root);
}
void ScriptParser::ergodic(ScriptParserNode *n)
{
    if(n == nullptr)
    {
        std::cout<<"NULL";
        return;
    }
    if(n->m_p == parser_null)
    {
        std::cout<<"NULL";
        return;
    }
    if(n->m_d == nullptr)
        std::cout<<"NULL";
    std::cout<<n->m_d.toStdString()<<"|"<<n->m_p<<"|(";
    ergodic(n->m_p_left_child);
    std::cout<<"),(";
    ergodic(n->m_p_right_child);
    std::cout<<")";
}
Test_End*/

void ScriptParser::eat(QString s)
{
    if(m_p_lexer->m_token->at(m_index).first == s)
        m_index++;
    else
        errorExit();
}

void ScriptParser::eat(SYN s)
{
    if(m_p_lexer->m_token->at(m_index).second == s)
        m_index++;
    else
        errorExit();
}

QPair<QString ,SYN> ScriptParser::getToken()
{
    return m_p_lexer->m_token->at(m_index++);
}

QPair<QString ,SYN> ScriptParser::curToken()
{
    return m_p_lexer->m_token->at(m_index);
}

QPair<QString ,SYN> ScriptParser::lookAhead()
{
    return m_p_lexer->m_token->at(m_index+1);
}

QPair<QString ,SYN> ScriptParser::lookAhead(int i)
{
    return m_p_lexer->m_token->at(m_index+i);
}

void ScriptParser::errorExit()
{
    //std::cout<<endl<<"Error: Token("<<this->m_index<<")["<<this->curToken().first.toStdString()<<"|"<<this->curToken().second<<"]"<<endl;
}

void ScriptParser::pStatementList(ScriptParserNode *node)
{
    node->set(nullptr, parser_sl, nullptr, nullptr);
    //分析文法
    //获取当前单词
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_front_body)
    {
        node->m_p_left_child = new ScriptParserNode();
        pCompoundStatement(node->m_p_left_child);
    }else{
        node->m_p_left_child = new ScriptParserNode();
        pStatement(node->m_p_left_child);
    }
    token = curToken();
    if(token.second != syn_end && token.second != syn_back_body)
    {
        node->m_p_right_child = new ScriptParserNode();
        pStatementList(node->m_p_right_child);
    }
}

void ScriptParser::pStatement(ScriptParserNode *node)
{
    node->set("", parser_s, nullptr, nullptr);
    //分析文法
    //获取当前单词
    QPair<QString ,SYN> token = curToken();
    QPair<QString ,SYN> aheadToken = lookAhead();
    node->m_p_left_child = new ScriptParserNode();
    if(token.second == syn_int || token.second == syn_float || token.second == syn_bool || token.second == syn_string)
    {
        pDeclare(node->m_p_left_child);
        eat(syn_semi);
    }else if(token.second == syn_id && aheadToken.second == syn_front_bracket)
    {
        pFunction(node->m_p_left_child);
        eat(syn_semi);
    }else if(token.second == syn_while)
    {
        pWhile(node->m_p_left_child);
    }else if(token.second == syn_if)
    {
        pIf(node->m_p_left_child);
    }else{
        pExpression(node->m_p_left_child);
    }
}

void ScriptParser::pCompoundStatement(ScriptParserNode *node)
{
    node->set("", parser_cs, nullptr, nullptr);
    //分析文法
    //获取当前单词
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_front_body)
    {
        eat(syn_front_body);
        token = curToken();
        if(token.second != syn_back_body)
        {
            node->m_p_left_child = new ScriptParserNode();
            pStatementList(node->m_p_left_child);
        }
        eat(syn_back_body);
    }else{
        node->m_p_left_child = new ScriptParserNode();
        pStatement(node->m_p_left_child);
    }
}

void ScriptParser::pFunction(ScriptParserNode *node)
{
    node->set("", parser_function, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pId(node->m_p_left_child);
    eat(syn_front_bracket);
    QPair<QString ,SYN> token = curToken();
    if(token.second != syn_back_bracket)
    {
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
    }
    eat(syn_back_bracket);
}

void ScriptParser::pIf(ScriptParserNode *node)
{
    node->set("if", parser_if, nullptr, nullptr);
    //分析文法
    eat(syn_if);
    eat(syn_front_bracket);
    node->m_p_left_child = new ScriptParserNode();
    pExpression(node->m_p_left_child);
    eat(syn_back_bracket);
    node->m_p_right_child = new ScriptParserNode();
    pIfBody(node->m_p_right_child);
}

void ScriptParser::pIfBody(ScriptParserNode *node)
{

    node->set("", parser_if_body, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pCompoundStatement(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_else)
    {
        eat(syn_else);
        token = curToken();
        if(token.second == syn_if)
        {
            node->m_p_right_child = new ScriptParserNode();
            pIf(node->m_p_right_child);
        }else{
            node->m_p_right_child = new ScriptParserNode();
            pCompoundStatement(node->m_p_right_child);
        }
    }
}

void ScriptParser::pWhile(ScriptParserNode *node)
{
    node->set("while", parser_while, nullptr, nullptr);
    //分析文法
    eat(syn_while);
    eat(syn_front_bracket);
    node->m_p_left_child = new ScriptParserNode();
    pExpression(node->m_p_left_child);
    eat(syn_back_bracket);
    node->m_p_right_child = new ScriptParserNode();
    pCompoundStatement(node->m_p_right_child);
}

void ScriptParser::pDeclare(ScriptParserNode *node)
{
    node->set("", parser_declare, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pType(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_id)
    {
        node->m_p_right_child = new ScriptParserNode();
        pDeclares(node->m_p_right_child);
    }else{
        errorExit();
    }
}

void ScriptParser::pDeclareBody(ScriptParserNode *node)
{
    node->set("", parser_declare_body, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pDeclareBasic(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_equal)
    {
        eat(syn_equal);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
    }
}

void ScriptParser::pDeclares(ScriptParserNode *node)
{
    node->set("", parser_declares, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pDeclareBody(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_comma)
    {
        eat(syn_comma);
        if(token.second == syn_id)
        {
            node->m_p_right_child = new ScriptParserNode();
            pDeclares(node->m_p_right_child);
        }else{
            errorExit();
        }
    }
}

void ScriptParser::pDeclareBasic(ScriptParserNode *node)
{
    node->set("", parser_declare_basic, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pId(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_front_array)
    {
        eat(syn_front_array);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
        eat(syn_back_array);
    }
}

void ScriptParser::pType(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    QPair<QString ,SYN> token = getToken();
    node->set(token.first, parser_type, nullptr, nullptr);
}

void ScriptParser::pId(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    QPair<QString ,SYN> token = getToken();
    node->set(token.first, parser_id, nullptr, nullptr);
}

void ScriptParser::pExpression(ScriptParserNode *node)
{
    //分析文法
    node->set(nullptr, parser_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pAssignmentExpression(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_comma)
    {
        eat(syn_comma);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
    }
}

void ScriptParser::pAssignmentExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_ass_exp, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_const_num || token.second == syn_const_char || token.second == syn_const_bool ||
        token.second == syn_str_lit || token.second == syn_id || token.second == syn_front_bracket ||
        token.second == syn_inc_op || token.second == syn_dec_op)
    {
        int index = this->m_index;
        pUnaryExpression(node->m_p_left_child);
        token = curToken();
        if(token.second == syn_left_assign || token.second == syn_right_assign || token.second == syn_add_assign ||
            token.second == syn_sub_assign || token.second == syn_mul_assign || token.second == syn_div_assign ||
            token.second == syn_mod_assign || token.second == syn_and_assign || token.second == syn_xor_assign ||
            token.second == syn_or_assign)
        {
            node->m_p_right_child = new ScriptParserNode();
            if(token.second == syn_left_assign)
            {
                node->m_syn = syn_left_assign;
                eat(syn_left_assign);
            }else if(token.second == syn_right_assign)
            {
                node->m_syn = syn_right_assign;
                eat(syn_right_assign);
            }else if(token.second == syn_add_assign)
            {
                node->m_syn = syn_add_assign;
                eat(syn_add_assign);
            }else if(token.second == syn_sub_assign)
            {
                node->m_syn = syn_sub_assign;
                eat(syn_sub_assign);
            }else if(token.second == syn_mul_assign)
            {
                node->m_syn = syn_mul_assign;
                eat(syn_mul_assign);
            }else if(token.second == syn_div_assign)
            {
                node->m_syn = syn_div_assign;
                eat(syn_div_assign);
            }else if(token.second == syn_mod_assign)
            {
                node->m_syn = syn_mod_assign;
                eat(syn_mod_assign);
            }else if(token.second == syn_and_assign)
            {
                node->m_syn = syn_and_assign;
                eat(syn_and_assign);
            }else if(token.second == syn_xor_assign)
            {
                node->m_syn = syn_xor_assign;
                eat(syn_xor_assign);
            }else if(token.second == syn_or_assign)
            {
                node->m_syn = syn_or_assign;
                eat(syn_or_assign);
            }
            pEqualityExpression(node->m_p_right_child);
        }else
        {
            delete node->m_p_left_child;
            node->m_p_left_child = new ScriptParserNode();
            this->m_index = index;
            pConditionalHeadExpression(node->m_p_left_child);
        }
    }else
    {
        pConditionalHeadExpression(node->m_p_left_child);
    }
}

void ScriptParser::pConditionalHeadExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_cond_head_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pLogicalOrExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_query)
    {
        eat(syn_query);
        node->m_p_right_child = new ScriptParserNode();
        pConditionalBodyExpression(node->m_p_right_child);
    }
}

void ScriptParser::pConditionalBodyExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_cond_body_exp, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pStatement(node->m_p_left_child);
    eat(syn_colon);
    node->m_p_right_child = new ScriptParserNode();
    pStatement(node->m_p_right_child);
}

void ScriptParser::pLogicalOrExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_logic_or_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pLogicalAndExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_or_op)
    {
        node->m_syn = syn_or_op;
        eat(syn_or_op);
        node->m_p_right_child = new ScriptParserNode();
        pLogicalOrExpression(node->m_p_right_child);
    }
}

void ScriptParser::pLogicalAndExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_logic_and_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pInclusiveOrExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_and_op)
    {
        node->m_syn = syn_and_op;
        eat(syn_and_op);
        node->m_p_right_child = new ScriptParserNode();
        pLogicalAndExpression(node->m_p_right_child);
    }
}

void ScriptParser::pInclusiveOrExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_inc_or_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pExclusiveOrExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_bitor)
    {
        node->m_syn = syn_bitor;
        eat(syn_bitor);
        node->m_p_right_child = new ScriptParserNode();
        pInclusiveOrExpression(node->m_p_right_child);
    }
}

void ScriptParser::pExclusiveOrExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_exc_or_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pAndExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_bitxor)
    {
        node->m_syn = syn_bitxor;
        eat(syn_bitxor);
        node->m_p_right_child = new ScriptParserNode();
        pExclusiveOrExpression(node->m_p_right_child);
    }
}

void ScriptParser::pAndExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_and_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pEqualityExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_bitand)
    {
        node->m_syn = syn_bitand;
        eat(syn_bitand);
        node->m_p_right_child = new ScriptParserNode();
        pAndExpression(node->m_p_right_child);
    }
}

void ScriptParser::pEqualityExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_equal_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pRelationalExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_eq_op)
    {
        node->m_syn = syn_eq_op;
        eat(syn_eq_op);
        node->m_p_right_child = new ScriptParserNode();
        pEqualityExpression(node->m_p_right_child);
    }else if(token.second == syn_ne_op)
    {
        node->m_syn = syn_ne_op;
        eat(syn_ne_op);
        node->m_p_right_child = new ScriptParserNode();
        pEqualityExpression(node->m_p_right_child);
    }
}

void ScriptParser::pRelationalExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_rela_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pShiftExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_left)
    {
        node->m_syn = syn_left;
        eat(syn_left);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }else if(token.second == syn_right)
    {
        node->m_syn = syn_right;
        eat(syn_right);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }else if(token.second == syn_le_op)
    {
        node->m_syn = syn_le_op;
        eat(syn_le_op);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }else if(token.second == syn_ge_op)
    {
        node->m_syn = syn_ge_op;
        eat(syn_ge_op);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }
}

void ScriptParser::pShiftExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_shift_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pAdditiveExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_left_op)
    {
        node->m_syn = syn_left_op;
        eat(syn_left_op);
        node->m_p_right_child = new ScriptParserNode();
        pShiftExpression(node->m_p_right_child);
    }else if(token.second == syn_right_op)
    {
        node->m_syn = syn_right_op;
        eat(syn_right_op);
        node->m_p_right_child = new ScriptParserNode();
        pShiftExpression(node->m_p_right_child);
    }
}

void ScriptParser::pAdditiveExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_add_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pMultiplicativeExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_add)
    {
        node->m_syn = syn_add;
        eat(syn_add);
        node->m_p_right_child = new ScriptParserNode();
        pAdditiveExpression(node->m_p_right_child);
    }else if(token.second == syn_sub)
    {
        node->m_syn = syn_sub;
        eat(syn_sub);
        node->m_p_right_child = new ScriptParserNode();
        pAdditiveExpression(node->m_p_right_child);
    }
}

void ScriptParser::pMultiplicativeExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_mul_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pUnaryExpression(node->m_p_left_child);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_mul)
    {
        node->m_syn = syn_mul;
        eat(syn_mul);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }else if(token.second == syn_div)
    {
        node->m_syn = syn_div;
        eat(syn_div);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }else if(token.second == syn_mod)
    {
        node->m_syn = syn_mod;
        eat(syn_mod);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }
}

void ScriptParser::pUnaryExpression(ScriptParserNode *node)
{
    node->set("", parser_unary_exp, nullptr, nullptr);
    //分析文法
    QPair<QString ,SYN> token = curToken();
    node->m_p_left_child = new ScriptParserNode();
    if(token.second == syn_inc_op)
    {
        node->m_syn = syn_inc_op;
        eat(syn_inc_op);
        pUnaryExpression(node->m_p_left_child);
    }else if(token.second == syn_dec_op)
    {
        node->m_syn = syn_dec_op;
        eat(syn_dec_op);
        pUnaryExpression(node->m_p_left_child);
    }else
    {
        pPostfixExpression(node->m_p_left_child);
    }

}

void ScriptParser::pPostfixExpression(ScriptParserNode *node)
{
    node->set("", parser_post_exp, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pPrimaryExpression(node->m_p_left_child);
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_front_array)
    {
        node->m_syn = syn_front_array;
        eat(syn_front_array);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
        eat(syn_back_array);
    }else if(token.second == syn_front_bracket)
    {
        node->m_syn = syn_front_bracket;
        eat(syn_front_bracket);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
        eat(syn_back_bracket);
    }
}

void ScriptParser::pPrimaryExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_prim_exp, nullptr, nullptr);
    //分析文法
    //获取下一个单词
    node->m_p_left_child = new ScriptParserNode();
    QPair<QString ,SYN> token = curToken();
    if(token.second == syn_id)
    {
        node->m_p_left_child = new ScriptParserNode();
        pId(node->m_p_left_child);
    }else if(token.second == syn_const_num || token.second == syn_const_bool || token.second == syn_const_char)
    {
        node->m_p_left_child = new ScriptParserNode();
        pConstant(node->m_p_left_child);
    }else if(token.second == syn_str_lit)
    {
        node->m_p_left_child = new ScriptParserNode();
        pStringLiteral(node->m_p_left_child);
    }else if(token.second == syn_front_bracket)
    {
        eat(syn_front_bracket);
        if(lookAhead().second != syn_back_bracket)
        node->m_p_left_child = new ScriptParserNode();
        pExpression(node->m_p_left_child);
        eat(syn_back_bracket);
    }
}

void ScriptParser::pConstant(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    QPair<QString ,SYN> token = getToken();
    if(token.second == syn_const_num)
        node->set(token.first, parser_constant_num, nullptr, nullptr);
    else if(token.second == syn_const_bool)
        node->set(token.first, parser_constant_bool, nullptr, nullptr);
    else if(token.second == syn_const_char)
        node->set(token.first, parser_constant_char, nullptr, nullptr);
    else
        errorExit();
}

void ScriptParser::pStringLiteral(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    QPair<QString ,SYN> token = getToken();
    node->set(token.first, parser_string_literal, nullptr, nullptr);
}
