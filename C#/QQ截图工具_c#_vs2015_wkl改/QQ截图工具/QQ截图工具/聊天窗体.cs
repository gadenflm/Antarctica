using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace QQ截图工具
{
    /// <summary>
    ///  有些人会问,为什么把Alt定义为1， Ctrl为2了
    ///  因为 http://msdn.microsoft.com/en-us/library/windows/desktop/ms646309(v=vs.85).aspx 列出了值的对应关系
    /// 定义辅助键的名称,也就是定义RegisterHotKey第三个参数的取值
    /// （将数字转化为字符，可以使大家更容易理解代码）
    /// </summary>
    [Flags]
    public enum KeyModifiers
    {
        None = 0,
        Alt = 1,
        Ctrl = 2,
        Shift = 4,
        WindowsKey = 8
    }

    public partial class 聊天窗体 : Form
    {
        public 聊天窗体()
        {
            InitializeComponent();
        }

        Cutter cutter = null;

        private void btnCutter_Click(object sender, EventArgs e)
        {
          //  Console.WriteLine(" btnCutter_Click");
            // 新建一个和屏幕大小相同的图片
            Bitmap CatchBmp = new Bitmap(Screen.AllScreens[0].Bounds.Width, Screen.AllScreens[0].Bounds.Height);
            
            // 创建一个画板，让我们可以在画板上画图
            // 这个画板也就是和屏幕大小一样大的图片
            // 我们可以通过Graphics这个类在这个空白图片上画图
            Graphics g = Graphics.FromImage(CatchBmp);

            // 把屏幕图片拷贝到我们创建的空白图片 CatchBmp中
            g.CopyFromScreen(new Point(0, 0), new Point(0, 0), new Size(Screen.AllScreens[0].Bounds.Width, Screen.AllScreens[0].Bounds.Height));

            // 创建截图窗体
            cutter = new Cutter();

            // 指示窗体的背景图片为屏幕图片
            cutter.BackgroundImage = CatchBmp;
            // 显示窗体
            //cutter.Show();
            // 如果Cutter窗体结束，则从剪切板获得截取的图片，并显示在聊天窗体的发送框中
            if (cutter.ShowDialog() == DialogResult.OK)
            {
                IDataObject iData = Clipboard.GetDataObject();
               DataFormats.Format format = DataFormats.GetFormat(DataFormats.Bitmap);
                if (iData.GetDataPresent(DataFormats.Bitmap))
                {
                    //richTextBox1.Paste(format);

                    // 清楚剪贴板的图片
                   //Clipboard.Clear();
                }
            }
        }
        private void prtsc()
        {
        //    Console.WriteLine(" btnCutter_Click");
            // 新建一个和屏幕大小相同的图片
            Bitmap CatchBmp = new Bitmap(Screen.AllScreens[0].Bounds.Width, Screen.AllScreens[0].Bounds.Height);

            // 创建一个画板，让我们可以在画板上画图
            // 这个画板也就是和屏幕大小一样大的图片
            // 我们可以通过Graphics这个类在这个空白图片上画图
            Graphics g = Graphics.FromImage(CatchBmp);

            // 把屏幕图片拷贝到我们创建的空白图片 CatchBmp中
            g.CopyFromScreen(new Point(0, 0), new Point(0, 0), new Size(Screen.AllScreens[0].Bounds.Width, Screen.AllScreens[0].Bounds.Height));

            // 创建截图窗体
            cutter = new Cutter();

            // 指示窗体的背景图片为屏幕图片
            cutter.BackgroundImage = CatchBmp;
            // 显示窗体
            //cutter.Show();
            // 如果Cutter窗体结束，则从剪切板获得截取的图片，并显示在聊天窗体的发送框中
            if (cutter.ShowDialog() == DialogResult.OK)
            {
                IDataObject iData = Clipboard.GetDataObject();
                DataFormats.Format format = DataFormats.GetFormat(DataFormats.Bitmap);
                if (iData.GetDataPresent(DataFormats.Bitmap))
                {
                    //richTextBox1.Paste(format);

                    // 清楚剪贴板的图片
                    //Clipboard.Clear();
                }
            }
        }

        private void 聊天窗体_Load(object sender, EventArgs e)
        {
            notifyIcon1.Icon = System.Drawing.SystemIcons.Application;
         //   uint ctrlHotKey = (uint)(KeyModifiers.Alt|KeyModifiers.Ctrl);
            uint AltHotKey = (uint)(KeyModifiers.Alt );
 
            HotKey.RegisterHotKey(Handle, 100, AltHotKey, Keys.X);
        }


        private void 聊天窗体_FormClosing(object sender, FormClosingEventArgs e)
        {

            HotKey.UnregisterHotKey(Handle, 100);
        }


        private void GlobalKeyProcess()
        {
            this.WindowState = FormWindowState.Minimized;

             Thread.Sleep(100);
           // Console.WriteLine("btnCutter.PerformClick");
            prtsc();
        }


        protected override void WndProc(ref Message m)
        {
  
            const int WM_HOTKEY = 0x0312;
            switch (m.Msg)
            {
                case WM_HOTKEY:
                    if (m.WParam.ToString() == "100")
                    {
                        GlobalKeyProcess();
                    }

                    break;
            }

   
            base.WndProc(ref m); 
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void 聊天窗体_SizeChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Minimized)
            {
                this.Hide();
                this.notifyIcon1.Visible = true;
            }
        }

        private void notifyIcon1_DoubleClick(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Normal)
            {
                this.WindowState = FormWindowState.Minimized;
                this.Hide();
            }
            else if (this.WindowState == FormWindowState.Minimized)
            {
                this.Show();
                this.WindowState = FormWindowState.Normal;
                this.Activate();
            }
        }

        private void exitMenuItem_Click(object sender, EventArgs e)
        {
            HotKey.UnregisterHotKey(Handle, 100);
            notifyIcon1.Visible = false;
            this.Close();
            this.Dispose();
            Application.Exit();
        }

        private void showMenuItem_Click(object sender, EventArgs e)
        {
            this.Show();
            this.WindowState = FormWindowState.Normal;
            this.Activate();
        }
    }
}
