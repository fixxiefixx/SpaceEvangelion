#include "gt_go_worm_bomb.h"
#include "bn_sprite_items_worm_bomb.h"
#include "bn_sound_items.h"
#include "gt_globals.h"
#include "gt_go_explosion.h"

#define __ go_worm_bomb

namespace
{
    bn::sprite_animate_action<2> _create_animation(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 2, bn::sprite_items::worm_bomb.tiles_item(), 0, 1);
    }

}

namespace gt
{
    __::__(bn::fixed_point pos, bn::point _tile_coord):
        _sprite(bn::sprite_items::worm_bomb.create_sprite(pos)),
        _action(_create_animation(_sprite)),
        tile_coord(_tile_coord)
    {
        //bn::sound_items::explosion.play(0.8);
        _sprite.set_z_order(-4);
        _sprite.set_bg_priority(2);
    }


    void __::update()
    {
        _sprite.set_x(_sprite.position().x() - bn::fixed(0.25));
        
        if(explosion_timer-- <= 0)
        {
            if(globals::tilemap != nullptr)
            {
                globals::tilemap->tile_explosion(tile_coord);
                globals::go_manager->add_object(new go_explosion(_sprite.position()));
            }
            destroy();
        }


        if(!_action.done())
        {
            _action.update();
        }
    }

}