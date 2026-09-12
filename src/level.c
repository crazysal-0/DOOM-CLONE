#include "level.h"

#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

Level level_load(const char* filename) {
        FILE* level_file = fopen(filename, "rb");

        if (!level_file) {
                fprintf(stderr, "Failed to open level file: %s\n", filename);
                return (Level){0};
        }

        Level level;

        if (fread(&level.width, sizeof(uint8_t), 1, level_file) != 1 ||
            fread(&level.height, sizeof(uint8_t), 1, level_file) != 1) {
                fprintf(stderr, "Failed to read level dimensions\n");
                fclose(level_file);
                return (Level){0};
        }

        size_t tile_count = level.width * level.height;

        level.tiles = malloc(tile_count);

        if (!level.tiles) {
                fprintf(stderr, "Failed to allocate memory for level tiles\n");
                fclose(level_file);
                return (Level){0};
        }

        if (fread(level.tiles, sizeof(uint8_t), tile_count, level_file) !=
            tile_count) {
                fprintf(stderr, "Failed to read level tiles\n");
                free(level.tiles);
                fclose(level_file);
                return (Level){0};
        }

        fclose(level_file);

        return level;
}

void level_free(Level* level) {
        free(level->tiles);
        level->tiles = NULL;
}

void level_draw(Level* level) {
        for (uint8_t y = 0; y < level->height; ++y) {
                for (uint8_t x = 0; x < level->width; ++x) {
                        uint8_t tile = level->tiles[y * level->width + x];

                        if (tile == 1) {
                                DrawRectangle(x * LEVEL_GRID_SIZE,
                                              y * LEVEL_GRID_SIZE,
                                              LEVEL_GRID_SIZE, LEVEL_GRID_SIZE,
                                              LEVEL_GRID_COlOR);
                        }
                }
        }
}

uint8_t level_get_tile(const Level* level, float x, float y) {
        int tile_x = x / GRID_SIZE;
        int tile_y = y / GRID_SIZE;

        if (tile_x < 0 || tile_x >= level->width || tile_y < 0 ||
            tile_y >= level->height)
                return 1;

        return level->tiles[tile_y * level->width + tile_x];
}