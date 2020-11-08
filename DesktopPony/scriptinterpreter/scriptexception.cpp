#include "scriptexception.h"


ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<Value> v){
    this->e = e;
    this->t = t;
    this->v = v;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i){
    this->e = e;
    this->t = t;
    this->i = i;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<QString> str){
    this->e = e;
    this->t = t;
    this->str = str;
}

ScriptException::ScriptException(EXCEPTION_TYPE e, QVector<TokenData> t, QVector<int> i, QVector<QString> str){
    this->e = e;
    this->t = t;
    this->i = i;
    this->str = str;
}
