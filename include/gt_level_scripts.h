#include "gt_level.h"
#include "levels/gt_testlevel.h"
#include "levels/gt_level1.h"
#include "levels/gt_level2.h"
#include "levels/gt_level3.h"

namespace gt
{

    struct level_info
    {
        const gt::level::lvl_action *actions;
        int size;
    };

    constexpr const level_info levels[] =
    {
        //{gt::testlevel_actions,sizeof(gt::testlevel_actions) / sizeof(gt::level::lvl_action)},
        {gt::level1_actions,sizeof(gt::level1_actions) / sizeof(gt::level::lvl_action)},
        {gt::level2_actions,sizeof(gt::level2_actions) / sizeof(gt::level::lvl_action)},
        {gt::level3_actions,sizeof(gt::level3_actions) / sizeof(gt::level::lvl_action)}
    };

    constexpr const int level_count = sizeof(levels) / sizeof(level_info);

}