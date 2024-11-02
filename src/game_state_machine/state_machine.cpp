
// state_machine.cpp

#include "state_machine.h"

StateMachine::StateMachine()  = default;
StateMachine::~StateMachine() = default;

void
StateMachine::on_update(float delta)
{
    // 如果当前状态为空，直接返回
    if(!current_state)
    {
        return;
    }

    // 如果还未初始化，调用on_enter函数
    if(need_init)
    {
        current_state->on_enter();
        need_init = false;
    }

    // 调用on_update函数
    current_state->on_update(delta);
}

// 设置初始状态
void
StateMachine::set_entry(const std::string& id)
{
    current_state = state_pool[id];
}

// 切换状态
void
StateMachine::switch_to(const std::string& id)
{
    // 如果存在当前状态和目标状态
    if(current_state && state_pool[id])
    {
        current_state->on_exit();
        current_state = state_pool[id];
        current_state->on_enter();
    }
}

// 注册状态
void
StateMachine::register_state(const std::string& id, StateNode* state_node)
{
    state_pool[id] = state_node;
}
