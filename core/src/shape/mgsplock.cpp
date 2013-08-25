// mgsplock.cpp: 实现 MgLockRW, MgShapesLock, MgDynShapeLock
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgshapedoc.h"
#include "mgcomposite.h"
#include "gidef.h"
#include <vector>

typedef std::pair<MgShapesLock::ShapesLocked, void*> ShapeObserver;
static std::vector<ShapeObserver>  s_shapeObservers;
static MgLockRW s_dynLock;

#ifdef _WIN32
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
void giSleep(int ms) { Sleep(ms); }
#else
#include <unistd.h>
void giSleep(int ms) { usleep(ms * 1000); }
#endif

// MgLockRW
//

MgLockRW::MgLockRW() : _editFlags(0)
{
    _counts[0] = _counts[1] = _counts[2] = 0;
}

bool MgLockRW::lockData(bool forWrite, int timeout)
{
    bool ret = false;
    
    if (1 == giInterlockedIncrement(_counts)) {     // first locked
        giInterlockedIncrement(_counts + (forWrite ? 2 : 1));
        ret = true;
    }
    else {
        ret = !forWrite && 0 == _counts[2];         // for read and not locked for write
        for (int i = 0; i < timeout && !ret; i += 25) {
            giSleep(25);
            ret = forWrite ? (!_counts[1] && !_counts[2]) : !_counts[2];
        }
        if (ret) {
            giInterlockedIncrement(_counts + (forWrite ? 2 : 1));
        }
        else {
            giInterlockedDecrement(_counts);
        }
    }
    
    return ret;
}

long MgLockRW::unlockData(bool forWrite)
{
    giInterlockedDecrement(_counts + (forWrite ? 2 : 1));
    return giInterlockedDecrement(_counts);
}

bool MgLockRW::firstLocked() const
{
    return _counts[0] == 1;
}

bool MgLockRW::lockedForRead() const
{
    return _counts[0] > 0;
}

bool MgLockRW::lockedForWrite() const
{
    return _counts[2] > 0;
}

// MgShapesLock
//

MgShapesLock::MgShapesLock(int flags, MgShapeDoc* d, int timeout) : doc(d)
{
    bool forWrite = (flags != 0);
    _mode = d && d->getLockData()->lockData(forWrite, timeout) ? (forWrite ? 2 : 1) : 0;
    if (_mode == 2 && flags == Unknown)
        _mode |= 4;
    if (_mode == 2 && doc->getLockData()->firstLocked()) {
        doc->getLockData()->setEditFlags(flags);
        for (std::vector<ShapeObserver>::iterator it = s_shapeObservers.begin();
             it != s_shapeObservers.end(); ++it) {
            (it->first)(doc, it->second, true);
        }
    }
}

MgShapesLock::~MgShapesLock()
{
    bool ended = false;
    
    if (locked() && doc) {
        MgComposite *group = (MgComposite *)doc->getCurrentShapes()->getOwner();
        if (group->isKindOf(MgComposite::Type())) {
            group->updateExtent();
        }
        ended = (0 == doc->getLockData()->unlockData((_mode & 2) != 0));
    }
    if (_mode == 2 && ended && doc) {
        doc->afterChanged();
        for (std::vector<ShapeObserver>::iterator it = s_shapeObservers.begin();
             it != s_shapeObservers.end(); ++it) {
            (it->first)(doc, it->second, false);
        }
    }
}

void MgShapesLock::registerObserver(ShapesLocked func, void* obj)
{
    if (func) {
        unregisterObserver(func, obj);
        s_shapeObservers.push_back(ShapeObserver(func, obj));
    }
}

void MgShapesLock::unregisterObserver(ShapesLocked func, void* obj)
{
    for (std::vector<ShapeObserver>::iterator it = s_shapeObservers.begin();
         it != s_shapeObservers.end(); ++it) {
        if (it->first == func && it->second == obj) {
            s_shapeObservers.erase(it);
            break;
        }
    }
}

bool MgShapesLock::locked() const
{
    return _mode != 0;
}

bool MgShapesLock::lockedForRead(MgShapeDoc* d)
{
    return d->getLockData()->lockedForRead();
}

bool MgShapesLock::lockedForWrite(MgShapeDoc* d)
{
    return d->getLockData()->lockedForWrite();
}

// MgDynShapeLock
//

MgDynShapeLock::MgDynShapeLock(bool forWrite, int timeout)
{
    _mode = s_dynLock.lockData(forWrite, timeout) ? (forWrite ? 2 : 1) : 0;
}

MgDynShapeLock::~MgDynShapeLock()
{
    if (locked()) {
        s_dynLock.unlockData(_mode == 2);
    }
}

bool MgDynShapeLock::locked() const
{
    return _mode != 0;
}

bool MgDynShapeLock::lockedForRead()
{
    return s_dynLock.lockedForRead();
}

bool MgDynShapeLock::lockedForWrite()
{
    return s_dynLock.lockedForWrite();
}
