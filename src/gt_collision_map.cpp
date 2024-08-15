#include "gt_collision_map.h"
#include "gt_util.h"
#include "bn_log.h"
#include "bn_string.h"

#define __ collision_map

namespace gt
{
    void __::add_game_object(game_object *obj)
    {
        bn::optional<bn::fixed_point> pos_opt = obj->get_pos();
        grid_coord coord = point_to_grid_coord(*pos_opt);
        bn::vector<game_object*,4> *cell = grid_coord_to_cell(coord);
        if(!cell->full())
        {
            cell->push_back(obj);
        }
        
        
    }

    void __::update_game_object(game_object *obj, bn::fixed_point previous_pos)
    {
        bn::optional<bn::fixed_point> pos_opt = obj->get_pos();
        BN_BASIC_ASSERT(pos_opt,"gameobject does not has a position");

        grid_coord coord = point_to_grid_coord(*pos_opt);
        grid_coord coord_prev = point_to_grid_coord(previous_pos);

        if(coord.x != coord_prev.x || coord.y != coord_prev.y)
        {
            bn::vector<game_object*,4> *cell = grid_coord_to_cell(coord);
            bn::vector<game_object*,4> *cell_prev = grid_coord_to_cell(coord_prev);
            remove_from_cell(obj,cell_prev);
            if(!cell->full())
            {
                cell->push_back(obj);
            }
        }
    }

    void __::remove_from_cell(game_object *obj, bn::vector<game_object*,4> *cell)
    {
        for(int i = 0, limit = cell->size();i<limit;++i)
            {
                if((*cell)[i]==obj)
                {
                    if(i != limit-1)
                    {
                        (*cell)[i]=(*cell)[limit-1];
                    }
                    (*cell).pop_back();
                    break;
                }
            }
    }


    void __::remove_game_object(game_object *obj)
    {
        bn::optional<bn::fixed_point> pos_opt = obj->get_pos();
        grid_coord coord = point_to_grid_coord(*pos_opt);
        bn::vector<game_object*,4> *cell = grid_coord_to_cell(coord);
        remove_from_cell(obj, cell);
        
    }

    game_object* __::get_colliding_object(game_object *obj)
    {
        bn::optional<bn::fixed_rect> rect_opt = obj-> get_collision_rect();
        BN_BASIC_ASSERT(rect_opt,"gameobject does not has a position");

        bn::fixed_rect rect = *rect_opt;
        return get_colliding_object(rect);
    }

    bn::vector<game_object*,4> __::get_colliding_objects(game_object *obj)
    {
        bn::vector<game_object*,4> result;
        bn::optional<bn::fixed_rect> rect_opt = obj-> get_collision_rect();
        BN_BASIC_ASSERT(rect_opt,"gameobject does not has a position");

        bn::fixed_rect rect = *rect_opt;
        return get_colliding_objects(rect);
    }

    game_object* __::get_colliding_object(bn::fixed_rect rect)
    {
        bn::fixed_rect orig_rect = rect;
        rect.set_width(rect.width() + 32);
        rect.set_height(rect.height() + 32);

        int abs_x_start = (rect.left().right_shift_integer() + (240 / 2));
        int abs_x_end = (rect.right().right_shift_integer() + (240 / 2));
        int abs_y_start = (rect.top().right_shift_integer() + (160 / 2));
        int abs_y_end = (rect.bottom().right_shift_integer() + (160 / 2));

        abs_x_start = clamp(abs_x_start,0, 240);
        abs_x_end = clamp(abs_x_end,0,240);
        abs_y_start = clamp(abs_y_start,0,160);
        abs_y_end = clamp(abs_y_end,0,160);

        int cell_x_start = abs_x_start  / cell_size;
        int cell_x_end = abs_x_end / cell_size;

        int cell_y_start = abs_y_start  / cell_size;
        int cell_y_end = abs_y_end / cell_size;

        cell_x_start = clamp(cell_x_start, 0, cell_count_x-1);
        cell_x_end = clamp(cell_x_end, 0, cell_count_x-1);
        
        cell_y_start = clamp(cell_y_start, 0, cell_count_y-1);
        cell_y_end = clamp(cell_y_end, 0, cell_count_y-1);

        for(int y = cell_y_start; y <= cell_y_end;y++)
        {
            for(int x = cell_x_start; x <= cell_x_end;x++)
            {
                bn::vector<game_object*,4> *cell = grid_coord_to_cell({x,y});
                for(int i = 0, limit = cell->size();i<limit;++i)
                {
                    game_object *testobj = (*cell)[i];
                    if(testobj->get_collision_rect()->intersects(orig_rect))
                        return testobj;

                }
            }
        }
        return nullptr;
    }
    
