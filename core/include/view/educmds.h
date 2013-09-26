//! \file educmds.h
//! \brief 定义示例应用的功能入口类 EduCmds

#ifndef TOUCHVG_EDUCMDS_H_
#define TOUCHVG_EDUCMDS_H_

#include "mgvector.h"

class GiCoreView;

//! 示例应用的功能入口类
/*! \ingroup CORE_VIEW
 */
struct EduCmds {
    //! 登记绘图命令和图形类型
    static void registerCmds(GiCoreView* coreView);

    //! 得到当前图形的各种度量尺寸
    static int getDimensions(GiCoreView* coreView, 
		mgvector<float>& vars, mgvector<char>& types);
};

#endif // TOUCHVG_EDUCMDS_H_
