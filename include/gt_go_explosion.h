#ifndef GT_GO_EXPLOSION_H
#define GT_GO_EXPLOSION_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{

    class go_explosion : public game_object
    {
        public:
            go_explosion(bn::fixed_point pos);
            virtual void update() override;
        
        private:
            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<14> _action;
    };

}

#endif