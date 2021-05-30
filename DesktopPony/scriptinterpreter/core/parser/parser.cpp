#include "parser.h"

ScriptParser::ScriptParser()
{
    this->m_index = 0;
}

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
    if(m == mode_cond) {
        this->pConditionalBodyExpression(this->m_p_root);
    } else {
        this->pStatementList(this->m_p_root);
    }
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
    if(m == mode_cond) {
        this->pConditionalBodyExpression(this->m_p_root);
    } else {
        this->pStatementList(this->m_p_root);
    }
}

void ScriptParser::eat(QString s)
{
    if(m_p_lexer->m_tokens->at(m_index).str == s.toStdString()) {
        m_index++;
    } else {
        errorExit();
    }
}

void ScriptParser::eat(SYN s)
{
    if(m_p_lexer->m_tokens->at(m_index).syn == s) {
        m_index++;
    } else {
        errorExit();
    }
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
    return m_p_lexer->m_tokens->at(m_index + 1);
}

TokenData ScriptParser::lookAhead(int i)
{
    return m_p_lexer->m_tokens->at(m_index + i);
}

TokenData ScriptParser::moveTokenIndex(int i)
{
    m_index = m_index + i < 0 ? 0 : m_index + i >= m_p_lexer->m_tokens->size() ? m_p_lexer->m_tokens->size() - 1 : m_index + i;
    return m_p_lexer->m_tokens->at(m_index);
}

void ScriptParser::errorExit()
{

}

void ScriptParser::pStatementList(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_sl, token);

    //文法：SL -> S
    node->m_p_left_child = new ScriptParserNode();
    pStatement(node->m_p_left_child);

    token = curToken();
    //文法：SL -> S, SL
    if(token.syn != syn_end && token.syn != syn_back_body) {
        node->m_p_right_child = new ScriptParserNode();
        pStatementList(node->m_p_right_child);
    }
}

void ScriptParser::pStatement(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_s, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：S -> DECLARE
    if(token.syn == syn_int || token.syn == syn_float || token.syn == syn_bool || token.syn == syn_string) {
        pDeclaration(node->m_p_left_child);
    }
    //文法：S -> CS
    else if(token.syn == syn_front_body) {
        pCompoundStatement(node->m_p_left_child);
    }
    //文法：S -> WHILE
    else if(token.syn == syn_while) {
        pWhile(node->m_p_left_child);
    }
    //文法：S -> IF
    else if(token.syn == syn_if) {
        pIf(node->m_p_left_child);
    }
    //文法：S -> JS
    else if(token.syn == syn_break || token.syn == syn_continue) {
        pJumpStatement(node->m_p_left_child);
    }
    //文法：S -> ES
    else {
        pExpressionStatement(node->m_p_left_child);
    }
}

void ScriptParser::pCompoundStatement(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_cs, token);

    //文法：CS -> { SL } | { }
    if(token.syn == syn_front_body) {
        eat(syn_front_body);
        token = curToken();
        if(token.syn != syn_back_body) {
            node->m_p_left_child = new ScriptParserNode();
            pStatementList(node->m_p_left_child);
        }
        eat(syn_back_body);
    }
}

void ScriptParser::pJumpStatement(ScriptParserNode *node)
{
    TokenData token = getToken();
    node->set(token, parser_js, token);
    eat(syn_semi);
}

void ScriptParser::pIf(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_if, token);

    //文法：if ( EXP ) IFBODY
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
    TokenData token = curToken();
    node->set(parser_if_body, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：IFBODY -> S
    pStatement(node->m_p_left_child);
    //文法：IFBODY -> S else S|IF
    token = curToken();
    if(token.syn == syn_else) {
        eat(syn_else);
        token = curToken();
        if(token.syn == syn_if) {
            node->m_p_right_child = new ScriptParserNode();
            pIf(node->m_p_right_child);
        } else {
            node->m_p_right_child = new ScriptParserNode();
            pStatement(node->m_p_right_child);
        }
    }
}

void ScriptParser::pWhile(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_while, token);

    //文法：WHILE -> while ( EXP )
    eat(syn_while);
    eat(syn_front_bracket);
    node->m_p_left_child = new ScriptParserNode();
    pExpression(node->m_p_left_child);
    eat(syn_back_bracket);
    node->m_p_right_child = new ScriptParserNode();
    pCompoundStatement(node->m_p_right_child);
}

