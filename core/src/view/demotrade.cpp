// demotrade.cpp
#include "demotrade.h"
#include "gicoreview.h"
#include <demotrade_.h>

void DemoTrade::registerCmds()
{
    DemoTrade_::registerCmds();
}

int DemoTrade::getDimensions(GiCoreView* coreView, 
                             mgvector<float>& vars, mgvector<char>& types)
{
    int n = vars.count() < types.count() ? vars.count() : types.count();
    return DemoTrade_::getDimensions(coreView->viewAdapter(), 
        vars.address(), types.address(), n);
}
