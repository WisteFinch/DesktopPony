/**
 * @file scriptinterpreter/limit.h
 * @brief 语法规则
 * @author WisteFinch
 * @date 2020.10.3
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

#ifndef LIMIT_H
#define LIMIT_H

#include <QStringList>

enum SYN{
    syn_null = -2,
    syn_error = -1,
    syn_end = 0,
    syn_const_num = 1,
    syn_const_bool,
    syn_const_char,
    syn_str_lit,
    syn_id,
    syn_if,         // if
    syn_else,       // else
    syn_do,         // for
    syn_while,      // while
    syn_for,        // for
    syn_break,      // break
    syn_continue,   // continue
    syn_return,     // return
    syn_int,        // int
    syn_float,      // float
    syn_string,     // string
    syn_char,     // char
    syn_bool,       // bool
    syn_ellipsis,   // ...
    syn_left_assign,// <<=
    syn_right_assign,// >>=
    syn_add_assign, // +=
    syn_sub_assign, // -=
    syn_mul_assign, // *=
    syn_div_assign, // /=
    syn_mod_assign, // %=
    syn_and_assign, // &=
    syn_xor_assign, // ^=
    syn_or_assign,  // |=
    syn_right_op,   // >>
    syn_left_op,    // <<
    syn_inc_op,     // ++
    syn_dec_op,     // --
    syn_ptr_op,     // ->
    syn_and_op,     // &&
    syn_or_op,      // ||
    syn_le_op,      // <=
    syn_ge_op,      // >=
    syn_eq_op,      // ==
    syn_ne_op,      // !=
    syn_semi,       // ;
    syn_front_body, // {
    syn_back_body,  // }
    syn_comma,      // ,
    syn_colon,      // :
    syn_equal,      // =
    syn_front_bracket,// (
    syn_back_bracket,// )
    syn_front_array,// [
    syn_back_array,// ]
    syn_node,       // .
    syn_bitand,     // &
    syn_not,        // !
    syn_bitnot,     // ~
    syn_sub,        // -
    syn_add,        // +
    syn_mul,        // *
    syn_div,        // /
    syn_mod,        // %
    syn_left,       // <
    syn_right,      // >
    syn_bitxor,     // ^
    syn_bitor,      // |
    syn_query,      // ?
    syn_comment,    // /*...*/
};

const QStringList g_syn_key_word_str = {"if", "else", "do", "while", "for", "int", "float", "string", "char, " "bool", "break", "continue", "return", "false", "true"};
const SYN g_syn_key_word_syn[15] = {syn_if, syn_else, syn_do, syn_while, syn_for, syn_int, syn_float, syn_string, syn_char, syn_bool, syn_break, syn_continue, syn_return, syn_const_bool, syn_const_bool};

enum PARSER{
    parser_null,
    parser_sl,
    parser_s,
    parser_cs,
    parser_es,
    parser_declare,
    parser_declare_body,
    parser_declares,
    parser_declare_basic,
    parser_type,
    parser_function,
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
    parser_constant_num,
    parser_constant_bool,
    parser_constant_char,
    parser_string_literal
};

#endif // LIMIT_H
