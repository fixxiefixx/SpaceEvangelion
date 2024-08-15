#include "gt_ui.h"
#include "bn_regular_bg_items_ui.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_memory.h"
#include "gt_util.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "gt_globals.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_blending_actions.h"
#include "common_variable_8x16_sprite_font.h"
#include "bn_sprite_text_generator.h"

#define __ ui


namespace gt
{

    __::ui():map_item(cells[0], bn::size(__::columns, __::rows)),
        bg_item(
                bn::regular_bg_items::ui.tiles_item(), bn::regular_bg_items::ui.palette_item(), map_item),
                bg_ptr(bg_item.create_bg(8,6*8)),
                bg_map(bg_ptr.map())
    {
        init();
        update_health();
        update_score();
        update_power();
        ui_needs_refresh = true;
        bg_ptr.set_priority(1);
    }


    void __::init()
    {
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
        bn::memory::copy(ref_cells[0], cells_count ,cells[0]);
        
        bn::regular_bg_map_cell empty_cell = ref_cells[map_item.cell_index(12, 20)];

        for(int y = 0; y < 19; y++)
        {
            for(int x = 0; x <30;x++)
            {
                cells[map_item.cell_index(x, y)] = empty_cell;
            }
        }
    }

    void __::update_health()
    {
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
        bn::regular_bg_map_cell health_empty_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(0, tiles_row_index)];
        bn::regular_bg_map_cell health_full_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(1, tiles_row_index)];
        bn::regular_bg_map_cell_info health_empty_ref_cell_info(health_empty_ref_cell);
        bn::regular_bg_map_cell_info health_full_ref_cell_info(health_full_ref_cell);

        bn::regular_bg_map_cell& start_cell = cells[map_item.cell_index(healthbar_start_decoration_column_index, statusbar_row_index)];
        bn::regular_bg_map_cell& end_cell = cells[map_item.cell_index(healthbar_end_decoration_column_index, statusbar_row_index)];
        if(boss_enabled)
        {
            start_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(0, 26)];
            end_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(1, 26)];
            int boss_full_tiles = _boss_health / 8;
            for(int i = 0; i<max_health;i++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(healthbar_column_index+i, statusbar_row_index)];

                int health_tile_x = 0;
                if(boss_full_tiles == i)
                {
                    health_tile_x = _boss_health % 8;
                }
                else
                {
                    if(boss_full_tiles > i)
                    {
                        health_tile_x = 7;
                    }
                }
                
                if(_health > i)
                {
                    health_tile_x += 8;
                }
                current_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(health_tile_x, healthbar_boss_row_index)];
            }
        }
        else
        {
            start_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(0, 25)];
            end_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(1, 25)];

            for(int i = 0; i<max_health;i++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(healthbar_column_index+i, statusbar_row_index)];
                if(_health > i)
                {
                    current_cell = health_full_ref_cell_info.cell();
                }
                else
                {
                    current_cell = health_empty_ref_cell_info.cell();
                }
                
            }
        }
        
    }



    int __::health()
    {
        return _health;
    }

    void __::set_health(int health)
    {
        health = clamp(health,0,max_health);
        if(health != _health)
        {
            _health = health;
            update_health();
            ui_needs_refresh = true;
        }
    }

    void __::update_score()
    {
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
        int num = 10000000;
        int test_score = _score;
        for(int i = 0; i< 8;i++)
        {
            int digit = test_score / num;
            test_score = test_score % num;
            num = num / 10;
            
            bn::regular_bg_map_cell digit_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(digit, numbers_row_index)];
            bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(score_column_index+i, statusbar_row_index)];
            current_cell = digit_ref_cell;
        }
    }

    int __::score()
    {
        return _score;
    }

    void __::set_score(int score)
    {
        score= clamp(score,0,max_score);
        if(score != _score)
        {
            _score = score;
            
            update_score();
            ui_needs_refresh = true;
        }
    }

    void __::game_over()
    {
        bn::music_items::gone_forever.play();
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
        bn::regular_bg_map_cell block_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(2, tiles_row_index)];
        for(int y = statusbar_row_index - 1;y>=0;y--)
        {
            for(int x = 0; x < 240/8;x++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x, y)];
                current_cell = block_ref_cell;
                
            }

           
            bg_map.reload_cells_ref();
            bn::core::update();
            bn::core::update();
            bn::core::update();
        }

        for(int y = 0; y < statusbar_row_index; y++)
        {
            for(int x = 0; x < 240/8;x++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x, y)];
                current_cell = ref_cells[map_item.cell_index(x, y)];
            }
            bg_map.reload_cells_ref();
            bn::core::update();
            bn::core::update();
            bn::core::update();
        }
        {
            bn::sprite_text_generator big_variable_text_generator(common::variable_8x16_sprite_font);
            big_variable_text_generator.set_center_alignment();
            big_variable_text_generator.set_bg_priority(1);
            bn::vector<bn::sprite_ptr, 32> text_sprites;
            big_variable_text_generator.generate(0, 64, "Continue? A: Yes, B: No", text_sprites);

            while(!bn::keypad::a_pressed() && !bn::keypad::b_pressed())
            {
                bn::core::update();
            }
        }
        if(bn::keypad::a_pressed())
            gt::globals::restart_scene = true;

        bg_ptr.set_blending_enabled(true);
        bn::blending::set_fade_alpha(0);
        bn::blending_fade_alpha_to_action action(60,1);
        while(!action.done())
        {
            action.update();
            bn::core::update();
        }

        globals::game_running = false;
    }

    void __::you_won()
    {
        bn::music_items::gone_forever.play();
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
        bn::regular_bg_map_cell block_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(2, tiles_row_index)];
        for(int y = statusbar_row_index - 1;y>=0;y--)
        {
            for(int x = 0; x < 240/8;x++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x, y)];
                current_cell = block_ref_cell;
                
            }

           
            bg_map.reload_cells_ref();
            bn::core::update();
            bn::core::update();
            bn::core::update();
        }

        for(int y = 0; y < statusbar_row_index; y++)
        {
            for(int x = 0; x < 240/8;x++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x, y)];
                current_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(x+32, y)];
            }
            bg_map.reload_cells_ref();
            bn::core::update();
            bn::core::update();
            bn::core::update();
        }
        
        while(!bn::keypad::a_pressed())
        {
            bn::core::update();
        }

        bg_ptr.set_blending_enabled(true);
        bn::blending::set_fade_alpha(0);
        bn::blending_fade_alpha_to_action action(60,1);
        while(!action.done())
        {
            action.update();
            bn::core::update();
        }

        globals::game_running = false;
    }

    void __::set_boss_enabled(bool enabled)
    {
        boss_enabled = enabled;
        update_health();
        ui_needs_refresh = true;
    }

    bool __::get_boss_enabled()
    {
        return boss_enabled;
    }


    void __::set_boss_health(int health, int max_health)
    {

        int new_boss_health =(int)( ((float)health / max_health) * max_boss_health);
        if(_boss_health != new_boss_health)
        {
            _boss_health = new_boss_health;
            update_health();
            ui_needs_refresh = true;
        }
    }


    int __::get_boss_health()
    {
        return _boss_health;
    }

    void __::copy_rect(int src_x, int src_y, int dst_x, int dst_y, int width, int height)
    {
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++)
            {
                bn::regular_bg_map_cell block_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(x + src_x, y + src_y)];
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x + dst_x, y + dst_y)];
                current_cell = block_ref_cell;
            }
        }
    }

    void __::copy_rect_random(int src_x, int src_y, int dst_x, int dst_y, int src_width, int src_height, int dst_width, int dst_height)
    {
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();

        for(int y=0;y<dst_height;y++)
        {
            for(int x=0;x<dst_width;x++)
            {
                int rnd_x = globals::random->get() % src_width + src_x;
                int rnd_y = globals::random->get() % src_height + src_y;
                bn::regular_bg_map_cell block_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(rnd_x, rnd_y)];
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x + dst_x, y + dst_y)];
                bn::regular_bg_map_cell_info ci(block_ref_cell);
                ci.set_horizontal_flip(globals::random->get() % 2 == 0);
                ci.set_vertical_flip(globals::random->get() % 2 == 0);
                current_cell = ci.cell();
            }
        }
    }

    void __::fill_rect(int dst_x, int dst_y, int width, int height, bn::regular_bg_map_cell cell)
    {
        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++)
            {
                bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(x + dst_x, y + dst_y)];
                current_cell = cell;
            }
        }
    }

    void __::update()
    {
        if(!paused)
        {
            if(showing_warning)
            {
                if(warning_visible)
                {
                    if(warning_on_timer-- <= 0)
                    {
                        set_warning_visible(false);
                        if(warning_counter-- <= 0)
                        {
                            showing_warning = false;
                        }
                        else
                        {
                            warning_off_timer = warning_off_duration;
                        }
                    }
                }
                else
                {
                    if(warning_off_timer-- <= 0)
                    {
                        set_warning_visible(true);
                        warning_on_timer = warning_on_duration;
                    }
                }
            }
        }

        if(ui_needs_refresh)
        {
            bg_map.reload_cells_ref();
            ui_needs_refresh = false;
        }
    }

    void __::show_warning()
    {
        warning_counter = warning_count;
        warning_on_timer = warning_on_duration;
        showing_warning = true;
        set_warning_visible(true);
    }

    void __::set_warning_visible(bool visible)
    {
        if(warning_visible == visible)
            return;
        
        warning_visible = visible;
        if(visible)
        {
            bn::sound_items::warning.play(0.8);
            copy_rect(warning_src_x, warning_src_y, warning_dst_x, warning_dst_y, warning_width, warning_height);
        }
        else
        {
            const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
            bn::regular_bg_map_cell empty_cell = ref_cells[map_item.cell_index(12, 20)];
            fill_rect(warning_dst_x, warning_dst_y, warning_width, warning_height, empty_cell);
        }
        ui_needs_refresh = true;
    }

    void __::set_power(int _power)
    {
        if(power != _power)
        {
            power = _power;
            update_power();
            ui_needs_refresh = true;
        }
    }


    int __::get_power()
    {
        return power;
    }

    void __::update_power()
    {
        const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
        bn::regular_bg_map_cell power_empty_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(power_empty_column, tiles_row_index)];
        bn::regular_bg_map_cell power_full_ref_cell = ref_cells[bn::regular_bg_items::ui.map_item().cell_index(power_full_column, tiles_row_index)];
        int powerbars = ((power+9)/10);
        for(int i = 0; i < power_width; i++)
        {
            bn::regular_bg_map_cell& current_cell = cells[map_item.cell_index(i + power_begin_column, statusbar_row_index)];
            if(powerbars > i)
            {
                current_cell = power_full_ref_cell;
            }
            else
            {
                current_cell = power_empty_ref_cell;
            }
        }
    }

    bool __::is_paused()
    {
        return paused;
    }

    void __::set_paused(bool _paused)
    {
        if(_paused != paused)
        {
            paused = _paused;
            update_paused();
            ui_needs_refresh = true;
        }
    }

    void __::update_paused()
    {
        if(paused)
        {
            copy_rect(pause_src_x, pause_src_y, pause_dst_x, pause_dst_y,pause_width, pause_height);
        }
        else
        {
            const bn::regular_bg_map_cell *ref_cells = bn::regular_bg_items::ui.map_item().cells_ptr();
            bn::regular_bg_map_cell empty_cell = ref_cells[map_item.cell_index(12, 20)];
            fill_rect(pause_dst_x, pause_dst_y,pause_width, pause_height, empty_cell);
        }
    }

    void __::make_garbage()
    {
        copy_rect_random(4,tiles_row_index,0,0,20,1,240/8, 160/8);
        ui_needs_refresh = true;
    }

}