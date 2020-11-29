#include "scriptinterpreter/parser/parser.h"

ScriptParser::ScriptParser()
{
    this->m_index = 0;
}

/*test
void ScriptParser::test(QString s)
{
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
*/

void ScriptParser::set(QString s)
{
    this->m_p_lexer = new ScriptLexer;
    this->m_p_lexer->divToken(s);
    this->m_p_root = new ScriptParserNode();
    this->pStatementList(this->m_p_root);
}

void ScriptParser::set(QString s, MODE m)
{
    this->m_p_lexer = new ScriptLexer;
    this->m_p_lexer->divToken(s);
    this->m_p_root = new ScriptParserNode();
    if(m == mode_cond)
        this->pConditionalBodyExpression(this->m_p_root);
    else
        this->pStatementList(this->m_p_root);
}

void ScriptParser::set(ScriptLexer *l)
{
    this->m_p_lexer = l;
    this->m_p_root = new ScriptParserNode();
    this->pStatementList(this->m_p_root);
}

void ScriptParser::set(ScriptLexer *l, MODE m)
{
    this->m_p_lexer = l;
    this->m_p_root = new ScriptParserNode();
    if(m == mode_cond)
        this->pConditionalBodyExpression(this->m_p_root);
    else
        this->pStatementList(this->m_p_root);
}

void ScriptParser::eat(QString s)
{
    if(m_p_lexer->m_tokens->at(m_index).str == s)
        m_index++;
    else
        errorExit();
}

void ScriptParser::eat(SYN s)
{
    if(m_p_lexer->m_tokens->at(m_index).syn == s)
        m_index++;
    else
        errorExit();
}

TokenData ScriptParser::getToken()
{
    return m_p_lexer->m_tokens->at(m_index++);
}

TokenData ScriptParser::curToken()
{
    return m_p_lexer->m_tokens->at(m_index);
}

TokenData ScriptParser::lookAhead()
{
    return m_p_lexer->m_tokens->at(m_index+1);
}

TokenData ScriptParser::lookAhead(int i)
{
    return m_p_lexer->m_tokens->at(m_index+i);
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
    TokenData token = curToken();
    node->m_p_left_child = new ScriptParserNode();
    pStatement(node->m_p_left_child);

    token = curToken();
    if(token.syn != syn_end && token.syn != syn_back_body)
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
    TokenData token = curToken();
    TokenData aheadToken = lookAhead();
    node->m_p_left_child = new ScriptParserNode();
    if(token.syn == syn_int || token.syn == syn_float || token.syn == syn_bool || token.syn == syn_string)
    {
        pDeclare(node->m_p_left_child);
    }else if(token.syn == syn_while)
    {
        pCompoundStatement(node->m_p_left_child);
    }else if(token.syn == syn_while)
    {
        pWhile(node->m_p_left_child);
    }else if(token.syn == syn_if)
    {
        pIf(node->m_p_left_child);
    }else if(token.syn == syn_break || token.syn == syn_continue)
    {
        pJumpStatement(node->m_p_left_child);
    }else{
        pExpressionStatement(node->m_p_left_child);
    }
}

