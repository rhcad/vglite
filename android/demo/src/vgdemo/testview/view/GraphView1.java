// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

package vgdemo.testview.view;

import android.app.Activity;
import android.content.Context;
import touchvg.view.GraphView;
import touchvg.view.ViewHelper;

public class GraphView1 extends GraphView {
    
    public GraphView1(Context context) {
        super(context);
        
        int flags = ((Activity) context).getIntent().getExtras().getInt("flags");
        final ViewHelper helper = new ViewHelper(this);
        
        if ((flags & 32) != 0) {
            helper.addShapesForTest();
            flags = flags & ~32;
        }
        if ((flags & 1) != 0) {
            helper.setCommand("select");
        }
        else if ((flags >> 1) == 1) {
            helper.setCommand("splines");
        }
        else if ((flags >> 1) == 2) {
            helper.setCommand("line");
        }
        else if ((flags >> 1) == 3) {
            helper.setCommand("lines");
        }
    }
}
