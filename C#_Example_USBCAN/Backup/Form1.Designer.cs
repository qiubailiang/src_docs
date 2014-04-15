namespace USBCAN
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
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.TextBoxSend = new System.Windows.Forms.TextBox();
            this.buttonSend = new System.Windows.Forms.Button();
            this.buttonOpen = new System.Windows.Forms.Button();
            this.buttonClose = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.listViewRecieve = new System.Windows.Forms.ListView();
            this.Index = new System.Windows.Forms.ColumnHeader();
            this.Time = new System.Windows.Forms.ColumnHeader();
            this.CanChannal = new System.Windows.Forms.ColumnHeader();
            this.Orientation = new System.Windows.Forms.ColumnHeader();
            this.Id = new System.Windows.Forms.ColumnHeader();
            this.Frame = new System.Windows.Forms.ColumnHeader();
            this.Type = new System.Windows.Forms.ColumnHeader();
            this.Length = new System.Windows.Forms.ColumnHeader();
            this.Data = new System.Windows.Forms.ColumnHeader();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxID = new System.Windows.Forms.TextBox();
            this.timerRecieve = new System.Windows.Forms.Timer(this.components);
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(231, 43);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 14);
            this.label1.TabIndex = 0;
            this.label1.Text = "发送数据：";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // TextBoxSend
            // 
            this.TextBoxSend.Location = new System.Drawing.Point(326, 37);
            this.TextBoxSend.Name = "TextBoxSend";
            this.TextBoxSend.Size = new System.Drawing.Size(178, 23);
            this.TextBoxSend.TabIndex = 1;
            this.TextBoxSend.Text = "11 22 33 44 55 66 77 88\r\n";
            // 
            // buttonSend
            // 
            this.buttonSend.Enabled = false;
            this.buttonSend.Location = new System.Drawing.Point(533, 37);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "发送";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // buttonOpen
            // 
            this.buttonOpen.Location = new System.Drawing.Point(23, 11);
            this.buttonOpen.Name = "buttonOpen";
            this.buttonOpen.Size = new System.Drawing.Size(75, 23);
            this.buttonOpen.TabIndex = 3;
            this.buttonOpen.Text = "打开";
            this.buttonOpen.UseVisualStyleBackColor = true;
            this.buttonOpen.Click += new System.EventHandler(this.buttonOpen_Click);
            // 
            // buttonClose
            // 
            this.buttonClose.Enabled = false;
            this.buttonClose.Location = new System.Drawing.Point(142, 11);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 3;
            this.buttonClose.Text = "关闭";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.listViewRecieve);
            this.groupBox1.Location = new System.Drawing.Point(19, 60);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(627, 425);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "接收";
            // 
            // listViewRecieve
            // 
            this.listViewRecieve.BackColor = System.Drawing.SystemColors.Window;
            this.listViewRecieve.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Index,
            this.Time,
            this.CanChannal,
            this.Orientation,
            this.Id,
            this.Frame,
            this.Type,
            this.Length,
            this.Data});
            this.listViewRecieve.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.listViewRecieve.Location = new System.Drawing.Point(10, 15);
            this.listViewRecieve.Name = "listViewRecieve";
            this.listViewRecieve.Size = new System.Drawing.Size(604, 395);
            this.listViewRecieve.TabIndex = 0;
            this.listViewRecieve.UseCompatibleStateImageBehavior = false;
            this.listViewRecieve.View = System.Windows.Forms.View.Details;
            // 
            // Index
            // 
            this.Index.Text = "序号";
            this.Index.Width = 40;
            // 
            // Time
            // 
            this.Time.Text = "时间";
            this.Time.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Time.Width = 80;
            // 
            // CanChannal
            // 
            this.CanChannal.Text = "CAN通道";
            this.CanChannal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Orientation
            // 
            this.Orientation.Text = "传输方向";
            this.Orientation.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Orientation.Width = 70;
            // 
            // Id
            // 
            this.Id.Text = "ID号";
            this.Id.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Frame
            // 
            this.Frame.Text = "帧类型";
            this.Frame.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Type
            // 
            this.Type.Text = "帧格式";
            this.Type.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Length
            // 
            this.Length.Text = "数据长度";
            this.Length.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Length.Width = 70;
            // 
            // Data
            // 
            this.Data.Text = "数据";
            this.Data.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Data.Width = 200;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(49, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(49, 14);
            this.label2.TabIndex = 5;
            this.label2.Text = "ID号：";
            // 
            // textBoxID
            // 
            this.textBoxID.Location = new System.Drawing.Point(111, 37);
            this.textBoxID.Name = "textBoxID";
            this.textBoxID.Size = new System.Drawing.Size(62, 23);
            this.textBoxID.TabIndex = 6;
            this.textBoxID.Text = "01 88 88 88";
            // 
            // timerRecieve
            // 
            this.timerRecieve.Enabled = true;
            this.timerRecieve.Interval = 5;
            this.timerRecieve.Tick += new System.EventHandler(this.timerRecieve_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(660, 507);
            this.Controls.Add(this.textBoxID);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.buttonOpen);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.TextBoxSend);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox TextBoxSend;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.Button buttonOpen;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListView listViewRecieve;
        private System.Windows.Forms.ColumnHeader Index;
        private System.Windows.Forms.ColumnHeader Time;
        private System.Windows.Forms.ColumnHeader CanChannal;
        private System.Windows.Forms.ColumnHeader Orientation;
        private System.Windows.Forms.ColumnHeader Id;
        private System.Windows.Forms.ColumnHeader Frame;
        private System.Windows.Forms.ColumnHeader Type;
        private System.Windows.Forms.ColumnHeader Length;
        private System.Windows.Forms.ColumnHeader Data;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxID;
        private System.Windows.Forms.Timer timerRecieve;


    }
}

