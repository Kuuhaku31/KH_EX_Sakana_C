
// dead.cpp

#include "enemy_state_nodes.h"

void
EnemyDeadState::on_enter()
{
    MessageBox(GetHWnd(), _T("YOU WIN!"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
    exit(0);
}
