#ifndef GT_COLLISION_MAP_H
#define GT_COLLISION_MAP_H

#include "gt_game_object.h"
#include "bn_vector.h"
#include "bn_list.h"

namespace gt
{
    class collision_map
    {
        public:
            void add_game_object(game_object *obj);
            void update_game_object(game_object *obj, bn::fixed_point previous_pos);
            void remove_game_object(game_object *obj);
            void clear();
            game_object* get_colliding_object(game_object *obj);
            bn::vector<game_object*,4> get_colliding_objects(game_object *obj);
            game_object* get_colliding_object(bn::fixed_rect rect);
            bn::vector<game_object*,4> get_colliding_objects(bn::fixed_rect rect);
            int get_object_count();
            void log_cells();
        
        private:
            struct grid_coord
            {
                int x;
                int y;
            };

            static const unsigned int map_width = 240;
            static const unsigned int map_heigth = 160;
            static const unsigned int cell_size = 32;
            static const unsigned int cell_count_x = (map_width / cell_size)+1;
            static const unsigned int cell_count_y = (map_heigth / cell_size)+1;

            bn::vector<game_object*,4> _map[cell_count_x * cell_count_y];

            void remove_from_cell(game_object *obj, bn::vector<game_object*,4> *cell);
            grid_coord point_to_grid_coord(const bn::fixed_point pos);
            bn::vector<game_object*,4>* grid_coord_to_cell(const grid_coord coord);
    };
}

#endif