#include "gt_go_enemy_rotobot.h"
#include "bn_sprite_items_rotobot.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "bn_blending.h"
#include "gt_go_explosion.h"

#define __ go_enemy_rotobot

#define MOVE_SPEED 1

namespace
{
    bn::sprite_animate_action<4> _create_animation_rotate(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 2, bn::sprite_items::rotobot.tiles_item(), 0, 6,7,8);
    }
}

namespace gt
{
    __::__(bn::fixed_point spawnpos):_sprite(bn::sprite_items::rotobot.create_sprite(spawnpos)),
        _action(_create_animation_rotate(_sprite))
    {
        _local_collision_rect=bn::fixed_rect(-0.5,-0.5,28,20);
        target_y = -spawnpos.y();
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
        

        if(prepare_timer > 0)
        {
            prepare_timer--;
        }
        else
        {
            if(rotate_timer-- > 0)
            {
                bn::fixed_point me_to_target = (*globals::player->get_pos()) - _sprite.position();
                bn::fixed angle_to_target = bn::degrees_atan2(me_to_target.y().right_shift_integer(), -me_to_target.x().right_shift_integer());
                
                bn::fixed angle = move_towards_angle(_sprite.rotation_angle(), angle_to_target, 1);
                if(angle < 0)
                    angle += 360;
                if(angle > 360)
                    angle -= 360;
                _sprite.set_rotation_angle(angle);
            }
        }

        bn::fixed_point velocity = bn::fixed_point(-bn::degrees_cos(_sprite.rotation_angle())*MOVE_SPEED, bn::degrees_sin(_sprite.rotation_angle())*MOVE_SPEED);
        pos += velocity;


        _sprite.set_position(pos);


        update_collision();

        if(_sprite.position().x() < -(240/2) -16 || _sprite.position().x() > (240/2)+32 ||
        _sprite.position().y() < -(160/2) -16 || _sprite.position().y() > (160/2)+32)
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
        

        if(!_action.done())
            _action.update();
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

}