/**
 * @file scriptinterpreter/scriptexceptionshell.h
 * @brief 脚本-外壳
 * @author WisteFinch
 * @date 2021.4.3
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

#ifndef SCRIPTINTERPRETERSHELL_H
#define SCRIPTINTERPRETERSHELL_H
#include "core/scriptinterpreter.h"
#include "data/localisation.h"
#include <QVector>

class ScriptInterpreterShell
{
public:
    ScriptInterpreterShell();

    /**
     * @brief 初始化
     * @param 本地化
     */
    void init(Localisation *l);

    /**
     * @brief 运行
     * @param 命令
     * @return 运行结果
     */
    QStringList run(QString str);

    QVector<ScriptException> m_exception_data; ///< 异常信息
private:
    /**
     * @brief 异常处理
     * @param 异常信息
     */
    void exceptionHandle(ScriptExceptionData &se);

    ValueData extraFunctions(QString functionName, QVector<ValueData> parameters, QMap<QString, QVector<QPair<int, Variable *> > *> *data, QVector<QVector<QString> *> *record, int layer); ///< 额外函数

    ScriptInterpreter *m_p_interpreter = nullptr; ///< 解释器内核
    Localisation *m_p_localisation = nullptr; ///< 本地化
};

#endif // SCRIPTINTERPRETERSHELL_H