void ScriptParser::pCompoundStatement(ScriptParserNode *node)
{
    node->set("", parser_cs, nullptr, nullptr);
    //分析文法
    //获取当前单词
    TokenData token = curToken();
    if(token.syn == syn_front_body)
    {
        eat(syn_front_body);
        token = curToken();
        if(token.syn != syn_back_body)
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

void ScriptParser::pJumpStatement(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    TokenData token = getToken();
    node->set(token.str, parser_js, nullptr, nullptr);
    eat(syn_semi);
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
    pStatement(node->m_p_left_child);
    TokenData token = curToken();
    if(token.syn == syn_else)
    {
        eat(syn_else);
        token = curToken();
        if(token.syn == syn_if)
        {
            node->m_p_right_child = new ScriptParserNode();
            pIf(node->m_p_right_child);
        }else{
            node->m_p_right_child = new ScriptParserNode();
            pStatement(node->m_p_right_child);
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
    pStatement(node->m_p_right_child);
}

void ScriptParser::pDeclare(ScriptParserNode *node)
{
    node->set("", parser_declare, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pType(node->m_p_left_child);
    TokenData token = curToken();
    if(token.syn == syn_id)
    {
        node->m_p_right_child = new ScriptParserNode();
        pDeclares(node->m_p_right_child);
    }else{
        errorExit();
    }
    eat(syn_semi);
}

void ScriptParser::pDeclareBody(ScriptParserNode *node)
{
    node->set("", parser_declare_body, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pDeclareBasic(node->m_p_left_child);
    TokenData token = curToken();
    if(token.syn == syn_equal)
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
    TokenData token = curToken();
    if(token.syn == syn_comma)
    {
        eat(syn_comma);
        if(token.syn == syn_id)
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
    TokenData token = curToken();
    if(token.syn == syn_front_array)
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
    TokenData token = getToken();
    node->set(token.str, parser_type, nullptr, nullptr);
}

void ScriptParser::pId(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    TokenData token = getToken();
    node->set(token.str, parser_id, nullptr, nullptr);
}

void ScriptParser::pExpressionStatement(ScriptParserNode *node)
{
    //分析文法
    node->set(nullptr, parser_es, nullptr, nullptr);
    TokenData token = curToken();
    if(token.syn == syn_semi)
    {
        eat(syn_semi);
        return;
    }
    node->m_p_left_child = new ScriptParserNode();
    pExpression(node->m_p_left_child);
    eat(syn_semi);
}

void ScriptParser::pExpression(ScriptParserNode *node)
{
    //分析文法
    node->set(nullptr, parser_exp, nullptr, nullptr);
    node->m_p_left_child = new ScriptParserNode();
    pAssignmentExpression(node->m_p_left_child);
    TokenData token = curToken();
    if(token.syn == syn_comma)
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
    TokenData token = curToken();
    if(token.syn == syn_const_int || token.syn == syn_const_float || token.syn == syn_const_char || token.syn == syn_const_bool ||
        token.syn == syn_str_lit || token.syn == syn_id || token.syn == syn_front_bracket ||
        token.syn == syn_inc_op || token.syn == syn_dec_op)
    {
        int index = this->m_index;
        pUnaryExpression(node->m_p_left_child);
        token = curToken();
        if(token.syn == syn_left_assign || token.syn == syn_right_assign || token.syn == syn_add_assign ||
            token.syn == syn_sub_assign || token.syn == syn_mul_assign || token.syn == syn_div_assign ||
            token.syn == syn_mod_assign || token.syn == syn_and_assign || token.syn == syn_xor_assign ||
            token.syn == syn_or_assign)
        {
            node->m_p_right_child = new ScriptParserNode();
            if(token.syn == syn_left_assign)
            {
                eat(syn_left_assign);
            }else if(token.syn == syn_right_assign)
            {
                eat(syn_right_assign);
            }else if(token.syn == syn_add_assign)
            {
                eat(syn_add_assign);
            }else if(token.syn == syn_sub_assign)
            {
                eat(syn_sub_assign);
            }else if(token.syn == syn_mul_assign)
            {
                eat(syn_mul_assign);
            }else if(token.syn == syn_div_assign)
            {
                eat(syn_div_assign);
            }else if(token.syn == syn_mod_assign)
            {
                eat(syn_mod_assign);
            }else if(token.syn == syn_and_assign)
            {
                eat(syn_and_assign);
            }else if(token.syn == syn_xor_assign)
            {
                eat(syn_xor_assign);
            }else if(token.syn == syn_or_assign)
            {
                eat(syn_or_assign);
            }
            node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_query)
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
    TokenData token = curToken();
    if(token.syn == syn_or_op)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_and_op)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_bitor)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_bitxor)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_bitand)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_eq_op)
    {
        node->m_t = token;
        eat(syn_eq_op);
        node->m_p_right_child = new ScriptParserNode();
        pEqualityExpression(node->m_p_right_child);
    }else if(token.syn == syn_ne_op)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_left)
    {
        node->m_t = token;
        eat(syn_left);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }else if(token.syn == syn_right)
    {
        node->m_t = token;
        eat(syn_right);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }else if(token.syn == syn_le_op)
    {
        node->m_t = token;
        eat(syn_le_op);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }else if(token.syn == syn_ge_op)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_left_op)
    {
        node->m_t = token;
        eat(syn_left_op);
        node->m_p_right_child = new ScriptParserNode();
        pShiftExpression(node->m_p_right_child);
    }else if(token.syn == syn_right_op)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_add)
    {
        node->m_t = token;
        eat(syn_add);
        node->m_p_right_child = new ScriptParserNode();
        pAdditiveExpression(node->m_p_right_child);
    }else if(token.syn == syn_sub)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_mul)
    {
        node->m_t = token;
        eat(syn_mul);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }else if(token.syn == syn_div)
    {
        node->m_t = token;
        eat(syn_div);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }else if(token.syn == syn_mod)
    {
        node->m_t = token;
        eat(syn_mod);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }
}

