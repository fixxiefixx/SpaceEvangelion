#ifndef GT_GO_ENEMY_RAZOR_H
#define GT_GO_ENEMY_RAZOR_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace gt
{
    class go_enemy_razor : public game_object
    {
        public:
            go_enemy_razor(bn::fixed_point spawnpos);
            ~go_enemy_razor();
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            struct blade_info
            {
                bn::sprite_ptr sprite;
                bn::fixed_point attached_local_pos;
                bn::fixed rotspeed = 3;
                blade_info(bn::fixed_point _attached_local_pos);
                bool attached = true;
                bool flying_to_player = false;
                bn::fixed_point velocity = bn::fixed_point(0,0);
                int pause_timer = 0;
            };

            static constexpr const int max_health = 200;
            static constexpr const bn::fixed idle_rotspeed = 3;
            static constexpr const bn::fixed flying_rotspeed = 10;
            static constexpr const bn::fixed max_velocity = 2;
            static constexpr const int pause_duration = 4 * 60;
            static constexpr const int follow_player_duration = 1 * 60;

            bn::sprite_ptr _sprite;
            bn::vector<blade_info,4> blades;
            int _health = max_health;
            int blink_timer = 0;
            int prepare_timer = 60*2;
            int time = 0;
            

            void update_collision();
            void update_blade(int blade_index);
            void update_blade_collision(int blade_index);
    };
}

#endif