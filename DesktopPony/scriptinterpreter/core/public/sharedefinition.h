/**
 * @file scriptinterpreter/core/public/sharedefinition.h
 * @brief 脚本-共享定义
 * @author WisteFinch
 * @date 2021.5.3
 *
 * MIT License
 * Copyright (c) 2019-2021 WisteFinch
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

#ifndef SHAREDEFINITION_H
#define SHAREDEFINITION_H

#include <map>
#include <vector>
#include <iostream>
#include <string>

typedef unsigned int uint;

enum VALUE_TYPE {
    type_int,   ///< 整型
    type_float, ///< 浮点型
    type_bool,  ///< 布尔型
    type_str,   ///< 字符串型
    //type_null,  ///< 空
    type_empty, ///< 无数据
    type_var,   ///< 变量指针
    type_id,    ///< 标识符
    type_void,  ///< 缺省
    type_continue,///< 继续
    type_break, ///< 中断
    type_error  ///< 错误
};

enum SCRIPT_EXCEPTION_TYPE {
    EXCEPTION_I001,   ///< 参数数量错误
    EXCEPTION_I002,   ///< 无效的运算
    EXCEPTION_I003,   ///< 数组大小不是整数类型
    EXCEPTION_I004,   ///< 重复定义
    EXCEPTION_I005,   ///< 数组下标不是整数
    EXCEPTION_I006,   ///< 无法增加指定类型的值
    EXCEPTION_I007,   ///< 无法减少指定类型的值
    EXCEPTION_I008,   ///< 表达式不可赋值
    EXCEPTION_I009,   ///< 无法初始化变量
    EXCEPTION_I010,   ///< 重复成员
    EXCEPTION_I011,   ///< 数组索引超出数组结尾
    EXCEPTION_I012,   ///< 下标值不是数组
    EXCEPTION_I013,   ///< 数组初始值必须是初始值列表
    EXCEPTION_I014,   ///< 设定初始值时元素多余
    EXCEPTION_I015,   ///< 继续语句不在循环语句中
    EXCEPTION_I016,   ///< 中断语句不在循环语句中
};

enum SYN {
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

struct TokenData {
    SYN syn = syn_null;
    std::string str;
    int line = 0;
    int row = 0;
};

#endif // SHAREDEFINITION_H
