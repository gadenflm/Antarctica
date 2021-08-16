namespace QQ截图工具
{
    partial class 聊天窗体
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
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(聊天窗体));
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.btnCutter = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label_read = new System.Windows.Forms.Label();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.exitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCutter
            // 
            this.btnCutter.Image = ((System.Drawing.Image)(resources.GetObject("btnCutter.Image")));
            this.btnCutter.Location = new System.Drawing.Point(3, 0);
            this.btnCutter.Name = "btnCutter";
            this.btnCutter.Size = new System.Drawing.Size(34, 25);
            this.btnCutter.TabIndex = 0;
            this.btnCutter.Tag = "";
            this.toolTip1.SetToolTip(this.btnCutter, "屏幕截图 Alt + X");
            this.btnCutter.UseVisualStyleBackColor = true;
            this.btnCutter.Click += new System.EventHandler(this.btnCutter_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(234)))), ((int)(((byte)(234)))), ((int)(((byte)(234)))));
            this.panel1.Controls.Add(this.btnCutter);
            this.panel1.Location = new System.Drawing.Point(1, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(431, 25);
            this.panel1.TabIndex = 3;
            // 
            // label_read
            // 
            this.label_read.AutoSize = true;
            this.label_read.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_read.Location = new System.Drawing.Point(12, 58);
            this.label_read.Name = "label_read";
            this.label_read.Size = new System.Drawing.Size(250, 168);
            this.label_read.TabIndex = 4;
            this.label_read.Text = "截图保存位置：桌面文件夹\r\n保存格式：JPEG\r\n命名方式：日期_时间\r\n热键：Alt+X\r\n\r\n框内双击左键截图，右键取消！！\r\n截图自动保存在剪贴板内。\r\n\r" +
    "\n";
            this.label_read.Click += new System.EventHandler(this.label1_Click);
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip1;
            this.notifyIcon1.Text = "wkl截图工具";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.DoubleClick += new System.EventHandler(this.notifyIcon1_DoubleClick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showMenuItem,
            this.exitMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(101, 48);
            // 
            // exitMenuItem
            // 
            this.exitMenuItem.Name = "exitMenuItem";
            this.exitMenuItem.Size = new System.Drawing.Size(100, 22);
            this.exitMenuItem.Text = "退出";
            this.exitMenuItem.Click += new System.EventHandler(this.exitMenuItem_Click);
            // 
            // showMenuItem
            // 
            this.showMenuItem.Name = "showMenuItem";
            this.showMenuItem.Size = new System.Drawing.Size(100, 22);
            this.showMenuItem.Text = "显示";
            this.showMenuItem.Click += new System.EventHandler(this.showMenuItem_Click);
            // 
            // 聊天窗体
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(272, 220);
            this.Controls.Add(this.label_read);
            this.Controls.Add(this.panel1);
            this.Name = "聊天窗体";
            this.Text = "wkl截图工具";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.聊天窗体_FormClosing);
            this.Load += new System.EventHandler(this.聊天窗体_Load);
            this.SizeChanged += new System.EventHandler(this.聊天窗体_SizeChanged);
            this.panel1.ResumeLayout(false);
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Button btnCutter;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label_read;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem exitMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showMenuItem;
    }
}

