#include "gt_go_explosion.h"
#include "bn_sprite_items_explosion.h"
#include "bn_sprite_items_explosion2.h"
#include "bn_sound_items.h"
#include "gt_globals.h"

#define __ go_explosion

namespace
{
    unsigned int sound_time = 0;

    bn::sprite_animate_action<14> _create_animation_explode(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_once(
            sprite, 2, bn::sprite_items::explosion.tiles_item(), 0, 1,2,3,4,5,6,7);
    }

    bn::sprite_animate_action<14> _create_animation_explode2(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_once(
            sprite, 1, bn::sprite_items::explosion2.tiles_item(), 0, 1,2,3,4,5,6,7,8,9,10,11,12,13);
    }
}

namespace gt
{
    __::go_explosion(bn::fixed_point pos):
        _sprite(bn::sprite_items::explosion2.create_sprite(pos)),
        _action(_create_animation_explode2(_sprite))
    {
        if(globals::time - sound_time > 10)
        {
            sound_time = globals::time;
            bn::sound_items::explosion.play(0.8);
        }
        
        _sprite.set_z_order(-4);
        _sprite.set_bg_priority(2);
    }


    void __::update()
    {
        if(_action.done())
        {
            destroy();
        }
        else
        {
            _action.update();
        }
    }

}