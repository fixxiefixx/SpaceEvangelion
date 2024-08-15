#include "gt_go_enemy_human_head.h"
#include "bn_sprite_items_human_head.h"
#include "bn_sprite_items_enemy_bullet_big_fast_charging.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "gt_go_explosion.h"
#include "bn_math.h"
#include "gt_go_enemy_bullet.h"
#include "gt_go_enemy_bullet_big_fast.h"
#include "bn_sound_items.h"
#include "gt_go_enemy_icobot.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_colors.h"

#define __ go_enemy_human_head
#define PI 3.14159265359

#define MIN_X 40
#define MAX_X (240 / 2) - 16

namespace
{
    bn::sprite_animate_action<60> _create_animation_rotate(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 1, bn::sprite_items::human_head.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
            21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59);
    }

    bn::sprite_animate_action<60> _create_animation_dying(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_once(
            sprite, 1, bn::sprite_items::human_head.tiles_item(), 60,60);
    }

    bn::sprite_animate_action<8> _create_animation_charging(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 2, bn::sprite_items::enemy_bullet_big_fast_charging.tiles_item(), 0, 1, 2, 3,4,5,6,7);
    }

}


namespace gt
{
    __::__(bn::fixed_point spawnpos):_sprite(bn::sprite_items::human_head.create_sprite(spawnpos)),
    _action(_create_animation_rotate(_sprite)),
    _sprite_charging(bn::sprite_items::enemy_bullet_big_fast_charging.create_sprite(500,0)),
    _charging_action(_create_animation_charging(_sprite_charging))
    {
        _sprite.set_z_order(-1);
        _sprite_charging.set_z_order(-2);
        _local_collision_rect = bn::fixed_rect(0,-1,40,60);
        _sprite_charging.set_visible(false);
        globals::ui->set_boss_enabled(true);
        globals::ui->set_boss_health(_health,max_health);
    }

    void __::follow_player()
    {
        bn::fixed_point pos = _sprite.position();

        //Movement
        sin_at = sin_at + bn::fixed(0.002);
        if(sin_at > 2 * PI)
            sin_at = 0;
        
        bn::fixed_point target_pos = *globals::player->get_pos();
        target_pos.set_x(target_pos.x() + 120);
        target_pos.set_y(target_pos.y() + sin(sin_at)*30);
        if(target_pos.x() < MIN_X)
            target_pos.set_x(MIN_X);
        if(target_pos.x() > MAX_X)
            target_pos.set_x(MAX_X);
        
        bn::fixed diff_x = target_pos.x() - pos.x();
        bn::fixed diff_y = target_pos.y() - pos.y();
        pos.set_x(pos.x() + (clamp( diff_x * bn::fixed(0.02),-0.5,0.5)));
        pos.set_y(pos.y() + (clamp( diff_y * bn::fixed(0.02),-0.2,0.2)));

        //pos.set_x(move_towards(pos.x(),target_pos.x(),0.5));
        //pos.set_y(move_towards(pos.y(),target_pos.y(),0.5));

        _sprite.set_position(pos);
    }

    void __::fly_away()
    {
        velocity_y -= 0.02;
        _sprite.set_x(_sprite.x() + velocity_y);
    }

