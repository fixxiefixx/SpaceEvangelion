#ifndef GT_GLOBALS_H
#define GT_GLOBALS_H

#include "bn_random.h"
#include "gt_game_object_manager.h"
#include "gt_go_player.h"
#include "gt_map.h"
#include "gt_collision_map.h"
#include "gt_level.h"
#include "gt_ui.h"
#include "gt_go_tilemap.h"

namespace gt::globals
{
    extern gt::game_object_manager *go_manager;
    extern gt::go_player *player;
    extern bn::random *random;
    extern bn::camera_ptr *camera;
    extern bool scene_running;
    extern gt::map *current_map;
    extern gt::collision_map *collision_map_player_bullets;
    extern gt::collision_map *collision_map_enemy_bullets;
    extern gt::level *lvl;
    extern gt::ui *ui;
    extern bool game_running;
    extern unsigned int time;
    extern gt::go_tilemap *tilemap;
    extern bool restart_scene;
}

#endif