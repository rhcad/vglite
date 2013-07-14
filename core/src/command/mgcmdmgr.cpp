//! \file mgcmdmgr.cpp
//! \brief 实现命令管理器类 MgCmdManager
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#include "mgcmdmgr.h"

MgCmdManager::MgCmdManager()
{
}

MgCmdManager::~MgCmdManager()
{
}

void MgCmdManager::dynDraw(const MgMotion& motion, GiGraphics& gs)
{
}

bool MgCmdManager::onGesture(const MgMotion& motion)
{
    return false;
}

bool MgCmdManager::twoFingersMove(const MgMotion& motion)
{
    return false;
}
