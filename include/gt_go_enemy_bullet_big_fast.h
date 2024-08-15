#ifndef GT_GO_ENEMY_BULLET_BIG_FAST_H
#define GT_GO_ENEMY_BULLET_BIG_FAST_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_bullet_big_fast : public game_object
    {
        public:
            go_enemy_bullet_big_fast(bn::fixed_point spawnpos,bn::fixed_point velocity);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;
            ~go_enemy_bullet_big_fast();
        private:
            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<4> _action;
            bn::fixed_point _velocity;
    };
}

#endif