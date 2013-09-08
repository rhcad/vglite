using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.Windows.Input;
using System.Globalization;
using touchvg.core;

namespace touchvg.view
{
    public class GraphView : GiView
    {
        public GiCoreView CoreView { get; private set; }
        DrawingGroup _mainDrawing;
        DrawingGroup _tempDrawing;
        MainCanvas _mainCanvas;
        TempCanvas _tempCanvas;

        public GraphView()
        {
            this.CoreView = new GiCoreView();
            this.CoreView.createView(this);
            this._mainDrawing = new DrawingGroup();
            this._tempDrawing = new DrawingGroup();
        }

        public void CreateControl(Panel container)
        {
            _mainCanvas = new MainCanvas(this.CoreView, this) { Width = container.ActualWidth, Height = container.ActualHeight };
            _tempCanvas = new TempCanvas(this.CoreView, this) { Width = container.ActualWidth, Height = container.ActualHeight };
            _tempCanvas.Background = new SolidColorBrush(Colors.Transparent);
            container.Children.Add(_mainCanvas);
            container.Children.Add(_tempCanvas);
            this.CoreView.onSize(this, (int)container.ActualWidth, (int)container.ActualHeight);
        }

        public override void Dispose()
        {
            this._mainCanvas.clean();
            this._mainCanvas = null;
            this._tempCanvas.clean();
            this._tempCanvas = null;

            this.CoreView.destoryView(this);
            this.CoreView.Dispose();
            this.CoreView = null;

            base.Dispose();
        }

        public string Command
        {
            get { return this.CoreView.command(); }
            set { this.CoreView.setCommand(this, value); }
        }

        private class MainCanvas : Canvas
        {
            GiCoreView _coreView;
            GiView _view;
            CanvasAdapter _adapter;

            public MainCanvas(GiCoreView coreView, GiView view)
            {
                this._coreView = coreView;
                this._view = view;
                this._adapter = new CanvasAdapter();
            }

            protected override void OnRender(DrawingContext dc)
            {
                base.OnRender(dc);
                this._adapter.BeginDraw(dc);
                this._coreView.drawAll(this._view, this._adapter);
                this._adapter.EndDraw();
            }

            public void clean()
            {
                _coreView = null;
                _view = null;
                if (_adapter != null)
                {
                    _adapter.Dispose();
                    _adapter = null;
                }
            }
        }

        private class TempCanvas : Canvas
        {
            GiCoreView _coreView;
            GiView _view;
            CanvasAdapter _adapter;
            GiMouseHelper _helper;

            public TempCanvas(GiCoreView coreView, GiView view)
            {
                this._coreView = coreView;
                this._view = view;
                this._adapter = new CanvasAdapter();
                _helper = new GiMouseHelper(this._view, this._coreView);
                this.MouseDown += new MouseButtonEventHandler(TempCanvas_MouseDown);
                this.MouseMove += new MouseEventHandler(TempCanvas_MouseMove);
                this.MouseUp += new MouseButtonEventHandler(TempCanvas_MouseUp);
            }

            public void clean()
            {
                _coreView = null;
                _view = null;
                if (_adapter != null)
                {
                    _adapter.Dispose();
                    _adapter = null;
                }
                if (_helper != null)
                {
                    _helper.Dispose();
                    _helper = null;
                }
            }

            void TempCanvas_MouseUp(object sender, MouseButtonEventArgs e)
            {
                Point pt = e.GetPosition(this);
                _helper.onMouseUp((float)pt.X, (float)pt.Y);
            }

            void TempCanvas_MouseMove(object sender, MouseEventArgs e)
            {
                Point pt = e.GetPosition(this);
                _helper.onMouseMove((float)pt.X, (float)pt.Y,
                    e.LeftButton == MouseButtonState.Pressed,
                    e.RightButton == MouseButtonState.Pressed);
            }

            void TempCanvas_MouseDown(object sender, MouseButtonEventArgs e)
            {
                if (e.LeftButton == MouseButtonState.Pressed)
                {
                    Point pt = e.GetPosition(this);
                    bool ctrl = Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl);
                    bool shift = Keyboard.IsKeyDown(Key.LeftShift) || Keyboard.IsKeyDown(Key.RightShift);
                    _helper.onLButtonDown((float)pt.X, (float)pt.Y, ctrl, shift);
                }
                else if (e.RightButton == MouseButtonState.Pressed)
                {
                    Point pt = e.GetPosition(this);
                    _helper.onRButtonDown((float)pt.X, (float)pt.Y);
                }
            }

            protected override void OnRender(DrawingContext dc)
            {
                base.OnRender(dc);
                this._adapter.BeginDraw(dc);
                this._coreView.dynDraw(this._view, this._adapter);
                this._adapter.EndDraw();
            }
        }

        public override void regenAll()
        {
            this._mainCanvas.InvalidateVisual();
            this._tempCanvas.InvalidateVisual();
        }

        public override void regenAppend()
        {
            regenAll();
        }

        public override void redraw()
        {
            this._tempCanvas.InvalidateVisual();
        }

        public override bool useFinger()
        {
            return false;
        }
    }
}
