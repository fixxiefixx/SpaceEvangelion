#include "gt_go_enemy_bullet.h"
#include "bn_sprite_items_tiny_bullet.h"
#include "gt_globals.h"
#include "bn_log.h"
#include "bn_sound_items.h"

#define __ go_enemy_bullet

namespace
{
    unsigned int sound_time = 0;

    bn::sprite_animate_action<4> _create_animation_rotate(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 2, bn::sprite_items::tiny_bullet.tiles_item(), 0, 1, 2, 3);
    }
}

namespace gt
{
    __::__(bn::fixed_point spawnpos,bn::fixed_point velocity):
        _sprite(bn::sprite_items::tiny_bullet.create_sprite(spawnpos)),
        _action(_create_animation_rotate(_sprite)),
        _velocity(velocity)
    {
        _local_collision_rect = bn::fixed_rect(-0.5,-0.5,6,6);
        globals::collision_map_enemy_bullets->add_game_object(this);
        if(abs(sound_time - globals::time) > 2)
        {
            sound_time = globals::time;
            bn::sound_items::enemy_small_bullet.play(0.6);
        }
    }

    void __::update()
    {

        bn::fixed_point pos = _sprite.position();
        bn::fixed_point old_pos = pos;
        pos.set_x(pos.x() + _velocity.x());
        pos.set_y(pos.y() + _velocity.y());
        _sprite.set_position(pos);
        //BN_LOG("bullet pos ",_sprite.position().x());
        globals::collision_map_enemy_bullets->update_game_object(this, old_pos);

        if(pos.x() < -(240 / 2) - 4 ||
            pos.x() > (240 / 2) + 4 ||
            pos.y() < -(160 / 2) - 4 ||
            pos.y() > (160 / 2) + 4)
        {
            destroy();

        }

        if(!_action.done())
            _action.update();

    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return _sprite.position();
    }

    __::~__()
    {
        globals::collision_map_enemy_bullets->remove_game_object(this);
    }

}