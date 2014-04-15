using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace USBCAN
{
    public partial class FormControl : Form
    {
        int Speed = 0;
        int Speedtogo = 0;

        int OilHeight = 0;
        int OilHeighttogo = 0;

        int Pressure = 0;
        int Pressuretogo = 0;

        int Temp = 0;
        int Temptogo = 0;

        public FormControl()
        {
            InitializeComponent();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            UpdateMeter();

        }
        void UpdateMeter() {
            if (Form1.htable.ContainsKey(Form1.speed)) {
                Speed =Convert.ToInt32( Form1.htable[Form1.speed]);

            }
            label6.Text = Speed.ToString();
            if (Form1.htable.ContainsKey(Form1.OilHeight)) {
                OilHeight =Convert.ToInt32( Form1.htable[Form1.OilHeight]);
            }
            label5.Text =Pressure.ToString();
            if(Form1.htable.ContainsKey(Form1.temp)){
                Temp =Convert.ToInt32( Form1.htable[Form1.temp]);
            }
            label7.Text = Temp.ToString();
        }
        void UpdateReading() {
            label5.Text = OilHeight.ToString();
            
        }
        private void FormControl_Load(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            
            Speedtogo+=10;
            Form1.htable[Form1.speedtogo] = Speedtogo;

        }

        private void button4_Click(object sender, EventArgs e)
        {
            if ((Speedtogo-10)>= 0)
            {
                Speedtogo -= 10;
                Form1.htable[Form1.speedtogo] = Speedtogo;
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != 8 && !Char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != 8 && !Char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void textBox3_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != 8 && !Char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void textBox4_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != 8 && !Char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Form1.htable[Form1.OilHeighttogo] = Convert.ToInt32 (Form1.htable[Form1.OilHeighttogo])+10;
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.ToString() !="")
            {
                Form1.htable[Form1.pressuretogo] = Convert.ToInt32(textBox1.Text.ToString());
            }
        }

        private void button10_Click(object sender, EventArgs e)
        {
            if (textBox2.Text.ToString()!="")
            Form1.htable[Form1.speedtogo] = Convert.ToInt32(textBox2.Text.ToString());
        }

        private void button11_Click(object sender, EventArgs e)
        {
            if (textBox3.Text.ToString() !="")
            Form1.htable[Form1.temptogo] = 50+Convert.ToInt32(textBox3.Text.ToString());
        }

        private void button12_Click(object sender, EventArgs e)
        {
            if (textBox4.Text.ToString() !="")
            Form1.htable[Form1.pressuretogo] = Convert.ToInt32(textBox4.Text.ToString());
        }

        private void button5_Click(object sender, EventArgs e)
        {

        }
    }
}
