//! \file TestMagnifier1.java
//! \brief 测试放大镜的布局视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import android.content.Context;
import android.graphics.Color;
import android.widget.FrameLayout;
import touchvg.view.GraphView;

//! 测试放大镜的布局视图类
public class TestMagnifier1 extends FrameLayout {

    public TestMagnifier1(Context context) {
        super(context);
                
        final GraphView1 view = new GraphView1(context);
        addView(view, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        
        final GraphView magView = new GraphView(context, view);
        addView(magView, new LayoutParams(200, 200));
        magView.setBackgroundColor(Color.GREEN);
    }

}
