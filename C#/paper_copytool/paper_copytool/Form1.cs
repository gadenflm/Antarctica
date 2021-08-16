using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;

namespace paper_copytool
{


    public partial class Form1 : Form
    {
        #region WindowsAPI

        [DllImport("User32.dll")]
        protected static extern int SetClipboardViewer(int hWndNewViewer);


        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern bool ChangeClipboardChain(IntPtr hWndRemove, IntPtr hWndNewNext);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern int SendMessage(IntPtr hwnd, int wMsg, IntPtr wParam, IntPtr lParam);
        #endregion
        IntPtr nextClipboardViewer;

        bool IsStart = true;
        string new_text = null;

        public Form1()
        {
            InitializeComponent();
            nextClipboardViewer = (IntPtr)SetClipboardViewer((int)Handle);
        }

        protected override void WndProc(ref Message m)
        {
            // defined in winuser.h
            const int WM_DRAWCLIPBOARD = 0x308;
            const int WM_CHANGECBCHAIN = 0x030D;

            switch (m.Msg)
            {
                case WM_DRAWCLIPBOARD:
                    pdf_copy();
                    SendMessage(nextClipboardViewer, m.Msg, m.WParam, m.LParam);
                    break;
                case WM_CHANGECBCHAIN:
                    if (m.WParam == nextClipboardViewer)
                        nextClipboardViewer = m.LParam;
                    else
                        SendMessage(nextClipboardViewer, m.Msg, m.WParam, m.LParam);
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }
        private void pdf_copy()
        {

            if (Clipboard.ContainsText())    //判断文本信息并处理
            {
                if (IsStart)
                {
                    string ex_text = Clipboard.GetText(TextDataFormat.Text);
                    new_text = null;
                    if (ex_text != new_text)
                    {
                        foreach (char c in ex_text)
                        {
                            if ((int)c != 13)
                            {
                                if ((int)c == 10)
                                {
                                    new_text = new_text + " ";
                                }
                                else
                                {
                                    new_text = new_text + c;
                                }
                            }
                        }
                        textBox_show.Text = new_text;
                        Clipboard.SetText(new_text);
                    }
                }
                else
                {
                    textBox_show.Text = Clipboard.GetText(); ;
                }

            }
            else
            {
                textBox_show.Text = "null";
            }
            }
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            ChangeClipboardChain(Handle, nextClipboardViewer);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Icon_pdf.Icon = SystemIcons.Application;
            Icon_pdf.Visible = false;
        }

        private void Icon_pdf_DoubleClick(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Normal)
            {
                this.WindowState = FormWindowState.Minimized;
                this.Hide();
                this.Icon_pdf.Visible = true;
            }
            else if (this.WindowState == FormWindowState.Minimized)
            {
                this.Show();
                this.Icon_pdf.Visible = false;
                this.WindowState = FormWindowState.Normal;
                this.Activate();
            }
        }

        private void ShowItem_Click(object sender, EventArgs e)
        {
            this.Show();
            this.WindowState = FormWindowState.Normal;
            this.Activate();
            this.Icon_pdf.Visible = false;
        }

        private void ExitItem_Click(object sender, EventArgs e)
        {
            ChangeClipboardChain(Handle, nextClipboardViewer);
            Icon_pdf.Visible = false;
            this.Close();
            this.Dispose();
            Application.Exit();
        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Minimized)
            {
                this.Hide();
                this.Icon_pdf.Visible = true;
            }
        }
    }

}
