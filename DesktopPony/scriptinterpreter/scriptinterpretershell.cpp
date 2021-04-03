#include "scriptinterpretershell.h"

ScriptInterpreterShell::ScriptInterpreterShell()
{

}

void ScriptInterpreterShell::init(Localisation *l)
{
    this->m_p_localisation = l;
    l = nullptr;
    this->m_p_interpreter = new ScriptInterpreter;
}

QStringList ScriptInterpreterShell::run(QString str)
{
    //清除异常信息
    this->m_exception_data.clear();

    QStringList list;
    QVector<Value> v;

    try {
        //运行命令
        v = this->m_p_interpreter->run(str);
    }  catch (ScriptExceptionData &se) {
        //处理异常
        exceptionHandle(se);
    }
    for (int i = 0; i < v.size() ; i++ ) {
        list.append(v[i].toData());
    }
    return list;
}

void ScriptInterpreterShell::exceptionHandle(ScriptExceptionData &se)
{
    ScriptException e;
    e.init(se, this->m_p_localisation);
    e.initInfo();
    this->m_exception_data.append(e);
}
