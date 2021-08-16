using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SP12T
{
    public partial class Form1 : Form
    {
        private static int interval_now; // 间隔时间统计
        private static int interval_sum; // 所需间隔时间 // 默认为10
        private static int timer_sta; // 0：初始化   1：step  2：run  3：pause  4：stop  5:back
        private static int p_i;        // 当前扫描下标，默认为0

        private static bool IsRunning;

      //  private static string PicFileRoot = @"D:\p_12\";   //图片文件根目录
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            allinit();
            string str;
            for (int i = 0; i < 32; i++)
            {
                str = i.ToString();
                comboBox_com.Items.Add("COM" + str);

            }
        }
        private void allinit()
        {
            interval_now = 0;
            timer_sta = 0;
            interval_sum = 10;
            p_i = 0;
            btn_set();
            pictureBox_SP12.Image = Properties.Resources.SP0;
        }
        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void btn_test_Click(object sender, EventArgs e)
        {
            int t12;
            string outputstr;
            try
            {
                t12 = Convert.ToInt32(textBox_input.Text, 10);
                if ((t12 > 0) && (t12 < 13))
                {
                    p_i = t12;
                    ComScan(t12);                                     ///////test btn
                    PicScan(t12);
                  //  outputstr = "open:" + textBox_input.Text;
                 //   textBox_show.AppendText(outputstr + "\r\n");
                }
                else
                {
                    MessageBox.Show("请输入1-12内的数字", "错误");
                }
            }
            catch
            {
                MessageBox.Show("请输入正确的数字", "错误");
            }
        }
        private void PicScan (int t12)
        {
            try
            {
                pictureBox_SP12.Image =(Image)Properties.Resources.ResourceManager.GetObject("SP"+t12.ToString());
                return;
            }
            catch
            {
                MessageBox.Show("错误，请检查!", "错误");
            }
        }
        private void ComScan(int t12)
        {
            byte[] Date = new byte[5];
            Date[0] = 0xAA;
            Date[3] = 0x0D;
            Date[4] = 0x0A;
            string up_31, down_31;
            up_31 = (t12 - 1).ToString();
            down_31 = (1 - 1).ToString();
            int a, b;
            if (serialPort_COM.IsOpen)
            {
                try
                {
                    a = Convert.ToInt32(up_31, 10) + 20;//加20是为了跳过0D的关键检测
                    Date[1] = Convert.ToByte(a);
                    b = Convert.ToInt32(down_31, 10) + 20;//10表示10进制
                    Date[2] = Convert.ToByte(b);
                    serialPort_COM.Write(Date, 0, 5);

                    string strdata = "当前打开:" + t12.ToString();
                    textBox_show.AppendText(strdata + "\r\n");
                }
                catch
                {
                    MessageBox.Show("错误，请检查!", "错误"); return;
                }
            }
            else
            {
                MessageBox.Show("error,open COM first", "error", MessageBoxButtons.OK);return;
            }
        }

        private void btn_opencom_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort_COM.PortName = comboBox_com.Text;
                serialPort_COM.Open();
                string str = "串口" + comboBox_com.Text + "已连接";
                textBox_show.AppendText(str + "\r\n");
                btn_set();
            }
            catch
            {
                MessageBox.Show("无法打开串口", "错误"); return;
            }
        }


        private void btn_clear_Click(object sender, EventArgs e)
        {
            allinit();
            SP12T_clear();
            btn_set();
         }

        private void btn_close_Click(object sender, EventArgs e)
        {
            try
            {
                allinit();
                SP12T_clear();
                serialPort_COM.Close();
                btn_set();
                string str = "串口" + serialPort_COM.PortName+ "已连接";
                textBox_show.AppendText(str + "\r\n");


            }
            catch
            {
                MessageBox.Show("can't close,please check", "error", MessageBoxButtons.OK); return;
            }
        }

        private void SP12T_clear()
        {

            byte[] Date = new byte[3] { 0x43, 0x0D, 0x0A };
            if (serialPort_COM.IsOpen)
            {
                try
                {
                    serialPort_COM.Write(Date, 0, 3);
                    PicScan(0);
                    p_i = 0;
                }
                catch
                {
                    MessageBox.Show("串口写入错误", "错误"); return;
                }

            }
            else
            {
                MessageBox.Show("error,open com first", "error", MessageBoxButtons.OK); return;
            }

        }

        private void btn_set()
        {
            if (serialPort_COM.IsOpen)
            {

                foreach (Control cBox in this.Controls)
                {
                    if (cBox is Button)
                    {
                        string lname = (cBox).Name;
                        (cBox as Button).Enabled = true;
                     }
                }
                btn_opencom.Enabled = false;
                textBox_input.Enabled = true;
                textBox_interval.Enabled = true;
            }
            else
            {
                foreach (Control cBox in this.Controls)
                {
                    if (cBox is Button)
                    {
                        string lname = (cBox).Name;
                        (cBox as Button).Enabled = false;
                      }
                }
                btn_opencom.Enabled = true;
                textBox_input.Enabled = false;
                textBox_interval.Enabled = false;
            }
            runbtn_set();
        }                 //根据串口接通情况，调控所有按钮的enable
         
        private void timer_run_Tick(object sender, EventArgs e)
        {
            if (timer_sta == 2)
            {
                if (interval_now > interval_sum - 1)
                {
                    if (p_i == 12)
                    {
                        p_i = 0;
                    }
                    p_i++;
                    ComScan(p_i);
                    PicScan(p_i);

                    interval_now = 0;
                }
                else
                {
                    interval_now++;
                }
            }
            runbtn_set();
        }

        private void btn_back_Click(object sender, EventArgs e)// timer_sta=5  back mode
        {

            p_i--;
            if (p_i <1)
            {
                p_i = 12;
            }
            ComScan(p_i);
            PicScan(p_i);

            interval_now = 0;
            runbtn_set();
        }

        private void btn_step_Click(object sender, EventArgs e)   //timer_sta=1 step mode
        {
            timer_sta = 1;
            if (p_i == 12)
            {
                p_i = 0;
            }
            p_i++;
            ComScan(p_i);
            PicScan(p_i);

            interval_now = 0;
            runbtn_set();
        }

        private void btn_run_Click(object sender, EventArgs e)  //timer_sta=2 run mode
        {
            timer_sta = 2;
            interval_now = 0;
            IsRunning = true;
            timer_run.Enabled = true;
           
        }

        private void btn_stop_Click(object sender, EventArgs e) //timer_sta=4 stop mode
        {
            timer_sta = 0;
            p_i = 0;
            interval_now = 0;
            SP12T_clear();
            IsRunning = false;
            timer_run.Enabled = false;
            runbtn_set();

        }

        private void btn_pause_Click(object sender, EventArgs e)//timer_sta=3 pause mode
        {
            timer_sta = 3;
            interval_now = 0;
            runbtn_set();
        } 

        private void comboBox_com_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox_interval_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void runbtn_set()      //调控5个run btn的enable
        {
            if (serialPort_COM.IsOpen)
            {
                if (timer_sta == 0)
                {
                    btn_step.Enabled = true;
                    btn_run.Enabled = true;
                    btn_back.Enabled = true;
                    btn_pause.Enabled = false;
                    btn_stop.Enabled = false;
                }
                if (timer_sta == 1)
                {
                    btn_step.Enabled = true;
                    btn_run.Enabled = true;
                    btn_back.Enabled = true;
                    btn_pause.Enabled = false;
                    btn_stop.Enabled = true;
                }
                if (timer_sta == 2)
                {
                    btn_step.Enabled = false;
                    btn_back.Enabled = false;
                    btn_run.Enabled = false;
                    btn_pause.Enabled = true;
                    btn_stop.Enabled = true;
                }
                if (timer_sta == 3)
                {
                    btn_step.Enabled = true;
                    btn_run.Enabled = true;
                    btn_back.Enabled = true;
                    btn_pause.Enabled = false;
                    btn_stop.Enabled = true;
                }
                if (timer_sta == 4)
                {
                    btn_step.Enabled = true;
                    btn_run.Enabled = true;
                    btn_back.Enabled = true;
                    btn_pause.Enabled = false;
                    btn_stop.Enabled = false;
                }
                if (timer_sta == 5)
                {
                    btn_step.Enabled = true;
                    btn_run.Enabled = true;
                    btn_back.Enabled = true;
                    btn_pause.Enabled = false;
                    btn_stop.Enabled = true;
                }
            }
            else
            {
                btn_step.Enabled = false;
                btn_run.Enabled = false;
                btn_pause.Enabled = false;
                btn_stop.Enabled = false;
                btn_back.Enabled = false;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void btn_interval_set_Click(object sender, EventArgs e)
        {
            int t_sum;
            try
            {
                t_sum = Convert.ToInt32(textBox_interval.Text, 10);
                if ((t_sum > 99) && (t_sum < 10001))
                {
                    if (t_sum % 100 != 0) { t_sum = t_sum - t_sum % 100; }
                    t_sum = t_sum / 100;
                    interval_sum = t_sum;
                }
                else
                {
                    MessageBox.Show("时间间隔范围：100ms-10s", "错误"); return;
                }
            }
            catch
            {
                MessageBox.Show("请输入正确的数字", "错误"); return;
            }

        }
    }
}
