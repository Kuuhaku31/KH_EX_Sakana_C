
// dead.cpp

#include "enemy_state_nodes.h"

void
EnemyDeadState::on_enter()
{
    MessageBox(GetHWnd(), _T("敌人死亡"), _T("挑战成功！"), MB_OK);
    exit(0);
}
