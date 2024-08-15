#include "gt_go_enemy_icobot.h"
#include "bn_sprite_items_enemy_icobot.h"
#include "bn_sprite_items_human_head.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "bn_blending.h"
#include "gt_go_explosion.h"
#include "bn_fixed_point.h"

#define __ go_enemy_icobot

#define MOVE_SPEED 1

namespace
{
    bn::sprite_animate_action<7> _create_animation_rotate(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 4, bn::sprite_items::enemy_icobot.tiles_item(), 0, 1, 2, 3, 4, 5, 6);
    }
}

namespace gt
{
    __::__(bn::fixed_point spawnpos, bool warp_in):_sprite(bn::sprite_items::enemy_icobot.create_sprite(spawnpos)),
        _action(_create_animation_rotate(_sprite))
    {
        _local_collision_rect=bn::fixed_rect(-0.5,-0.5,28,20);
        warping_in = warp_in;
        if(warp_in)
        {
            velocity = bn::fixed_point(0, 0);
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
            if(warping_in)
            {
                _sprite.set_visible(!_sprite.visible());
            }
            if(prepare_timer == 0)
            {
                _sprite.set_visible(true);
                bn::fixed_point me_to_player = (*globals::player->get_pos()) - _sprite.position();
                target_velocity = normalize(me_to_player) * bn::fixed(5);
            }
        }
        velocity = move_towards(velocity,target_velocity,bn::fixed(0.035));
        pos += velocity;

        _sprite.set_position(pos);

        if(!warping_in || prepare_timer <= 0)
            update_collision();

        if(_sprite.position().x() < -(240/2) -16 || _sprite.position().x() > (240/2)+32
        || _sprite.position().y() < -(160/2) -16 || _sprite.position().y() > (160/2)+16)
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