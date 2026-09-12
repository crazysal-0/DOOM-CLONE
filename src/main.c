#include <raylib.h>

#include "constants.h"
#include "level.h"
#include "player.h"

int main(void) {
        // Init
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_LABEL);

        Player player = player_make(PLAYER_STARTING_POSITION, PLAYER_SIZE,
                                    PLAYER_SPEED, PLAYER_COLOR);
        Level level = level_load("res/test.dat");
        if (!level.tiles) {
                CloseWindow();
                return 1;
        }

        while (!WindowShouldClose()) {
                // Update
                float delta = GetFrameTime();

                player_update(&player, &level, delta);

                // Draw
                BeginDrawing();

                ClearBackground(BLACK);

                level_draw(&level);
                player_draw(&player);

                EndDrawing();
        }

        CloseWindow();

        return 0;
}  //