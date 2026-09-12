#pragma once

#include <stdint.h>

typedef struct {
        uint8_t width;
        uint8_t height;
        uint8_t* tiles;
} Level;

Level level_load(const char* filename);
void level_free(Level* level);
void level_draw(Level* level);
uint8_t level_get_tile(const Level* level, float x, float y);