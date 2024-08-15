#include "gt_globals.h"
#include "bn_random.h"
#include "gt_game_object_manager.h"
#include "gt_map.h"
#include "gt_go_player.h"
#include "gt_collision_map.h"

namespace gt::globals
{
    gt::game_object_manager *go_manager = nullptr;
    gt::go_player *player = nullptr;
    bn::random *random = nullptr;
    bn::camera_ptr *camera = nullptr;
    bool scene_running = false;
    gt::map *current_map = nullptr;
    gt::collision_map *collision_map_player_bullets = nullptr;
    gt::collision_map *collision_map_enemy_bullets = nullptr;
    gt::level *lvl=nullptr;
    gt::ui *ui=nullptr;
    bool game_running=false;
    unsigned int time = 0;
    gt::go_tilemap *tilemap = nullptr;
    bool restart_scene = false;
}