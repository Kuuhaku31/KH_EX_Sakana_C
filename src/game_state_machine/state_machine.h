
// state_machine.h

#pragma once

#include "state_node.h"

#include <string>
#include <unordered_map>

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void on_update(float delta);

    void set_entry(const std::string& id);                             // 设置初始状态
    void switch_to(const std::string& id);                             // 切换状态
    void register_state(const std::string& id, StateNode* state_node); // 注册状态

private:
    bool       need_init     = true;    // 是否需要初始化
    StateNode* current_state = nullptr; // 当前状态

    // 状态池
    std::unordered_map<std::string, StateNode*> state_pool;
};
