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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.label1 = new System.Windows.Forms.Label();
            this.TextBoxSend = new System.Windows.Forms.TextBox();
            this.buttonSend = new System.Windows.Forms.Button();
            this.buttonOpen = new System.Windows.Forms.Button();
            this.buttonClose = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.listViewRecieve = new System.Windows.Forms.ListView();
            this.Index = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Time = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.CanChannal = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Orientation = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Id = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Frame = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Type = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Length = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Data = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxID = new System.Windows.Forms.TextBox();
            this.timerRecieve = new System.Windows.Forms.Timer(this.components);
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.button6 = new System.Windows.Forms.Button();
            this.up_down_value = new System.Windows.Forms.TextBox();
            this.x_org_label = new System.Windows.Forms.Label();
            this.y_org_label = new System.Windows.Forms.Label();
            this.button7 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.MAP_X_TEXT = new System.Windows.Forms.TextBox();
            this.MAP_Y_TEXT = new System.Windows.Forms.TextBox();
            this.button10 = new System.Windows.Forms.Button();
            this.POINT_NUM_TEXT = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
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
            this.TextBoxSend.TextChanged += new System.EventHandler(this.TextBoxSend_TextChanged);
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
            this.groupBox1.Location = new System.Drawing.Point(3, 107);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(630, 231);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "接收";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
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
            this.listViewRecieve.Location = new System.Drawing.Point(6, 22);
            this.listViewRecieve.Name = "listViewRecieve";
            this.listViewRecieve.Size = new System.Drawing.Size(599, 198);
            this.listViewRecieve.TabIndex = 0;
            this.listViewRecieve.UseCompatibleStateImageBehavior = false;
            this.listViewRecieve.View = System.Windows.Forms.View.Details;
            this.listViewRecieve.SelectedIndexChanged += new System.EventHandler(this.listViewRecieve_SelectedIndexChanged);
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
            this.timerRecieve.Interval = 50;
            this.timerRecieve.Tick += new System.EventHandler(this.timerRecieve_Tick);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(701, 303);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 14);
            this.label4.TabIndex = 9;
            this.label4.Text = "转速：";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(759, 303);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(47, 33);
            this.label5.TabIndex = 10;
            this.label5.Text = "00";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // button2
            // 
            this.button2.Image = global::USBCAN.Resource1.down_arrow;
            this.button2.Location = new System.Drawing.Point(724, 212);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(56, 71);
            this.button2.TabIndex = 12;
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Image = ((System.Drawing.Image)(resources.GetObject("button1.Image")));
            this.button1.Location = new System.Drawing.Point(724, 129);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(56, 77);
            this.button1.TabIndex = 11;
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "250kps",
            "500kps",
            "800kps",
            "1000kps"});
            this.comboBox1.Location = new System.Drawing.Point(743, 35);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 22);
            this.comboBox1.TabIndex = 16;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(660, 38);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 14);
            this.label6.TabIndex = 17;
            this.label6.Text = "波特率设置";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(632, 66);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(105, 14);
            this.label7.TabIndex = 19;
            this.label7.Text = "适配器工作方式";
            // 
            // comboBox2
            // 
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            "自发自受模式",
            "正常工作模式",
            "测试模式"});
            this.comboBox2.Location = new System.Drawing.Point(743, 63);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(121, 22);
            this.comboBox2.TabIndex = 18;
            this.comboBox2.SelectedIndexChanged += new System.EventHandler(this.comboBox2_SelectedIndexChanged);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(883, 35);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(116, 23);
            this.button6.TabIndex = 27;
            this.button6.Text = "illume laser";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // up_down_value
            // 
            this.up_down_value.Location = new System.Drawing.Point(663, 100);
            this.up_down_value.Name = "up_down_value";
            this.up_down_value.Size = new System.Drawing.Size(56, 23);
            this.up_down_value.TabIndex = 28;
            this.up_down_value.Text = "88";
            this.up_down_value.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // x_org_label
            // 
            this.x_org_label.AutoSize = true;
            this.x_org_label.Location = new System.Drawing.Point(743, 100);
            this.x_org_label.Name = "x_org_label";
            this.x_org_label.Size = new System.Drawing.Size(56, 14);
            this.x_org_label.TabIndex = 29;
            this.x_org_label.Text = "label13";
            // 
            // y_org_label
            // 
            this.y_org_label.AutoSize = true;
            this.y_org_label.Location = new System.Drawing.Point(808, 100);
            this.y_org_label.Name = "y_org_label";
            this.y_org_label.Size = new System.Drawing.Size(56, 14);
            this.y_org_label.TabIndex = 30;
            this.y_org_label.Text = "label14";
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(533, 66);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(75, 23);
            this.button7.TabIndex = 31;
            this.button7.Text = "Save";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(662, 174);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(56, 71);
            this.button8.TabIndex = 32;
            this.button8.Text = "left";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(786, 174);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(56, 71);
            this.button9.TabIndex = 33;
            this.button9.Text = "right";
            this.button9.UseVisualStyleBackColor = true;
            this.button9.Click += new System.EventHandler(this.button9_Click);
            // 
            // MAP_X_TEXT
            // 
            this.MAP_X_TEXT.Location = new System.Drawing.Point(232, 78);
            this.MAP_X_TEXT.Name = "MAP_X_TEXT";
            this.MAP_X_TEXT.Size = new System.Drawing.Size(56, 23);
            this.MAP_X_TEXT.TabIndex = 34;
            this.MAP_X_TEXT.Text = "88";
            // 
            // MAP_Y_TEXT
            // 
            this.MAP_Y_TEXT.Location = new System.Drawing.Point(326, 79);
            this.MAP_Y_TEXT.Name = "MAP_Y_TEXT";
            this.MAP_Y_TEXT.Size = new System.Drawing.Size(56, 23);
            this.MAP_Y_TEXT.TabIndex = 35;
            this.MAP_Y_TEXT.Text = "88";
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(394, 79);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(110, 23);
            this.button10.TabIndex = 36;
            this.button10.Text = "SET map point";
            this.button10.UseVisualStyleBackColor = true;
            this.button10.Click += new System.EventHandler(this.button10_Click);
            // 
            // POINT_NUM_TEXT
            // 
            this.POINT_NUM_TEXT.Location = new System.Drawing.Point(142, 78);
            this.POINT_NUM_TEXT.Name = "POINT_NUM_TEXT";
            this.POINT_NUM_TEXT.Size = new System.Drawing.Size(56, 23);
            this.POINT_NUM_TEXT.TabIndex = 37;
            this.POINT_NUM_TEXT.Text = "01";
            this.POINT_NUM_TEXT.TextChanged += new System.EventHandler(this.POINT_NUM_TEXT_TextChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(294, 83);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(14, 14);
            this.label13.TabIndex = 38;
            this.label13.Text = "Y";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(203, 82);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(14, 14);
            this.label14.TabIndex = 39;
            this.label14.Text = "X";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(38, 82);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(98, 14);
            this.label15.TabIndex = 40;
            this.label15.Text = "MAP POINT NUM";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(883, 62);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(116, 23);
            this.button3.TabIndex = 41;
            this.button3.Text = "disable laser";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click_1);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1204, 534);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.POINT_NUM_TEXT);
            this.Controls.Add(this.button10);
            this.Controls.Add(this.MAP_Y_TEXT);
            this.Controls.Add(this.MAP_X_TEXT);
            this.Controls.Add(this.button9);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.y_org_label);
            this.Controls.Add(this.x_org_label);
            this.Controls.Add(this.up_down_value);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
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
            this.Text = "主窗体";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Click += new System.EventHandler(this.Form1_Click);
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
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.TextBox up_down_value;
        private System.Windows.Forms.Label x_org_label;
        private System.Windows.Forms.Label y_org_label;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.TextBox MAP_X_TEXT;
        private System.Windows.Forms.TextBox MAP_Y_TEXT;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.TextBox POINT_NUM_TEXT;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Button button3;


    }
}

