#ifndef GT_GO_ENEMY_CYLINDER_H
#define GT_GO_ENEMY_CYLINDER_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_cylinder : public game_object
    {
        public:
            go_enemy_cylinder(bn::fixed_point spawnpos);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            enum MOVE_STATE
            {
                PREPARING,
                BREAKING,
                SHOOTING,
                WAITING,
                ACCELERATING
            };


            bn::sprite_ptr _sprite;
            int _health = 10;
            int blink_timer = 0;
            int prepare_timer = 40;
            bn::fixed velocity_y = -1;
            const int shoot_interval = 3;
            int shoot_timer = 0;
            int shoot_count = 10;
            int wait_timer = 60;
            MOVE_STATE move_state = MOVE_STATE::PREPARING;

            void update_collision();
    };
}

#endif