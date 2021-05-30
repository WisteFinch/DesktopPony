/**
 * @file scriptinterpreter/core/public/scriptvariabledata.h
 * @brief 脚本-变量数据
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

#ifndef SCRIPTVARIABLEDATA_H
#define SCRIPTVARIABLEDATA_H

#include "scriptexceptiondata.h"
#include "sharedefinition.h"

class ValueData
{
public:
    int _int = 0;
    float _float = 0;
    bool _bool = false;
    std::string _str;
    ValueData *_var = nullptr;
    std::string _id;
    VALUE_TYPE type = type_empty;
    void set(int i);
    void set(float f );
    void set(bool b);
    void set(std::string s);
    void set(ValueData *v);
    bool isInt();
    int castInt();
    std::string castStr();
    std::string toInfo();
    std::string getType();
    ~ValueData();
};

class VariableDimensionData
{
public:
    ~VariableDimensionData();

    /**
     * @brief 初始化维度
     * @param 维度大小
     * @param 当前维度序号
     */
    void init(std::vector<int> *dimensionSize, int subscripte);

    /**
     * @brief 设置值
     * @param 下标
     * @param 值
     */
    void setValue(std::vector<int> *subscripte, ValueData *v);

    /**
     * @brief 获取值
     * @param 下标
     * @return 值
     */
    ValueData *getValue(std::vector<int> *subscripte);

    /**
     * @brief 获取值指针
     * @param 下标
     * @return 值指针
     */
    ValueData *getValuePointer(std::vector<int> *subscripte);

private:
    std::vector<VariableDimensionData *> *m_p_child = nullptr; ///< 子维度
    ValueData *m_p_value = nullptr; ///< 值
    int m_current_dimension = -1; ///< 当前维度序号
    int m_dimension_size = -1; ///< 维度大小
    bool m_is_value_inited = false;
};

#endif // SCRIPTVARIABLEDATA_H
