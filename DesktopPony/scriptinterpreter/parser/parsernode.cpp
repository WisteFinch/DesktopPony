#include "scriptinterpreter/parser/parsernode.h"

ScriptParserNode::ScriptParserNode()
{
}

ScriptParserNode::~ScriptParserNode()
{
    delete m_p_left_child;
    delete m_p_right_child;
}

void ScriptParserNode::set(QString d, PARSER p, ScriptParserNode *l, ScriptParserNode *r)
{
    this->m_p_left_child = l;
    this->m_p_right_child = r;
    this->m_d = d;
    this->m_p = p;
}

void ScriptParserNode::set(QString d, PARSER p, ScriptParserNode *l, ScriptParserNode *r, SYN syn)
{
    this->m_p_left_child = l;
    this->m_p_right_child = r;
    this->m_d = d;
    this->m_p = p;
    this->m_syn = syn;
}
