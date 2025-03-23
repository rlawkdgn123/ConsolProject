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
            render::InitScreen(); // 화면 영역 지정 / 버퍼 생성 / 커서 제한 등 기본 설정
            game::playerMovableRect = render::GetPlayerMovableRect();
            //render::DrawBorder(); // 벽을 그려 놓자!

            // 플레이어 시작 위치 설정
            game::prePlayerPos.X = game::playerMovableRect.Left + (game::playerMovableRect.Left + game::playerMovableRect.Right) / 2;
            game::prePlayerPos.Y = game::playerMovableRect.Bottom - 2;

            game::curPlayerPos.X = game::prePlayerPos.X;
            game::curPlayerPos.Y = game::prePlayerPos.Y;

            //// 노가다로 만드는 적
            //for (int i = 0; i < global::ENEMY_CNT; i++)
            //{
            //    global::consoleEnemy[i].character = 'A' + i;
            //    global::consoleEnemy[i].localPos.X = i * 10;
            //    global::consoleEnemy[i].localPos.Y = 0; // Y 는 고정.
            //}
            render::DrawPlayer();
            //DrawEnemy();
        }

    }
}
