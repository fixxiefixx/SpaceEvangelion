#include "gt_go_enemy_pongboss.h"
#include "bn_sprite_items_boss1_together.h"
#include "bn_sprite_items_boss1_brain.h"
#include "bn_sprite_items_boss1_left.h"
#include "bn_sprite_items_boss1_right.h"
#include "gt_util.h"
#include "gt_globals.h"
#include "bn_sprites_mosaic.h"
#include "bn_blending.h"
#include "gt_go_explosion.h"
#include "bn_math.h"
#include "gt_go_enemy_bullet.h"

#define __ go_enemy_pongboss

#define MOVE_SPEED 1

namespace gt
{
    __::__(bn::fixed_point spawnpos) : _sprite(bn::sprite_items::boss1_together.create_sprite(spawnpos))
    {
        _local_collision_rect = bn::fixed_rect(0, 0, 32, 64);
        globals::ui->set_boss_enabled(true);
        globals::ui->set_boss_health(_health,max_health);
    }

    void __::update_ball_bounce(bn::fixed_rect ball_rect, bn::fixed_rect paddle_rect)
    {
        if(!ball_rect.intersects(paddle_rect))
            return;

        const bn::fixed margin = 4;
        bool left = ball_rect.x() < paddle_rect.x();
        bool top = ball_rect.y() < paddle_rect.y();
        if(left)
        {
            if(ball_rect.right()-margin <= paddle_rect.left())
            {
                velocity.set_x(-bn::abs(velocity.x()));
            }
            else
            {
                if(top)
                {
                    velocity.set_y(-bn::abs(velocity.y()));
                }
                else
                {
                    velocity.set_y(bn::abs(velocity.y()));
                }
            }
        }
        else //right
        {
            
            if(ball_rect.left()+margin >= paddle_rect.right())
            {
                velocity.set_x(bn::abs(velocity.x()));
            }
            else
            {
                if(top)
                {
                    velocity.set_y(-bn::abs(velocity.y()));
                }
                else
                {
                    velocity.set_y(bn::abs(velocity.y()));
                }
            }
        }
    }

    void __::update_collision()
    {
        bn::vector<game_object *, 8> bullets = globals::collision_map_player_bullets->get_colliding_objects(this);
        if (bullets.size() > 0)
        {
            bool health_decreased = false;
            for (int i = 0, limit = bullets.size(); i < limit; ++i)
            {
                game_object *bullet = bullets[i];

                bn::fixed bullet_y_to_my_y = (*bullet->get_pos()).y() - _sprite.y();

                if(bn::abs(bullet_y_to_my_y) <= 16)
                {
                    _health--;
                    if(_health < 0)
                    {
                        _health = 0;
                    }
                    else
                    {
                        health_decreased = true;
                    }
                }
                bullet->destroy();
            }
            if(health_decreased)
            {
                bn::sprites_mosaic::set_stretch(0.2);
                _sprite.set_mosaic_enabled(true);
                blink_timer = 5;
                globals::ui->set_boss_health(_health, max_health);

                if (_health <= 0)
                {
                    _sprite.set_z_order(0);
                    move_state = MOVE_STATE::DESTROYING;
                }
            }
        }

        bn::fixed_rect player_rect = *globals::player->get_collision_rect();
        bn::fixed_rect ball_rect = *get_collision_rect();

        if (player_rect.intersects(ball_rect))
        {
            globals::player->do_damage(1);
        }

        if(sprite_left.has_value())
        {
            bn::fixed_rect rect = bn::fixed_rect(sprite_left->position().x()+local_paddle_left_rect.x(),
                sprite_left->position().y()+local_paddle_left_rect.y(),local_paddle_left_rect.width(),local_paddle_left_rect.height());
            if(rect.intersects(player_rect))
            {
                globals::player->do_damage(1);
            }

            update_ball_bounce(ball_rect,rect);
        }

        if(sprite_right.has_value())
        {
            bn::fixed_rect rect = bn::fixed_rect(sprite_right->position().x()+local_paddle_right_rect.x(),
                sprite_right->position().y()+local_paddle_right_rect.y(),local_paddle_right_rect.width(),local_paddle_right_rect.height());
            if(rect.intersects(player_rect))
            {
                globals::player->do_damage(1);
            }

            update_ball_bounce(ball_rect,rect);
        }

    }

    void __::update_move_to_center()
    {
        bn::fixed_point pos = _sprite.position();
        bn::fixed_point target = bn::fixed_point(0,0);
        pos = move_towards(pos, target, 1);
        if(pos == target)
        {
            move_state = MOVE_STATE::MOVING_AROUND;
        }
        _sprite.set_position(pos);
    }

    void __::update_moving_around()
    {
        bn::fixed_point pos = _sprite.position();
        bn::fixed_point target = bn::fixed_point(bn::degrees_sin(time/3)* bn::fixed(100),bn::degrees_cos(time)*bn::fixed(50));
        pos = move_towards(pos, target, 2);

        //Shooting
        if(shoot_timer-- <= 0)
        {
            shoot_timer = shoot_duration;
            bn::fixed_point me_to_player = *(globals::player->get_pos()) - _sprite.position();
            bool left = me_to_player.x() < 0;
            if(left)
            {
                shoot_bullets_wide(_sprite.position() + bn::fixed_point(-16,0),true);
            }
            else
            {
                shoot_bullets_wide(_sprite.position() + bn::fixed_point(16,0),false);
            }
        }

        if(_health <= health_seperating)
        {
            move_state = MOVE_STATE::WOBBLE_AT_CENTER;
        }
        _sprite.set_position(pos);
        time++;
    }

