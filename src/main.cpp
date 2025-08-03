#include <raylib.h>
#include "game.h"
#include "colors.h"

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main() {
    InitWindow(500, 620, "Tetris"); // initialize game window (width, height, title)
    SetTargetFPS(60); // 60 frames per second

    Font font = LoadFontEx("Font/ARCADECLASSIC.TTF", 64, 0, 0);

    Game game = Game();

    while(WindowShouldClose() == false) {
        game.HandleInput();
        if (EventTriggered(0.3)) {
            game.MoveBlockDown();
        }
        BeginDrawing();
        ClearBackground(lightPurple);
        DrawTextEx(font, "Score", {362, 15}, 30, 2, WHITE);
        DrawTextEx(font, "Next", {372, 175}, 30, 2, WHITE);
        if (game.gameOver) {
            DrawTextEx(font, "Game Over", {332, 450}, 30, 2, WHITE);
        }
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lighterPurple);
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lighterPurple);
        game.Draw();
        EndDrawing();
    }

    CloseWindow(); // You have to destroy window at some point
}
