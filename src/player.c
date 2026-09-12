#include "player.h"

Player player_make(Vector2 position, int size, float speed, Color color) {
        return (Player){
                .position = position,
                .size = size,
                .speed = speed,
                .color = color,
        };
}

void player_draw(Player* pPlayer) {
        DrawRectangle(pPlayer->position.x, pPlayer->position.y, pPlayer->size, pPlayer->size,  pPlayer->color);
}

void player_update(Player* pPlayer, float delta) {
        if (IsKeyDown(KEY_W)) pPlayer->position.y -= pPlayer->speed * delta;
        if (IsKeyDown(KEY_S)) pPlayer->position.y += pPlayer->speed * delta;
        if (IsKeyDown(KEY_A)) pPlayer->position.x -= pPlayer->speed * delta;
        if (IsKeyDown(KEY_D)) pPlayer->position.x += pPlayer->speed * delta;
}