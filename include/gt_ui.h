#ifndef GT_UI_H
#define GT_UI_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"


namespace gt
{

    class ui
    {
        public:
            ui();
            int health();
            void set_health(int health);
            int score();
            void set_score(int score);
            void set_boss_enabled(bool enabled);
            bool get_boss_enabled();
            void set_boss_health(int health, int max_health);
            int get_boss_health();
            void game_over();
            void you_won();
            void show_warning();
            void update();
            void set_power(int power);
            int get_power();
            bool is_paused();
            void set_paused(bool _paused);
            void make_garbage();
            static constexpr int max_boss_health = 79;
            static constexpr int max_power = 70;
            static constexpr int max_health = 10;
        private:
            static constexpr int columns = 32;
            static constexpr int rows = 32;
            static constexpr int cells_count = columns * rows;
            static constexpr int tiles_row_index = 20;
            static constexpr int numbers_row_index = 21;
            static constexpr int statusbar_row_index = 19;
            static constexpr int healthbar_column_index = 10;
            static constexpr int healthbar_boss_row_index = 24;
            static constexpr int healthbar_start_decoration_column_index = 9;
            static constexpr int healthbar_end_decoration_column_index = 20;
            static constexpr int warning_width = 9;
            static constexpr int warning_height = 2;
            static constexpr int warning_src_x = 10;
            static constexpr int warning_dst_x = 11;
            static constexpr int warning_src_y = 22;
            static constexpr int warning_dst_y = 2;
            static constexpr int score_column_index = 1;
            static constexpr int power_empty_column = 10;
            static constexpr int power_full_column = 11;
            static constexpr int power_begin_column = 22;
            static constexpr int power_width = 7;
            static constexpr int pause_src_x = 19;
            static constexpr int pause_src_y = 22;
            static constexpr int pause_dst_x = 12;
            static constexpr int pause_dst_y = 9;
            static constexpr int pause_width = 7;
            static constexpr int pause_height = 2;
            
            
            static constexpr int max_score = 99999999;

            alignas(int) bn::regular_bg_map_cell cells[cells_count];
            bn::regular_bg_map_item map_item;
            bn::regular_bg_item bg_item;
            bn::regular_bg_ptr bg_ptr;
            bn::regular_bg_map_ptr bg_map;
            int _health = max_health;
            int _boss_health = max_boss_health;
            int _score = 0;
            bool boss_enabled = false;
            const int warning_count = 6;
            const int warning_on_duration = 30;
            const int warning_off_duration = 10;
            int warning_on_timer = 0;
            int warning_off_timer = 0;
            int warning_counter = 0;
            bool showing_warning = false;
            bool warning_visible = false;
            bool ui_needs_refresh = false;
            int power = max_power;
            bool paused = false;

            void init();
            void update_health();
            void update_score();
            void copy_rect(int src_x, int src_y, int dst_x, int dst_y, int width, int height);
            void copy_rect_random(int src_x, int src_y, int dst_x, int dst_y, int src_width, int src_height, int dst_width, int dst_height);
            void fill_rect(int dst_x, int dst_y, int width, int height, bn::regular_bg_map_cell cell);
            void set_warning_visible(bool visible);
            void update_power();
            void update_paused();

    };

}

#endif