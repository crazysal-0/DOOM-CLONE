#pragma once

#include <raylib.h>

#include "level.h"

typedef struct Player {
        Vector2 position;
        float direction;
        int size;
        float speed;
        float turn_speed;
        Color color;
} Player;

Player player_make(Vector2 position, int size, float speed, float turn_speed,
                   Color color);

void player_draw(Player* pPlayer);

void player_update(Player* pPlayer, const Level* pLevel, float delta);
