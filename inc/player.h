#pragma once

#include <raylib.h>

typedef struct Player {
        Vector2 position;
        int size;
        float speed;
        Color color;
} Player;

Player player_make(Vector2 position, int size, float speed, Color color);

void player_draw(Player* pPlayer);

void player_update(Player* pPlayer, float delta);