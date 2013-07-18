// mgactions.cpp: 实现上下文动作分发接口
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgcmdmgr.h"
#include <mgselect.h>
#include <mgshapetype.h>
#include <mglog.h>

bool MgCmdManagerImpl::showInDrawing(const MgMotion* sender, const MgShape* shape)
{
    return showInSelect(sender, kMgSelDraw, shape, Box2d(sender->pointM, 0, 0));
}

bool MgCmdManagerImpl::showInSelect(const MgMotion* sender, int selState, const MgShape* shape, const Box2d& selbox)
{
    int actions[12];
    int n = 0;
    bool isslines = (shape && selState == kMgSelOneShape
                     && shape->shapec()->isKindOf(kMgShapeSplines));
    bool isOpenLines = (shape && selState == kMgSelOneShape
                        && shape->shapec()->isKindOf(kMgShapeLines)
                        && !shape->shapec()->isClosed());
    bool locked = shape && shape->shapec()->getFlag(kMgShapeLocked);
    bool fixedLength = shape && shape->shapec()->getFlag(kMgFixedLength);
    
    switch (selState) {
        case kMgSelNone:
            break;
            
        case kMgSelMultiShapes:
            if (shape && shape->getParent()
                && shape->getParent()->getOwner()->isKindOf(kMgShapeDoc)) {
                actions[n++] = kMgActionGroup;
            }
        case kMgSelOneShape:
            if (!locked && !sender->view->useFinger()) {
                actions[n++] = kMgActionDelete;
            }
            actions[n++] = kMgActionClone;
            if (!locked && shape
                && (shape->shapec()->isKindOf(kMgShapeImage)
                    || shape->shapec()->isKindOf(kMgShapeLine))) {
                actions[n++] = fixedLength ? kMgActionFreeLength : kMgActionFixedLength;
            }
            actions[n++] = locked ? kMgActionUnlocked : kMgActionLocked;
            if (selState == kMgSelOneShape && !locked) {
                actions[n++] = kMgActionEditVertex;
            }
            else {
                //actions[n++] = kMgActionSelReset;
            }
            if (selState == kMgSelOneShape
                && shape && shape->shapec()->isClosed()
                && !shape->shapec()->getFlag(kMgRotateDisnable)
                && !shape->shapec()->getFlag(kMgShapeLocked)
                && (shape->shapec()->isKindOf(kMgShapeParallelogram)
                    || shape->shapec()->isKindOf(kMgShapeLines)) ) {
                actions[n++] = kMgActionOverturn;
            }
            break;
            
        case kMgSelVertexes:
            if ((isslines || isOpenLines)
                && shape && !shape->shapec()->getFlag(kMgShapeLocked)) {
                //actions[n++] = closed ? kMgActionOpened : kMgActionClosed;
                actions[n++] = kMgActionAddVertex;
            }
            if (!locked && shape && shape->shapec()->isKindOf(kMgShapeLine)) {
                actions[n++] = fixedLength ? kMgActionFreeLength : kMgActionFixedLength;
            }
            //actions[n++] = locked ? kMgActionUnlocked : kMgActionLocked;
            actions[n++] = kMgActionHideVertex;
            break;
            
        case kMgSelVertex:
            if ((isslines || isOpenLines)
                && shape && !shape->shapec()->getFlag(kMgShapeLocked)) {
                //actions[n++] = closed ? kMgActionOpened : kMgActionClosed;
                actions[n++] = kMgActionDelVertex;
            }
            if (!locked && shape && shape->shapec()->isKindOf(kMgShapeLine)) {
                actions[n++] = fixedLength ? kMgActionFreeLength : kMgActionFixedLength;
            }
            //actions[n++] = locked ? kMgActionUnlocked : kMgActionLocked;
            actions[n++] = kMgActionHideVertex;
            break;
            
        case kMgSelDraw:
            break;
    }
    
    if (selState > kMgSelNone && selState <= kMgSelVertex && shape) {
        const MgBaseShape* sp = shape->shapec();
        if (!locked && sp->isKindOf(kMgShapeLine)) {
            actions[n++] = kMgActionBreak;
        }
        if (!locked && sp->isClosed() && sp->getPointCount() == 4) {
            if (sp->isKindOf(kMgShapeLine)
                || sp->isKindOf(kMgShapeRect)
                || sp->isKindOf(kMgShapeDiamond)
                || (sp->isKindOf(kMgShapeLines) && sp->isClosed())
                || sp->isKindOf(kMgShapeParallelogram)) {
                actions[n++] = kMgActionBreak;
            }
        }
        if (sp->isKindOf(kMgShapeGroup)) {
            actions[n++] = kMgActionUngroup;
        }
    }
    
    actions[n++] = 0;
    
    if (!selbox.isEmpty()) {
        sender->view->redraw();
    }
    
    Box2d box(selbox.isEmpty() ? Box2d(sender->pointM, 0, 0) : selbox);
    box *= sender->view->xform()->modelToDisplay();
    
    return sender->view->showContextActions(selState, actions, box, shape);
}

void MgCmdManagerImpl::doAction(const MgMotion* sender, int action)
{
    MgView* view = sender->view;
    MgSelection *sel = getSelection(view);
    bool ret = true;
    
    switch (action) {
        case kMgActionSelAll:
            if (sel) sel->selectAll(sender);
            break;
            
        case kMgActionSelReset:
            if (sel) sel->resetSelection(sender);
            break;
            
        case kMgActionDraw:
            setCommand(sender, "@draw");
            break;
            
        case kMgActionCancel:
            setCommand(sender, "select");
            break;
            
        case kMgActionDelete:
            if (sel) sel->deleteSelection(sender);
            break;
            
        case kMgActionClone:
            if (sel) sel->cloneSelection(sender);
            break;

        case kMgActionGroup:
            if (sel) sel->groupSelection(sender);
            break;

        case kMgActionUngroup:
            if (sel) sel->ungroupSelection(sender);
            break;
            
        case kMgActionBreak:
            setCommand(sender, "break");
            break;
            
        case kMgActionCorner:
            setCommand(sender, "corner");
            break;
            
        case kMgActionFixedLength:
        case kMgActionFreeLength:
            if (sel) sel->setFixedLength(sender, !sel->isFixedLength(view));
            break;
            
        case kMgActionLocked:
        case kMgActionUnlocked:
            if (sel) sel->setLocked(sender, !sel->isLocked(view));
            break;
            
        case kMgActionEditVertex:
        case kMgActionHideVertex:
            if (sel) sel->setEditMode(sender, !sel->isEditMode(view));
            break;
            
        case kMgActionClosed:
        case kMgActionOpened:
            if (sel) sel->switchClosed(sender);
            break;
            
        case kMgActionAddVertex:
            if (sel) sel->insertVertext(sender);
            break;
            
        case kMgActionDelVertex:
            if (sel) sel->deleteVertext(sender);
            break;
            
        case kMgActionOverturn:
            if (sel) sel->overturnPolygon(sender);
            break;
            
        default: {
            MgCommand* cmd = getCommand(sender->view, true);
            ret = cmd && cmd->doContextAction(sender, action);
            if (!ret) {
                const char* name = _drawcmd.c_str();
                if (kMgAction3Views == action)
                    name = "cube";
                cmd = findCommand(name);
                ret = cmd && cmd->doContextAction(sender, action);
            }
            break;
        }
    }
}
