#include "lexer.h"

ScriptLexer::ScriptLexer()
{
    m_tokens = new QVector<TokenData>;
}

QVector<TokenData> *ScriptLexer::divToken(QString str)
{
    m_str = str;
    m_point = -1;
    m_line = 1;
    m_row = 0;
    while(m_point < m_str.size()) {
        m_token_num.clear();
        SYN syn = judge();
        TokenData token;
        token.syn = syn;
        token.str = this->m_token_num.toStdString();
        token.line = this->m_line;
        token.row = m_row;
        m_tokens->push_back(token);
    }
    return m_tokens;
}

SYN ScriptLexer::judge()
{
    m_ch = getNext();//读取下一个字符
    while(m_ch == ' ' || m_ch == '\t' || m_ch == '\n') {
        if(m_ch == '\n' || m_ch == '\t') {
            m_line++;
            m_row = 0;
        }
        m_ch = getNext();
    }

    if((m_ch >= 'a' && m_ch <= 'z') || (m_ch >= 'A' && m_ch <= 'Z') || m_ch == '_') {
        //为标识符或者变量名
        while((m_ch >= 'a' && m_ch <= 'z') || (m_ch >= 'A' && m_ch <= 'Z') || (m_ch >= '0' && m_ch <= '9') || m_ch == '_') {
            m_token_num += m_ch;
            m_ch = getNext();
        }
        m_point--;
        m_row--;
        //返回关键词
        for(int i = 0; i < g_syn_key_word_str.size(); i++) {
            if(g_syn_key_word_str[i] == m_token_num) {
                return g_syn_key_word_syn[i];
            }
        }
        //返回Letter
        return syn_id;
    } else if(m_ch >= '0' && m_ch <= '9') {
        //为数字
        int point = 0;
        while((m_ch >= '0' && m_ch <= '9') || m_ch == '.') {
            if(m_ch == '.') {
                point++;
            }
            m_token_num += m_ch;
            m_ch = getNext();
        }
        m_point--;
        m_row--;
        if(point > 1) {
            //=======================================error===============================
        }
        return point == 0 ? syn_const_int : syn_const_float;
    } else {
        //为符号
        m_token_num += m_ch;
        switch(m_ch.cell()) {
        case ';':
            return syn_semi;
        case '{':
            return syn_front_body;
        case '}':
            return syn_back_body;
        case ',':
            return syn_comma;
        case '(':
            return syn_front_bracket;
        case ')':
            return syn_back_bracket;
        case '[':
            return syn_front_array;
        case ']':
            return syn_back_array;
        case '?':
            return syn_query;
        case '.':
            if(get(1) == '.') {
                if(get(2) == '.') {
                    m_point += 2;
                    m_row += 2;
                    m_token_num += "..";
                    return syn_ellipsis;
                } else {
                    return syn_error;
                }
            } else {
                return syn_node;
            }
        case '>':
            if(get(1) == '>') {
                if(get(2) == '=') {
                    m_point += 2;
                    m_row += 2;
                    m_token_num += ">=";
                    return syn_right_assign;
                } else {
                    m_point++;
                    m_row++;
                    m_token_num += ">";
                    return syn_right_op;
                }
            } else if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_ge_op;
            } else {
                return syn_right;
            }
        case '<':
            if(get(1) == '<') {
                if(get(2) == '=') {
                    m_point += 2;
                    m_row += 2;
                    m_token_num += "<=";
                    return syn_left_assign;
                } else {
                    m_point++;
                    m_row++;
                    m_token_num += "<";
                    return syn_left_op;
                }
            } else if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_le_op;
            } else if(get(1) == '%') {
                m_point++;
                m_row++;
                m_token_num += "%";
                return syn_front_body;
            } else if(get(1) == ':') {
                m_point++;
                m_row++;
                m_token_num += ":";
                return syn_front_array;
            } else {
                return syn_left;
            }
        case '+':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_add_assign;
            } else if(get(1) == '+') {
                m_point++;
                m_row++;
                m_token_num += "+";
                return syn_inc_op;
            } else {
                return syn_add;
            }
        case '-':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_sub_assign;
            } else if(get(1) == '-') {
                m_point++;
                m_row++;
                m_token_num += "-";
                return syn_dec_op;
            } else if(get(1) == '>') {
                m_point++;
                m_row++;
                m_token_num += ">";
                return syn_ptr_op;
            } else {
                return syn_sub;
            }
        case '*':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_mul_assign;
            } else {
                return syn_mul;
            }
        case '/':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_div_assign;
            } else if(get(1) == '*') {
                m_ch = getNext();
                m_token_num += m_ch;
                while(!(get(1) == '*' && get(2) == '/')) {
                    if(m_point >= m_str.size()) {
                        return syn_error;
                    }
                    m_ch = getNext();
                    m_token_num += m_ch;
                }
                m_ch = getNext();

                m_point += 2;
                m_row += 2;
                m_token_num += "*/";
                return syn_comment;
            } else {
                return syn_div;
            }
        case '%':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_mod_assign;
            } else if(get(1) == ">") {
                m_point++;
                m_row++;
                m_token_num += ">";
                return syn_back_body;
            } else {
                return syn_mod;
            }
        case '&':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_and_assign;
            } else if(get(1) == '&') {
                m_point++;
                m_token_num += "&";
                return syn_and_op;
            }
            return syn_bitand;
        case '^':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_xor_assign;
            } else {
                return syn_bitxor;
            }
        case '|':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_or_assign;
            } else if(get(1) == '|') {
                m_point++;
                m_row++;
                m_token_num += "|";
                return syn_or_op;
            } else {
                return syn_bitor;
            }
        case '=':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_eq_op;
            } else {
                return syn_equal;
            }
        case '!':
            if(get(1) == '=') {
                m_point++;
                m_row++;
                m_token_num += "=";
                return syn_ne_op;
            } else {
                return syn_not;
            }
        case '\"':
            while(!(get(1) == '\"')) {
                if(m_point >= m_str.size()) {
                    return syn_error;
                }
                m_ch = getNext();
                m_token_num += m_ch;
            }
            m_ch = getNext();
            m_point++;
            m_row++;
            m_token_num += "\"";
            return syn_str_lit;
        case '\'':
            while(!(get(1) == '\'')) {
                if(m_point >= m_str.size()) {
                    return syn_error;
                }
                m_ch = getNext();
                m_token_num += m_ch;
            }
            m_ch = getNext();
            m_point++;
            m_row++;
            m_token_num += "\"";
            return syn_const_char;
        case ':':
            if(get(1) == '>') {
                m_point++;
                m_row++;
                m_token_num += ">";
                return syn_back_array;
            } else {
                return syn_colon;
            }
        case '$':
            if(m_point == m_str.size()) {
                return syn_end;
            } else {
                return syn_error;
            }
        default:
            return syn_error;
        }
    }
}

inline QChar ScriptLexer::getNext()
{
    m_row++;
    return ++m_point == m_str.size() ? QChar('$') : m_str[m_point];
}

inline QChar ScriptLexer::get(int n)
{
    QChar c = m_point + n >= m_str.size() ? QChar('$') : m_str[m_point + n];
    return c;
}
