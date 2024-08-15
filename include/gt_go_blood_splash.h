#ifndef GT_GO_BLOOD_SPLASH_H
#define GT_GO_BLOOD_SPLASH_H

#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"
#include "gt_game_object.h"
#include "bn_sound_handle.h"

namespace gt
{
    class go_blood_splash : public game_object
    {
        public:
            go_blood_splash(bn::fixed_point pos);
            virtual void update() override;
        private:
            bn::sprite_ptr _sprite;
            bn::sprite_animate_action<7> _action;
    };
}

#endif