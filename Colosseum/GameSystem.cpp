#include <stdio.h>
#include <windows.h>
#include <random>

#include "RenderSystem.h"
#include "GameSystem.h"
#include "TimeSystem.h"
namespace global {
    namespace game {
        void StartGame()
        {
            render::InitScreen(); // ȭ�� ���� ���� / ���� ���� / Ŀ�� ���� �� �⺻ ����
            game::playerMovableRect = render::GetPlayerMovableRect();
            //render::DrawBorder(); // ���� �׷� ����!

            // �÷��̾� ���� ��ġ ����
            game::prePlayerPos.X = game::playerMovableRect.Left + (game::playerMovableRect.Left + game::playerMovableRect.Right) / 2;
            game::prePlayerPos.Y = game::playerMovableRect.Bottom - 2;

            game::curPlayerPos.X = game::prePlayerPos.X;
            game::curPlayerPos.Y = game::prePlayerPos.Y;

            //// �밡�ٷ� ����� ��
            //for (int i = 0; i < global::ENEMY_CNT; i++)
            //{
            //    global::consoleEnemy[i].character = 'A' + i;
            //    global::consoleEnemy[i].localPos.X = i * 10;
            //    global::consoleEnemy[i].localPos.Y = 0; // Y �� ����.
            //}
            render::DrawPlayer();
            //DrawEnemy();
        }

    }
}