    bn::vector<game_object*,4> __::get_colliding_objects(bn::fixed_rect rect)
    {
        bn::fixed_rect orig_rect = rect;
        bn::vector<game_object*,4> result;
        rect.set_width(rect.width() + 16);
        rect.set_height(rect.height() + 16);

        int abs_x_start = (rect.left().right_shift_integer() + (240 / 2));
        int abs_x_end = (rect.right().right_shift_integer() + (240 / 2));
        int abs_y_start = (rect.top().right_shift_integer() + (160 / 2));
        int abs_y_end = (rect.bottom().right_shift_integer() + (160 / 2));

        abs_x_start = clamp(abs_x_start,0, 240);
        abs_x_end = clamp(abs_x_end,0,240);
        abs_y_start = clamp(abs_y_start,0,160);
        abs_y_end = clamp(abs_y_end,0,160);

        int cell_x_start = abs_x_start  / cell_size;
        int cell_x_end = abs_x_end / cell_size;

        int cell_y_start = abs_y_start  / cell_size;
        int cell_y_end = abs_y_end / cell_size;

        cell_x_start = clamp(cell_x_start, 0, cell_count_x-1);
        cell_x_end = clamp(cell_x_end, 0, cell_count_x-1);
        
        cell_y_start = clamp(cell_y_start, 0, cell_count_y-1);
        cell_y_end = clamp(cell_y_end, 0, cell_count_y-1);

        for(int y = cell_y_start; y <= cell_y_end;y++)
        {
            for(int x = cell_x_start; x <= cell_x_end;x++)
            {
                bn::vector<game_object*,4> *cell = grid_coord_to_cell({x,y});
                for(int i = 0, limit = cell->size();i<limit;++i)
                {
                    game_object *testobj = (*cell)[i];
                    if(testobj->get_collision_rect()->intersects(orig_rect))
                    {
                        if(!result.full())
                            result.push_back(testobj);
                    }
                }
            }
        }
        return result;
    }

    int __::get_object_count()
    {
        int count = 0;
        for(int x = 0;x < cell_count_x*cell_count_y;x++)
        {
            bn::vector<game_object*,4> *cell = &_map[x];
            count += cell->size();
        }
        return count;
    }

    void __::log_cells()
    {
        for(int y = 0; y < cell_count_y; y++)
        {
            bn::string<32> text;
            bn::ostringstream text_stream(text);
            for(int x = 0; x < cell_count_x; x++)
            {
                auto cell = grid_coord_to_cell({x,y});
                int cnt = cell->size();
                text_stream.append(cnt);
                text_stream.append(",");
            }
            BN_LOG(text);
        }
    }

    __::grid_coord __::point_to_grid_coord(const bn::fixed_point pos)
    {
        int abs_x = (pos.x().right_shift_integer() + (240 / 2));
        int abs_y = (pos.y().right_shift_integer() + (160 / 2));

        abs_x=clamp(abs_x,0,240);
        abs_y=clamp(abs_y,0,160);

        int cell_x = abs_x  / cell_size;
        int cell_y = abs_y / cell_size;

        cell_x = clamp(cell_x, 0, cell_count_x-1);
        cell_y = clamp(cell_y, 0, cell_count_y-1);


        return grid_coord {cell_x, cell_y};
        //return grid_coord {0,0};
    }

    bn::vector<game_object*,4>* __::grid_coord_to_cell(const grid_coord coord)
    {
        BN_BASIC_ASSERT(coord.x >= 0);
        BN_BASIC_ASSERT(coord.x < cell_count_x);
        BN_BASIC_ASSERT(coord.y >= 0);
        BN_BASIC_ASSERT(coord.y < cell_count_y);
        return &_map[coord.x + (cell_count_x * coord.y)];
    }

}