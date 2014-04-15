using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace USBCAN
{
    public partial class FormGear : Form
    {
        public FormGear()
        {
            InitializeComponent();
        }

        private void FormGear_Load(object sender, EventArgs e)
        {

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
        void UpdateMeters() {
        
        }

    }
}
