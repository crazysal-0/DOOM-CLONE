#include "player.h"

#include <math.h>

#include "constants.h"
#include "level.h"

static bool player_can_move(const Level* level, float x, float y) {
        float right = x + PLAYER_SIZE - 1;
        float bottom = y + PLAYER_SIZE - 1;

        return level_get_tile(level, x, y) == 0 &&
               level_get_tile(level, right, y) == 0 &&
               level_get_tile(level, x, bottom) == 0 &&
               level_get_tile(level, right, bottom) == 0;
}

Player player_make(Vector2 position, int size, float speed, Color color) {
        return (Player){
            .position = position,
            .size = size,
            .speed = speed,
            .color = color,
        };
}

void player_draw(Player* pPlayer) {
        DrawRectangle(pPlayer->position.x, pPlayer->position.y, pPlayer->size,
                      pPlayer->size, pPlayer->color);
}

void player_update(Player* pPlayer, const Level* pLevel, float delta) {
        if (IsKeyDown(KEY_W)) {
                float next_y = pPlayer->position.y - pPlayer->speed * delta;

                if (player_can_move(pLevel, pPlayer->position.x, next_y))
                        pPlayer->position.y = next_y;
        }

        if (IsKeyDown(KEY_S)) {
                float next_y = pPlayer->position.y + pPlayer->speed * delta;

                if (player_can_move(pLevel, pPlayer->position.x, next_y))
                        pPlayer->position.y = next_y;
        }

        if (IsKeyDown(KEY_A)) {
                float next_x = pPlayer->position.x - pPlayer->speed * delta;

                if (player_can_move(pLevel, next_x, pPlayer->position.y))
                        pPlayer->position.x = next_x;
        }

        if (IsKeyDown(KEY_D)) {
                float next_x = pPlayer->position.x + pPlayer->speed * delta;

                if (player_can_move(pLevel, next_x, pPlayer->position.y))
                        pPlayer->position.x = next_x;
        }
}