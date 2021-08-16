namespace paper_copytool
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
            this.textBox_show = new System.Windows.Forms.TextBox();
            this.Icon_pdf = new System.Windows.Forms.NotifyIcon(this.components);
            this.context_Icon = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ShowItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ExitItem = new System.Windows.Forms.ToolStripMenuItem();
            this.context_Icon.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox_show
            // 
            this.textBox_show.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox_show.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox_show.Location = new System.Drawing.Point(0, 0);
            this.textBox_show.MaxLength = 120000;
            this.textBox_show.Multiline = true;
            this.textBox_show.Name = "textBox_show";
            this.textBox_show.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_show.Size = new System.Drawing.Size(1187, 573);
            this.textBox_show.TabIndex = 0;
            this.textBox_show.Text = "null";
            // 
            // Icon_pdf
            // 
            this.Icon_pdf.ContextMenuStrip = this.context_Icon;
            this.Icon_pdf.Text = "Paper_copytool";
            this.Icon_pdf.Visible = true;
            this.Icon_pdf.DoubleClick += new System.EventHandler(this.Icon_pdf_DoubleClick);
            // 
            // context_Icon
            // 
            this.context_Icon.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ShowItem,
            this.ExitItem});
            this.context_Icon.Name = "context_Icon";
            this.context_Icon.Size = new System.Drawing.Size(108, 48);
            // 
            // ShowItem
            // 
            this.ShowItem.Name = "ShowItem";
            this.ShowItem.Size = new System.Drawing.Size(152, 22);
            this.ShowItem.Text = "Show";
            this.ShowItem.Click += new System.EventHandler(this.ShowItem_Click);
            // 
            // ExitItem
            // 
            this.ExitItem.Name = "ExitItem";
            this.ExitItem.Size = new System.Drawing.Size(152, 22);
            this.ExitItem.Text = "Exit";
            this.ExitItem.Click += new System.EventHandler(this.ExitItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(1184, 561);
            this.Controls.Add(this.textBox_show);
            this.Name = "Form1";
            this.Text = "Paper_copytool";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.SizeChanged += new System.EventHandler(this.Form1_SizeChanged);
            this.context_Icon.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_show;
        private System.Windows.Forms.NotifyIcon Icon_pdf;
        private System.Windows.Forms.ContextMenuStrip context_Icon;
        private System.Windows.Forms.ToolStripMenuItem ShowItem;
        private System.Windows.Forms.ToolStripMenuItem ExitItem;
    }
}

