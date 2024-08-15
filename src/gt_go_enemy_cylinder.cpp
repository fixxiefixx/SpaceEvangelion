#include "gt_go_enemy_cylinder.h"
#include "bn_sprite_items_enemy_cylinder.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "bn_blending.h"
#include "gt_go_explosion.h"
#include "gt_go_enemy_bullet.h"
#include "gt_go_enemy_bullet_big_fast.h"
#include "bn_random.h"

#define __ go_enemy_cylinder

#define MOVE_SPEED 1

namespace gt
{
    __::__(bn::fixed_point spawnpos):_sprite(bn::sprite_items::enemy_cylinder.create_sprite(spawnpos))
    {
        _local_collision_rect=bn::fixed_rect(0,0,32,32);
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
    }

    void __::update()
    {
        bn::fixed_point pos = _sprite.position();

        switch(move_state)
        {
            case MOVE_STATE::PREPARING:
            {
                prepare_timer--;
                if(prepare_timer <= 0)
                {
                    move_state = MOVE_STATE::BREAKING;
                }
            }break;
            case MOVE_STATE::BREAKING:
            {
                velocity_y = move_towards(velocity_y,0,0.01);
                if(velocity_y >= bn::fixed(-0.05))
                {
                    velocity_y = -0.05;
                    move_state = MOVE_STATE::SHOOTING;
                }
            }break;
            case MOVE_STATE::SHOOTING:
            {
                if(shoot_timer > 0)
                {
                    shoot_timer--;
                }
                else
                {
                    /*bn::fixed_point me_to_player = *globals::player->get_pos() - _sprite.position();
                    bn::fixed angle = bn::degrees_atan2(me_to_player.x().right_shift_integer(),me_to_player.y().right_shift_integer());*/

                    bn::fixed angle = -90 + globals::random->get_int(-40,40);
                    bn::fixed_point vel = bn::fixed_point(degrees_sin(angle),degrees_cos(angle));
                    globals::go_manager->add_object(new go_enemy_bullet(bn::fixed_point(-14,2) + _sprite.position(),vel));
                    if(shoot_count-- > 0)
                    {
                        shoot_timer = shoot_interval;
                    }
                    else
                    {
                        move_state = MOVE_STATE::WAITING;
                    }
                }
            }break;
            case MOVE_STATE::WAITING:
            {
                if(wait_timer-- <= 0)
                {
                    move_state = MOVE_STATE::ACCELERATING;
                }
            }break;
            case MOVE_STATE::ACCELERATING:
            {
                velocity_y -= 0.02;
            }break;
        }

        pos.set_x(pos.x() + velocity_y);
        _sprite.set_position(pos);


        update_collision();

        if(_sprite.position().x() < -(240/2) -16 || _sprite.position().x() > (240/2)+32)
        {
            globals::go_manager->destroy_object(this);
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

}