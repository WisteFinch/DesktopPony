#include "scriptvariable.h"

void Variable::init(QString id, ValueData *v)
{
    this->m_id = id;
    this->m_p_value = new ValueData;
    *this->m_p_value = *v;
    this->m_p_value->_id = m_id.toStdString();
    this->m_type = v->type;
}

void Variable::init(QString id, VALUE_TYPE t)
{
    this->m_id = id;
    this->m_p_value = new ValueData;
    this->m_p_value->type = t;
    this->m_p_value->_id = m_id.toStdString();
    this->m_type = t;
}

void Variable::init(QString id, std::vector<int> size, VALUE_TYPE t)
{
    this->m_id = id;
    this->m_type = t;
    this->m_is_array = true;
    this->m_dimension_size = size;
    this->m_dimension_amount = size.size();
    this->m_p_array = new VariableDimensionData;
    this->m_p_array->init(&this->m_dimension_size, 0);
}

void Variable::setValue(ValueData *v)
{
    if(this->m_p_value == nullptr) {
        this->m_p_value = new ValueData;
    }
    *this->m_p_value = *v;
    v->_id = this->m_id.toStdString();
}

void Variable::setValue(std::vector<int> subscripte, ValueData *v)
{
    v->_id = this->m_id.toStdString();
    if(subscripte.size() > static_cast<uint>(this->m_dimension_amount)) {
        //==================ERROR I012===================
    }
    try {
        this->m_p_array->setValue(&subscripte, v);
    } catch (ScriptException &se) {
        throw se;
    }
}

ValueData Variable::getValue()
{
    return *this->m_p_value;
}

ValueData Variable::getValue(std::vector<int> subscripte)
{
    if(subscripte.size() > static_cast<uint>(this->m_dimension_amount)) {
        //==================ERROR I012===================
    }
    ValueData v;
    try {
        v = *this->m_p_array->getValue(&subscripte);
    } catch (ScriptException &se) {
        throw se;
    }
    if(v.type == type_empty) {
        v.type = this->m_type;
    }
    return v;
}

ValueData Variable::getValuePointer()
{
    if(this->m_is_array) {
        //==================ERROR I008===================
    }
    ValueData *var;
    try {
        var = getElementPointer();
    }  catch(ScriptException &se) {
        throw se;
    }
    ValueData v;
    v.set(var);
    v._id = this->m_id.toStdString();
    return v;
}

ValueData *Variable::getElementPointer()
{
    return this->m_p_value;
}

ValueData Variable::getValuePointer(std::vector<int> subscripte)
{
    ValueData v;
    try {
        v.set(getElementPointer(subscripte));
    } catch(ScriptException &se) {
        throw se;
    }
    v._id = this->m_id.toStdString();
    return v;
}

ValueData *Variable::getElementPointer(std::vector<int> subscripte)
{
    if(!this->m_is_array) {
        //==================ERROR I012===================
    }
    if(subscripte.size() > static_cast<uint>(this->m_dimension_amount)) {
        //==================ERROR I012===================
    }
    ValueData *var;
    try {
        var = this->m_p_array->getValue(&subscripte);
    } catch (ScriptException &se) {
        throw se;
    }
    if(var->type != this->m_type) {
        var->type = this->m_type;
        var->_id = this->m_id.toStdString();
    }
    return var;
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
