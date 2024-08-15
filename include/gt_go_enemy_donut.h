#ifndef GT_GO_ENEMY_DONUT_H
#define GT_GO_ENEMY_DONUT_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_donut : public game_object
    {
        public:
            go_enemy_donut(bn::fixed_point spawnpos);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            bn::sprite_ptr _sprite;
            int _health = 5;
            int blink_timer = 0;
            int prepare_timer = 60*2;
            bn::sprite_animate_action<19> _action;
            bn::fixed target_y = 0;

            void update_collision();
    };
}

#endif