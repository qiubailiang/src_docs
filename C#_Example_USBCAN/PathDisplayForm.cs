using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;

namespace USBCAN
{
    public partial class PathDisplayForm : Form
    {
        int OrgX = 10;
        int OrgY = 10;
        String fileContent;
        ArrayList path_list;
        public void DrawCoorodinates_withColor(int x, int y, Color col)
        {
            //    //pen 画线

            Pen myPen = new Pen(col);

            Graphics g = this.CreateGraphics();
            g.DrawEllipse(myPen, x, y, 5, 5);


        }
        public void DrawAxis(int org_x, int org_y, int length, Color col)
        {
            Pen myPen = new Pen(col, 3);
            Graphics g = this.CreateGraphics();
            g.DrawLine(myPen, (float)org_x, (float)org_y, (float)(org_x + length), (float)org_y);
            g.DrawLine(myPen, (float)org_x, (float)org_y, (float)(org_x), ((float)org_y + length));
        }
       
        public PathDisplayForm()
        {
            InitializeComponent();
        }

  
        private void PathDisplayForm_Load(object sender, EventArgs e)
        {
            path_list = new ArrayList();
            DrawAxis(OrgX, OrgY, 1000, Color.Blue);

        }

        private void PathDisplayForm_Click(object sender, EventArgs e)
        {
            OrgX = MousePosition.X;
            OrgY = MousePosition.Y;
            DrawAxis(OrgX, OrgY, 1000, Color.Blue);
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openFileDialog1 = new System.Windows.Forms.OpenFileDialog();

            openFileDialog1.Filter = "文本文件(*.txt)|*.txt|所有文件(*.*)|*.*";

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string FileName = openFileDialog1.FileName;
                // your file-processing code
                StreamReader sr = new StreamReader(FileName);
                fileContent = sr.ReadToEnd();

                String[] path_points = fileContent.Split('<', '>');


                path_list.Clear();
                for (int i = 0; i < path_points.GetLength(0); i++)
                {
                    if (path_points[i] != null && path_points[i] != "")
                    {
                        path_list.Add(path_points[i]);


                    }
                }

                DrawAxis(OrgX, OrgY, 1000, Color.Blue);

                for (int i = 0; i < path_list.Count; i++)
                {
                    Coor Apoint = new Coor();
                    String[] x_y = new String[2];
                    if (path_list[i].ToString().Contains("Map"))
                    {
                        path_list[i] = path_list[i].ToString().Replace("Map", "");
                        x_y = path_list[i].ToString().Split(',');
                        DrawCoorodinates_withColor(Convert.ToInt32(x_y[0]), Convert.ToInt32(x_y[1]), Color.Blue);

                    }
                    else
                    {
                        x_y = path_list[i].ToString().Split(',');
                        DrawCoorodinates_withColor(Convert.ToInt32(x_y[0]), Convert.ToInt32(x_y[1]), Color.Black);


                    }

                }

            }
        }
    }
}