void ScriptParser::pDeclaration(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_declaration, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：DECLARATION -> DECLARATIONTYPESPEC INITDECLARATORLIST
    pDeclarationTypeSpecifier(node->m_p_left_child);
    token = curToken();
    if(token.syn == syn_id) {
        node->m_p_right_child = new ScriptParserNode();
        pInitDeclaratorList(node->m_p_right_child);
    } else {
        errorExit();
    }
    eat(syn_semi);
}

void ScriptParser::pInitDeclaratorList(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_init_declarator_list, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：INITDECLARATORLIST -> INITDECLARATOR
    pInitDeclarator(node->m_p_left_child);
    token = curToken();
    //文法：INITDECLARATORLIST -> INITDECLARATOR, INITDECLARATORLIST
    if(token.syn == syn_comma) {
        eat(syn_comma);
        token = curToken();
        if(token.syn == syn_id) {
            node->m_p_right_child = new ScriptParserNode();
            pInitDeclaratorList(node->m_p_right_child);
        } else {
            errorExit();
        }
    }
}

void ScriptParser::pInitDeclarator(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_init_declarator, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：INITDECLARATOR -> DECLARATOR
    pDeclarator(node->m_p_left_child);
    token = curToken();
    //文法：INITDECLARATOR -> DECLARATOR = INITIALIZER
    if(token.syn == syn_equal) {
        eat(syn_equal);
        node->m_p_right_child = new ScriptParserNode();
        pInitializer(node->m_p_right_child);
    }
}

void ScriptParser::pDeclarator(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_declarator, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：DECLARATOR -> ID
    pId(node->m_p_left_child);
    token = curToken();
    //文法：DECLARATOR -> ID DCLSIZE
    if(token.syn == syn_front_array) {
        eat(syn_front_array);
        node->m_p_right_child = new ScriptParserNode();
        pDeclaratorSize(node->m_p_right_child);
        eat(syn_back_array);
    }
}

void ScriptParser::pDeclaratorSize(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_declarator_size, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：DCLSIZE -> [ CONST ]
    eat(syn_front_array);
    pConstant(node->m_p_left_child);
    eat(syn_back_array);
    token = curToken();
    //文法：DCLSIZE -> [ CONST ] DCLSIZE
    if(token.syn == syn_front_array) {
        node->m_p_right_child = new ScriptParserNode();
        pDeclaratorSize(node->m_p_right_child);
    }
}

void ScriptParser::pDeclarationTypeSpecifier(ScriptParserNode *node)
{
    //文法：DECLARATIONTYPESPEC -> int │ bool │ string │ float
    TokenData token = getToken();
    node->set(token, parser_declaration_type_specifier, token);
}

void ScriptParser::pInitializer(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_init, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：INITIALIZER -> { INITIALIZERLIST } | { INITIALIZERLIST , }
    if(token.syn == syn_front_body) {
        eat(syn_front_body);
        this->pInitializerList(node->m_p_left_child);
        token = curToken();
        if(token.syn == syn_comma) {
            eat(syn_comma);
        }
        eat(syn_back_body);
    }
    //文法：INITIALIZER -> AEXP
    else {
        this->pAssignmentExpression(node->m_p_left_child);
    }
}

void ScriptParser::pInitializerList(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_init_list, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：INITIALIZERLIST -> INITIALIZER
    pInitializer(node->m_p_left_child);
    token = curToken();
    //文法：INITIALIZERLIST -> INITIALIZER, INITIALIZERLIST
    if(token.syn == syn_comma && lookAhead(1).syn != syn_back_body) {
        eat(syn_comma);
        node->m_p_right_child = new ScriptParserNode();
        pInitializerList(node->m_p_right_child);
    }
}

void ScriptParser::pId(ScriptParserNode *node)
{
    TokenData token = getToken();
    node->set(token, parser_id, token);
}

void ScriptParser::pExpressionStatement(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_es, token);

    //文法：ES -> ;
    if(token.syn == syn_semi) {
        eat(syn_semi);
        return;
    }
    //文法：ES -> EXP
    node->m_p_left_child = new ScriptParserNode();
    pExpression(node->m_p_left_child);
    eat(syn_semi);
}

void ScriptParser::pExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：EXP -> AEXP
    pAssignmentExpression(node->m_p_left_child);
    token = curToken();
    //文法：EXP -> AEXP, EXP
    if(token.syn == syn_comma) {
        eat(syn_comma);
        node->m_p_right_child = new ScriptParserNode();
        pExpression(node->m_p_right_child);
    }
}

