// tradecmd.cpp
#include "tradecmd.h"

void mgRegisterTradeCmds()
{
    //MgShapeT<MgCube>::registerCreator();
    //registerCommand<MgCmdDrawCube>();
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
