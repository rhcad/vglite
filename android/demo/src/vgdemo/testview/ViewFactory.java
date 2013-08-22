//! \file ViewFactory.java
//! \brief 基于普通View类的绘图测试视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview;

import java.util.ArrayList;
import java.util.List;

//! 测试视图的构造列表类
public class ViewFactory {

    public static class DummyItem {

        public String id;
        public int flags;
        public String title;

        public DummyItem(String id, int flags, String title) {
            this.id = id;
            this.flags = flags;
            this.title = title;
        }

        @Override
        public String toString() {
            return title;
        }
    }

    public static List<DummyItem> ITEMS = new ArrayList<DummyItem>();

    static {
        addItem("vgdemo.testview.view.GraphView1", 1<<1, "GraphView splines");
        addItem("vgdemo.testview.view.GraphView1", 2<<1, "GraphView line");
        addItem("vgdemo.testview.view.GraphView1", 3<<1, "GraphView lines");
        addItem("vgdemo.testview.view.GraphView1", 1, "GraphView select");
        addItem("vgdemo.testview.view.TestMagnifier1", 1, "TestMagnifier");
        addItem("vgdemo.testview.view.TestMagnifier1", 16|1, "TestMagnifier, 2 views");
        
        addItem("vgdemo.testview.view.GraphView", 0, "TestOneView");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|0, "TestOneSurfaceView");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|0|0x100000, "TestOneSurfaceView, back");
        addItem("vgdemo.testview.view.GraphViewCached", 0, "GraphViewCached");
        
        addItem("vgdemo.testview.view.TestDoubleViews", 0|2, "Test2Views, std view+view");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|2, "Test2Views, top surface+view");
        addItem("vgdemo.testview.view.TestDoubleViews", 0|4, "Test2Views, std view+surface");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|4, "Test2Views, top surface+surface");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|2|0x100000, "Test2Views, back surface+view");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|4|0x100000, "Test2Views, back surface+surface");
        addItem("vgdemo.testview.view.TestDoubleViews", 8|4, "Test2Views, cachedview+surface");
        addItem("vgdemo.testview.view.TestDoubleViews", 8|2, "Test2Views, cachedview+view");
        
        addItem("vgdemo.testview.view.LargeView2", 0|2, "Test2Views, scroll view+view");
        addItem("vgdemo.testview.view.LargeView2", 1|2, "Test2Views, scroll surface+view");
        addItem("vgdemo.testview.view.LargeView2", 0|4, "Test2Views, scroll view+surface");
        addItem("vgdemo.testview.view.LargeView2", 1|4, "Test2Views, scroll surface+surface");
        addItem("vgdemo.testview.view.LargeView2", 0|0, "TestOneView, scroll");
        addItem("vgdemo.testview.view.LargeView2", 1|0, "TestOneSurfaceView, scroll");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|2|0x100000, "Test2Views, scroll back surface+view");
        addItem("vgdemo.testview.view.TestDoubleViews", 1|4|0x100000, "Test2Views, scroll back surface+surface");
        addItem("vgdemo.testview.view.LargeView2", 8|4, "Test2Views, scroll cachedview+surface");
        addItem("vgdemo.testview.view.LargeView2", 8|2, "Test2Views, scroll cachedview+surface");
        
        addItem("vgdemo.testview.canvas.GraphView1", 0x01, "testRect");
        addItem("vgdemo.testview.canvas.GraphView1", 0x02, "testLine");
        addItem("vgdemo.testview.canvas.GraphView1", 0x04, "testTextAt");
        addItem("vgdemo.testview.canvas.GraphView1", 0x08, "testEllipse");
        addItem("vgdemo.testview.canvas.GraphView1", 0x10, "testQuadBezier");
        addItem("vgdemo.testview.canvas.GraphView1", 0x20, "testCubicBezier");
        addItem("vgdemo.testview.canvas.GraphView1", 0x40, "testPolygon");
        addItem("vgdemo.testview.canvas.GraphView1", 0x80|0x40|0x02, "testClearRect");
        addItem("vgdemo.testview.canvas.GraphView1", 0x100, "testClipPath");
        addItem("vgdemo.testview.canvas.GraphView1", 0x200, "testHandle");
        addItem("vgdemo.testview.canvas.GraphView2", 0x400, "testDynCurves");
        
        addItem("vgdemo.testview.canvas.SurfaceView1", 0x20, "testCubicBezier in SurfaceView");
        addItem("vgdemo.testview.canvas.SurfaceView1", 0x80|0x40|0x02, "testClearRect in SurfaceView");
        addItem("vgdemo.testview.canvas.SurfaceView2", 0x20, "testCubicBezier in SurfaceView with thread");
        addItem("vgdemo.testview.canvas.SurfaceView2", 0x02, "testLine in SurfaceView with thread");
        addItem("vgdemo.testview.canvas.SurfaceView2", 0x400, "testDynCurves in SurfaceView with touch");
        addItem("vgdemo.testview.canvas.SurfaceView3", 0x400, "testDynCurves in SurfaceView with thread");
        addItem("vgdemo.testview.canvas.SurfaceView3", 0x400|0x1000, "testDynCurves(OPAQUE) with thread");
        
        addItem("vgdemo.testview.canvas.LargeView1", 0x04|0x10000, "testTextAt in large view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x04|0x20000, "testTextAt in large surface view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x20|0x10000, "testCubicBezier in large view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x20|0x20000, "testCubicBezier in large surface view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x200|0x10000, "testHandle in large view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x200|0x20000, "testHandle in large surface view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x400|0x10000, "testDynCurves in large view");
        addItem("vgdemo.testview.canvas.LargeView1", 0x400|0x20000, "testDynCurves in large surface view");
    }
    
    private static void addItem(String id, int flags, String title) {
        ITEMS.add(new DummyItem(id, flags, title));
    }
}
