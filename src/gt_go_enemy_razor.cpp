#include "gt_go_enemy_razor.h"
#include "bn_sprite_items_razor.h"
#include "bn_sprite_items_blade.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "bn_blending.h"
#include "gt_go_explosion.h"

#define __ go_enemy_razor

#define MOVE_SPEED 1


namespace gt
{
    __::blade_info::blade_info(bn::fixed_point _attached_local_pos):attached_local_pos(_attached_local_pos),
    sprite(bn::sprite_items::blade.create_sprite(0,0))
    {

    }


    __::__(bn::fixed_point spawnpos):_sprite(bn::sprite_items::razor.create_sprite(spawnpos))
    {
        _local_collision_rect = bn::fixed_rect(0,0,26,26);
        _sprite.set_z_order(-1);
        blades.push_back(blade_info(bn::fixed_point(5-16,0)));
        blades.push_back(blade_info(bn::fixed_point(0,4-16)));
        blades.push_back(blade_info(bn::fixed_point(16-5,0)));
        blades.push_back(blade_info(bn::fixed_point(0,16-4)));

        blades[0].pause_timer = pause_duration;
        blades[1].pause_timer = pause_duration*2;
        blades[2].pause_timer = pause_duration*3;
        blades[3].pause_timer = pause_duration*4;

        globals::ui->set_boss_enabled(true);
        globals::ui->set_boss_health(_health, max_health);
    }

    void __::update_blade_collision(int blade_index)
    {
        blade_info &bi = blades[blade_index];
        if((*globals::player->get_collision_rect()).intersects(*get_collision_rect(bi.sprite.position())))
        {
            globals::player->do_damage(1);
        }
    }

    void __::update_collision()
    {
        bn::vector<game_object*,8> bullets = globals::collision_map_player_bullets->get_colliding_objects(this);
        if(bullets.size() > 0)
        {   
            for(int i = 0, limit = bullets.size();i<limit;++i)
            {
                bullets[i]->destroy();
                _health--;
            }
            if(_health < 0)
                _health = 0;
        
            globals::ui->set_boss_health(_health, max_health);
            bn::sprites_mosaic::set_stretch(0.2);
            _sprite.set_mosaic_enabled(true);
            //bn::blending::set_intensity_alpha(0.8);
            //_sprite.set_blending_enabled(true);
            blink_timer = 5;
            
            if(_health<=0)
            {
                globals::ui->set_score(globals::ui->score() + 2);
                globals::go_manager->destroy_object(this);
                globals::go_manager->add_object(new go_explosion(_sprite.position()));
            }
        }

        if((*globals::player->get_collision_rect()).intersects(*get_collision_rect()))
        {
            globals::player->do_damage(1);
        }

        for(int i = 0; i< 4; i++)
        {
            update_blade_collision(i);
        }
    }

    void __::update_blade(int blade_index)
    {
        blade_info &bi = blades[blade_index];
        bn::fixed_point pos = bi.sprite.position();
        if(bi.attached)
        {
            pos = _sprite.position() + bi.attached_local_pos;
            if(bi.pause_timer-- <= 0)
            {
                bi.attached = false;
                bi.velocity = normalize(bi.attached_local_pos) * bn::fixed(max_velocity);
                bi.pause_timer = follow_player_duration;
                bi.flying_to_player = true;
            }
        }
        else
        {
            if(bi.pause_timer-- <= 0)
            {
                bi.flying_to_player = false;
            }

            if(bi.flying_to_player)
            {
                bn::fixed_point blade_to_player = (*globals::player->get_pos()) - pos;
                bn::fixed_point target_velocity = normalize(blade_to_player) * max_velocity;
                bi.velocity = move_towards(bi.velocity,target_velocity, 0.05);
            }
            else
            {
                
                bn::fixed_point blade_to_socket = (_sprite.position() + bi.attached_local_pos) - pos;
                if(aprox_length(blade_to_socket) < 8)
                {
                    bi.velocity = bn::fixed_point(0,0);
                    bi.attached = true;
                    bi.pause_timer = pause_duration;
                }
                else
                {
                    bn::fixed_point target_velocity = normalize(blade_to_socket) * max_velocity;
                    bi.velocity = move_towards(bi.velocity,target_velocity, 0.1);
                }

            }
            pos = pos + bi.velocity;
        }

        bi.sprite.set_position(pos);
        bn::fixed rot = bi.sprite.rotation_angle();
        bi.rotspeed = move_towards(bi.rotspeed, 10, 0.02);
        rot += bi.rotspeed;
        if(rot > 360)
            rot-= 360;
        bi.sprite.set_rotation_angle(rot);
    }

    void __::update()
    {
        bn::fixed_point pos = _sprite.position();
        
        bn::fixed_point target = bn::fixed_point(bn::degrees_sin(time/3)* bn::fixed(20),bn::degrees_cos(time)*bn::fixed(20));

        pos = move_towards(pos,target,bn::fixed(0.5));

        _sprite.set_position(pos);

        for(int i = 0; i < 4; i++)
        {
            update_blade(i);
        }

        update_collision();


        if(blink_timer > 0)
        {
            blink_timer--;
            if(blink_timer <=0)
            {
                //_sprite.set_blending_enabled(false);
                _sprite.set_mosaic_enabled(false);
            }
        }
        time++;
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

    __::~__()
    {
        globals::lvl->set_trigger();
        globals::ui->set_boss_enabled(false);
    }

}