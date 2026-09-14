#include <raylib.h>

#include "constants.h"
#include "level.h"
#include "player.h"
#include "renderer.h"

int main(void) {
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_LABEL);
        SetTargetFPS(60);

        ToggleFullscreen();
        DisableCursor();

        RenderTexture2D target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

        Player player =
            player_make(PLAYER_STARTING_POSITION, PLAYER_SIZE, PLAYER_SPEED,
                        PLAYER_TURN_SPEED, PLAYER_COLOR);

        Level level = level_load("res/test.dat");

        if (!level.tiles) {
                UnloadRenderTexture(target);
                CloseWindow();
                return 1;
        }

        while (!WindowShouldClose()) {
                float delta = GetFrameTime();

                player_update(&player, &level, delta);

                BeginTextureMode(target);

                ClearBackground(BLACK);

                renderer_draw(&level, &player);

                EndTextureMode();

                BeginDrawing();

                ClearBackground(BLACK);

                DrawTexturePro(target.texture,
                               (Rectangle){
                                   0,
                                   0,
                                   WINDOW_WIDTH,
                                   -WINDOW_HEIGHT,
                               },
                               (Rectangle){
                                   0,
                                   0,
                                   GetScreenWidth(),
                                   GetScreenHeight(),
                               },
                               (Vector2){
                                   0,
                                   0,
                               },
                               0.0f, WHITE);

                EndDrawing();
        }

        level_free(&level);
        UnloadRenderTexture(target);
        CloseWindow();

        return 0;
}