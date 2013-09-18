//! \file demotrade.h
//! \brief 定义示例行业应用的功能入口类 DemoTrade

#ifndef __TOUCHVG_DEMOTRADE_H_
#define __TOUCHVG_DEMOTRADE_H_

#include "mgvector.h"

class GiCoreView;

//! 示例行业应用的功能入口类
/*! \ingroup CORE_VIEW
 */
struct DemoTrade {
    //! 登记绘图命令和图形类型
    static void registerCmds();

    //! 得到当前图形的各种度量尺寸
    static int getDimensions(GiCoreView* coreView, 
		mgvector<float>& vars, mgvector<char>& types);
};

#endif // __TOUCHVG_DEMOTRADE_H_