    void __::update()
    {
        switch(move_state)
        {
            case MOVE_STATE::PREPARING:
            {
                if(prepare_timer-- <= 0)
                {
                    shoot_wide_counter = shoot_wide_count;
                    move_state = MOVE_STATE::SHOOT_WIDE;
                }

                follow_player();
            }break;
            case MOVE_STATE::SHOOT_WIDE:
            {
                follow_player();
                if(shoot_timer > 0)
                {
                    shoot_timer--;
                }
                else
                {
                    shoot_timer = shoot_wide_interval;
                    shoot_bullets_wide();
                    if(shoot_wide_counter-- <= 0)
                    {
                        wait1_timer = wait1_time;
                        move_state = MOVE_STATE::WAIT1;
                    }
                }
            }break;
            case MOVE_STATE::WAIT1:
            {
                follow_player();
                if(wait1_timer-- <= 0)
                {
                    shoot_burst_counter = shoot_burst_count;
                    move_state = MOVE_STATE::SHOOT_BURST;
                }
            }break;
            case MOVE_STATE::SHOOT_BURST:
            {
                follow_player();
                if(shoot_timer > 0)
                {
                    shoot_timer--;
                }
                else
                {
                    shoot_timer = shoot_burst_interval;
                    shoot_bullet_burst();
                    if(shoot_burst_counter-- <= 0)
                    {
                        wait2_timer = wait2_time;
                        move_state = MOVE_STATE::WAIT2;
                    }
                }
            }break;
            case MOVE_STATE::WAIT2:
            {
                follow_player();
                if(wait2_timer-- <= 0)
                {
                    bn::sound_items::enemy_bullet_big_fast_charging.play(0.8);
                    charge_timer = charge_time;
                    _sprite_charging.set_visible(true);
                    move_state = MOVE_STATE::CHARGING;
                }
            }break;
            case MOVE_STATE::CHARGING:
            {
                follow_player();
                if(charge_timer-- <= 0)
                {
                    _sprite_charging.set_visible(false);
                    shoot_big_counter = shoot_big_count;
                    move_state = MOVE_STATE::SHOOT_BIG;
                }
                else
                {
                    _sprite_charging.set_position(_sprite.position() + bn::fixed_point(-14,2));
                    _charging_action.update();
                }
            }break;
            case MOVE_STATE::SHOOT_BIG:
            {
                follow_player();
                if(shoot_timer > 0)
                {
                    shoot_timer--;
                }
                else
                {
                    shoot_timer = shoot_big_interval;
                    shoot_big();
                    if(shoot_big_counter-- <= 0)
                    {
                        wait3_timer = wait3_time;
                        move_state = MOVE_STATE::WAIT3;
                    }
                }
            }break;
            case MOVE_STATE::WAIT3:
            {
                follow_player();
                if(wait3_timer-- <= 0)
                {

                    spawn_icobots_counter = spawn_icobots_count;
                    move_state = MOVE_STATE::SPAWN_ICOBOTS;
                    
                }
            }break;
            case MOVE_STATE::FLY_AWAY:
            {
                fly_away();
                if(_sprite.position().x() < -(240/2) -16)
                {
                    globals::go_manager->destroy_object(this);
                }
            }break;
            case MOVE_STATE::SPAWN_ICOBOTS:
            {
                follow_player();
                if(spawn_icobots_timer > 0)
                {
                    spawn_icobots_timer--;
                }
                else
                {
                    spawn_icobots_timer = spawn_icobots_interval;
                    bn::fixed_point spawn_pos = bn::fixed_point(globals::random->get_fixed(-120,120), globals::random->get_fixed(-80,72));
                    globals::go_manager->add_object(new go_enemy_icobot(spawn_pos, true));
                    bn::sound_items::warping.play(1);

                    if(spawn_icobots_counter-- <= 0)
                    {
                        wait4_timer = wait4_time;
                        move_state = MOVE_STATE::WAIT4;
                    }
                }
            }break;
            case MOVE_STATE::WAIT4:
            {
                follow_player();
                if(wait4_timer-- <= 0)
                {
                   
                    shoot_wide_counter = shoot_wide_count;
                    move_state = MOVE_STATE::SHOOT_WIDE;
                    
                }
            }break;
            case MOVE_STATE::DYING:
            {
                
                if(die_explosion_counter < die_explosion_count / 2)
                {
                    _sprite.set_visible(globals::time % 4 < 2);
                }

                if(die_explosion_timer-- < 0)
                {
                    die_explosion_timer = die_explosion_interval;

                    
                    bn::fixed_point offset = bn::fixed_point(globals::random->get_fixed(-32,32), globals::random->get_fixed(-32,32));
                    globals::go_manager->add_object(new go_explosion(_sprite.position() + offset));

                    if(die_explosion_counter-- <= 0)
                    {
                        globals::ui->set_score(globals::ui->score() + 10);
                        globals::go_manager->destroy_object(this);
                        destroy();
                    }
                }
            }break;
        }



        //Bullet collision
        if(move_state != MOVE_STATE::DYING)
        {
            bn::vector<game_object*,4> bullets = globals::collision_map_player_bullets->get_colliding_objects(this);
            if(bullets.size() > 0)
            {   
                for(int i = 0, limit = bullets.size();i<limit;++i)
                {
                    bullets[i]->destroy();
                    _health--;
                    if(_health < 0)
                        _health = 0;
                    globals::ui->set_boss_health(_health,max_health);
                }
                
                //bn::sprites_mosaic::set_stretch(0.1);
                //_sprite.set_mosaic_enabled(true);
                bn::sprite_palette_ptr pal = _sprite.palette();
                pal.set_fade(bn::colors::red, 0.5);
                blink_timer = 3;
                
                if(_health<=0)
                {
                    _action = _create_animation_dying(_sprite);
                    move_state = MOVE_STATE::DYING;
                }
            }
        }

        if((*globals::player->get_collision_rect()).intersects(*get_collision_rect()))
        {
            globals::player->do_damage(1);
        }

        if(blink_timer > 0)
        {
            blink_timer--;
            if(blink_timer <=0)
            {
                //_sprite.set_blending_enabled(false);
                //_sprite.set_mosaic_enabled(false);
                bn::sprite_palette_ptr pal = _sprite.palette();
                pal.set_fade(bn::colors::red, 0);
            }
        }

        if(!_action.done())
            _action.update();
            
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

    void __::shoot_bullets_wide()
    {
        for(int i = -4; i <=4; i++)
        {
            bn::fixed angle = -90 + (i * 16);
            bn::fixed_point vel = bn::fixed_point(degrees_sin(angle),degrees_cos(angle));
            if(_health < max_health / 2)
            vel *= bn::fixed(2);
            globals::go_manager->add_object(new go_enemy_bullet(bn::fixed_point(-14,2) + _sprite.position(),vel));
        }
    }

    void __::shoot_bullet_burst()
    {
        bn::fixed angle = -90 + globals::random->get_int(-40,40);
        bn::fixed_point vel = bn::fixed_point(degrees_sin(angle),degrees_cos(angle));
        if(_health < max_health / 2)
            vel *= bn::fixed(2);
        globals::go_manager->add_object(new go_enemy_bullet(bn::fixed_point(-14,2) + _sprite.position(),vel));
    }

    void __::shoot_big()
    {
        globals::go_manager->add_object(new go_enemy_bullet_big_fast(bn::fixed_point(-14-10,2) + _sprite.position(),bn::fixed_point(-6,0)));
    }

    __::~__()
    {
        globals::lvl->set_trigger();
        globals::ui->set_boss_enabled(false);
    }

}