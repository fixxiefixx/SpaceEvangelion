#include "bn_fixed_point.h"
#include "bn_math.h"
#include "bn_string_view.h"
#include "definitions.h"
#include "gt_globals.h"

namespace gt
{
    bn::fixed dot_product(bn::fixed_point a, bn::fixed_point b)
    {
        return (a.x() * b.x()) + (a.y() * b.y());
    }

    bn::fixed length_sqr(bn::fixed_point a)
    {
        return (a.x() * a.x()) + (a.y() * a.y());
    }

    bn::fixed length(bn::fixed_point a)
    {
        return bn::sqrt(length_sqr(a));
    }

    bn::fixed aprox_length(bn::fixed_point a)
    {
        return abs(a.x()) + abs(a.y());
    }

    bn::fixed_point normalize(bn::fixed_point a)
    {
        bn::fixed len = length(a);
        if(len == 0)
            return bn::fixed_point(0, 0);
        return a / len;
    }

    int clamp(int i, int min, int max)
    {
        if(i < min)
            return min;
        if(i > max)
            return max;
        return i;
    }

    bn::fixed clamp(bn::fixed i, bn::fixed min, bn::fixed max)
    {
        if(i < min)
            return min;
        if(i > max)
            return max;
        return i;
    }

    bn::fixed move_towards(bn::fixed current, bn::fixed target, bn::fixed maxDistanceDelta)
    {
        bn::fixed diff = target - current;
        if(abs(diff) <= maxDistanceDelta)
        {
            return target;
        }
        else
        {
            if(diff < 0)
            {
                return current - maxDistanceDelta;
            }
            else
            {
                return current + maxDistanceDelta;
            }
        }
    }

    bn::fixed_point move_towards(bn::fixed_point current, bn::fixed_point target, bn::fixed maxDistanceDelta)
    {
        bn::fixed diffX = target.x() - current.x();
        bn::fixed diffY = target.y() - current.y();
        bn::fixed distSqr = diffX * diffX + diffY * diffY;
        bool reached = distSqr == bn::fixed(0) || (maxDistanceDelta >= bn::fixed(0) && distSqr <= maxDistanceDelta * maxDistanceDelta);
        if(reached)
        {
            return target;
        }
        else
        {
            bn::fixed dist = bn::sqrt(distSqr);
            return bn::fixed_point(current.x() + diffX / dist * maxDistanceDelta, current.y() + diffY / dist * maxDistanceDelta);
        }
    }

    

     bn::fixed move_towards_angle(bn::fixed current, bn::fixed target, bn::fixed maxDelta)
     {
        bn::fixed total_rotation = target - current;
        while (total_rotation < -180) total_rotation += 360;
        while ( total_rotation >  180) total_rotation -= 360;
        return current + bn::min(maxDelta, bn::max(-maxDelta,total_rotation));

     }

    bn::string_view itemdef_to_stringview(Def item)
    {
        switch(item)
        {
            default:
            case Def::null:return "nothing";
        }
    }

    
    

}