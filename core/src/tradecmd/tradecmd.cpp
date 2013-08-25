// tradecmd.cpp
#include "tradecmd.h"

void mgRegisterTradeCmds()
{
    //MgShapeT<MgCube>::registerCreator();
    //mgRegisterCommand<MgCmdDrawCube>();
}

int mgGetDimensions(MgView* view, float* vars, char* types, int count)
{
    for (int i = 0; i < count; i++) {
        vars[i] = 0;
        types[i] = 0;
    }
    MgCommand* cmd = view->cmds()->getCommand();
    return cmd ? cmd->getDimensions(view, vars, types, count) : 0;
}

void TradeCmd::onUnloadCmds()
{
}

void TradeCmd::drawInTriangle(MgCommandDraw*, GiGraphics*)
{
}

bool TradeCmd::selectActionHided(const MgMotion*)
{
    return false;
}

void TradeCmd::addShapeActions(int*, int &, const MgShape*)
{
}

bool TradeCmd::doAction(const MgMotion*, int)
{
    return false;
}

bool TradeCmd::doEndAction(const MgMotion*, int)
{
    return false;
}

void TradeCmd::onLineAdded(const MgMotion*, MgShape*)
{
}

void TradeCmd::onInitSelCmd(const MgMotion*)
{
}

void TradeCmd::drawInSelect(const MgShape*, int,
                            const MgMotion*, GiGraphics*, bool)
{
}

void TradeCmd::onSelectTouchEnded(const MgMotion*, int, int, int, int, int, const int*)
{
}
