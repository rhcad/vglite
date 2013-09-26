// Copyright (c) 2012-2013, https://github.com/rhcad/touchvg

package vgdemo.testview.view;

import android.app.Activity;
import android.content.Context;
import touchvg.view.GraphView;
import touchvg.view.ViewHelper;
import vgdemo.app.R;

public class GraphView1 extends GraphView {
    private static final int[] HANDLEIDS = {
        R.drawable.vgdot1, R.drawable.vgdot2, R.drawable.vgdot3,
        R.drawable.vg_lock, R.drawable.vg_unlock, 0 };
    
    public GraphView1(Context context) {
        super(context);
        
        int flags = ((Activity) context).getIntent().getExtras().getInt("flags");
        final ViewHelper helper = new ViewHelper(this);
        
        helper.setContextButtonImages(null, null, HANDLEIDS);
        
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
