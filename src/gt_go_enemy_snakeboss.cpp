#include "gt_go_enemy_snakeboss.h"
#include "bn_sprite_items_boss_snake.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "bn_blending.h"
#include "gt_go_explosion.h"
#include "bn_math.h"
#include "bn_assert.h"
#include "gt_go_enemy_bullet.h"

#define __ go_enemy_snakeboss

#define MOVE_SPEED 1

namespace
{

}

namespace gt
{

    __::body_info::body_info():sprite(bn::sprite_items::boss_snake.create_sprite(200,0,2))
    {

    }

    __::__(bn::fixed_point spawnpos):_sprite(bn::sprite_items::boss_snake.create_sprite(spawnpos))
    {
        _local_collision_rect=bn::fixed_rect(-0.5,-0.5,20,20);
        target_y = -spawnpos.y();

        for(int i = 0; i < body_count; i++)
        {
            body_info info;
            bodys.push_back(info);
        }

        for(int i = 0; i < hist_count; i++)
        {
            poshist[i] = bn::fixed_point(200,0);
        }

        globals::ui->set_boss_enabled(true);
        globals::ui->set_boss_health(_health, max_health);
    }

    void __::do_damage(int amount)
    {
        _health -= amount;
        if(_health < 0)
            _health = 0;
        
        globals::ui->set_boss_health(_health, max_health);

        
        if(_health<=0)
        {
            dying = true;
            globals::go_manager->add_object(new go_explosion(_sprite.position()));
            _sprite.set_visible(false);
        }
    }

    void __::update_collision()
    {
        bool all_bodys_cleared = true;
        for(int i = 0; i < body_count; i++)
        {
            body_info &body = bodys[i];



            if((*globals::player->get_collision_rect()).intersects(*get_collision_rect(body.sprite.position())))
            {
                globals::player->do_damage(1);
            }
            bn::fixed_point pos = body.sprite.position();
            bn::vector<game_object*,8> bullets = globals::collision_map_player_bullets->get_colliding_objects(
                bn::fixed_rect(_local_collision_rect->x()+pos.x(),_local_collision_rect->y() + pos.y(), _local_collision_rect->width(), _local_collision_rect->height()));
            if(bullets.size() > 0)
            {   
                int amount = 0;
                for(int i = 0, limit = bullets.size();i<limit;++i)
                {
                    bullets[i]->destroy();
                    amount++;
                }

                if(amount > 0)
                {
                    if(body.health > 0)
                    {
                        body.health -= amount;
                        if(body.health <= 0)
                        {
                            body.health = 0;
                            body.sprite.set_item(bn::sprite_items::boss_snake,1);
                            globals::go_manager->add_object(new go_explosion(body.sprite.position()));
                        }
                    }
                    else
                    {
                        do_damage(amount);
                    }
                    
                    if(_health > 0)
                    {
                        body.blink_timer = 5;
                        bn::sprites_mosaic::set_stretch(0.2);
                        body.sprite.set_mosaic_enabled(true);
                    }
                }
                
                
            }

            if(body.health > 0)
            {
                all_bodys_cleared = false;
            }

        }

        //if(all_bodys_cleared)
        {
            bn::vector<game_object*,8> bullets = globals::collision_map_player_bullets->get_colliding_objects(this);
            if(bullets.size() > 0)
            {   
                int amount = 0;
                for(int i = 0, limit = bullets.size();i<limit;++i)
                {
                    bullets[i]->destroy();
                    amount++;
                }
                if(amount > 0)
                {
                    bn::sprites_mosaic::set_stretch(0.2);
                    _sprite.set_mosaic_enabled(true);
                    blink_timer = 5;
                    do_damage(amount);
                }
            }
        }

        if((*globals::player->get_collision_rect()).intersects(*get_collision_rect()))
        {
            globals::player->do_damage(1);
        }

        
    }

