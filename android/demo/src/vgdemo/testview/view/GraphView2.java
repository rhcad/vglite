// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.testview.view;

import android.content.Context;
import android.widget.FrameLayout;
import touchvg.view.GraphViewHelper;

public class GraphView2 extends FrameLayout {
    
    public GraphView2(Context context) {
        super(context);
        
        final GraphViewHelper helper = new GraphViewHelper();
        
        helper.setView(new GraphView1(context));
        helper.addViewsToLayout(this, context);
    }
}
