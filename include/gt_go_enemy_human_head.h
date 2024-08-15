#ifndef GT_GO_ENEMY_HUMAN_HEAD_H
#define GT_GO_ENEMY_HUMAN_HEAD_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_human_head : public game_object
    {
        public:
            go_enemy_human_head(bn::fixed_point spawnpos);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;
            virtual ~go_enemy_human_head() override;

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
                SPAWN_ICOBOTS,
                WAIT4,
                FLY_AWAY,
                DYING
            };

            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<60> _action;
            bn::sprite_ptr _sprite_charging;
            bn::sprite_animate_action<8> _charging_action;
            const int max_health = 1000;
            int _health = max_health;
            int blink_timer = 0;
            bn::fixed sin_at = 0;
            const int shoot_wide_interval = 30;
            const int shoot_burst_interval = 4;
            const int shoot_big_interval = 10;
            const int shoot_wide_count = 30;
            const int shoot_burst_count = 80;
            const int shoot_big_count = 34;
            int shoot_wide_counter = shoot_wide_count;
            int shoot_burst_counter = shoot_burst_count;
            int shoot_big_counter = shoot_big_count;
            int shoot_timer = 0;
            int prepare_timer = 2*60;
            const int wait1_time = 120;
            const int wait2_time = 60*2;
            const int charge_time = 60*2;
            const int wait3_time = 60*2;
            const int wait4_time = 60*4;
            const int spawn_icobots_count = 30;
            const int spawn_icobots_interval = 25;
            const int die_explosion_count = 80;
            const int die_explosion_interval = 6;
            int wait1_timer = wait1_time;
            int wait2_timer = wait2_time;
            int charge_timer = charge_time;
            int wait3_timer = wait3_time;
            int wait4_timer = wait4_time;
            int spawn_icobots_timer = spawn_icobots_interval;
            int spawn_icobots_counter = spawn_icobots_count;
            int die_explosion_counter = die_explosion_count;
            int die_explosion_timer = die_explosion_interval;

            int sequence_count = 200;
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