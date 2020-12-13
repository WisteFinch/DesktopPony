/**
 * @file scriptinterpreter/limit.h
 * @brief 脚本-语法规则
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2020 WisteFinch
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

#ifndef SCRIPTLIMIT_H
#define SCRIPTLIMIT_H

#include <QStringList>
#include <QVector>

enum SYN{
    syn_null = -2,  ///< 标记 空
    syn_error = -1, ///< 标记 错误
    syn_end = 0,    ///< 标记 结尾
    syn_const_int,  ///< 类型 整型
    syn_const_float,///< 类型 浮点型
    syn_const_bool, ///< 类型 布尔型
    syn_const_char, ///< 类型 字符
    syn_str_lit,    ///< 类型 字符串
    syn_id,         ///< 类型 标识符
    syn_if,         ///< 关键字 "if"
    syn_else,       ///< 关键字 "else"
    syn_do,         ///< 关键字 "for"
    syn_while,      ///< 关键字 "while"
    syn_for,        ///< 关键字 "for"
    syn_break,      ///< 关键字 "break"
    syn_continue,   ///< 关键字 "continue"
    syn_return,     ///< 关键字 "return"
    syn_int,        ///< 关键字 "int"
    syn_float,      ///< 关键字 "float"
    syn_string,     ///< 关键字 "string"
    syn_char,     ///< 关键字 "char"
    syn_bool,       ///< 关键字 "bool"
    syn_ellipsis,   ///< 关键字 "..."
    syn_left_assign,///< 符号 "<<="
    syn_right_assign,///< 符号 ">>="
    syn_add_assign, ///< 符号 "+="
    syn_sub_assign, ///< 符号 "-="
    syn_mul_assign, ///< 符号 "*="
    syn_div_assign, ///< 符号 "/="
    syn_mod_assign, ///< 符号 "%="
    syn_and_assign, ///< 符号 "&="
    syn_xor_assign, ///< 符号 "^="
    syn_or_assign,  ///< 符号 "|="
    syn_right_op,   ///< 符号 ">>"
    syn_left_op,    ///< 符号 "<<"
    syn_inc_op,     ///< 符号 "++"
    syn_dec_op,     ///< 符号 "--"
    syn_ptr_op,     ///< 符号 "->"
    syn_and_op,     ///< 符号 "&&"
    syn_or_op,      ///< 符号 "||"
    syn_le_op,      ///< 符号 "<="
    syn_ge_op,      ///< 符号 ">="
    syn_eq_op,      ///< 符号 "=="
    syn_ne_op,      ///< 符号 "!="
    syn_semi,       ///< 符号 ";"
    syn_front_body, ///< 符号 "{"
    syn_back_body,  ///< 符号 "}"
    syn_comma,      ///< 符号 ","
    syn_colon,      ///< 符号 ":"
    syn_equal,      ///< 符号 "="
    syn_front_bracket,///< 符号 "("
    syn_back_bracket,///< 符号 ")"
    syn_front_array,///< 符号 "["
    syn_back_array,///< 符号 "]"
    syn_node,       ///< 符号 "."
    syn_bitand,     ///< 符号 "&"
    syn_not,        ///< 符号 "!"
    syn_bitnot,     ///< 符号 "~"
    syn_sub,        ///< 符号 "-"
    syn_add,        ///< 符号 "+"
    syn_mul,        ///< 符号 "*"
    syn_div,        ///< 符号 "/"
    syn_mod,        ///< 符号 "%"
    syn_left,       ///< 符号 "<"
    syn_right,      ///< 符号 ">"
    syn_bitxor,     ///< 符号 "^"
    syn_bitor,      ///< 符号 "|"
    syn_query,      ///< 符号 "?"
    syn_comment,    ///< 符号 "/*...*/"
};

const QStringList g_syn_key_word_str = {"if", "else", "do", "while", "for", "int", "float", "string", "char, " "bool", "break", "continue", "return", "false", "true"}; ///< 关键字列表
const SYN g_syn_key_word_syn[15] = {syn_if, syn_else, syn_do, syn_while, syn_for, syn_int, syn_float, syn_string, syn_char, syn_bool, syn_break, syn_continue, syn_return, syn_const_bool, syn_const_bool}; ///< 关键字列表

enum PARSER{
    parser_null,
    parser_sl,
    parser_s,
    parser_cs,
    parser_es,
    parser_js,
    parser_declaration,     ///< 声明
    parser_init_declarator_list,///< 初始化声明列表
    parser_init_declarator, ///< 初始化声明
    parser_declarator,      ///<声明符
    parser_declaration_type_specifier,///< 类型说明符
    parser_if,
    parser_if_body,
    parser_while,
    parser_id,
    parser_exp,
    parser_ass_exp,
    parser_cond_head_exp,
    parser_cond_body_exp,
    parser_ass_op,
    parser_logic_or_exp,
    parser_logic_and_exp,
    parser_inc_or_exp,
    parser_exc_or_exp,
    parser_and_exp,
    parser_equal_exp,
    parser_rela_exp,
    parser_shift_exp,
    parser_add_exp,
    parser_mul_exp,
    parser_unary_exp,
    parser_unary_op,
    parser_post_exp,
    parser_prim_exp,
    parser_constant_int,
    parser_constant_float,
    parser_constant_bool,
    parser_constant_char,
    parser_string_literal
};

struct TokenData{
    SYN syn = syn_null;
    QString str = nullptr;
    int line = 0;
    int row = 0;
};

enum VALUE_TYPE{
    type_int,   ///< 整型
    type_float, ///< 浮点型
    type_bool,  ///< 布尔型
    type_str,   ///< 字符串型
    //type_null,  ///< 空
    type_empty, ///< 无数据
    type_var,   ///< 变量指针
    type_id,    ///< 标识符
    type_void,   ///< 缺省
    type_error  ///< 错误
};

struct Value{
    int _int = 0;
    float _float = 0;
    bool _bool = false;
    QString _str = nullptr;
    Value *_var = nullptr;
    QString _id = nullptr;
    VALUE_TYPE type = type_empty;
    void set(int i){type = type_int; _int = i;}
    void set(float f ){type = type_float; _float = f;}
    void set(bool b){type = type_bool; _bool = b;}
    void set(QString s){type = type_str; _str = s;}
    ~Value(){_var = nullptr;}
};

struct Variable
{
    void setVariable(VALUE_TYPE t){type = t;}
    void setVariable(VALUE_TYPE t, int s){type = t; size = s; isArray = true; v.resize(s);}
    QVector<Value> v;
    int size;
    bool isArray = false;
    VALUE_TYPE type = type_empty;
};

#endif // SCRIPTLIMIT_H