void ScriptParser::pAssignmentExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_ass_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：AEXP -> UEXP = AEXP
    //          -> UEXP += AEXP
    //          -> UEXP -= AEXP
    //          -> UEXP *= AEXP
    //          -> UEXP /= AEXP
    //          -> UEXP %= AEXP
    //          -> UEXP &= AEXP
    //          -> UEXP ^= AEXP
    //          -> UEXP |= AEXP
    if(token.syn == syn_const_int ||
            token.syn == syn_const_float ||
            token.syn == syn_const_char ||
            token.syn == syn_const_bool ||
            token.syn == syn_str_lit ||
            token.syn == syn_id ||
            token.syn == syn_front_bracket ||
            token.syn == syn_inc_op ||
            token.syn == syn_dec_op) {
        int index = this->m_index;
        pUnaryExpression(node->m_p_left_child);
        token = curToken();
        if(token.syn == syn_equal ||
                token.syn == syn_left_assign ||
                token.syn == syn_right_assign ||
                token.syn == syn_add_assign ||
                token.syn == syn_sub_assign ||
                token.syn == syn_mul_assign ||
                token.syn == syn_div_assign ||
                token.syn == syn_mod_assign ||
                token.syn == syn_and_assign ||
                token.syn == syn_xor_assign ||
                token.syn == syn_or_assign) {
            node->m_p_right_child = new ScriptParserNode();
            moveTokenIndex(1);
            node->m_d = token;
            pAssignmentExpression(node->m_p_right_child);
        }
        //文法：AEXP -> CHEXP
        else {
            delete node->m_p_left_child;
            node->m_p_left_child = new ScriptParserNode();
            this->m_index = index;
            pConditionalHeadExpression(node->m_p_left_child);
        }
    }
    //文法：AEXP -> CHEXP
    else {
        pConditionalHeadExpression(node->m_p_left_child);
    }
}

void ScriptParser::pConditionalHeadExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_cond_head_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：CHEXP -> LOREXP
    pLogicalOrExpression(node->m_p_left_child);
    token = curToken();
    //文法：CHEXP -> LOREXP ? CBEXP
    if(token.syn == syn_query) {
        eat(syn_query);
        node->m_p_right_child = new ScriptParserNode();
        pConditionalBodyExpression(node->m_p_right_child);
    }
}

void ScriptParser::pConditionalBodyExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_cond_body_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：CBEXP -> AEXP : CHEXP
    pStatement(node->m_p_left_child);
    eat(syn_colon);
    node->m_p_right_child = new ScriptParserNode();
    pStatement(node->m_p_right_child);
}

void ScriptParser::pLogicalOrExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_logic_or_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：LOREXP -> LANDEXP
    pLogicalAndExpression(node->m_p_left_child);
    token = curToken();
    //文法：LOREXP -> LANDEXP || LOREXP
    if(token.syn == syn_or_op) {
        node->m_d = token;
        eat(syn_or_op);
        node->m_p_right_child = new ScriptParserNode();
        pLogicalOrExpression(node->m_p_right_child);
    }
}

void ScriptParser::pLogicalAndExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_logic_and_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：LANDEXP -> IOREXP
    pInclusiveOrExpression(node->m_p_left_child);
    token = curToken();
    //文法：LANDEXP -> IOREXP && LANDEXP
    if(token.syn == syn_and_op) {
        node->m_d = token;
        eat(syn_and_op);
        node->m_p_right_child = new ScriptParserNode();
        pLogicalAndExpression(node->m_p_right_child);
    }
}

void ScriptParser::pInclusiveOrExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_inc_or_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：IOREXP -> EOREXP
    pExclusiveOrExpression(node->m_p_left_child);
    token = curToken();
    //文法：IOREXP -> EOREXP | IOREXP
    if(token.syn == syn_bitor) {
        node->m_d = token;
        eat(syn_bitor);
        node->m_p_right_child = new ScriptParserNode();
        pInclusiveOrExpression(node->m_p_right_child);
    }
}

void ScriptParser::pExclusiveOrExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_exc_or_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：EOREXP -> ANDEXP
    pAndExpression(node->m_p_left_child);
    token = curToken();
    //文法：EOREXP -> ANDEXP ^ EOREXP
    if(token.syn == syn_bitxor) {
        node->m_d = token;
        eat(syn_bitxor);
        node->m_p_right_child = new ScriptParserNode();
        pExclusiveOrExpression(node->m_p_right_child);
    }
}

