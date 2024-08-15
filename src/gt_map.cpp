#include "gt_map.h"
#include "bn_log.h"
#include "gt_globals.h"
#include "gt_util.h"
#include "bn_regular_bg_ptr.h"

namespace gt
{
    map::map(bn::camera_ptr camera, map_info m_info):
    _bg(_m_info.bg_item.create_bg(0,0)),
    _m_info(m_info)
    {
        //_m_info=m_info;
        _bg.set_camera(camera);
        _column_count = _m_info.bg_item.map_item().dimensions().width();
        _row_count = _m_info.bg_item.map_item().dimensions().height();
        _mapobjects_origin_offset = bn::point( (_column_count/2)*-8, (_row_count/2)*-8);
        BN_LOG("map origin offset ",_mapobjects_origin_offset.x(),",",_mapobjects_origin_offset.y());
    }



    uint8_t map::mapobject_at(bn::point tile_coord)
    {
        BN_ASSERT(tile_coord.x()>=0 && tile_coord.y()>=0 &&
            tile_coord.x() < _column_count && tile_coord.y() < _row_count,
            "tile_coord out of range ",tile_coord.x());
        int index = (tile_coord.y()*_column_count)+tile_coord.x();
        return _m_info.mapobjects_data[index];
    }

    bn::point map::position_to_tile_coord(bn::fixed_point pos)
    {
        return bn::point((pos.x().right_shift_integer()-_mapobjects_origin_offset.x())/8,
         (pos.y().right_shift_integer()-_mapobjects_origin_offset.y())/8);
    }

    uint8_t map::position_to_mapobject(bn::fixed_point pos)
    {
        return mapobject_at(position_to_tile_coord(pos));
    }

    bool map::check_rect_collision(bn::fixed_rect rect)
    {

        bn::fixed_rect testrect(rect.position()-_mapobjects_origin_offset, rect.dimensions());

        for(bn::fixed i = testrect.left(); i < testrect.left() + testrect.width(); i += 8)
        for(bn::fixed j = testrect.top(); j < testrect.top() + testrect.height(); j += 8) {
           if(mapobject_at(bn::point(i.right_shift_integer()/8,j.right_shift_integer()/8))==1)
            return true;
       }
        return false;
    }



    void map::update()
    {
        
    }


    bn::regular_bg_ptr map::bg()
    {
        return _bg;
    }


}