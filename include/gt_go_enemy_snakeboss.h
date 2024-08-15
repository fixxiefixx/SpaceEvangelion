#ifndef GT_GO_ENEMY_SNAKEBOSS_H
#define GT_GO_ENEMY_SNAKEBOSS_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace gt
{
    class go_enemy_snakeboss : public game_object
    {
        public:
            go_enemy_snakeboss(bn::fixed_point spawnpos);
            ~go_enemy_snakeboss();
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            static constexpr const int body_count = 12;
            static constexpr const int hist_count_per_body = 30;
            static constexpr const int hist_count = hist_count_per_body * body_count;
            static constexpr const int shoot_interval = 2 * 60;
            static constexpr const int body_max_health = 20;
            static constexpr const int die_interval = 14;
            static constexpr const int change_target_interval = 5 * 60;

            struct body_info
            {
                bn::sprite_ptr sprite;
                int health = body_max_health;
                int blink_timer = 0;
                body_info();
            };

            bn::sprite_ptr _sprite;
            bn::vector<body_info, body_count> bodys;
            const int max_health = 200;
            int _health = max_health;
            int blink_timer = 0;
            int prepare_timer = 60*2;
            bn::fixed target_y = 0;
            int shoot_timer = shoot_interval;

            bn::fixed_point poshist[hist_count];
            int hist_index = 0;
            bool dying = false;
            int die_timer = die_interval;
            int die_index = 0;
            bool targeting_player = true;
            int change_target_timer = change_target_interval;
            bn::fixed_point target_pos = bn::fixed_point(0,0);


            void update_collision();
            void update_body(int body_index);
            void shoot_bullets_wide(bn::fixed_point pos, bn::fixed angle);
            void do_damage(int amount);
            void update_dying();
    };
}

#endif