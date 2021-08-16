namespace SP12T
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btn_test = new System.Windows.Forms.Button();
            this.serialPort_COM = new System.IO.Ports.SerialPort(this.components);
            this.textBox_input = new System.Windows.Forms.TextBox();
            this.textBox_show = new System.Windows.Forms.TextBox();
            this.comboBox_com = new System.Windows.Forms.ComboBox();
            this.btn_opencom = new System.Windows.Forms.Button();
            this.btn_clear = new System.Windows.Forms.Button();
            this.timer_run = new System.Windows.Forms.Timer(this.components);
            this.btn_close = new System.Windows.Forms.Button();
            this.btn_back = new System.Windows.Forms.Button();
            this.btn_step = new System.Windows.Forms.Button();
            this.btn_run = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.btn_pause = new System.Windows.Forms.Button();
            this.textBox_interval = new System.Windows.Forms.TextBox();
            this.btn_interval_set = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.pictureBox_SP12 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_SP12)).BeginInit();
            this.SuspendLayout();
            // 
            // btn_test
            // 
            this.btn_test.Font = new System.Drawing.Font("宋体", 10F);
            this.btn_test.Location = new System.Drawing.Point(248, 70);
            this.btn_test.Name = "btn_test";
            this.btn_test.Size = new System.Drawing.Size(100, 26);
            this.btn_test.TabIndex = 0;
            this.btn_test.Text = "测试";
            this.btn_test.UseVisualStyleBackColor = true;
            this.btn_test.Click += new System.EventHandler(this.btn_test_Click);
            // 
            // serialPort_COM
            // 
            this.serialPort_COM.BaudRate = 115200;
            // 
            // textBox_input
            // 
            this.textBox_input.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox_input.Location = new System.Drawing.Point(107, 70);
            this.textBox_input.Name = "textBox_input";
            this.textBox_input.Size = new System.Drawing.Size(121, 26);
            this.textBox_input.TabIndex = 1;
            // 
            // textBox_show
            // 
            this.textBox_show.Location = new System.Drawing.Point(514, 19);
            this.textBox_show.Multiline = true;
            this.textBox_show.Name = "textBox_show";
            this.textBox_show.ReadOnly = true;
            this.textBox_show.Size = new System.Drawing.Size(229, 77);
            this.textBox_show.TabIndex = 2;
            this.textBox_show.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // comboBox_com
            // 
            this.comboBox_com.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.comboBox_com.FormattingEnabled = true;
            this.comboBox_com.Location = new System.Drawing.Point(107, 21);
            this.comboBox_com.Name = "comboBox_com";
            this.comboBox_com.Size = new System.Drawing.Size(121, 24);
            this.comboBox_com.TabIndex = 3;
            this.comboBox_com.SelectedIndexChanged += new System.EventHandler(this.comboBox_com_SelectedIndexChanged);
            // 
            // btn_opencom
            // 
            this.btn_opencom.Font = new System.Drawing.Font("宋体", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btn_opencom.Location = new System.Drawing.Point(248, 19);
            this.btn_opencom.Name = "btn_opencom";
            this.btn_opencom.Size = new System.Drawing.Size(100, 26);
            this.btn_opencom.TabIndex = 4;
            this.btn_opencom.Text = "打开串口";
            this.btn_opencom.UseVisualStyleBackColor = true;
            this.btn_opencom.Click += new System.EventHandler(this.btn_opencom_Click);
            // 
            // btn_clear
            // 
            this.btn_clear.Font = new System.Drawing.Font("宋体", 10F);
            this.btn_clear.Location = new System.Drawing.Point(368, 70);
            this.btn_clear.Name = "btn_clear";
            this.btn_clear.Size = new System.Drawing.Size(100, 26);
            this.btn_clear.TabIndex = 6;
            this.btn_clear.Text = "初始化";
            this.btn_clear.UseVisualStyleBackColor = true;
            this.btn_clear.Click += new System.EventHandler(this.btn_clear_Click);
            // 
            // timer_run
            // 
            this.timer_run.Tick += new System.EventHandler(this.timer_run_Tick);
            // 
            // btn_close
            // 
            this.btn_close.Font = new System.Drawing.Font("宋体", 10F);
            this.btn_close.Location = new System.Drawing.Point(368, 19);
            this.btn_close.Name = "btn_close";
            this.btn_close.Size = new System.Drawing.Size(100, 26);
            this.btn_close.TabIndex = 7;
            this.btn_close.Text = "关闭串口";
            this.btn_close.UseVisualStyleBackColor = true;
            this.btn_close.Click += new System.EventHandler(this.btn_close_Click);
            // 
            // btn_back
            // 
            this.btn_back.Image = global::SP12T.Properties.Resources.btn_back;
            this.btn_back.Location = new System.Drawing.Point(275, 865);
            this.btn_back.Name = "btn_back";
            this.btn_back.Size = new System.Drawing.Size(50, 50);
            this.btn_back.TabIndex = 8;
            this.btn_back.UseVisualStyleBackColor = true;
            this.btn_back.Click += new System.EventHandler(this.btn_back_Click);
            // 
            // btn_step
            // 
            this.btn_step.Image = global::SP12T.Properties.Resources.btn_step;
            this.btn_step.Location = new System.Drawing.Point(375, 865);
            this.btn_step.Name = "btn_step";
            this.btn_step.Size = new System.Drawing.Size(50, 50);
            this.btn_step.TabIndex = 9;
            this.btn_step.UseVisualStyleBackColor = true;
            this.btn_step.Click += new System.EventHandler(this.btn_step_Click);
            // 
            // btn_run
            // 
            this.btn_run.Image = global::SP12T.Properties.Resources.btn_run;
            this.btn_run.Location = new System.Drawing.Point(475, 865);
            this.btn_run.Name = "btn_run";
            this.btn_run.Size = new System.Drawing.Size(50, 50);
            this.btn_run.TabIndex = 10;
            this.btn_run.UseVisualStyleBackColor = true;
            this.btn_run.Click += new System.EventHandler(this.btn_run_Click);
            // 
            // btn_stop
            // 
            this.btn_stop.Image = global::SP12T.Properties.Resources.btn_stop;
            this.btn_stop.Location = new System.Drawing.Point(675, 865);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(50, 50);
            this.btn_stop.TabIndex = 11;
            this.btn_stop.UseVisualStyleBackColor = true;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // btn_pause
            // 
            this.btn_pause.Image = global::SP12T.Properties.Resources.btn_pause;
            this.btn_pause.Location = new System.Drawing.Point(575, 865);
            this.btn_pause.Name = "btn_pause";
            this.btn_pause.Size = new System.Drawing.Size(50, 50);
            this.btn_pause.TabIndex = 12;
            this.btn_pause.UseVisualStyleBackColor = true;
            this.btn_pause.Click += new System.EventHandler(this.btn_pause_Click);
            // 
            // textBox_interval
            // 
            this.textBox_interval.Location = new System.Drawing.Point(769, 75);
            this.textBox_interval.Name = "textBox_interval";
            this.textBox_interval.Size = new System.Drawing.Size(100, 21);
            this.textBox_interval.TabIndex = 13;
            this.textBox_interval.TextChanged += new System.EventHandler(this.textBox_interval_TextChanged);
            // 
            // btn_interval_set
            // 
            this.btn_interval_set.Location = new System.Drawing.Point(875, 73);
            this.btn_interval_set.Name = "btn_interval_set";
            this.btn_interval_set.Size = new System.Drawing.Size(75, 23);
            this.btn_interval_set.TabIndex = 14;
            this.btn_interval_set.Text = "Set";
            this.btn_interval_set.UseVisualStyleBackColor = true;
            this.btn_interval_set.Click += new System.EventHandler(this.btn_interval_set_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 12F);
            this.label1.Location = new System.Drawing.Point(37, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 16);
            this.label1.TabIndex = 15;
            this.label1.Text = "COM口：";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 12F);
            this.label2.Location = new System.Drawing.Point(37, 74);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(48, 16);
            this.label2.TabIndex = 16;
            this.label2.Text = "1-12:";
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.MenuBar;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font("宋体", 10F);
            this.textBox1.Location = new System.Drawing.Point(769, 34);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(181, 33);
            this.textBox1.TabIndex = 17;
            this.textBox1.Text = "开关切换间隔（ms）：\r\n范围（100ms-10s），默认为1s";
            // 
            // pictureBox_SP12
            // 
            this.pictureBox_SP12.Image = global::SP12T.Properties.Resources.SP0;
            this.pictureBox_SP12.Location = new System.Drawing.Point(50, 120);
            this.pictureBox_SP12.Name = "pictureBox_SP12";
            this.pictureBox_SP12.Size = new System.Drawing.Size(900, 900);
            this.pictureBox_SP12.TabIndex = 5;
            this.pictureBox_SP12.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(984, 961);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_interval_set);
            this.Controls.Add(this.textBox_interval);
            this.Controls.Add(this.btn_pause);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.btn_run);
            this.Controls.Add(this.btn_step);
            this.Controls.Add(this.btn_back);
            this.Controls.Add(this.btn_close);
            this.Controls.Add(this.btn_clear);
            this.Controls.Add(this.pictureBox_SP12);
            this.Controls.Add(this.btn_opencom);
            this.Controls.Add(this.comboBox_com);
            this.Controls.Add(this.textBox_show);
            this.Controls.Add(this.textBox_input);
            this.Controls.Add(this.btn_test);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_SP12)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_test;
        private System.Windows.Forms.TextBox textBox_input;
        private System.Windows.Forms.TextBox textBox_show;
        public System.IO.Ports.SerialPort serialPort_COM;
        private System.Windows.Forms.ComboBox comboBox_com;
        private System.Windows.Forms.Button btn_opencom;
        private System.Windows.Forms.PictureBox pictureBox_SP12;
        private System.Windows.Forms.Button btn_clear;
        private System.Windows.Forms.Timer timer_run;
        private System.Windows.Forms.Button btn_close;
        private System.Windows.Forms.Button btn_back;
        private System.Windows.Forms.Button btn_step;
        private System.Windows.Forms.Button btn_run;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.Button btn_pause;
        private System.Windows.Forms.TextBox textBox_interval;
        private System.Windows.Forms.Button btn_interval_set;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox1;
    }
}

