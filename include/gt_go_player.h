#ifndef GT_GO_PLAYER_H
#define GT_GO_PLAYER_H

#include "gt_game_object.h"
#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"
#include "bn_fixed_point.h"

namespace gt
{
    class go_player : public game_object
    {
        public:
            go_player(const bn::camera_ptr& camera);
            virtual void update() override;
            virtual void do_damage(int amount) override;
            virtual bn::optional<bn::fixed_point> get_pos() override;
            bool is_vulnerable();
            void collect_item(Def item);
            void add_bubble_time(int time);
            void add_overcharge_time(int time);
        private:

            enum MOVE_STATE{
                MOVING,
                DYING
            };

            bn::sprite_ptr _sprite;
            bn::sprite_ptr _bubble_sprite;
            bn::sprite_animate_action<2> _action;
            int shoot_timer = 0;
            int blink_timer = 0;
            bn::fixed_point velocity = bn::fixed_point(0,0);
            MOVE_STATE move_state = MOVE_STATE::MOVING;
            int dying_timer = 60*2;
            int bubble_timer = 0;
            int overcharge_timer = 0;

            void update_moving();
            void update_shooting();
            void update_collision();
            void update_dying();
            void get_out_of_tilemap();
    };
}

#endif