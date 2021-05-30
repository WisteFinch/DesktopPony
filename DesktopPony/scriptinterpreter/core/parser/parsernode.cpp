#include "parsernode.h"

ScriptParserNode::ScriptParserNode()
{
}

ScriptParserNode::~ScriptParserNode()
{
    delete m_p_left_child;
    delete m_p_right_child;
}

void ScriptParserNode::set(TokenData d, PARSER p, ScriptParserNode *l, ScriptParserNode *r, TokenData t)
{
    this->m_p_left_child = l;
    this->m_p_right_child = r;
    this->m_d = d;
    this->m_p = p;
    this->m_t = t;
}

void ScriptParserNode::set(PARSER p, ScriptParserNode *l, ScriptParserNode *r, TokenData t)
{
    this->m_p_left_child = l;
    this->m_p_right_child = r;
    this->m_p = p;
    this->m_t = t;
}

void ScriptParserNode::set(TokenData d, PARSER p, TokenData t)
{
    this->m_d = d;
    this->m_p = p;
    this->m_t = t;
}

void ScriptParserNode::set(PARSER p, TokenData t)
{
    this->m_p = p;
    this->m_t = t;
}

ScriptParserDebugNode::ScriptParserDebugNode()
{

}

ScriptParserDebugNode::~ScriptParserDebugNode()
{
    this->m_link_node = nullptr;
    delete this->m_p_left_child;
    delete this->m_p_right_child;
}

void ScriptParserDebugNode::set(ScriptParserNode *n)
{
    ParserName *s = new ParserName;
    _set(n, nullptr, s);
    AbridgeParserName(nullptr, nullptr);
    delete s;
}

void ScriptParserDebugNode::_set(ScriptParserNode *n, QString parserName, ParserName *parserDebugInfo)
{
    this->m_link_node = n; // 设置原节点链接

    // 获取parser名称
    parserName += parserDebugInfo->getParserName(n->m_p) + '>';

    // 获取子节点
    ScriptParserDebugNode *l = new ScriptParserDebugNode();
    ScriptParserDebugNode *r = new ScriptParserDebugNode();
    if(n->m_p_left_child != nullptr) {
        l->_set(n->m_p_left_child, parserName, parserDebugInfo);
    }
    if(n->m_p_right_child != nullptr) {
        r->_set(n->m_p_right_child, parserName, parserDebugInfo);
    }

    // 更新节点信息
    this->m_intact_parser_name = parserName.left(parserName.size() - 1);
    int last = this->m_intact_parser_name.lastIndexOf('>');
    if(last == -1) {
        this->m_parser_name = this->m_intact_parser_name;
    } else {
        this->m_parser_name = this->m_intact_parser_name.right(this->m_intact_parser_name.size() - last - 1);
    }
    if(n->m_d.syn != syn_null || (l->m_is_node && r->m_is_node)) {
        if(l->m_is_node) {
            this->m_p_left_child = l;
        } else {
            delete l;
        }
        if(r->m_is_node) {
            this->m_p_right_child = r;
        } else {
            delete r;
        }
        this->m_is_node = true;
    } else if(l->m_is_node) {
        moveNode(l);
        delete r;
    } else if(r->m_is_node) {
        moveNode(r);
        delete l;
    } else {
        delete l;
        delete r;
    }
    l = nullptr;
    r = nullptr;
}

QString ScriptParserDebugNode::getAbridgedParserName(QString s)
{
    if(s.right(1) == '>') {
        s = s.left(s.size() - 1);
    }
    int first = s.indexOf('>');
    if(first == -1) {
        return s;
    }
    int last = s.lastIndexOf('>');
    if(first == last) {
        return s;
    }
    return s.left(first) + ">...>" + s.right(s.size() - last - 1);
}

void ScriptParserDebugNode::moveNode(ScriptParserDebugNode *n)
{
    this->m_p_left_child = n->m_p_left_child;
    this->m_p_right_child = n->m_p_right_child;
    this->m_link_node = n->m_link_node;
    this->m_abridged_parser_name = n->m_abridged_parser_name;
    this->m_is_node = true;
    n->breakLink();
    delete n;
}

void ScriptParserDebugNode::breakLink()
{
    this->m_p_left_child = nullptr;
    this->m_p_right_child = nullptr;
    this->m_link_node = nullptr;
    this->m_abridged_parser_name = nullptr;
}

QString ScriptParserDebugNode::print()
{
    QString info = ergodicNode(nullptr, nullptr);
    qDebug().noquote() << info;

    return info;
}

void ScriptParserDebugNode::AbridgeParserName(QString parentName, QString parentIntactName)
{
    this->m_abridged_parser_name = getAbridgedParserName(this->m_intact_parser_name.right(this->m_intact_parser_name.size() + parentName.size() - parentIntactName.size()));
    if(this->m_p_left_child != nullptr) {
        this->m_p_left_child->AbridgeParserName(this->m_parser_name, this->m_intact_parser_name);
    }
    if(this->m_p_right_child != nullptr) {
        this->m_p_right_child->AbridgeParserName(this->m_parser_name, this->m_intact_parser_name);
    }
}

QString ScriptParserDebugNode::ergodicNode(QString head, QString attached)
{
    QString info;
    QString mhead = head + "  ";
    mhead.replace("├", "│").replace("└", " ");
    QString l, r;
    if(this->m_p_right_child != nullptr) {
        r = this->m_p_right_child->ergodicNode(mhead + "└", "R:");
    }
    if(this->m_p_left_child != nullptr) {
        l = this->m_p_left_child->ergodicNode(mhead + (r.isEmpty() ? "└" : "├"), "L:");
    }
    QString lr = l + r;
    info += head + " " + attached + this->m_abridged_parser_name + " \n";
    info += mhead + (lr.isEmpty() ? "└" : "├") + " S:" + ((this->m_link_node->m_d.str.size() == 0 ? true : false) ? "NULL" : QString::fromStdString(this->m_link_node->m_d.str)) + " \n";
    info += lr;
    return info;
}
