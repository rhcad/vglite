//! \file educmds_.h
//! \brief 定义示例应用内部入口类 EduCmds_

#if !defined(TOUCHVG_INTERNAL_EDUCMDS_H_) && !defined(SWIG)
#define TOUCHVG_INTERNAL_EDUCMDS_H_

class MgView;

//! 示例应用内部入口类
/*! \ingroup CORE_COMMAND
 */
struct EduCmds_ {
    //! 登记绘图命令和图形
    static void registerCmds(MgView* view);

    //! 得到当前图形的各种度量尺寸
    static int getDimensions(MgView* view, float* vars, char* types, int n);
};

#endif // TOUCHVG_INTERNAL_EDUCMDS_H_
