#include "renderer.h"

#include <math.h>
#include <raylib.h>

#include "constants.h"
#include "renderer.h"

static float shoot_ray(const Level* level, const Player* player,
                       float direction) {
        float ray_x = sinf(direction);
        float ray_y = -cosf(direction);

        float x = player->position.x;
        float y = player->position.y;

        float distance = 0.0f;

        while (1) {
                x += ray_x;
                y += ray_y;
                distance += 1.0f;

                if (level_get_tile(level, x, y) == 1) break;
        }

        return distance;
}

void renderer_draw(const Level* level, const Player* player) {
        const float field_of_view = 1.0472f;
        const int ray_count = 100;

        for (int i = 0; i < ray_count; ++i) {
                float ray_offset =
                    -field_of_view / 2.0f + field_of_view * i / (ray_count - 1);

                float ray_direction = player->direction + ray_offset;

                float distance = shoot_ray(level, player, ray_direction);

                float corrected_distance =
                    distance * cosf(ray_direction - player->direction);

                float wall_height = 10000.0f / corrected_distance;

                float screen_x = (float)i / (ray_count - 1) * WINDOW_WIDTH;

                float screen_y = WINDOW_HEIGHT / 2.0f - wall_height / 2.0f;

                DrawRectangle(screen_x, screen_y,
                              WINDOW_WIDTH / (float)ray_count + 1.0f,
                              wall_height, WALL_COLOR);
        }
}