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
        int OrgX = 100;
        int OrgY = 100;
        String fileContent;
        ArrayList path_list;
        String writebuf="";
        Coor[] Maps = new Coor[10];
        Coor[] Mapz = new Coor[10];

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
            for (int i = 0; i < 10; i++)
            {
                Mapz[i] = new Coor();
                Mapz[i].X = 36000 - i * 3600;
                //Map[i].y=-800+1600/9*i;
//                Map[i].y = -90;
                Mapz[i].Y = -80;
            }
    
                for ( int i = 0; i < 10; i = i + 2)
                {

                    Mapz[i].Y = +80;
                }


                for (int i = 0; i < 10; i++)
                {
                    Maps[i] = new Coor();
                    Maps[i].X = 36000 - i * 3600;
                    //Map[i].y=-800+1600/9*i;
                    //                Map[i].y = -90;
                    Maps[i].Y = 450;
                }


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
            FileStream fs1;

            if (!File.Exists("D:\\processed.txt"))
            {
                //StreamWriter sw = new StreamWriter("D:\\processed.txt", false);

                fs1 = new FileStream("D:\\processed.txt", FileMode.Create, FileAccess.Write);//创建写入文件 
                

            }
            else
            {
                fs1 = new FileStream("D:\\processed.txt", FileMode.Open, FileAccess.Write);
              
                
            }
            StreamWriter sw = new StreamWriter(fs1);
            
            
            System.Windows.Forms.OpenFileDialog openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
             //StreamWriter sw = new StreamWriter("txtwriter.txt", false);

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
                        x_y = path_list[i].ToString().Split(' ');
                        DrawCoorodinates_withColor(Convert.ToInt32(x_y[1]), Convert.ToInt32(x_y[3]), Color.Blue);

                    }
                    else
                    {
                        path_list[i] = path_list[i].ToString().Replace("Real ", "");
                        
                        x_y = path_list[i].ToString().Split(' ');
                        if (x_y.GetLength(0) >= 4)
                        {
                            if (x_y[1]!=""&&!x_y[1].Contains("\n")&&x_y[3]!=""&&!x_y[3].Contains("\n"))
                            {
                                if (Convert.ToInt32( x_y[1])>Convert.ToInt32( x_y[3]))
                                {
                                    String tempswap = "";
                                    tempswap = x_y[3];
                                    x_y[3] = x_y[1];
                                    x_y[1]=tempswap;

                                }
                                DrawCoorodinates_withColor(Convert.ToInt32(x_y[1])/100+OrgX, Convert.ToInt32(x_y[3])/100+OrgY, Color.Black);
                                writebuf =  x_y[1] + " " + x_y[3] ;  
                                sw.WriteLine(writebuf);


                            }

                          
                        }

                    }

                }

            }
            sw.Close();
            fs1.Close();
            //sw.Close();
        }
    }
}
