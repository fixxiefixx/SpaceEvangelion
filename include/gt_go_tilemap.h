#ifndef GT_GO_TILEMAP
#define GT_GO_TILEMAP

#include "gt_game_object.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"

namespace gt
{
    class go_tilemap : public game_object
    {
        public:
            go_tilemap(int map_index);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;
            bool do_bullet_collision(bn::fixed_point pos);
            bn::point pos_to_cell_coord(bn::fixed_point pos);
            bn::fixed_point cell_coord_to_pos(bn::point cell_coord);
            bool is_cell_pos_inside_grid(bn::point cell_pos);
            bool is_colliding_with(bn::fixed_rect rect);
            void tile_explosion(bn::point cell_pos);
            ~go_tilemap();
        
        private:
            void update_cell(int cell_x, int cell_y);

            bn::regular_bg_map_cell *cells; // Important: align with size of int
            bn::regular_bg_map_item map_item;
            bn::regular_bg_item bg_item;
            bn::regular_bg_ptr bg_ptr;
            bn::regular_bg_map_ptr bg_map;
            bn::regular_bg_map_cell cell_empty = 0;
            bn::regular_bg_map_cell cell_stick_up = 0;
            bn::regular_bg_map_cell cell_stick_down = 0;
            bn::regular_bg_map_cell cell_stick_left = 0;
            bn::regular_bg_map_cell cell_stick_right = 0;
            bn::regular_bg_map_cell cell_pipe_horizontal = 0;
            bn::regular_bg_map_cell cell_pipe_vertical = 0;
            bn::regular_bg_map_cell cell_corner_top_left = 0;
            bn::regular_bg_map_cell cell_corner_top_right = 0;
            bn::regular_bg_map_cell cell_corner_bottom_left = 0;
            bn::regular_bg_map_cell cell_corner_bottom_right = 0;
            bn::regular_bg_map_cell cell_edge_up = 0;
            bn::regular_bg_map_cell cell_edge_down = 0;
            bn::regular_bg_map_cell cell_edge_left = 0;
            bn::regular_bg_map_cell cell_edge_right = 0;
            bn::regular_bg_map_cell cell_full = 0;
            bn::regular_bg_map_cell cell_tumor = 0;
            bn::regular_bg_map_cell cell_enemy = 0;
            bn::regular_bg_map_cell cell_explosive = 0;
            bool needs_rebuild = false;
    };
}

#endif