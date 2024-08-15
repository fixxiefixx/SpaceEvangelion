#include "gt_level.h"
#include "gt_go_enemy_cubebot.h"
#include "gt_go_enemy_robot_head.h"
#include "gt_go_enemy_human_head.h"
#include "gt_go_enemy_donut.h"
#include "gt_go_enemy_cylinder.h"
#include "gt_go_enemy_pongboss.h"
#include "gt_go_enemy_icobot.h"
#include "gt_go_enemy_snakeboss.h"
#include "gt_go_enemy_razor.h"
#include "gt_go_enemy_rotobot.h"
#include "gt_go_powerup.h"
#include "gt_go_liveup.h"
#include "gt_go_tilemap.h"
#include "bn_music.h"
#include "bn_music_items.h"

#define __ level

namespace gt
{
    void __::set_actions(const lvl_action * lvl_actions, unsigned int count)
    {
        current_level_actions = lvl_actions;
        current_lvl_actions_count = count;
        at_action = 0;
        wait_frames = 0;
    }

    __::level(const lvl_action * lvl_actions, unsigned int count)
    {
        set_actions(lvl_actions, count);
    }

    void __::set_trigger()
    {
        trigger = true;
    }

    bool __::execute_action()
    {
        if(at_action < current_lvl_actions_count)
        {
            lvl_action action = current_level_actions[at_action];
            switch(action.cmd)
            {
                case LVL_CMD::SPAWN_ENEMY_CUBEBOT:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_cubebot(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_ROBOT_HEAD:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_robot_head(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_HUMAN_HEAD:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_human_head(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_DONUT:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_donut(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_CYLINDER:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_cylinder(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_PONGBOSS:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_pongboss(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_ICOBOT:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_icobot(bn::fixed_point(action.arg1,action.arg2), false));
                }break;
                case LVL_CMD::SPAWN_ENEMY_SNAKEBOSS:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_snakeboss(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_RAZOR:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_razor(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_ENEMY_ROTOBOT:
                {
                    gt::globals::go_manager->add_object(new gt::go_enemy_rotobot(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::WAIT_FRAMES:
                {
                    wait_frames = action.arg1.right_shift_integer();
                    waiting_state = WAITING_STATE::WAITING_FRAMES;
                }break;
                case LVL_CMD::WAIT_FOR_TRIGGER:
                {
                    trigger = false;
                    waiting_state = WAITING_STATE::WAITING_TRIGGER;
                }break;
                case LVL_CMD::WARNING:
                {
                    gt::globals::ui->show_warning();
                }break;
                case LVL_CMD::STOP_MUSIC:
                {
                    if(bn::music::playing())
                        bn::music::stop();
                }break;
                case LVL_CMD::PLAY_MUSIC:
                {
                    int music_index = action.arg1.right_shift_integer();
                    switch(music_index)
                    {
                        case 0:bn::music_items::biotech.play();break;
                        case 1:bn::music_items::cyber_spider.play();break;
                        case 2:bn::music_items::chips_kdance8.play();break;
                        case 3:bn::music_items::neon_techno.play();break;
                    }
                }break;
                case LVL_CMD::SPAWN_POWERUP:
                {
                    gt::globals::go_manager->add_object(new gt::go_powerup(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_LIVEUP:
                {
                    gt::globals::go_manager->add_object(new gt::go_liveup(bn::fixed_point(action.arg1,action.arg2)));
                }break;
                case LVL_CMD::SPAWN_TILESET_OBJECT:
                {
                    gt::globals::go_manager->add_object(new gt::go_tilemap(action.arg1.right_shift_integer()));
                }break;
            }
            at_action++;
        }
        else
        {
            at_action = 0;
            return false;
        }
        return true;
    }

    bool __::update()
    {
        switch(waiting_state)
        {
            case WAITING_STATE::NONE:
            {
                return execute_action();
            }break;
            case WAITING_STATE::WAITING_FRAMES:
            {
                wait_frames--;
                if(wait_frames <= 0)
                {
                    waiting_state = WAITING_STATE::NONE;
                }
            }break;
            case WAITING_STATE::WAITING_TRIGGER:
            {
                if(trigger)
                {
                    waiting_state = WAITING_STATE::NONE;
                }
            }
        }
        return true;
    }
}