using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace USBCAN
{
    public partial class FormMeters : Form
    {
        Image probe ;

        VerticalProgressBar TempVp;
        VerticalProgressBar OilVp;

        int SpeedMeter = 0;
        int MaxSpeed = 76;

        int FuelMeter = 0;
        
        int OilHeight = 0;
        int MaxOilHeight = 120;

        int tempriture = 0;
        int Maxtemp = 120;

        int Pressure = 0;
        int MaxPressure =14;

        public FormMeters()
        {
            InitializeComponent();
        }

        private void FormMeters_Load(object sender, EventArgs e)
        {
            Point LocatinPoint = new Point(450, 250);
            int PointInterval = 25;

            probe = Resource1.probe;

            TempVp = new VerticalProgressBar();
            OilVp = new VerticalProgressBar();

            TempVp.Location = LocatinPoint;
            TempVp.Width=20;
            TempVp.Height = 100;
            TempVp.Value = 10;
            TempVp.Maximum = 500;
            OilVp.Maximum = 500;
 
            OilVp.Location = new Point(LocatinPoint.X+PointInterval, LocatinPoint.Y);
            OilVp.Width = 20;
            OilVp.Height = 100;
            OilVp.Value = 30;

            this.Controls.Add(TempVp);
            this.Controls.Add(OilVp);


        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Form1.htable.Contains(Form1.speed))
            {
                SpeedMeter = Convert.ToInt32(Form1.htable[Form1.speed]);
            }
          //
            if (Form1.htable.Contains(Form1.OilHeight))
            {
                OilHeight = Convert.ToInt32(Form1.htable[Form1.OilHeight]);
            }
            if (Form1.htable.Contains(Form1.pressure))
            {
                Pressure = Convert.ToInt32(Form1.htable[Form1.pressure]);
            }
            if (Form1.htable.Contains(Form1.temp))
            {
                tempriture = Convert.ToInt32(Form1.htable[Form1.temp]);
            }

          //  Pressure = Convert.ToInt32( Form1.htable[Form1.pressure]);
          //  tempriture =Convert.ToInt32( Form1.htable[Form1.temp]);

            UpdateMeters();

        }

        /// <summary>
        /// rotating the images
        /// </summary>
        /// <param name="filename"></param>
        /// <param name="angle"></param>
        /// <returns></returns>
        public Image RotateImg(string filename, int angle)
        {

            return RotateImg(GetSourceImg(filename), angle);

        }

        public Image GetSourceImg(string filename)
        {
            Image img;
            img = Bitmap.FromFile(filename);
            return img;

        }
        public Image RotateImg(Image b, int angle)
        {
            angle = angle % 360;
            //弧度转换
            double radian = angle * Math.PI / 180.0;
            double cos = Math.Cos(radian);
            double sin = Math.Sin(radian);
            //原图的宽和高
            int w = b.Width;
            int h = b.Height;
            int W = (int)(Math.Max(Math.Abs(w * cos - h * sin), Math.Abs(w * cos + h * sin)));
            int H = (int)(Math.Max(Math.Abs(w * sin - h * cos), Math.Abs(w * sin + h * cos)));
            //目标位图
            Bitmap dsImage = new Bitmap(W, H);

            System.Drawing.Graphics g = System.Drawing.Graphics.FromImage(dsImage);

            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear;

            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            //计算偏移量
            Point Offset = new Point((W - w) / 2, (H - h) / 2);


            //构造图像显示区域：让图像的中心与窗口的中心点一致

            Rectangle rect = new Rectangle(Offset.X, Offset.Y, w, h);

            Point center = new Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
            //  Point center = new Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);//original 

            g.TranslateTransform(center.X, center.Y);

            g.RotateTransform(360 - angle);


            //恢复图像在水平和垂直方向的平移

            g.TranslateTransform(-center.X, -center.Y);

            g.DrawImage(b, rect);
            //重至绘图的所有变换
            g.ResetTransform();
            g.Save();
            g.Dispose();

            //保存旋转后的图片

            b.Dispose();

            dsImage.Save("FocusPoint.jpg", System.Drawing.Imaging.ImageFormat.Jpeg);

            return dsImage;

        }
        /// <summary>
        /// 更新界面
        /// </summary>
        void UpdateMeters()
        {
            probe = Resource1.pro;
            int SpeedAngle = (int)(SpeedMeter * (-3.55) - 180);
            int TempAngle = (int)(tempriture * (-3.55) - 180);
            int PressureAngle = (int)(Pressure * (-3.55) - 180);

            //int OilHeightAngle=OilHeight/MaxOilHeight;
            SpeedpictureBox.Image = RotateImg(probe, SpeedAngle);
            PressurepictureBox.Image = RotateImg(Resource1.pro, PressureAngle);
            OilHeightpicturebox.Image = RotateImg(Resource1.pro, TempAngle);
            if (OilHeight < OilVp.Maximum)
            {
                OilVp.Value = OilHeight;
            }
            if (tempriture < TempVp.Maximum)
            {
                TempVp.Value = tempriture;
            }
        }

        private void label5_Click(object sender, EventArgs e)
        {

        }




    }
}
