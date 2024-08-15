#include "gt_go_player_bullet.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_player_bullet.h"
#include "bn_sprite_items_small_yellow_bullet.h"
#include "bn_sprite_items_tiny_pink_bullet.h"
#include "bn_sprite_items_bigboom.h"
#include "gt_globals.h"
#include "gt_util.h"

#define __ go_player_bullet

#define MOVE_SPEED 5

namespace
{
    bn::sprite_animate_action<3> _create_animation_bullet(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_once(
            sprite, 1, bn::sprite_items::player_bullet.tiles_item(), 0, 1);
    }
}

namespace gt
{
    __::__(bn::fixed_point spawnpos, bn::fixed yoffset):_sprite(bn::sprite_items::player_bullet.create_sprite(spawnpos)),
        _yoffset(yoffset+spawnpos.y())
        ,_action(_create_animation_bullet(_sprite))
    {
        _sprite.set_z_order(1);
        _local_collision_rect=bn::optional<bn::fixed_rect>(bn::fixed_rect(0, 3.5 - 8, 4, 13));

        globals::collision_map_player_bullets->add_game_object(this);
    }

    void __::update()
    {
        bn::fixed_point pos = _sprite.position();
        bn::fixed_point old_pos = pos;
        pos.set_x(pos.x()+MOVE_SPEED);
        if(wait_frames > 0)
        {
            wait_frames--;
        }
        else
        {
            pos.set_y(move_towards(pos.y(),_yoffset,2));
        }
        _sprite.set_position(pos);
        globals::collision_map_player_bullets->update_game_object(this, old_pos);

        if(globals::tilemap != nullptr)
        {
            if(globals::tilemap->do_bullet_collision(_sprite.position()))
            {
                globals::go_manager->destroy_object(this);
            }
        }

        if(_sprite.position().x() >= (240/2) + 8)
        {
            globals::go_manager->destroy_object(this);
        }

        if(!_action.done())
            _action.update();
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

    __::~go_player_bullet()
    {
        globals::collision_map_player_bullets->remove_game_object(this);
    }

}