#include "gt_go_player.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_player_small.h"
#include "bn_sprite_items_green_bubble.h"
#include "bn_keypad.h"
#include "gt_globals.h"
#include "gt_go_player_bullet.h"
#include "bn_log.h"
#include "bn_sound_items.h"
#include "bn_music.h"

#define __ go_player

#define MOVE_SPEED 1.5
#define SHOOT_INTERVAL 8
#define BLINK_DURATION 60

namespace
{
    bn::sprite_animate_action<2> _create_animation_flap(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 4, bn::sprite_items::player_small.tiles_item(), 0, 1);
    }
}

namespace gt
{
    __::go_player(const bn::camera_ptr &camera):
        _sprite(bn::sprite_items::player_small.create_sprite(-50,0)),
        _bubble_sprite(bn::sprite_items::green_bubble.create_sprite(-50,0)),
        _action(_create_animation_flap(_sprite))
    {
        _local_collision_rect=bn::optional<bn::fixed_rect>(bn::fixed_rect(-1.5, 0.5, 6, 6));
        _bubble_sprite.set_visible(false);
    }


    void __::update_moving()
    {
        bn::fixed_point pos = _sprite.position();
        bn::fixed_point dir;
        if(bn::keypad::up_held())
        {
            dir.set_y(-MOVE_SPEED);
        }
        if(bn::keypad::down_held())
        {
            dir.set_y(MOVE_SPEED);
        }
        if(bn::keypad::left_held())
        {
            dir.set_x(-MOVE_SPEED);
        }
        if(bn::keypad::right_held())
        {
            dir.set_x(MOVE_SPEED);
        }
        pos += dir;

        if(pos.x() < -(240 / 2))
            pos.set_x(-(240 / 2));

        if(pos.x() > 240 / 2)
            pos.set_x(240 / 2);

        if(pos.y() < -(160/2))
            pos.set_y(-(160/2));

        if(pos.y() > (160 / 2-8))
            pos.set_y((160 / 2)-8);
        
        if(globals::tilemap != nullptr)
        {
            if(globals::tilemap->is_colliding_with(*get_collision_rect()))
            {
                //get_out_of_tilemap();
                return;
            }
        }

        _sprite.set_position(pos);
    }

    void __::update_shooting()
    {
        if(shoot_timer > 0)
            shoot_timer--;
        
        if((bn::keypad::a_held() || bn::keypad::b_held() || overcharge_timer > 0) && shoot_timer <= 0)
        {
            bn::sound_items::playershoot.play(0.3);
            shoot_timer = SHOOT_INTERVAL;
            int weapon_level = 0;
            if(bn::keypad::b_held() || overcharge_timer > 0)
            {
                weapon_level = (globals::ui->get_power()+9)/10;
                if(overcharge_timer <= 0)
                    globals::ui->set_power(bn::max(globals::ui->get_power() - 1,0));
            }
            if(weapon_level == 0)
            {
            globals::go_manager->add_object(
                new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),0));
            }
            else if(weapon_level == 1)
            {
                globals::go_manager->add_object(
                    new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),-4));
                globals::go_manager->add_object(
                    new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),4));
            }
            else if(weapon_level >= 2)
            {
                globals::go_manager->add_object(
                new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),0));
                globals::go_manager->add_object(
                    new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),-8));
                globals::go_manager->add_object(
                    new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),8));
            }

            if(weapon_level >= 3)
            {
                globals::go_manager->add_object(
                    new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),-100));
                globals::go_manager->add_object(
                    new go_player_bullet(_sprite.position() + bn::fixed_point(5,-1),100));
            }
            
        }
    }

    void __::update_collision()
    {
        game_object* bullet = globals::collision_map_enemy_bullets->get_colliding_object(this);
        if(bullet != nullptr)
        {
            do_damage(1);
        }
        else if(globals::tilemap != nullptr)
        {
            if(globals::tilemap->is_colliding_with(*get_collision_rect()))
            {
                do_damage(1);

                get_out_of_tilemap();
            }
        }
        
    }

    void __::update_dying()
    {
        dying_timer--;
        if(dying_timer <= 0)
        {
            globals::ui->game_over();
        }
        else
        {
            velocity.set_y(velocity.y() + bn::fixed(0.05));
            bn::fixed_point pos = _sprite.position();
            pos += velocity;
            _sprite.set_position(pos);
        }
        
    }

    void __::update()
    {
        switch(move_state)
        {
            case MOVE_STATE::MOVING:
            {
                update_moving();
                update_shooting();
                update_collision();
            }break;
            case MOVE_STATE::DYING:
            {
                update_dying();
            }break;
        }

        if(bubble_timer > 0)
        {
            _bubble_sprite.set_position(_sprite.position());
            bubble_timer--;
            if(bubble_timer<=0)
            {
                _bubble_sprite.set_visible(false);
            }
            else
            {
                _bubble_sprite.set_visible(!_bubble_sprite.visible());
            }
        }

        if(overcharge_timer > 0)
            overcharge_timer--;
        

        if(!_action.done())
            _action.update();
        
        _sprite.set_visible((blink_timer/2)%2 == 0);
        if(blink_timer > 0)
        {
            blink_timer--;
        }
    }

    void __:: do_damage(int amount)
    {
        if(blink_timer <= 0 && globals::ui->health() > 0)
        {
            if(bubble_timer > 0)
            {
                blink_timer = BLINK_DURATION;
                bubble_timer = 0;
                _bubble_sprite.set_visible(false);
            }
            else
            {
                blink_timer = BLINK_DURATION;
                globals::ui->set_health(globals::ui->health() - 1);
                if(globals::ui->health() > 0)
                {
                    bn::sound_items::player_hit.play(1);
                }
                else
                {
                    bn::sound_items::player_die.play(1);
                    velocity = bn::fixed_point(-1,-1);
                    move_state = MOVE_STATE::DYING;
                    bn::music::stop();
                }
            }
        }
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }


    void __::add_bubble_time(int time)
    {
        bubble_timer += time;
    }

    void __::add_overcharge_time(int time)
    {
        overcharge_timer+=time;
    }

    void __::get_out_of_tilemap()
    {
        bn::fixed dist = 1;
        while(dist <= 16)
        { 
            bn::fixed_point testpos = _sprite.position();
            testpos.set_x(testpos.x()-dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            testpos = _sprite.position();
            testpos.set_x(testpos.x()+dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            testpos = _sprite.position();
            testpos.set_y(testpos.y()-dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }
            
            testpos = _sprite.position();
            testpos.set_y(testpos.y()+dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            //Diagonal

            testpos = _sprite.position() + bn::fixed_point(dist,dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            testpos = _sprite.position() + bn::fixed_point(-dist,-dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            testpos = _sprite.position() + bn::fixed_point(-dist,dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            testpos = _sprite.position() + bn::fixed_point(dist,-dist);

            if(!globals::tilemap->is_colliding_with(*get_collision_rect(testpos)))
            {
                _sprite.set_position(testpos);
                return;
            }

            

            dist = dist + bn::fixed(1);
        }

 

    }

}