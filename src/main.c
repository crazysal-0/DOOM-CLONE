#include <raylib.h>
#include "constants.h"
#include "player.h"

int main(void) {
        // Init
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_LABEL);

        Player player = player_make(PLAYER_STARTING_POSITION, PLAYER_SIZE, PLAYER_SPEED, PLAYER_COLOR);

        while (!WindowShouldClose()) {
                // Update
                float delta = GetFrameTime();

                player_update(&player, delta);

                // Draw
                BeginDrawing();

                player_draw(&player);

                ClearBackground(BACKGROUND_COLOR);

                EndDrawing();
        }

        CloseWindow();

        return 0;
}