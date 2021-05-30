#include "scriptvariabledata.h"

void ValueData::set(int i)
{
    this->type = type_int;
    _int = i;
}
void ValueData::set(float f )
{
    this->type = type_float;
    _float = f;
}
void ValueData::set(bool b)
{
    this->type = type_bool;
    _bool = b;
}
void ValueData::set(std::string s)
{
    this->type = type_str;
    _str = s;
}
void ValueData::set(ValueData *v)
{
    this->type = type_var;
    _var = v;
}
bool ValueData::isInt()
{
    return this->type == type_int ? true : this->type == type_bool ? true : false;
}
int ValueData::castInt()
{
    return this->type == type_int ? _int : this->type == type_bool ? _bool : 0;
}
std::string ValueData::castStr()
{
    if(this->type == type_str) {
        return this->_str;
    } else if (this->type == type_int) {
        return std::to_string(this->_int);
    } else if (this->type == type_float) {
        return std::to_string(this->_float);
    } else if (this->type == type_bool) {
        return this->_bool ? "true" : "false";
    } else if (this->type == type_var) {
        return "varriable";
    } else if (this->type == type_id) {
        return this->_id;
    } else if (this->type == type_break) {
        return "break";
    } else if (this->type == type_continue) {
        return "continue";
    } else if (this->type == type_void) {
        return "void";
    } else if (this->type == type_empty) {
        return "indefinite";
    } else {
        return "error";
    }
}
std::string ValueData::toInfo()
{
    if(this->type == type_int) {
        return "int(" + std::to_string(this->_int) + ")";
    } else if(this->type == type_float) {
        return "float(" + std::to_string(this->_float) + ")";
    } else if(this->type == type_bool) {
        if(this->_bool) {
            return "bool(true)";
        } else {
            return "bool(false)";
        }
    } else if(this->type == type_str) {
        return "string(" + this->_str + ")";
    } else if(this->type == type_id) {
        return "id(" + this->_id + ")";
    } else if(this->type == type_var) {
        return "var(" + this->_var->toInfo() + ")";
    } else {
        return "null";
    }
}
std::string ValueData::getType()
{
    if(this->type == type_int) {
        return "int";
    } else if(this->type == type_float) {
        return "float";
    } else if(this->type == type_bool) {
        return "bool";
    } else if(this->type == type_str) {
        return "string";
    } else if(this->type == type_id) {
        return "id";
    } else if(this->type == type_var) {
        return "var";
    } else {
        return "null";
    }
}
ValueData::~ValueData()
{
    _var = nullptr;
}


void VariableDimensionData::init(std::vector<int> *dimensionSize, int subscripte)
{
    //维度数大于等于当前维度序号
    if(dimensionSize->size() > static_cast<uint>(subscripte)) {
        //初始化当前维度
        this->m_current_dimension = subscripte;
        this->m_dimension_size = dimensionSize->at(subscripte);
        //初始化子维度
        this->m_p_child = new std::vector<VariableDimensionData *>;
        for(int i = 0; i < this->m_dimension_size; i++) {
            VariableDimensionData *v = new VariableDimensionData;
            v->init(dimensionSize, subscripte + 1);
            this->m_p_child->push_back(v);
        }
    } else {
        this->m_current_dimension = -1; //维度序号为-1：存储值
    }
}

void VariableDimensionData::setValue(std::vector<int> *subscripte, ValueData *v)
{
    if(this->m_current_dimension != -1) {
        if(subscripte->at(this->m_current_dimension) > this->m_dimension_size) {
            //==================ERROR I011===================
        }
        try {
            this->m_p_child->at(subscripte->at(this->m_current_dimension))->setValue(subscripte, v);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    } else {
        if(!this->m_is_value_inited) { //值未初始化
            this->m_p_value = new ValueData;
            this->m_is_value_inited = true;
        }
        *this->m_p_value = *v;
    }
}

ValueData *VariableDimensionData::getValue(std::vector<int> *subscripte)
{
    if(this->m_current_dimension != -1) {
        if(subscripte->at(this->m_current_dimension) > this->m_dimension_size) {
            //==================ERROR I011===================
        }
        try {
            return this->m_p_child->at(subscripte->at(this->m_current_dimension))->getValue(subscripte);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    } else {
        if(this->m_p_value == nullptr) {
            this->m_p_value = new ValueData;
            this->m_is_value_inited = true;
        }
        return this->m_p_value;
    }
}

ValueData *VariableDimensionData::getValuePointer(std::vector<int> *subscripte)
{
    if(this->m_current_dimension != -1) {
        if(subscripte->at(this->m_current_dimension) > this->m_dimension_size) {
            //==================ERROR I011===================
        }
        try {
            return this->m_p_child->at(subscripte->at(this->m_current_dimension))->getValue(subscripte);
        } catch (ScriptExceptionData &se) {
            throw se;
        }
    } else {
        if(this->m_p_value == nullptr) {
            this->m_p_value = new ValueData;
            this->m_is_value_inited = true;
        }
        return this->m_p_value;
    }
}

VariableDimensionData::~VariableDimensionData()
{
    if(this->m_current_dimension == -1) {
        delete this->m_p_value;
    } else {
        delete this->m_p_child;
    }
}
