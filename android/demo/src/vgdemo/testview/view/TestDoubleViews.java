//! \file TestDoubleViews.java
//! \brief 测试双视图布局的测试视图类
// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import touchvg.view.GraphSurfaceView;
import touchvg.view.GraphView;
import android.app.Activity;
import android.content.Context;
import android.widget.FrameLayout;

//! 测试双视图布局的测试视图类
public class TestDoubleViews extends FrameLayout {

	public TestDoubleViews(Context context) {
		super(context);
		
		int flags = ((Activity) context).getIntent().getExtras().getInt("flags");
		
		if (flags == 0) {
			GraphView view = new GraphView(context);
			LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
			addView(view, params);
			addView(view.getDynDrawView(), params);
		}
		else {
			GraphSurfaceView view = new GraphSurfaceView(context);
			LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
			addView(view, params);
			addView(view.getDynDrawView(), params);
		}
	}

}
