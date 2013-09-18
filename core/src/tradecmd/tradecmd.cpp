// tradecmd.cpp
#include "tradecmd.h"
#include "demotrade_.h"
#include <mgcmd.h>

void DemoTrade_::registerCmds()
{
    //MgShapeT<MgCube>::registerCreator();
    //mgRegisterCommand<MgCmdDrawCube>();
}

int DemoTrade_::getDimensions(MgView* view, float* vars, char* types, int n)
{
    MgCommand* cmd = view->cmds()->getCommand();
    for (int i = 0; i < n; i++) {
        vars[i] = 0;
        types[i] = 0;
    }
    return cmd ? cmd->getDimensions(view, vars, types, n) : 0;
}

void TradeCmd::onUnloadCommands()
{
}

void TradeCmd::drawInShapeCommand(const char*, MgCommandDraw*, GiGraphics*)
{
}

bool TradeCmd::selectActionsNeedHided(const MgMotion*)
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

void TradeCmd::onShapeAdded(const MgMotion*, MgShape*)
{
}

void TradeCmd::onDocLoaded(const MgMotion*)
{
}

void TradeCmd::onEnterSelectCommand(const MgMotion*)
{
}

void TradeCmd::drawInSelectCommand(const MgShape*, int, const MgMotion*, GiGraphics*)
{
}

void TradeCmd::onSelectTouchEnded(const MgMotion*, int, int, int, int, int, const int*)
{
}
