//! \file ContextAction.java
//! \brief Android绘图视图类

package touchvg.view;

import touchvg.jni.GiCoreView;
import touchvg.jni.Ints;
import android.content.Context;
import android.graphics.Rect;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

//! 绘图视图的上下文操作布局类
public class ContextAction {

    private GiCoreView mCoreView;
    private GraphView mView;
    private Context mContext;
    private RelativeLayout mButtonLayout;
    private int[] mImageIDs;
    private static final String[] BUTTON_CAPTIONS = { null, "全选", "重选", "绘图",
            "取消", "删除", "克隆", "定长", "不定长", "锁定", "解锁", "编辑", "返回",
            "闭合", "不闭合", "加点", "删点", "成组", "解组", "翻转" };
    
    public ContextAction(Context context, GiCoreView core, GraphView view) {
        this.mContext = context;
        this.mCoreView = core;
        this.mView = view;
    }
    
    public void release() {
        this.mContext = null;
        this.mCoreView = null;
        this.mView = null;
    }
    
    public boolean isVisible() {
        return mButtonLayout != null;
    }

    public boolean showActions(Ints actions, float x, float y, float w, float h) {
        removeButtonLayout();

        int n = actions.count();
        if (n == 0) {
            return true;
        }

        final Rect selbox = new Rect((int) x, (int) y, (int) (x + w), (int) (y + h));
        mButtonLayout = new RelativeLayout(mContext);

        if (selbox.height() < (n < 7 ? 40 : 80)) {
            selbox.inset(0, (selbox.height() - (n < 7 ? 40 : 80)) / 2);
        }
        if (selbox.width() < ((n == 3 || n > 4) ? 120 : 40)) {
            selbox.inset((selbox.width() - ((n == 3 || n > 4) ? 120 : 40)) / 2, 0);
        }
        selbox.inset(-12, -15);

        for (int i = 0, index = 0; i < n; i++) {
            if (addContextAction(n, index, actions.get(i), selbox, mButtonLayout)) {
                index++;
            }
        }

        final ViewGroup f = (ViewGroup) mView.getParent();
        if (mButtonLayout != null) {
            f.addView(mButtonLayout);
        }
        
        return isVisible();
    }

    private boolean addContextAction(int n, int index, int action,
            final Rect selbox, final RelativeLayout layout) {
        int xc, yc;

        switch (index) {
        case 0:
            if (n == 1) {
                xc = selbox.centerX();
                yc = selbox.top;    // MT
            } else {
                xc = selbox.left;
                yc = selbox.top;    // LT
            }
            break;
        case 1:
            if (n == 3) {
                xc = selbox.centerX();
                yc = selbox.top;    // MT
            } else {
                xc = selbox.right;
                yc = selbox.top;    // RT
            }
            break;
        case 2:
            if (n == 3) {
                xc = selbox.right;
                yc = selbox.top;    // RT
            } else {
                xc = selbox.right;
                yc = selbox.bottom; // RB
            }
            break;
        case 3:
            xc = selbox.left;
            yc = selbox.bottom;     // LB
            break;
        case 4:
            xc = selbox.centerX();
            yc = selbox.top;        // MT
            break;
        case 5:
            xc = selbox.centerX();
            yc = selbox.bottom;     // MB
            break;
        case 6:
            xc = selbox.right;
            yc = selbox.centerY();  // RM
            break;
        case 7:
            xc = selbox.left;
            yc = selbox.centerY();  // LM
            break;
        default:
            return false;
        }

        final Button btn = new Button(mContext);
        boolean hasImage = setButtonBackground(btn, action);

        btn.setId(action);
        btn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                int action = v.getId();
                removeButtonLayout();
                mCoreView.doContextAction(action);
            }
        });
        btn.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                mView.getParent().requestDisallowInterceptTouchEvent(
                        event.getActionMasked() != MotionEvent.ACTION_UP);
                return false;
            }
        });

        final LayoutParams params = new LayoutParams(
                LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        params.leftMargin = xc - (hasImage ? 32 : 64) / 2;
        params.topMargin = yc - (hasImage ? 36 : 40) / 2;
        if (layout != null) {
            layout.addView(btn, params);
        }
        
        return true;
    }
    
    public void removeButtonLayout() {
        if (mButtonLayout != null) {
            final ViewGroup f = (ViewGroup) mView.getParent();
            f.removeView(mButtonLayout);
            mButtonLayout = null;
        }
    }

    private boolean setButtonBackground(Button button, int action) {
        if (mImageIDs != null && action < mImageIDs.length
                && action > 0 && mImageIDs[action] != 0) {
            button.setBackgroundResource(mImageIDs[action]);
            return true;
        }
        if (action > 0 && action < BUTTON_CAPTIONS.length) {
            button.setText(BUTTON_CAPTIONS[action]);
        }
        return false;
    }

    public int[] getContextButtonImages() {
        return mImageIDs;
    }

    public void setContextButtonImages(int[] imageIDs) {
        this.mImageIDs = imageIDs;
    }
}
