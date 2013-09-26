// eduobserver.cpp
#include "educmds_.h"
#include <cmdsubject.h>
#include <mgcmd.h>
#include <mgspfactory.h>
#include <mgshapet.h>

class EduCmdObserver : public CmdObserverDefault
{
public:
    EduCmdObserver() {}
};

static EduCmdObserver _observer;

void EduCmds_::registerCmds(MgView* view)
{
    view->getCmdSubject()->registerObserver(&_observer);

    //MgShapeT<MgCube>::registerCreator(view->getShapeFactory());
    //mgRegisterCommand<MgCmdDrawCube>(view);
}

int EduCmds_::getDimensions(MgView*, float*, char*, int)
{
    return 0;
}
