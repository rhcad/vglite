//! \file testcanvas.h
//! \brief Define the testing class: TestCanvas.
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

#ifndef VGLITE_TESTCANVAS_H
#define VGLITE_TESTCANVAS_H

class GiCanvas;

//! The testing class for GiCanvas.
/*! \ingroup GROUP_GRAPH
 */
struct TestCanvas {
    static void initRand();
    static int randInt(int minv, int maxv);
    static float randFloat(float minv, float maxv);
    
    static void test(GiCanvas& canvas, int bits, int n = 100, bool randStyle = true);
    
    static void testRect(GiCanvas& canvas, int n);
    static void testLine(GiCanvas& canvas, int n);
    static void testEllipse(GiCanvas& canvas, int n);
    static void testQuadBezier(GiCanvas& canvas, int n);
    static void testCubicBezier(GiCanvas& canvas, int n);
    static void testPolygon(GiCanvas& canvas, int n);
    static void testClipPath(GiCanvas& canvas, int n);
    static void testHandle(GiCanvas& canvas, int n);
    static void testDynCurves(GiCanvas& canvas);
    static void testTextAt(GiCanvas& canvas, int n);
};

#endif // VGLITE_TESTCANVAS_H
