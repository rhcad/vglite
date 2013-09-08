using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using touchvg.view;
namespace WpfDemo
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public Window1()
        {
            InitializeComponent();
            this.Loaded += new RoutedEventHandler(Window1_Loaded);
            this.Unloaded += new RoutedEventHandler(Window1_Unloaded);
        }
        string[] _commands = new string[] {
            "选择",     "select",
            "删除",      "erase",
            "随手画",    "splines",
            "直线段",    "line",
            "矩形",      "rect",
            "正方形",    "square",
            "椭圆",      "ellipse",
            "圆",        "circle",
            "三角形",    "triangle",
            "菱形",      "diamond",
            "多边形",    "polygon",
            "四边形",    "quadrangle",
            "折线",      "lines",
            "平行四边形", "parallelogram",
            "网格",      "grid",
            "三点圆弧",  "arc3p",
            "圆心圆弧",  "arc_cse",
            "切线圆弧",  "arc_tan",
            "坐标系测试",  "xfdemo",
            "点击测试",  "hittest"};
        private GraphView _view;

        void Window1_Loaded(object sender, RoutedEventArgs e)
        {
            _view = new GraphView();
            _view.CreateControl(canvas1);

            List<KeyValuePair<string, string>> commandSource = new List<KeyValuePair<string, string>>();
            for (int i = 0; i < _commands.Length; i += 2)
            {
                commandSource.Add(new KeyValuePair<string, string>(_commands[i], _commands[i + 1]));
            }
            this.comboBox1.DisplayMemberPath = "Key";
            this.comboBox1.SelectedValuePath = "Value";
            this.comboBox1.ItemsSource = commandSource;
            this.comboBox1.SelectedIndex = 0;
        }

        void Window1_Unloaded(object sender, RoutedEventArgs e)
        {
        }

        private void comboBox1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (_view != null)
                _view.Command = comboBox1.SelectedValue.ToString();
        }

        private void redBtn_Click(object sender, RoutedEventArgs e)
        {
        }

        private void blueBtn_Click(object sender, RoutedEventArgs e)
        {
        }
    }
}
