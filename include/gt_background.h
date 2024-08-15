#ifndef GT_BACKROUND_H
#define GT_BACKGROUND_H

#include "bn_regular_bg_ptr.h"
#include "bn_blending_actions.h"
#include "bn_optional.h"

namespace gt
{

    class background
    {
        public:
            background(int level);
            void update();
            void set_level(int level, bool fade);
            int level();
        private:
            int bg_index = 0;
            int _level= 0;
            bool fading_in = false;
            bool fading_out = false;
            bn::optional<bn::blending_fade_alpha_to_action> blending_action_opt;
            bn::regular_bg_ptr bg;
    };

}


#endif