    void __::update_body(int body_index)
    {
        int hist_index_for_body = hist_index - (body_index + 1) * hist_count_per_body;
        if(hist_index_for_body < 0)
            hist_index_for_body += hist_count;
        if(hist_index_for_body >= hist_count)
            hist_index_for_body -= hist_count;
        BN_BASIC_ASSERT(hist_index_for_body < hist_count,"hist_index_for_body < hist_count");
        BN_BASIC_ASSERT(hist_index_for_body >= 0,"hist_index_for_body >= 0");
        body_info &body = bodys[body_index];
        body.sprite.set_position(poshist[hist_index_for_body]);

        if(body.blink_timer > 0)
        {
            if(--body.blink_timer <= 0)
            {
                body.sprite.set_mosaic_enabled(false);
            }
        }
    }

    void __::update_dying()
    {
        if(die_timer-- <= 0)
        {
            die_timer = die_interval;
            body_info &body = bodys[die_index++];
            body.sprite.set_visible(false);
            globals::go_manager->add_object(new go_explosion(body.sprite.position()));
            if(die_index >= body_count)
            {
                globals::ui->set_score(globals::ui->score() + 2);
                globals::go_manager->destroy_object(this);
            }
        }
    }

    void __::update()
    {
        if(dying)
        {
            update_dying();
        }
        else
        {
            bn::fixed_point pos = _sprite.position();

            change_target_timer--;
            if(change_target_timer <= 0)
            {
                change_target_timer = change_target_interval;
                targeting_player = !targeting_player;
                if(!targeting_player)
                {
                    target_pos = bn::fixed_point(globals::random->get_fixed(-120,120),globals::random->get_fixed(-80,80));
                }
            }

            bn::fixed_point me_to_target = (targeting_player ? (*globals::player->get_pos()) : target_pos) - _sprite.position();
            bn::fixed angle_to_target = bn::degrees_atan2(me_to_target.y().right_shift_integer(), -me_to_target.x().right_shift_integer());
            
            bn::fixed angle = hist_index < hist_count -50 ? move_towards_angle(_sprite.rotation_angle(), angle_to_target, 1) : _sprite.rotation_angle();
            angle += bn::degrees_sin(hist_index*7)*bn::fixed(2);
            if(angle < 0)
                angle += 360;
            if(angle > 360)
                angle -= 360;
            _sprite.set_rotation_angle(angle);

            bn::fixed_point velocity = bn::fixed_point(-bn::degrees_cos(angle)*MOVE_SPEED, bn::degrees_sin(angle)*MOVE_SPEED);
            pos += velocity;

            poshist[hist_index++] = pos;
            if(hist_index >= hist_count)
                hist_index = 0;
            _sprite.set_position(pos);

            for(int i = 0; i < body_count; i++)
            {
                update_body(i);
            }
            if(shoot_timer-- <= 0)
            {
                shoot_timer = shoot_interval;
                bn::fixed_point shoot_pos = pos + bn::fixed_point(-bn::degrees_cos(angle)*16, bn::degrees_sin(angle)*16);
                shoot_bullets_wide(shoot_pos,angle);
            }
            update_collision();
        }
        

        

        if(blink_timer > 0)
        {
            blink_timer--;
            if(blink_timer <=0)
            {
                //_sprite.set_blending_enabled(false);
                _sprite.set_mosaic_enabled(false);
            }
        }

    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

    void __::shoot_bullets_wide(bn::fixed_point pos,bn::fixed angle)
    {
        for(int i = -5; i <=5; i++)
        {
            bn::fixed angle2 = angle + (i * 16) - 90;
            bn::fixed_point vel = bn::fixed_point(degrees_sin(angle2),degrees_cos(angle2)) * bn::fixed(1.5);
            globals::go_manager->add_object(new go_enemy_bullet(pos,vel));
        }
    }

    __::~__()
    {
        globals::lvl->set_trigger();
        globals::ui->set_boss_enabled(false);
    }

}