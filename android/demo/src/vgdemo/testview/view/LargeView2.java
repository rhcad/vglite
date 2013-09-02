//! \file LargeView2.java
//! \brief 测试长幅绘图的滚动视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

package vgdemo.testview.view;

import android.content.Context;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ScrollView;

//! 测试长幅绘图的滚动视图类
public class LargeView2 extends ScrollView {

    public LargeView2(Context context) {
        super(context);
        this.createContentView(context);
    }

    private void createContentView(Context context) {
        final View view = new TestDoubleViews(context);
        final FrameLayout layout = new FrameLayout(context);
        layout.addView(view, new LayoutParams(LayoutParams.MATCH_PARENT, 2048));
        this.addView(layout);
    }
}
