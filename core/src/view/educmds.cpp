// educmds.cpp
#include "educmds.h"
#include "gicoreview.h"
#include <educmds_.h>

void EduCmds::registerCmds(GiCoreView* coreView)
{
    EduCmds_::registerCmds(coreView->viewAdapter());
}

int EduCmds::getDimensions(GiCoreView* coreView, 
                           mgvector<float>& vars, mgvector<char>& types)
{
    int n = vars.count() < types.count() ? vars.count() : types.count();
    return EduCmds_::getDimensions(coreView->viewAdapter(), 
        vars.address(), types.address(), n);
}
