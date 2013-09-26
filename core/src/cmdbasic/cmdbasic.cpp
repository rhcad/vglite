// cmdbasic.cpp: ʵ�ֻ�����ͼ������� MgBasicCommands
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "cmdbasic.h"
#include "mgcmderase.h"
#include "mgdrawrect.h"
#include "mgdrawline.h"
#include "mgdrawlines.h"
#include "mgdrawfreelines.h"
#include "mgdrawsplines.h"
#include "mgdrawtriang.h"
#include "mgdrawarc.h"
#include "mgdrawellipse.h"
#include "mgdrawdiamond.h"
#include "mgdrawcircle.h"
#include "mgdrawsquare.h"
#include "mgdrawquadrangle.h"
#include "mgdrawparallel.h"
#include "mgdrawgrid.h"

void MgBasicCommands::registerCmds(MgView* view)
{
	mgRegisterCommand<MgCmdErase>(view);
    mgRegisterCommand<MgCmdDrawRect>(view);
    mgRegisterCommand<MgCmdDrawSquare>(view);
    mgRegisterCommand<MgCmdDrawEllipse>(view);
    mgRegisterCommand<MgCmdDrawCircle>(view);
    mgRegisterCommand<MgCmdDrawDiamond>(view);
    mgRegisterCommand<MgCmdDrawLine>(view);
    mgRegisterCommand<MgCmdDrawFixedLine>(view);
    mgRegisterCommand<MgCmdDrawPolygon>(view);
    mgRegisterCommand<MgCmdDrawQuadrangle>(view);
    mgRegisterCommand<MgCmdDrawLines>(view);
    mgRegisterCommand<MgCmdDrawFreeLines>(view);
    mgRegisterCommand<MgCmdDrawSplines>(view);
    mgRegisterCommand<MgCmdDrawSplineMouse>(view);
    mgRegisterCommand<MgCmdDrawTriangle>(view);
    mgRegisterCommand<MgCmdParallel>(view);
    mgRegisterCommand<MgCmdDrawGrid>(view);
    mgRegisterCommand<MgCmdArc3P>(view);
    mgRegisterCommand<MgCmdArcCSE>(view);
    mgRegisterCommand<MgCmdArcTan>(view);
}
