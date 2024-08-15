#include "gt_go_powerup.h"
#include "bn_sprite_items_powerup.h"
#include "bn_math.h"
#include "gt_globals.h"
#include "bn_sound_items.h"

#define __ go_powerup

namespace gt
{

    __::__(bn::fixed_point spawnpos):_sprite(bn::sprite_items::powerup.create_sprite(spawnpos))
    {
        _local_collision_rect = bn::fixed_rect(0,0,32,32);
        start_y = spawnpos.y();
        _sprite.set_bg_priority(2);
    }

    void __::update()
    {
        bn::fixed_point pos = _sprite.position();
        pos.set_x(pos.x()-move_speed);
        pos.set_y(bn::degrees_sin(time*2)*bn::fixed(32));
        _sprite.set_position(pos);

        if(pos.x() < -(240 / 2)-16)
        {
            destroy();
        }

        if(intersects(globals::player))
        {
            bn::sound_items::powerup.play(0.7);
            if(globals::ui->get_power() >=  globals::ui->max_power)
            {
                globals::player->add_overcharge_time(5*60);
            }

            globals::ui->set_power(bn::min(globals::ui->get_power()+20, globals::ui->max_power));
            destroy();
        }

        time++;
    }


    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

}