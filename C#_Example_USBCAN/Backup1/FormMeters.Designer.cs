namespace USBCAN
{
    partial class FormMeters
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.OilHeightpicturebox = new System.Windows.Forms.PictureBox();
            this.SpeedpictureBox = new System.Windows.Forms.PictureBox();
            this.PressurepictureBox = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.OilHeightpicturebox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpeedpictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PressurepictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(127, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "压力表";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(322, 49);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "转速表";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(446, 365);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "液位";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(113, 408);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "燃油液位表";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(481, 365);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 12);
            this.label5.TabIndex = 9;
            this.label5.Text = "温度";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // OilHeightpicturebox
            // 
            this.OilHeightpicturebox.BackgroundImage = global::USBCAN.Resource1.jpeg11;
            this.OilHeightpicturebox.Location = new System.Drawing.Point(64, 235);
            this.OilHeightpicturebox.Name = "OilHeightpicturebox";
            this.OilHeightpicturebox.Size = new System.Drawing.Size(161, 170);
            this.OilHeightpicturebox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.OilHeightpicturebox.TabIndex = 3;
            this.OilHeightpicturebox.TabStop = false;
            // 
            // SpeedpictureBox
            // 
            this.SpeedpictureBox.BackgroundImage = global::USBCAN.Resource1.WatchBack;
            this.SpeedpictureBox.Location = new System.Drawing.Point(231, 76);
            this.SpeedpictureBox.Name = "SpeedpictureBox";
            this.SpeedpictureBox.Size = new System.Drawing.Size(214, 213);
            this.SpeedpictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.SpeedpictureBox.TabIndex = 1;
            this.SpeedpictureBox.TabStop = false;
            // 
            // PressurepictureBox
            // 
            this.PressurepictureBox.BackgroundImage = global::USBCAN.Resource1.jpeg11;
            this.PressurepictureBox.Location = new System.Drawing.Point(64, 76);
            this.PressurepictureBox.Name = "PressurepictureBox";
            this.PressurepictureBox.Size = new System.Drawing.Size(157, 153);
            this.PressurepictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.PressurepictureBox.TabIndex = 0;
            this.PressurepictureBox.TabStop = false;
            // 
            // FormMeters
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(638, 491);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.OilHeightpicturebox);
            this.Controls.Add(this.SpeedpictureBox);
            this.Controls.Add(this.PressurepictureBox);
            this.Name = "FormMeters";
            this.Text = "仪表";
            this.Load += new System.EventHandler(this.FormMeters_Load);
            ((System.ComponentModel.ISupportInitialize)(this.OilHeightpicturebox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpeedpictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PressurepictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox PressurepictureBox;
        private System.Windows.Forms.PictureBox SpeedpictureBox;
        private System.Windows.Forms.PictureBox OilHeightpicturebox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Timer timer1;
    }
}