void ScriptParser::pAndExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_and_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：ANDEXP -> EQUALEXP
    pEqualityExpression(node->m_p_left_child);
    token = curToken();
    //文法：ANDEXP -> EQUALEXP & ANDEXP
    if(token.syn == syn_bitand) {
        node->m_d = token;
        eat(syn_bitand);
        node->m_p_right_child = new ScriptParserNode();
        pAndExpression(node->m_p_right_child);
    }
}

void ScriptParser::pEqualityExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_equal_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：EQUALEXP -> RELAEXP
    pRelationalExpression(node->m_p_left_child);

    token = curToken();
    //文法：EQUALEXP -> RELAEXP == EQUALEXP
    if(token.syn == syn_eq_op) {
        node->m_d = token;
        eat(syn_eq_op);
        node->m_p_right_child = new ScriptParserNode();
        pEqualityExpression(node->m_p_right_child);
    }
    //文法：EQUALEXP -> RELAEXP != EQUALEXP
    else if(token.syn == syn_ne_op) {
        node->m_d = token;
        eat(syn_ne_op);
        node->m_p_right_child = new ScriptParserNode();
        pEqualityExpression(node->m_p_right_child);
    }
}

void ScriptParser::pRelationalExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_rela_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：RELAEXP -> SHIFTEXP
    pShiftExpression(node->m_p_left_child);

    token = curToken();
    //文法：RELAEXP -> SHIFTEXP < RELAEXP
    if(token.syn == syn_left) {
        node->m_d = token;
        eat(syn_left);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }
    //文法：RELAEXP -> SHIFTEXP > RELAEXP
    else if(token.syn == syn_right) {
        node->m_d = token;
        eat(syn_right);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }
    //文法：RELAEXP -> SHIFTEXP <= RELAEXP
    else if(token.syn == syn_le_op) {
        node->m_d = token;
        eat(syn_le_op);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }
    //文法：RELAEXP -> SHIFTEXP >= RELAEXP
    else if(token.syn == syn_ge_op) {
        node->m_d = token;
        eat(syn_ge_op);
        node->m_p_right_child = new ScriptParserNode();
        pRelationalExpression(node->m_p_right_child);
    }
}

void ScriptParser::pShiftExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_shift_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：SHIFTEXP -> ADDEXP
    pAdditiveExpression(node->m_p_left_child);

    token = curToken();
    //文法：SHIFTEXP -> ADDEXP << SHIFTEXP
    if(token.syn == syn_left_op) {
        node->m_d = token;
        eat(syn_left_op);
        node->m_p_right_child = new ScriptParserNode();
        pShiftExpression(node->m_p_right_child);
    }
    //文法：SHIFTEXP -> ADDEXP >> SHIFTEXP
    else if(token.syn == syn_right_op) {
        node->m_d = token;
        eat(syn_right_op);
        node->m_p_right_child = new ScriptParserNode();
        pShiftExpression(node->m_p_right_child);
    }
}

void ScriptParser::pAdditiveExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_add_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：ADDEXP -> MULTEXP
    pMultiplicativeExpression(node->m_p_left_child);

    token = curToken();
    //文法：ADDEXP -> MULTEXP + ADDEXP
    if(token.syn == syn_add) {
        node->m_d = token;
        eat(syn_add);
        node->m_p_right_child = new ScriptParserNode();
        pAdditiveExpression(node->m_p_right_child);
    }
    //文法：ADDEXP -> MULTEXP - ADDEXP
    else if(token.syn == syn_sub) {
        node->m_d = token;
        eat(syn_sub);
        node->m_p_right_child = new ScriptParserNode();
        pAdditiveExpression(node->m_p_right_child);
    }
}

void ScriptParser::pMultiplicativeExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_mul_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：MULTEXP -> UEXP
    pUnaryExpression(node->m_p_left_child);

    token = curToken();
    //文法：MULTEXP -> UEXP * MULTEXP
    if(token.syn == syn_mul) {
        node->m_d = token;
        eat(syn_mul);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }
    //文法：MULTEXP -> UEXP / MULTEXP
    else if(token.syn == syn_div) {
        node->m_d = token;
        eat(syn_div);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }
    //文法：MULTEXP -> UEXP % MULTEXP
    else if(token.syn == syn_mod) {
        node->m_d = token;
        eat(syn_mod);
        node->m_p_right_child = new ScriptParserNode();
        pMultiplicativeExpression(node->m_p_right_child);
    }
}

