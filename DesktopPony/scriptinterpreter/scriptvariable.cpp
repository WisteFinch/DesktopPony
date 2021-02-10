#include "scriptvariable.h"

// Class Variable begin
void Variable::init(QString id, Value *v)
{
    this->m_id = id;
    this->m_p_value = new Value;
    *this->m_p_value = *v;
    this->m_p_value->_id = m_id;
    this->m_type = v->type;
}

void Variable::init(QString id, VALUE_TYPE t)
{
    this->m_id = id;
    this->m_p_value = new Value;
    this->m_p_value->type = t;
    this->m_p_value->_id = m_id;
    this->m_type = t;
}

void Variable::init(QString id, QVector<int> *size, VALUE_TYPE t)
{
    this->m_id = id;
    this->m_type = t;
    this->m_is_array = true;
    this->m_dimension_size = *size;
    this->m_dimension_amount = size->size();
    this->m_p_array = new VariableDimension;
    this->m_p_array->init(&this->m_dimension_size, 1);
}

void Variable::setValue(Value *v)
{
    *this->m_p_value = *v;
    v->_id = this->m_id;
}

void Variable::setValue(QVector<int> *subscripte, Value *v)
{
    v->_id = this->m_id;
    if(subscripte->size() > this->m_dimension_amount) {
        //==================ERROR C012===================
    }
    try {
        this->m_p_array->setValue(subscripte, v);
    } catch (ScriptException &se) {
        throw se;
    }
}

Value Variable::getValue()
{
    return *this->m_p_value;
}

Value Variable::getValue(QVector<int> *subscripte)
{
    if(subscripte->size() > this->m_dimension_amount) {
        //==================ERROR C012===================
    }
    Value v;
    try {
        v = *this->m_p_array->getValue(subscripte);
    } catch (ScriptException &se) {
        throw se;
    }
    if(v.type == type_empty) {
        v.type = this->m_type;
    }
    return v;
}

Value Variable::getValuePointer()
{
    if(this->m_is_array) {
        //==================ERROR C008===================
    }
    Value *var = this->m_p_value;
    Value v;
    v.set(var);
    v._id = this->m_id;
    return v;
}

Value Variable::getValuePointer(QVector<int> *subscripte)
{
    if(!this->m_is_array) {
        //==================ERROR C012===================
    }
    if(subscripte->size() > this->m_dimension_amount) {
        //==================ERROR C012===================
    }
    Value *var;
    try {
        var = this->m_p_array->getValue(subscripte);
    } catch (ScriptException &se) {
        throw se;
    }
    Value v;
    v.set(var);
    v._id = this->m_id;
    return v;
}

VALUE_TYPE Variable::getType()
{
    return this->m_type;
}

Variable::~Variable()
{
    if(this->m_is_array) {
        delete this->m_p_array;
    } else {
        if(this->m_p_value != nullptr) {
            delete this->m_p_value;
        }
    }
}
// Class Variable end

// Class VariableDimension begin
void VariableDimension::init(QVector<int> *dimensionSize, int subscripte)
{
    //维度数大于等于当前维度序号
    if(dimensionSize->size() >= subscripte) {
        //初始化当前维度
        this->m_current_dimension = subscripte;
        this->m_dimension_size = dimensionSize->at(subscripte);
        //初始化子维度
        this->m_p_child = new QVector<VariableDimension *>;
        for(int i = 0; i < this->m_dimension_size; i++) {
            VariableDimension *v = new VariableDimension;
            v->init(dimensionSize, subscripte + 1);
            this->m_p_child->push_back(v);
        }
    } else {
        this->m_current_dimension = -1; //维度序号为-1：存储值
    }
}

void VariableDimension::setValue(QVector<int> *subscripte, Value *v)
{
    if(this->m_current_dimension != -1) {
        if(subscripte->at(this->m_current_dimension) > this->m_dimension_size) {
            //==================ERROR C011===================
        }
        try {
            this->m_p_child->at(subscripte->at(this->m_current_dimension))->setValue(subscripte, v);
        } catch (ScriptException &se) {
            throw se;
        }
    } else {
        if(!this->m_is_value_inited) { //值未初始化
            this->m_p_value = new Value;
            this->m_is_value_inited = true;
        }
        *this->m_p_value = *v;
    }
}

Value *VariableDimension::getValue(QVector<int> *subscripte)
{
    if(this->m_current_dimension != -1) {
        if(subscripte->at(this->m_current_dimension) > this->m_dimension_size) {
            //==================ERROR C011===================
        }
        try {
            return this->m_p_child->at(subscripte->at(this->m_current_dimension))->getValue(subscripte);
        } catch (ScriptException &se) {
            throw se;
        }
    } else {
        return this->m_p_value;
    }
}

VariableDimension::~VariableDimension()
{
    if(this->m_current_dimension == -1) {
        delete this->m_p_value;
    } else {
        delete this->m_p_child;
    }
}
