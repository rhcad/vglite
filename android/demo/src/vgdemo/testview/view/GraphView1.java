// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import android.app.Activity;
import android.content.Context;
import touchvg.view.GraphView;

public class GraphView1 extends GraphView {
	
	public GraphView1(Context context) {
        super(context);
        
        int flags = ((Activity) context).getIntent().getExtras().getInt("flags");
        
        if ((flags & 1) != 0) {
        	setCommand("splines");
        }
        else if ((flags & 2) != 0) {
        	addShapesForTest();
        	setCommand("select");
        }
	}
}
