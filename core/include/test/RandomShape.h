#ifndef _TEST_RANDOMSHAPE_H
#define _TEST_RANDOMSHAPE_H

#include "mgshapes.h"
#include "mgshape.h"

struct RandomParam
{
    int lineCount;
    int rectCount;
    int arcCount;
    int curveCount;
    bool randomLineStyle;
    
    static void init();

    RandomParam(int curve = 10) : lineCount(10), rectCount(0), arcCount(10)
    , curveCount(curve), randomLineStyle(true) {}

    int getShapeCount() const { return lineCount + rectCount + arcCount + curveCount; }
    int addShapes(MgShapes* shapes);
    void setShapeProp(GiContext* context);

    static float RandF(float dMin, float dMax);
    static int RandInt(int nMin, int nMax);
    static unsigned char RandUInt8(int nMin, int nMax);
};

#endif // _TEST_RANDOMSHAPE_H
