#ifndef GT_GO_POWERUP_H
#define GT_GO_POWERUP_H

#include "gt_game_object.h"
#include "bn_sprite_ptr.h"

namespace gt
{

    class go_powerup : public game_object
    {
        public:
            go_powerup(bn::fixed_point spawnpos);
            virtual void update() override;
            virtual bn::optional<bn::fixed_point> get_pos() override;

            private:
                bn::sprite_ptr _sprite;
                int time = 0;
                const bn::fixed move_speed = 1.5;
                bn::fixed start_y = 0;
    };

}

#endif