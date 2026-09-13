#include "player.h"

#include <math.h>

#include "constants.h"
#include "level.h"

static bool player_can_move(const Level* pLevel, const Player* pPlayer, float x,
                            float y) {
        float radius = pPlayer->size;

        int min_x = (int)floorf((x - radius) / GRID_SIZE);
        int max_x = (int)floorf((x + radius) / GRID_SIZE);
        int min_y = (int)floorf((y - radius) / GRID_SIZE);
        int max_y = (int)floorf((y + radius) / GRID_SIZE);

        for (int tile_y = min_y; tile_y <= max_y; ++tile_y) {
                for (int tile_x = min_x; tile_x <= max_x; ++tile_x) {
                        if (tile_x < 0 || tile_x >= pLevel->width ||
                            tile_y < 0 || tile_y >= pLevel->height)
                                return false;

                        if (pLevel->tiles[tile_y * pLevel->width + tile_x] != 1)
                                continue;

                        float wall_left = tile_x * GRID_SIZE;
                        float wall_right = wall_left + GRID_SIZE;
                        float wall_top = tile_y * GRID_SIZE;
                        float wall_bottom = wall_top + GRID_SIZE;

                        float closest_x =
                            fmaxf(wall_left, fminf(x, wall_right));
                        float closest_y =
                            fmaxf(wall_top, fminf(y, wall_bottom));

                        float dx = x - closest_x;
                        float dy = y - closest_y;

                        if (dx * dx + dy * dy < radius * radius) return false;
                }
        }

        return true;
}

Player player_make(Vector2 position, int size, float speed, float turn_speed,
                   Color color) {
        return (Player){
            .position = position,
            .size = size,
            .speed = speed,
            .turn_speed = turn_speed,
            .color = color,
        };
}

void player_draw(Player* pPlayer) {
        DrawCircle(pPlayer->position.x, pPlayer->position.y, pPlayer->size,
                   pPlayer->color);
}

void player_update(Player* pPlayer, const Level* pLevel, float delta) {
        pPlayer->direction += GetMouseDelta().x * 0.003f;

        float move_x = 0.0f;
        float move_y = 0.0f;

        if (IsKeyDown(KEY_W)) {
                move_x += sinf(pPlayer->direction);
                move_y -= cosf(pPlayer->direction);
        }

        if (IsKeyDown(KEY_S)) {
                move_x -= sinf(pPlayer->direction);
                move_y += cosf(pPlayer->direction);
        }

        if (IsKeyDown(KEY_A)) {
                move_x -= cosf(pPlayer->direction);
                move_y -= sinf(pPlayer->direction);
        }

        if (IsKeyDown(KEY_D)) {
                move_x += cosf(pPlayer->direction);
                move_y += sinf(pPlayer->direction);
        }

        if (move_x != 0.0f || move_y != 0.0f) {
                float next_x =
                    pPlayer->position.x + move_x * pPlayer->speed * delta;

                float next_y =
                    pPlayer->position.y + move_y * pPlayer->speed * delta;

                if (player_can_move(pLevel, pPlayer, next_x, next_y)) {
                        pPlayer->position.x = next_x;
                        pPlayer->position.y = next_y;
                }
        }
}