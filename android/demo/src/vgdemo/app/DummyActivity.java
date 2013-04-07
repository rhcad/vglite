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
        
        //int color = Color.LTGRAY;
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        //layout.setBackgroundColor(color);
        layout.setBackgroundResource(R.drawable.translucent_bg);    // 测试图片平铺
        this.setContentView(layout);
        
        Bundle bundle = this.getIntent().getExtras();
        View view = null;
        
        try {
            Class<?> c = Class.forName(bundle.getString("className"));
            Constructor<?> c1 = c.getDeclaredConstructor(new Class[]{ Context.class });
            c1.setAccessible(true);
            view = (View)c1.newInstance(new Object[]{this});
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        if (view != null) {
            layout.addView(view, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
            //view.setBackgroundColor(color);             // 将只应用在画布上，视图重载后仍然是透明色
            this.setTitle(bundle.getString("title"));
        }
    }
    
    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