    void __::update_wobble_at_center()
    {
        bn::fixed_point pos = _sprite.position();
        bn::fixed_point target = bn::fixed_point(bn::degrees_sin((time*100)/3)* bn::fixed(10),bn::degrees_cos(time*100)*bn::fixed(10));
        pos = move_towards(pos, target, 2);
        if(wobble_timer-- <= 0)
        {
            move_state = MOVE_STATE::SEPERATING;
            _sprite.set_item(bn::sprite_items::boss1_brain);
            _sprite.set_z_order(-1);
            _local_collision_rect = bn::fixed_rect(0,0,32,32);
            sprite_left = bn::sprite_items::boss1_left.create_sprite(_sprite.position());
            sprite_right = bn::sprite_items::boss1_right.create_sprite(_sprite.position());
        }
        _sprite.set_position(pos);
        time++;
    }

    void __::update_seperating()
    {
        bn::fixed_point pos_left = sprite_left->position();
        pos_left.set_x(move_towards(pos_left.x(),left_x,2));
        sprite_left->set_position(pos_left);

        bn::fixed_point pos_right = sprite_right->position();
        pos_right.set_x(move_towards(pos_right.x(),right_x,2));
        sprite_right->set_position(pos_right);

        if(seperating_timer-- <= 0)
        {
            move_state = MOVE_STATE::PLAYING_PONG;
            velocity = bn::fixed_point(1.5,1.5);
        }
    }

    void __::update_playing_pong()
    {
        //Update brain pos
        bn::fixed_point pos = _sprite.position();
        pos = pos + velocity;
        if(pos.x() < ball_min_x || pos.x() > ball_max_x)
        {
            velocity.set_x(bn::abs(velocity.x()));
        }
        if(pos.x() > ball_max_x)
        {
            velocity.set_x(-bn::abs(velocity.x()));
        }
        if(pos.y() < ball_min_y)
        {
            velocity.set_y(bn::abs(velocity.y()));
        }
        if(pos.y() > ball_max_y)
        {
            velocity.set_y(-bn::abs(velocity.y()));
        }
        _sprite.set_position(pos);

        //Update paddle left pos
        bn::fixed_point paddle_left_pos = sprite_left->position();
        bn::fixed target_y = clamp(globals::player->get_pos()->y(), paddle_min_y, paddle_max_y);
        paddle_left_pos.set_y(move_towards(paddle_left_pos.y(),target_y,1));
        sprite_left->set_position(paddle_left_pos);

        //Update paddle right pos
        bn::fixed_point paddle_right_pos = sprite_right->position();
        target_y = clamp(pos.y(), paddle_min_y, paddle_max_y);
        paddle_right_pos.set_y(move_towards(paddle_right_pos.y(),target_y,1));
        sprite_right->set_position(paddle_right_pos);

        //Paddle left shooting
        if(pos.x() < -(240/2)-16)
        {
            if(shoot_timer-- <= 0)
            {
                shoot_timer = shoot_duration_angry;
                shoot_bullets_wide(paddle_left_pos, false);
            }
        }

        //Ball shooting
        if(ball_shoot_timer-- <= 0)
        {
            ball_shoot_timer = ball_shoot_duration;
            globals::go_manager->add_object(new go_enemy_bullet(pos,bn::fixed_point(1,1)));
            globals::go_manager->add_object(new go_enemy_bullet(pos,bn::fixed_point(-1,1)));
            globals::go_manager->add_object(new go_enemy_bullet(pos,bn::fixed_point(-1,-1)));
            globals::go_manager->add_object(new go_enemy_bullet(pos,bn::fixed_point(1,-1)));
        }
    }

    void __::update_destroying()
    {
        if(destroy_explosion_timer-- <= 0)
        {
            destroy_explosion_timer = destroy_explosion_interval;
            globals::go_manager->add_object(new go_explosion(_sprite.position() + 
                bn::fixed_point(globals::random->get_fixed(-16,16),globals::random->get_fixed(-16,16))));
            if(destroy_explosion_counter-- <= 0)
            {
                globals::ui->set_score(globals::ui->score() + 50);
                destroy();
            }
        }
        if(sprite_left.has_value() && sprite_right.has_value())
        {
            sprite_left->set_position(sprite_left->position() + bn::fixed_point(-1,0));
            sprite_right->set_position(sprite_right->position() + bn::fixed_point(1,0));
        }
    }

    void __::update()
    {
        switch (move_state)
        {
            case MOVE_STATE::MOVE_TO_CENTER:
            {
                update_move_to_center();
            }break;
            case MOVE_STATE::MOVING_AROUND:
            {
                update_moving_around();
            }break;
            case MOVE_STATE::WOBBLE_AT_CENTER:
            {
                update_wobble_at_center();
            }break;
            case MOVE_STATE::SEPERATING:
            {
                update_seperating();
            }break;
            case MOVE_STATE::PLAYING_PONG:
            {
                update_playing_pong();
            }break;
            case MOVE_STATE::DESTROYING:
            {
                update_destroying();
            }break;
        }

        update_collision();

        if (blink_timer > 0)
        {
            blink_timer--;
            if (blink_timer <= 0)
            {
                _sprite.set_mosaic_enabled(false);
            }
        }
    }

    void __::shoot_bullets_wide(bn::fixed_point pos,bool left)
    {
        for(int i = -4; i <=4; i++)
        {
            bn::fixed angle = (left ? -90 : 90) + (i * 16);
            bn::fixed_point vel = bn::fixed_point(degrees_sin(angle),degrees_cos(angle));
            globals::go_manager->add_object(new go_enemy_bullet(pos,vel));
        }
    }

    bn::optional<bn::fixed_point> __::get_pos()
    {
        return bn::optional<bn::fixed_point>(_sprite.position());
    }

    __::~__()
    {
        globals::lvl->set_trigger();
        globals::ui->set_boss_enabled(false);
    }

}