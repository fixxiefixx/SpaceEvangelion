#include "gt_go_tilemap.h"
#include "bn_regular_bg_items_fleshwall.h"
#include "bn_memory.h"
#include "bn_log.h"
#include "gt_globals.h"
#include "gt_go_blood_splash.h"
#include "gt_go_enemy_bullet.h"
#include "gt_go_worm_bomb.h"

#define __ go_tilemap

namespace
{
    constexpr const bn::regular_bg_item map_items[] = {
        bn::regular_bg_items::fleshwall};

}

namespace gt
{
    __::__(int map_index) : cells((bn::regular_bg_map_cell *)bn::memory::ewram_alloc(map_items[map_index].map_item().cells_count() * sizeof(bn::regular_bg_map_cell))),
                            map_item(cells[0], map_items[map_index].map_item().dimensions()),
                            bg_item(map_items[map_index].tiles_item(), map_items[map_index].palette_item(), map_item),
                            bg_ptr(bg_item.create_bg(0, 0)),
                            bg_map(bg_ptr.map())
    {
        bg_ptr.set_priority(2);
        bn::memory::copy(*map_items[map_index].map_item().cells_ptr(), map_items[map_index].map_item().cells_count(), cells[0]);

        bg_ptr.set_position((bg_map.dimensions().width() * 8) / 2 - (240 / 2), (bg_map.dimensions().height() * 8) / 2 - (160 / 2));

        cell_empty = map_item.cell(0, 0);
        cell_stick_up = map_item.cell(0, 31);
        cell_stick_down = map_item.cell(1, 31);
        cell_stick_left = map_item.cell(2, 31);
        cell_stick_right = map_item.cell(3, 31);
        cell_pipe_horizontal = map_item.cell(4, 31);
        cell_pipe_vertical = map_item.cell(5, 31);
        cell_corner_top_left = map_item.cell(6, 31);
        cell_corner_top_right = map_item.cell(7, 31);
        cell_corner_bottom_left = map_item.cell(8, 31);
        cell_corner_bottom_right = map_item.cell(9, 31);
        cell_edge_up = map_item.cell(10, 31);
        cell_edge_down = map_item.cell(11, 31);
        cell_edge_left = map_item.cell(12, 31);
        cell_edge_right = map_item.cell(13, 31);
        cell_full = map_item.cell(14, 31);
        cell_tumor = map_item.cell(16, 31);
        cell_enemy = map_item.cell(17, 31);
        cell_explosive = map_item.cell(18, 31);

        // update all cells
        /*
        int width = map_item.dimensions().width();
        int height = map_item.dimensions().height();
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                update_cell(x, y);
            }
        }
        */
        bg_map.reload_cells_ref();
        globals::tilemap = this;
    }

