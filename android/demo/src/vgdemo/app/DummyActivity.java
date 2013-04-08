// Copyright (c) 2012-2013, https://github.com/rhcad/vglite

package vgdemo.app;

import java.lang.reflect.Constructor;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;

public class DummyActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Bundle bundle = getIntent().getExtras();
        int flags = bundle.getInt("flags");
        View view = null;
        
        try {
            Class<?> c = Class.forName(bundle.getString("className"));
            Constructor<?> c1 = c.getDeclaredConstructor(new Class[]{ Context.class });
            c1.setAccessible(true);
            view = (View)c1.newInstance(new Object[]{this});
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        if ((flags & 0x100000) != 0) {		// 有底部SurfaceView
        	this.setContentView(view);
        }
        else {
        	LinearLayout layout = new LinearLayout(this);
        	layout.setOrientation(LinearLayout.VERTICAL);
        	layout.setBackgroundResource(R.drawable.translucent_bg);    // 测试图片平铺
            this.setContentView(layout);
            
            if (view != null) {
                layout.addView(view, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
            }
        }
        this.setTitle(bundle.getString("title"));
    }
    
    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