void ScriptParser::pUnaryExpression(ScriptParserNode *node)
{
    node->set("", parser_unary_exp, nullptr, nullptr);
    //分析文法
    TokenData token = curToken();
    node->m_p_left_child = new ScriptParserNode();
    if(token.syn == syn_inc_op)
    {
        node->m_t = token;
        eat(syn_inc_op);
        pUnaryExpression(node->m_p_left_child);
    }else if(token.syn == syn_dec_op)
    {
        node->m_t = token;
        eat(syn_dec_op);
        pUnaryExpression(node->m_p_left_child);
    }else
    {
        pPostfixExpression(node->m_p_left_child);
    }

}

void ScriptParser::pPostfixExpression(ScriptParserNode *node)
{
    node->set(nullptr, parser_post_exp, nullptr, nullptr);
    //分析文法
    node->m_p_left_child = new ScriptParserNode();
    pPrimaryExpression(node->m_p_left_child);
    TokenData token = curToken();
    if(token.syn == syn_front_array)
    {
        node->m_t = token;
        eat(syn_front_array);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
        eat(syn_back_array);
    }else if(token.syn == syn_front_bracket)
    {
        node->m_t = token;
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
    TokenData token = curToken();
    if(token.syn == syn_id)
    {
        node->m_t = token;
        node->m_p_left_child = new ScriptParserNode();
        pId(node->m_p_left_child);
    }else if(token.syn == syn_const_int || token.syn == syn_const_float || token.syn == syn_const_bool || token.syn == syn_const_char)
    {
        node->m_t = token;
        node->m_p_left_child = new ScriptParserNode();
        pConstant(node->m_p_left_child);
    }else if(token.syn == syn_str_lit)
    {
        node->m_t = token;
        node->m_p_left_child = new ScriptParserNode();
        pStringLiteral(node->m_p_left_child);
    }else if(token.syn == syn_front_bracket)
    {
        node->m_t = token;
        eat(syn_front_bracket);
        if(lookAhead().str != syn_back_bracket)
        node->m_p_left_child = new ScriptParserNode();
        pExpression(node->m_p_left_child);
        eat(syn_back_bracket);
    }
}

void ScriptParser::pConstant(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    TokenData token = getToken();
    if(token.syn == syn_const_int)
        node->set(token.str, parser_constant_int, nullptr, nullptr);
    else if(token.syn == syn_const_float)
        node->set(token.str, parser_constant_float, nullptr, nullptr);
    else if(token.syn == syn_const_bool)
        node->set(token.str, parser_constant_bool, nullptr, nullptr);
    else if(token.syn == syn_const_char)
        node->set(token.str, parser_constant_char, nullptr, nullptr);
    else
        errorExit();
}

void ScriptParser::pStringLiteral(ScriptParserNode *node)
{
    //分析文法
    //获取下一个单词
    TokenData token = getToken();
    node->set(token.str, parser_string_literal, nullptr, nullptr);
}
