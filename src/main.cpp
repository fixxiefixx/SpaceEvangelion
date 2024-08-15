#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_player.h"
#include "bn_music_items.h"
#include "gt_game_object_manager.h"
#include "gt_globals.h"
#include "gt_go_enemy_cubebot.h"
#include "gt_collision_map.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_regular_bg_items_bg.h"
#include "bn_regular_bg_items_menu.h"
#include "bn_regular_bg_items_gbajam24_bg.h"
#include "bn_regular_bg_items_white_bg.h"
#include "gt_level.h"
#include "gt_level_scripts.h"
#include "gt_ui.h"
#include "bn_music.h"
#include "bn_sound_items.h"
#include "bn_assert.h"
#include "gt_background.h"
#include "common_variable_8x16_sprite_font.h"
#include "bn_sprite_text_generator.h"
#include "bn_blending_actions.h"
#include "bn_sram.h"
#include "bn_string.h"
#include "bn_bg_palettes.h"


#define BG_SCROLL_SPEED 0.1
#define SRAM_MAGIC_NUMBER 4867

namespace
{
    struct sram_data
    {
        int magic_number = 0;
        int score = 0;
    };
}

void save_score(int score)
{
    sram_data cart_sram_data;
    cart_sram_data.magic_number = SRAM_MAGIC_NUMBER;
    cart_sram_data.score = score;
    bn::sram::write(cart_sram_data);
}

bn::optional<int> load_score()
{
    sram_data cart_sram_data;
    bn::sram::read(cart_sram_data);

    if(cart_sram_data.magic_number == SRAM_MAGIC_NUMBER)
    {
        BN_LOG("valid score loaded: ",cart_sram_data.score);
        return bn::optional<int>(cart_sram_data.score);
    }
    return bn::optional<int>();
}

void menu_scene()
{
    bn::regular_bg_ptr menu_bg = bn::regular_bg_items::menu.create_bg(8,6*8);

    menu_bg.set_blending_enabled(true);
    bn::blending::set_fade_alpha(1);
    
    bn::blending_fade_alpha_to_action action(60,0);
    while(!action.done())
    {
        action.update();
        bn::core::update();
    }

    bn::sprite_text_generator big_variable_text_generator(common::variable_8x16_sprite_font);
    big_variable_text_generator.set_center_alignment();
    bn::vector<bn::sprite_ptr, 32> text_sprites;
    big_variable_text_generator.generate(0, 0, "> START", text_sprites);
    bn::optional<int> score_opt = load_score();
    if(score_opt)
    {
        bn::string<32> text;
        bn::ostringstream text_stream(text);
        text_stream.append("Highscore: ");
        text_stream.append(*score_opt);
        big_variable_text_generator.generate(0,32,text,text_sprites);
    }


    while(true)
    {
        if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
        {
            bn::core::update();
            break;
        }
        bn::core::update();
    }
}

void game_scene(int &level_index)
{
    bn::bg_palettes::set_transparent_color(bn::color(0,0,0));
    gt::globals::game_running = true;
    gt::game_object_manager go_manager =  gt::game_object_manager();
    gt::globals::go_manager = &go_manager;
    bn::random rand = bn::random();
    gt::globals::random = &rand;
    bn::camera_ptr cam = bn::camera_ptr::create(0, 0);
    gt::globals::camera = &cam;
    gt::collision_map collmap_player_bullets;
    gt::collision_map collmap_enemy_bullets;
    gt::globals::collision_map_player_bullets = &collmap_player_bullets;
    gt::globals::collision_map_enemy_bullets = &collmap_enemy_bullets;
    auto player = new gt::go_player(cam);
    gt::globals::player=player;
    go_manager.add_object(player);

    gt::background bg(level_index);


    gt::ui ui_instance;
    gt::globals::ui = &ui_instance;

    //gt::level lvl = gt::level(gt::testlevel_actions,sizeof(gt::testlevel_actions) / sizeof(gt::level::lvl_action));
    gt::level lvl = gt::level(gt::levels[level_index].actions, gt::levels[level_index].size);
    
    gt::globals::lvl = &lvl;

    bn::music_items::biotech.play();


    bool music_was_playing=false;
    while(gt::globals::game_running)
    {
        if(bn::keypad::start_pressed())
        {
            ui_instance.set_paused(!ui_instance.is_paused());
            if(ui_instance.is_paused())
            {
                bn::sound_items::pause.play(0.7);
                music_was_playing = bn::music::playing();
                if(music_was_playing)
                {
                    
                    bn::music::pause();
                }
            }
            else
            {
                if(music_was_playing)
                {
                    bn::music::resume();
                }
            }
        }

        if(bn::keypad::select_pressed())
        //if(gt::globals::time % 2 == 0)
        {
           // ui_instance.make_garbage();
        }

        if(!ui_instance.is_paused())
        {
            bool level_has_ended = !lvl.update();
            go_manager.update();
            bg.update();
            gt::globals::time++;

            if(level_has_ended)
            {
                level_index++;

                if(level_index >= gt::level_count)
                {
                    //BN_ERROR("YOU WON");
                    //while(true);
                    gt::globals::ui->you_won();
                }
                lvl.set_actions(gt::levels[level_index].actions, gt::levels[level_index].size);
                bg.set_level(level_index, true);
            }
        }
        ui_instance.update();
        bn::core::update();
    }
    bn::optional<int> highscore_opt = load_score();
    if(!highscore_opt || (*highscore_opt) < gt::globals::ui->score())
    {
        save_score(gt::globals::ui->score());
    }
}


void test_rounding()
{
    bn::fixed fval = 0.4;
    bn::fixed fvalneg = -0.4;

    int ival = fval.right_shift_integer();
    int ivalneg = fvalneg.right_shift_integer();
    BN_LOG("ival: ",ival);
    BN_LOG("ivalneg: ",ivalneg);
}

void gbajam_splash_scene()
{
    bool abort=false;
    

    bn::regular_bg_ptr splash_bg=bn::regular_bg_items::gbajam24_bg.create_bg(8,6*8);
    splash_bg.set_priority(2);
    splash_bg.set_blending_enabled(true);

    bn::blending::set_fade_alpha(1);
    bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
    
    bn::blending_fade_alpha_to_action action(60,0);
    while(!action.done() && !abort)
    {
        action.update();
        bn::core::update();
        if(bn::keypad::start_pressed())
            abort=true;
    }

    int framesToWait=60*2;
    while(framesToWait-- && !abort)
    {
        bn::core::update();
        if(bn::keypad::start_pressed())
            abort=true;
    }

    action=bn::blending_fade_alpha_to_action(60,1);
    while(!action.done() && !abort)
    {
        action.update();
        bn::core::update();
        if(bn::keypad::start_pressed())
            abort=true;
    }
    bn::blending::set_fade_alpha(0);
}

int main()
{
    bn::core::init(bn::optional<bn::color>(bn::color(31,31,31))); // White
    test_rounding();
    bn::music_items::gone_forever.play();
    gbajam_splash_scene();
    
    while(true)
    {
        menu_scene();
        int level_index = 0;
        gt::globals::restart_scene = true;
        while(gt::globals::restart_scene)
        {
            gt::globals::restart_scene = false;
            game_scene(level_index);

        }
    }
}
