#include "gt_go_enemy_bullet_big_fast.h"
#include "bn_sprite_items_enemy_bullet_big_fast.h"
#include "gt_globals.h"
#include "bn_log.h"
#include "bn_sound_items.h"

#define __ go_enemy_bullet_big_fast

namespace
{
    unsigned int sound_time = 0;

    bn::sprite_animate_action<4> _create_animation_rotate(bn::sprite_ptr sprite)
    {
        return bn::create_sprite_animate_action_forever(
            sprite, 2, bn::sprite_items::enemy_bullet_big_fast.tiles_item(), 0, 1, 2, 1);
    }
}

namespace gt
{
    __::__(bn::fixed_point spawnpos,bn::fixed_point velocity):
        _sprite(bn::sprite_items::enemy_bullet_big_fast.create_sprite(spawnpos)),
        _action(_create_animation_rotate(_sprite)),
        _velocity(velocity)
    {
        _local_collision_rect = bn::fixed_rect(-10,0,42,30);
        globals::collision_map_enemy_bullets->add_game_object(this);
        if(abs(sound_time - globals::time) > 2)
        {
            sound_time = globals::time;
            bn::sound_items::enemy_bullet_big_fast.play(0.8);
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

        if(pos.x() < -(240 / 2) - 64 ||
            pos.x() > (240 / 2) + 64 ||
            pos.y() < -(160 / 2) - 64 ||
            pos.y() > (160 / 2) + 64)
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