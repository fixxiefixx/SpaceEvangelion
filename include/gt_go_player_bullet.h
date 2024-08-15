#ifndef GT_GO_PLAYER_BULLET_H
#define GT_GO_PLAYER_BULLET_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_player_bullet : public game_object
    {
        public:
            go_player_bullet(bn::fixed_point spawnpos, bn::fixed yoffset);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;
            virtual ~go_player_bullet() override;
            
        private:
            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<3> _action;
            bn::fixed _yoffset;
            int wait_frames = 2;
    };
}

#endif