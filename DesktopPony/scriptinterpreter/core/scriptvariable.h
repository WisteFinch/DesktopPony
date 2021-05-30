/**
 * @file scriptinterpreter/core/scriptvariable.h
 * @brief 脚本-变量
 * @author WisteFinch
 * @date 2021.2.2
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

#ifndef SCRIPTVARIABLE_H
#define SCRIPTVARIABLE_H

#include <QString>
#include <vector>
#include "scriptlimit.h"
#include "scriptexception.h"
#include "public/scriptvariabledata.h"

class Variable
{
public:
    ~Variable();

    /**
     * @brief 初始化变量
     * @param 变量名称
     * @param 值
     */
    void init(QString id, ValueData *v);

    /**
     * @brief 初始化变量
     * @param 变量名称
     * @param 值类型
     */
    void init(QString id, VALUE_TYPE t);

    /**
     * @brief 初始化变量数组
     * @param 变量名称
     * @param 维度大小
     * @param 值类型
     */
    void init(QString id, std::vector<int> size, VALUE_TYPE t);


    /**
     * @brief 设置值
     * @param 值
     */
    void setValue(ValueData *v);

    /**
     * @brief 设置数组值
     * @param 下标
     * @param 值
     */
    void setValue(std::vector<int> subscripte, ValueData *v);

    /**
     * @brief 获取值
     * @return 值
     */
    ValueData getValue();

    /**
     * @brief 获取数组值
     * @param 下标
     * @return 值
     */
    ValueData getValue(std::vector<int> subscripte);

    /**
     * @brief 获取值指针
     * @return 值指针
     */
    ValueData getValuePointer();

    /**
     * @brief 获取元素指针
     * @return 元素指针
     */
    ValueData *getElementPointer();

    /**
     * @brief 获取数组值指针
     * @param 下标
     * @return 值指针
     */
    ValueData getValuePointer(std::vector<int> subscripte);

    /**
     * @brief 获取数组元素指针
     * @param 下标
     * @return 元素指针
     */
    ValueData *getElementPointer(std::vector<int> subscripte);

    /**
     * @brief 获取维度大小
     * @return 维度大小
     */
    std::vector<int> getSize()
    {
        return this->m_dimension_size;
    }

    VALUE_TYPE getType();

    bool isArray()
    {
        return this->m_is_array;
    }

private:
    VariableDimensionData *m_p_array = nullptr; ///< 维度
    VALUE_TYPE m_type = type_empty; ///< 值类型
    ValueData *m_p_value = nullptr; ///< 值
    int m_dimension_amount = -1; ///< 总维度数
    std::vector<int> m_dimension_size; ///< 维度大小
    bool m_is_array = false; ///< 是否为数组
    QString m_id; ///< 变量名
};
#endif // SCRIPTVARIABLE_H
