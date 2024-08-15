#ifndef GT_LEVEL_H
#define GT_LEVEL_H

#include "bn_fixed.h"


namespace gt
{

    class level
    {
        public:
                enum LVL_CMD
                {
                    SPAWN_ENEMY_CUBEBOT, //(x,y)
                    SPAWN_ENEMY_ROBOT_HEAD, //(x,y)
                    SPAWN_ENEMY_HUMAN_HEAD, //(x,y)
                    SPAWN_ENEMY_DONUT, //(x,y)
                    SPAWN_ENEMY_CYLINDER, //(x,y)
                    SPAWN_ENEMY_PONGBOSS, //(x,y)
                    SPAWN_ENEMY_ICOBOT, //(x,y)
                    SPAWN_ENEMY_SNAKEBOSS, //(x,y)
                    SPAWN_ENEMY_RAZOR, //(x,y)
                    SPAWN_ENEMY_ROTOBOT, //(x,y)
                    SPAWN_POWERUP, //(x,y)
                    SPAWN_LIVEUP, //(x,y)
                    SPAWN_TILESET_OBJECT, //(index)
                    WAIT_FRAMES, //(frames)
                    WAIT_FOR_TRIGGER,
                    WARNING,
                    PLAY_MUSIC, //(music index)
                    STOP_MUSIC
                };

    

                struct lvl_action
                {
                    LVL_CMD cmd;
                    bn::fixed arg1;
                    bn::fixed arg2;
                    bn::fixed arg3;
                    bn::fixed arg4;
                };

                

            level(const lvl_action * lvl_actions, unsigned int count);
            void set_trigger();
            bool update();
            void set_actions(const lvl_action * lvl_actions, unsigned int count);
        private:
            enum WAITING_STATE
            {
                NONE,
                WAITING_FRAMES,
                WAITING_TRIGGER
            };

            bool execute_action();
            WAITING_STATE waiting_state = WAITING_STATE::NONE;
            const lvl_action *current_level_actions = nullptr;
            unsigned int at_action = 0;
            int wait_frames = 0;
            unsigned int current_lvl_actions_count = 0;
            bool trigger = false;
    };

}

#endif