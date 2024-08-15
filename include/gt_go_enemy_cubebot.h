#ifndef GT_GO_ENEMY_CUBEBOT_H
#define GT_GO_ENEMY_CUBEBOT_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"
#include "gt_globals.h"

namespace gt
{
    class go_enemy_cubebot : public game_object
    {
        public:
            go_enemy_cubebot(bn::fixed_point spawnpos);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<8> _action;
            int _health = 5;
            int blink_timer = 0;
            bn::fixed velocity_x = -2.5;
            bn::fixed velocity_y = 0;
    };
}

#endif