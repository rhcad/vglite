//! \file demotrade_.h
//! \brief 定义示例行业应用内部入口类 DemoTrade_

#if !defined(__TOUCHVG_INTERNAL_DEMOTRADE_H_) && !defined(SWIG)
#define __TOUCHVG_INTERNAL_DEMOTRADE_H_

class MgView;

//! 示例行业应用内部入口类
/*! \ingroup CORE_COMMAND
 */
struct DemoTrade_ {
    //! 登记绘图命令和图形
    static void registerCmds();

    //! 得到当前图形的各种度量尺寸
    static int getDimensions(MgView* view, float* vars, char* types, int n);
};

#endif // __TOUCHVG_INTERNAL_DEMOTRADE_H_
