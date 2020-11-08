#ifndef SCRIPTEXCEPTION_H
#define SCRIPTEXCEPTION_H

#include <QString>
#include <QVector>
#include "scriptinterpreter/scriptlimit.h"

class ScriptException
{
public:
    enum EXCEPTION_TYPE{
        C001, ///< 参数数量错误
        C002, ///< 无效的运算
        C003, ///< 数组大小不是整数类型
        C004, ///< 重复定义
    };

    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<Value> v);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<QString> str);
    ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i, QVector<QString> str);



    EXCEPTION_TYPE e;
    QVector<TokenData> t;
    QVector<int> i;
    QVector<QString> str;
    QVector<Value> v;
};

#endif // SCRIPTEXCEPTION_H
