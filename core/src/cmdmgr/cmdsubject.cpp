// cmdsubject.cpp
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgcmdmgr_.h"
#include <cmdsubject.h>
#include <vector>

class CmdSubjectImpl : public CmdSubject
{
public:
    CmdSubjectImpl() {}

private:
    virtual void registerObserver(CmdObserver* observer) {
        if (observer) {
            unregisterObserver(observer);
            _arr.push_back(observer);
        }
    }
    virtual void unregisterObserver(CmdObserver* observer) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            if (*it == observer) {
                _arr.erase(it);
                break;
            }
        }
    }
    virtual void onDocLoaded(const MgMotion* sender) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->onDocLoaded(sender);
        }
    }
    virtual void onEnterSelectCommand(const MgMotion* sender) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->onEnterSelectCommand(sender);
        }
    }
    virtual void onUnloadCommands(MgCmdManager* sender) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->onUnloadCommands(sender);
        }
    }
    virtual bool selectActionsNeedHided(const MgMotion* sender) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            if ((*it)->selectActionsNeedHided(sender))
                return true;
        }
        return false;
    }
    virtual void addShapeActions(const MgMotion* sender,
        int* actions, int &n, const MgShape* shape) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->addShapeActions(sender, actions, n, shape);
        }
    }
    virtual bool doAction(const MgMotion* sender, int action) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            if ((*it)->doAction(sender, action))
                return true;
        }
        return false;
    }
    virtual bool doEndAction(const MgMotion* sender, int action) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            if ((*it)->doEndAction(sender, action))
                return true;
        }
        return false;
    }
    virtual void onShapeAdded(const MgMotion* sender, MgShape* shape) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->onShapeAdded(sender, shape);
        }
    }
    virtual void drawInShapeCommand(const MgMotion* sender, const char* name, 
        MgCommandDraw* cmd, GiGraphics* gs) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->drawInShapeCommand(sender, name, cmd, gs);
        }
    }
    virtual void drawInSelectCommand(const MgMotion* sender, 
        const MgShape* shape, int handleIndex, GiGraphics* gs) {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->drawInSelectCommand(sender, shape, handleIndex, gs);
        }
    }
    virtual void onSelectTouchEnded(const MgMotion* sender, int shapeid,
        int handleIndex, int snapid, int snapHandle,
        int count, const int* ids)
    {
        for (Iterator it = _arr.begin(); it != _arr.end(); ++it) {
            (*it)->onSelectTouchEnded(sender, shapeid, 
                handleIndex, snapid, snapHandle, count, ids);
        }
    }

private:
    typedef std::vector<CmdObserver*> Observers;
    typedef Observers::iterator Iterator;
    Observers _arr;
};

CmdSubject* MgCmdManagerImpl::getCmdSubject()
{
    static CmdSubjectImpl obj;
    return &obj;
}