    void __::update_cell(int cell_x, int cell_y)
    {
        if (map_item.cell(cell_x, cell_y) == cell_empty)
        {
            cells[map_item.cell_index(cell_x, cell_y)] = cell_empty;
            return;
        }

        if(map_item.cell(cell_x, cell_y + 32) == cell_tumor)
        {
            cells[map_item.cell_index(cell_x, cell_y)] = cell_tumor;
            return;
        }

        bn::point left_pos = bn::point(cell_x - 1, cell_y);
        bn::point right_pos = bn::point(cell_x + 1, cell_y);
        bn::point up_pos = bn::point(cell_x, cell_y - 1);
        bn::point down_pos = bn::point(cell_x, cell_y + 1);
        bool left = is_cell_pos_inside_grid(left_pos) ? map_item.cell(left_pos) != cell_empty : true;
        bool right = is_cell_pos_inside_grid(right_pos) ? map_item.cell(right_pos) != cell_empty : true;
        bool up = is_cell_pos_inside_grid(up_pos) ? map_item.cell(up_pos) != cell_empty : true;
        bool down = is_cell_pos_inside_grid(down_pos) ? map_item.cell(down_pos) != cell_empty : true;

        if (left)
        {

            if (right)
            {
                if (up)
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_edge_down;
                        return;
                    }
                }
                else //! up
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_pipe_horizontal;
                        return;
                    }
                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_edge_up;
                        return;
                    }
                }
            }
            else //! right
            {

                if (up)
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_corner_bottom_right;
                        return;
                    }
                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_edge_right;
                        return;
                    }
                }
                else //! up
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_stick_right;
                        return;
                    }
                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_corner_top_right;
                        return;
                    }
                }
            }
        }
        else // !left
        {
            if (right)
            {
                if (up)
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_corner_bottom_left;
                        return;
                    }

                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_edge_left;
                        return;
                    }
                }
                else //! up
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_stick_left;
                        return;
                    }
                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_corner_top_left;
                        return;
                    }
                }
            }
            else //! right
            {
                if (up)
                {
                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_stick_down;
                        return;
                    }
                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_pipe_vertical;
                        return;
                    }
                }
                else //! up
                {
                    if (down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_stick_up;
                        return;
                    }

                    if (!down)
                    {
                        cells[map_item.cell_index(cell_x, cell_y)] = cell_empty;
                        return;
                    }
                }
            }
        }

        cells[map_item.cell_index(cell_x, cell_y)] = cell_full;
    }

    void __::update()
    {
        bn::fixed_point pos = bg_ptr.position();
        pos.set_x(pos.x() - bn::fixed(0.25));
        bg_ptr.set_position(pos);

        if (needs_rebuild)
        {
            needs_rebuild = false;
            bg_map.reload_cells_ref();
        }

        if (pos.x().right_shift_integer() < -(map_item.dimensions().width() * 8) / 2)
        {
            destroy();
        }
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(bg_ptr.position());
    }

    bn::point __::pos_to_cell_coord(bn::fixed_point pos)
    {
        bn::fixed diffx = pos.x() - bg_ptr.x();
        bn::fixed diffy = pos.y() - bg_ptr.y();

        int cell_x = diffx.right_shift_integer() / 8 + bg_map.dimensions().width() / 2;
        int cell_y = diffy.right_shift_integer() / 8 + bg_map.dimensions().height() / 2;

        if (diffx < bn::fixed(0))
            cell_x--;
        if (diffy < bn::fixed(0))
            cell_y--;

        return bn::point(cell_x, cell_y);
    }

    bn::fixed_point __::cell_coord_to_pos(bn::point cell_coord)
    {
        bn::fixed pos_x = bg_ptr.x() + bn::fixed((cell_coord.x() - bg_map.dimensions().width() / 2) * 8) + 4; 
        bn::fixed pos_y = bg_ptr.y() + bn::fixed((cell_coord.y() - bg_map.dimensions().height() / 2) * 8) + 4; 
        return bn::fixed_point(pos_x, pos_y);
    }

    bool __::is_cell_pos_inside_grid(bn::point cell_pos)
    {
        if (cell_pos.x() < 0)
            return false;
        if (cell_pos.y() < 0)
            return false;

        if (cell_pos.x() >= bg_map.dimensions().width())
            return false;
        if (cell_pos.y() >= bg_map.dimensions().height())
            return false;

        return true;
    }

    bool __::do_bullet_collision(bn::fixed_point pos)
    {
        bn::point cell_pos = pos_to_cell_coord(pos);
        // BN_LOG("cell_pos",cell_pos.x()," ",cell_pos.y());

        if (!is_cell_pos_inside_grid(cell_pos))
            return false;
        bn::regular_bg_map_cell cell = map_item.cell(cell_pos);
        if (cell != cell_empty)
        {
            if(cell != cell_tumor)
            {
                bn::regular_bg_map_cell logic_cell = cells[map_item.cell_index(cell_pos.x(),cell_pos.y()+32)];
                bool contains_enemy = logic_cell == cell_enemy;
                bool contains_explosive = logic_cell == cell_explosive;

                cells[map_item.cell_index(cell_pos)] = cell_empty;

                globals::ui->set_score(globals::ui->score() + 1);

                bn::point left_pos = bn::point(cell_pos.x() - 1, cell_pos.y());
                bn::point right_pos = bn::point(cell_pos.x() + 1, cell_pos.y());
                bn::point up_pos = bn::point(cell_pos.x(), cell_pos.y() - 1);
                bn::point down_pos = bn::point(cell_pos.x(), cell_pos.y() + 1);

                if (is_cell_pos_inside_grid(left_pos))
                    update_cell(left_pos.x(), left_pos.y());

                if (is_cell_pos_inside_grid(right_pos))
                    update_cell(right_pos.x(), right_pos.y());

                if (is_cell_pos_inside_grid(up_pos))
                    update_cell(up_pos.x(), up_pos.y());

                if (is_cell_pos_inside_grid(down_pos))
                    update_cell(down_pos.x(), down_pos.y());

                needs_rebuild = true;

                if(contains_enemy)
                {
                    globals::go_manager->add_object(new go_enemy_bullet(cell_coord_to_pos(cell_pos),bn::fixed_point(-1,0)));
                }
                else if(contains_explosive)
                {
                    globals::go_manager->add_object(new go_worm_bomb(cell_coord_to_pos(cell_pos), cell_pos));
                }
                else
                {
                    globals::go_manager->add_object(new go_blood_splash(cell_coord_to_pos(cell_pos)));
                }
            }
            return true;
        }
        return false;
    }

    bool __::is_colliding_with(bn::fixed_rect rect)
    {
        bn::point top_left = pos_to_cell_coord(rect.top_left());
        bn::point bottom_right = pos_to_cell_coord(rect.bottom_right());
        for(int y = top_left.y(); y <= bottom_right.y(); y++)
        {
            for(int x = top_left.x(); x <= bottom_right.x(); x++)
            {
                bn::point cell_pos = bn::point(x, y);
                if(is_cell_pos_inside_grid(cell_pos))
                {
                    if(map_item.cell(cell_pos) != cell_empty)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void __::tile_explosion(bn::point cell_pos)
    {
        for(int y = cell_pos.y() - 1;y <= cell_pos.y() + 1; y++)
        {
            for(int x = cell_pos.x() - 1; x <= cell_pos.x() + 1; x++)
            {
                bn::point cell_pos = bn::point(x, y);
                if(is_cell_pos_inside_grid(cell_pos))
                {
                    bn::regular_bg_map_cell cell = cells[map_item.cell_index(cell_pos)];
                    bn::regular_bg_map_cell logic_cell = cells[map_item.cell_index(cell_pos.x(),cell_pos.y()+32)];
                    if(cell != cell_empty && logic_cell != cell_tumor)
                    {
                        if(logic_cell == cell_explosive)
                        {
                            globals::go_manager->add_object(new go_worm_bomb(cell_coord_to_pos(cell_pos), cell_pos));
                        }
                        cells[map_item.cell_index(cell_pos)] = cell_empty;
                    }
                }
            }
        }

        for(int y = cell_pos.y() - 2;y <= cell_pos.y() + 2; y++)
        {
            for(int x = cell_pos.x() - 2; x <= cell_pos.x() + 2; x++)
            {
                bn::point cell_pos = bn::point(x, y);
                if(is_cell_pos_inside_grid(cell_pos))
                {
                    update_cell(cell_pos.x(), cell_pos.y());
                }
            }
        }
        needs_rebuild = true;
    }

    __::~__()
    {
        globals::tilemap = nullptr;
        bn::memory::ewram_free(cells);
        BN_LOG("tilemap destroyed");
    }

}