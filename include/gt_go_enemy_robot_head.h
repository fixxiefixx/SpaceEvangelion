#ifndef GT_GO_ENEMY_ROBOT_HEAD_H
#define GT_GO_ENEMY_ROBOT_HEAD_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_robot_head : public game_object
    {
        public:
            go_enemy_robot_head(bn::fixed_point spawnpos);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;
            virtual ~go_enemy_robot_head() override;

        private:
            enum MOVE_STATE
            {
                PREPARING,
                SHOOT_WIDE,
                WAIT1,
                SHOOT_BURST,
                WAIT2,
                CHARGING,
                SHOOT_BIG,
                WAIT3,
                FLY_AWAY
            };

            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<4> _action;
            bn::sprite_ptr _sprite_charging;
            bn::sprite_animate_action<8> _charging_action;
            const int max_health = 200;
            int _health = max_health;
            int blink_timer = 0;
            bn::fixed sin_at = 0;
            const int shoot_wide_interval = 30;
            const int shoot_burst_interval = 4;
            const int shoot_big_interval = 10;
            const int shoot_wide_count = 5;
            const int shoot_burst_count = 20;
            const int shoot_big_count = 20;
            int shoot_wide_counter = shoot_wide_count;
            int shoot_burst_counter = shoot_burst_count;
            int shoot_big_counter = shoot_big_count;
            int shoot_timer = 0;
            int prepare_timer = 2*60;
            const int wait1_time = 120;
            const int wait2_time = 60*2;
            const int charge_time = 60*2;
            const int wait3_time = 60*2;
            int wait1_timer = wait1_time;
            int wait2_timer = wait2_time;
            int charge_timer = charge_time;
            int wait3_timer = wait3_time;
            int sequence_count = 2;
            bn::fixed velocity_y = 0;

            MOVE_STATE move_state = MOVE_STATE::PREPARING;

            void shoot_bullets_wide();
            void shoot_bullet_burst();
            void shoot_big();
            void follow_player();
            void fly_away();
    };
}

#endif