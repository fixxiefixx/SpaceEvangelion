#ifndef GT_GO_WORM_BOMB_H
#define GT_GO_WORM_BOMB_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{

    class go_worm_bomb : public game_object
    {
        public:
            go_worm_bomb(bn::fixed_point pos, bn::point _tile_coord);
            virtual void update() override;
        
        private:
            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<2> _action;
            int explosion_timer = 15;
            bn::point tile_coord;
    };

}

#endif