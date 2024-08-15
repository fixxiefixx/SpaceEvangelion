#ifndef GT_MAP_INFO_H
#define GT_MAP_INFO_H

#include "bn_regular_bg_item.h"
#include "gt_objinfo.h"
#include "definitions.h"

namespace gt
{
    class map_info
    {
        public:
            Def name;
            bn::regular_bg_item bg_item;
            const uint8_t* mapobjects_data;
            const objinfo *objinfos;
            uint16_t objinfo_count;

            map_info(Def _name,bn::regular_bg_item _bg_item, const uint8_t* _mapobjects_data,
                 uint16_t _area_count,
                const objinfo *_objinfos, uint16_t _objinfo_count);
    };
}

#endif