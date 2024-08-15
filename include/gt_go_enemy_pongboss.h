#ifndef GT_GO_ENEMY_PONGBOSS_H
#define GT_GO_ENEMY_PONGBOSS_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"

namespace gt
{
    class go_enemy_pongboss : public game_object
    {
        public:
            go_enemy_pongboss(bn::fixed_point spawnpos);
            ~go_enemy_pongboss();
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

        private:
            enum MOVE_STATE{
                MOVE_TO_CENTER,
                MOVING_AROUND,
                WOBBLE_AT_CENTER,
                SEPERATING,
                PLAYING_PONG,
                DESTROYING
            };

            const int health_seperating = 100;
            const bn::fixed left_x = -240/2 + 16;
            const bn::fixed right_x = 240/2 - 16;
            const bn::fixed ball_min_x = -(240) + 14;
            const bn::fixed ball_max_x = 240 - 14;
            const bn::fixed ball_min_y = -(160/2) + 14;
            const bn::fixed ball_max_y = 160/2 - 8 - 14;
            const bn::fixed paddle_min_y = -160/2 + 64/2;
            const bn::fixed paddle_max_y = 160/2 - 64/2 - 8;
            const bn::fixed_rect local_paddle_left_rect = bn::fixed_rect(-8,0,16,64);
            const bn::fixed_rect local_paddle_right_rect = bn::fixed_rect(8,0,16,64);

            bn::sprite_ptr _sprite;
            bn::optional<bn::sprite_ptr> sprite_left = bn::optional<bn::sprite_ptr>();
            bn::optional<bn::sprite_ptr> sprite_right = bn::optional<bn::sprite_ptr>();
            const int max_health = 200;
            int _health = max_health;
            int blink_timer = 0;
            MOVE_STATE move_state = MOVE_STATE::MOVE_TO_CENTER;
            int time = 0;
            int wobble_timer = 3 * 60;
            const int shoot_duration = 60;
            const int shoot_duration_angry = 30;
            int shoot_timer = shoot_duration;
            const int ball_shoot_duration = 2*60;
            int ball_shoot_timer = ball_shoot_duration;
            int seperating_timer = 60;
            int destroy_explosion_counter = 30;
            const int destroy_explosion_interval = 10;
            int destroy_explosion_timer = 0;
            bn::fixed_point velocity = bn::fixed_point(0,0);

            void update_collision();
            void update_move_to_center();
            void update_moving_around();
            void update_wobble_at_center();
            void update_seperating();
            void update_playing_pong();
            void update_destroying();
            void shoot_bullets_wide(bn::fixed_point pos,bool left);
            void update_ball_bounce(bn::fixed_rect ball_rect, bn::fixed_rect paddle_rect);
    };
}

#endif