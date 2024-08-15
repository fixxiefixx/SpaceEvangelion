#ifndef GT_MAP_H
#define GT_MAP_H

#include "bn_core.h"
#include "bn_point.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"
#include "bn_rect.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_camera_ptr.h"
#include "gt_map_info.h"

namespace gt
{
    class map
    {
        public:
            map(bn::camera_ptr camera, map_info m_info);
            uint8_t mapobject_at(bn::point tile_coord);
            bn::point position_to_tile_coord(bn::fixed_point pos);
            uint8_t position_to_mapobject(bn::fixed_point pos);
            bool check_rect_collision(bn::fixed_rect rect);
            void update();
            bn::regular_bg_ptr bg();

        private:
            map_info _m_info;
            int _column_count;
            int _row_count;
            bn::point _mapobjects_origin_offset;
            bn::regular_bg_ptr _bg;
            void apply_camera_bounds(bn::fixed_rect area);
    };
}

#endif