void ScriptParser::pUnaryExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_unary_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：UEXP	-> ++PEXP
    if(token.syn == syn_inc_op) {
        node->m_d = token;
        eat(syn_inc_op);
        pUnaryExpression(node->m_p_left_child);
    }
    //文法：UEXP	-> ++PEXP
    else if(token.syn == syn_dec_op) {
        node->m_d = token;
        eat(syn_dec_op);
        pUnaryExpression(node->m_p_left_child);
    }
    //文法：UEXP	-> PEXP
    else {
        pPostfixExpression(node->m_p_left_child);
    }

}

void ScriptParser::pPostfixExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_post_exp, token);
    node->m_p_left_child = new ScriptParserNode();
    //文法：PEXP -> PRIMEXP
    pPrimaryExpression(node->m_p_left_child);
    token = curToken();
    if(token.syn == syn_front_array ||
            token.syn == syn_front_bracket) {
        //文法：PEXP -> PRIMEXP VARINDEX
        if(token.syn == syn_front_array) {
            node->m_d = token;
            node->m_p_right_child = new ScriptParserNode();
            pVariableSubscripte(node->m_p_right_child);
        }
        //文法：PEXP -> PRIMEXP ( EXP )
        else if(token.syn == syn_front_bracket) {
            node->m_d = token;
            eat(syn_front_bracket);
            node->m_p_right_child = new ScriptParserNode();
            pExpression(node->m_p_right_child);
            eat(syn_back_bracket);
        }
    }
    pPostfixExpressionAdditive(node);
}

void ScriptParser::pPostfixExpressionAdditive(ScriptParserNode *node)
{
    TokenData token = curToken();
    while(token.syn == syn_inc_op || token.syn == syn_dec_op) {
        moveTokenIndex(1);
        ScriptParserNode *child = new ScriptParserNode(*node);
        node->set(token, parser_post_exp, child, nullptr, child->m_t);
        token = curToken();
    }
}

void ScriptParser::pPrimaryExpression(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_prim_exp, token);
    node->m_p_left_child = new ScriptParserNode();

    //文法：PRIMEXP -> ID
    if(token.syn == syn_id) {
        pId(node->m_p_left_child);
    }
    //文法：PRIMEXP -> CONST
    else if(token.syn == syn_const_int || token.syn == syn_const_float || token.syn == syn_const_bool || token.syn == syn_const_char) {
        pConstant(node->m_p_left_child);
    }
    //文法：PRIMEXP -> STR
    else if(token.syn == syn_str_lit) {
        pStringLiteral(node->m_p_left_child);
    }
    //文法：PRIMEXP ->  ( EXP )
    else if(token.syn == syn_front_bracket) {
        eat(syn_front_bracket);
        if(lookAhead().syn != syn_back_bracket) {
            pExpression(node->m_p_left_child);
        }
        eat(syn_back_bracket);
    }
}

void ScriptParser::pConstant(ScriptParserNode *node)
{
    TokenData token = getToken();
    if(token.syn == syn_const_int) {
        node->set(token, parser_constant_int, token);
    } else if(token.syn == syn_const_float) {
        node->set(token, parser_constant_float, token);
    } else if(token.syn == syn_const_bool) {
        node->set(token, parser_constant_bool, token);
    } else if(token.syn == syn_const_char) {
        node->set(token, parser_constant_char, token);
    } else if(token.syn == syn_str_lit) {
        node->set(token, parser_string_literal, token);
    } else {
        errorExit();
    }
}

void ScriptParser::pStringLiteral(ScriptParserNode *node)
{
    TokenData token = getToken();
    node->set(token, parser_string_literal, token);
}

void ScriptParser::pVariableSubscripte(ScriptParserNode *node)
{
    TokenData token = curToken();
    node->set(parser_variable_subscripte, token);

    //文法：VARSUB -> [ EXP ]
    eat(syn_front_array);
    node->m_p_left_child = new ScriptParserNode();
    pExpression(node->m_p_left_child);
    eat(syn_back_array);

    token = curToken();
    //文法：VARSUB -> [ EXP ] VARSUB
    if(token.syn == syn_front_array) {
        node->m_p_right_child = new ScriptParserNode();
        pVariableSubscripte(node->m_p_right_child);
    }
}
