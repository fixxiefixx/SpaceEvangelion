#ifndef GT_GO_ENEMY_ICOBOT_H
#define GT_GO_ENEMY_ICOBOT_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_icobot : public game_object
    {
        public:
            go_enemy_icobot(bn::fixed_point spawnpos, bool warp_in);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            bn::sprite_ptr _sprite;
            int _health = 5;
            int blink_timer = 0;
            int prepare_timer = 80;
            bn::sprite_animate_action<19> _action;
            bn::fixed_point velocity = bn::fixed_point(-2.5,0);
            bn::fixed_point target_velocity = bn::fixed_point(0,0);
            bool warping_in = false;

            void update_collision();
    };
}

#endif