using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections;
using USBCAN.Properties;
using System.IO;

namespace USBCAN
{
    public partial class Form1 : Form
    {
        /// <summary>
        /// Form level Variables
        /// </summary>
       // private static int MyAngle = 0; //旋转角度[-360,360]
        private static int InitAngle = -210;
        private Image img;
        public static Hashtable htable;
        private FormGear gearform;
        private FormControl controlform;
        private FormMeters meterform;
        VerticalProgressBar TempVpOnMain;
        VerticalProgressBar OilHeightVpOnMain;
        /// <summary>
        /// data segment  DS 
        /// </summary>
        string PARK = "PARK";
        string D1 = "D1";
        string D2 = "D2";
        string D3 = "D3";
        string D4 = "D4";
        string REVERS = "REVERS";
        ArrayList alist;
        int ArrayCounter = 0;
        int SpeedToSend = 0;
        int GearState = 0;
        public static string SpeedID = "C7 F7 88 00";
        public static string GearID = "18 FE F1 1";
        public static string TempID = "C7 F7 70 00";
        public static string OilHeightID = "C7 F7 E0 00";
        public static string PressureID = "67 80 20 00";
        public static string WatchID = "50000000";

        public static string SelSpeedID = "00 00 00 0A";
        
        public static string speed = "speed";
        public static string speedtogo = "speedtogo";
        private int LastSpeed = 0;

        public static string temp = "temp";
        public static string temptogo = "temptogo";
        private int LastTemp = 0;

        public static string OilHeight = "OilHeight";
        public static string OilHeighttogo = "OilHeighttogo";
        private int LastOilHeight = 0;

        public static string gear = "gear";
        public static string geartogo = "geartogo";

        public static string pressure = "pressure";
        public static string pressuretogo = "pressuretogo";
        private int LastPressure = 0;

        int SpeedAngle = 0;
        int MaxSpeed = 100;

        Int32 distance = 0;
        Int32 angle = 0;

        Int32 X_actual;
        Int32 Y_actual;

        String file_name;
        String file_txt;
        Coor valid_coord;
        /// <summary>
        /// claim ends
        /// </summary>
        /// protacals
        ///Gear  out  01000000H 
        ///Gear input 02000000H
        ///Temp input 10000000H
        ///
        PVCI_CAN_OBJ CurrentBuff;
        [StructLayout(LayoutKind.Sequential)] 
        public struct PVCI_INIT_CONFIG
        {
            public uint AccCode;
            public uint AccMask;
            public uint Reserved;
            public byte Filter;
            public byte kCanBaud;
            public byte Timing0;
            public byte Timing1;
            public byte Mode;
            public byte CanRx_IER;
        }
        [StructLayout(LayoutKind.Sequential)]   
        public  struct  PVCI_CAN_OBJ
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]//声明数组大小为4
            public byte [] ID; //4
            public UInt32 TimeStamp;//C++的UINT对应32位
            public byte TimeFlag;
            public byte SendType;
            public byte RemoteFlag; //=0
            public byte ExternFlag; //=1
            public byte DataLen; //=8
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]//声明数组大小为8
            public byte[] Data; //8
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]//声明数组大小为3
            public byte[] Reserved;//3
        }
        [DllImport("VCI_CAN.dll",EntryPoint = "VCI_OpenDevice")]
        public static extern int VCI_OpenDevice(uint Devtype, uint Devindex, uint Reserved);
        [DllImport("VCI_CAN.dll", EntryPoint = "VCI_StartCAN")]
        public static extern int VCI_StartCAN(uint Devtype, uint Devindex, uint CANInd);
        [DllImport("VCI_CAN.dll", EntryPoint = "VCI_CloseDevice")]
        public static extern int VCI_CloseDevice(int DeviceType, int DeviceInd);
        [DllImport("VCI_CAN.dll", EntryPoint = "VCI_SetReference")]
        public static extern int VCI_SetReference(uint DeviceType, uint DeviceInd, uint CANInd, uint RefType, ref byte pData);
        [DllImport("VCI_CAN.dll", EntryPoint = "VCI_InitCAN")]  
        public static extern int VCI_InitCAN(uint DevType, uint DevIndex, uint CANIndex, ref PVCI_INIT_CONFIG pInitConfig);
        [DllImport("VCI_CAN.dll", EntryPoint = "VCI_Transmit")]
        public static extern int VCI_Transmit(uint DevType, uint DevIndex, uint CANIndex, ref PVCI_CAN_OBJ pSend);
        [DllImport("VCI_CAN.dll", EntryPoint = "VCI_Receive")]
        public static extern int VCI_Receive(uint DevType, uint DevIndex, uint CANIndex, [Out]/*声明参数为输出*/ PVCI_CAN_OBJ[] pReceive);
        uint m_nNum = 0;//发送和接收的总帧数
        public Form1()
        {
            InitializeComponent();
        }
        public void DrawCoorodinates_withColor(int x, int y, Color col)
        {
            // //pen 画线
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
        private void label1_Click(object sender, EventArgs e)
        {
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            file_txt = "";
            ORG_X = 1000;
            ORG_Y = 500;

            string A = "00";
            string B = "00";
            string C = "1";
            string R = "0"+ B + C;
            img = Resource1.littleprobe;
            gearform = new FormGear();
            alist = new ArrayList();
            alist.Add(PARK);
            alist.Add(D1);
            alist.Add(D2);
            alist.Add(D3);
            alist.Add(D4);
            alist.Add(REVERS);
            ///the following codes
            ///initialize the hashtable
            ///
            htable = new Hashtable();
            htable.Add("speed", 0);
            htable.Add("gear", PARK);
            htable.Add("temp", 0);
            htable.Add("OilHeight",0);
            htable.Add(pressure, 0);
            htable.Add(speedtogo, 0);
            htable.Add(pressuretogo, 0);
            htable.Add(OilHeighttogo, 0);
            htable.Add(temptogo, 0);
            ///
            ///Init the ProgressBars
            ///
            int gap = 30;
            TempVpOnMain = new VerticalProgressBar();
            OilHeightVpOnMain = new VerticalProgressBar();
            TempVpOnMain.Location = new Point(750, 330);
            TempVpOnMain.Width = 20;
            TempVpOnMain.Height = 100;
            TempVpOnMain.Maximum = 500;
            OilHeightVpOnMain.Location = new Point(800+gap, 330);
            OilHeightVpOnMain.Width = 20;
            OilHeightVpOnMain.Height = 100;
            OilHeightVpOnMain.Maximum = 500;
            //PressureProgressBar.Maximum = 500;
            
            this.Controls.Add(TempVpOnMain);
            this.Controls.Add(OilHeightVpOnMain);
            DrawAxis(ORG_X, ORG_Y, 500, Color.Blue);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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
        /// <summary>
        /// rotation function over
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>



        private void buttonOpen_Click(object sender, EventArgs e)
        {
            if (VCI_OpenDevice(2, 0, 0) != 1)
            {
                MessageBox.Show("Open USB_CAN fail");
            }
            else
            {
                PVCI_INIT_CONFIG[] config = new PVCI_INIT_CONFIG[1];
                config[0].AccCode = 0x80000008;
                config[0].AccMask = 0xFFFFFFFF;
                config[0].Reserved = 204;
                config[0].Filter = 0;
                config[0].kCanBaud = 15;
                /////////////
                ////////Config the baund rate
                config[0].Timing0 =0x01;// 0x00;
                config[0].Timing1 = 0x1C;//0x14;
                /////
                config[0].CanRx_IER = 1;
                config[0].Mode =0;//1~自发自收模式，0~正常工作模式

                if (VCI_InitCAN(2, 0, 0, ref config[0]) != 1)
                {
                    MessageBox.Show("Init USB_CAN fail");
                    return;
                }
            
                if (VCI_StartCAN(2, 0, 0) != 1)
                {
                    MessageBox.Show("Start USB_CAN fail");
                    return;
                }
            }
            buttonOpen.Enabled = false;
            buttonClose.Enabled = true;
            buttonSend.Enabled=true;
            MessageBox.Show("启动成功");

        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            int count = VCI_CloseDevice(2, 0);
            if (count == 1)
            {
                buttonOpen.Enabled = true;
                buttonClose.Enabled = false;
                buttonSend.Enabled = false;
                MessageBox.Show("close sucessful");
            }
            else if (count == 0)
            {
                MessageBox.Show("close fail");
            }
            else
            {
                MessageBox.Show("device not open");
            }

        }
        public int HexChar(char c)
        {
            if ((c >= '0') && (c <= '9'))
                return c - 0x30;
            else if ((c >= 'A') && (c <= 'F'))
                return c - 'A' + 10;
            else if ((c >= 'a') && (c <= 'f'))
                return c - 'a' + 10;
            else
                return 0x10;
        }

        public int Str2Hex(string str)
        {
            int len = str.Length;
            if (len == 2)
            {
                int a = HexChar(str[0]);
                int b = HexChar(str[1]);
                if (a == 16 || b == 16)
                {
                    MessageBox.Show("format error！");
                    return 256;
                }
                else
                {
                    return a * 16 + b;

                }

            }
            else
            {
                MessageBox.Show("len must be 2");
                return 256;
            }
        }
        public void DrawCoorodinates(int x,int y) 
        {
            //    //pen 画线

          Pen myPen = new Pen(Color.Black);

          Graphics g = this.CreateGraphics();
          g.DrawEllipse(myPen, x, y, 5, 5);

        
        }
        public void DrawCoorodinates_with_pen(int x, int y,Pen p)
        {
            //    //pen 画线

            
            Graphics g = this.CreateGraphics();
            g.DrawEllipse(p, x, y, 5, 5);


        }
        private void buttonSend_Click(object sender, EventArgs e)
        {
            DrawCoorodinates(1000, 50);
            PVCI_CAN_OBJ sendbuf = new PVCI_CAN_OBJ();
            sendbuf.ID = new byte[4];
            sendbuf.Data = new byte[8];
            sendbuf.Reserved = new byte[3];
            string[] str5 = new string[32];
            byte[] buf = new byte[50];
            byte[] SendID = new byte[10];
            string strtemp, strtemp1;
            string str1;
            int len, datanum = 0, IDnum = 0, newflag = 1;
            string strSendID =textBoxID.Text.ToString();
            char[] m_strSendID = strSendID.ToCharArray();

            len = m_strSendID.Length;
            for (int i = 0; i < len; i++)
            {
                strtemp = m_strSendID[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {
                    newflag = 0;
                    strtemp = m_strSendID[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[IDnum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendID[i + 1].ToString();

                        if (strtemp1 == " ")
                            str5[IDnum] = "0" + strtemp;
                        else
                            str5[IDnum] = strtemp + strtemp1;
                    }

                    SendID[IDnum] = (byte)Str2Hex(str5[IDnum]);
                    IDnum++;
                    if (IDnum >= 4)
                        break;
                }
            }
            newflag = 1;

            string strSendData = TextBoxSend.Text.ToString();
            char[] m_strSendData = strSendData.ToCharArray();
            len = m_strSendData.Length;
            //m_strSendData.GetLength();
            for (int i = 0; i < len; i++)
            {

                strtemp = m_strSendData[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {

                    newflag = 0;
                    strtemp = m_strSendData[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[datanum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendData[i + 1].ToString();

                        if (strtemp1 == " ")
                        {
                            str5[datanum] = "0" + strtemp;

                        }
                        else
                            str5[datanum] = strtemp + strtemp1;

                    }
                    buf[datanum] = (byte)Str2Hex(str5[datanum]);
                    datanum++;
                    if (datanum >= 8)
                        break;

                }

            }
            byte m_nSendFrameType = 0;
            byte m_nSendFrameFormat = 0;
            byte m_radioIDFormat = 0;
            //////////
            /////CHANGED
            sendbuf.ExternFlag = 1;//m_nSendFrameType;//1
            sendbuf.DataLen = (byte)datanum;//8
            sendbuf.RemoteFlag = m_nSendFrameFormat;//0
            if (m_nSendFrameFormat == 1)//if remote frame, data is invalid
                for (int i = 0; i < datanum; i++)
                    buf[i] = 0;
            byte[] SendID2 = new byte[4];
            /////////////////////////////////
            /////////////////brlow changed
            if ((sendbuf.ExternFlag) == 1)//Extend ID progress
            {
                if (m_radioIDFormat != 0)//将直接ID值进行转换到CAN2.0B格式
                {
                   // SendID[0] &= 0x1F;
                    SendID2[0] = (byte)(SendID[0]<<3);
                    SendID2[1] = (byte)(SendID[1] <<3);
                    SendID2[2] = (byte)(SendID[2]<<3);
                    SendID2[3] = (byte)(SendID[3]<<3);
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //	sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];
                }
                else
                {
                  //  SendID[3] = (byte)(SendID[3] & 0xF8);//the last 3 bits is invalid,clear to 0
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = (byte)(SendID[i] << 3);
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }
            else//Basic CAN
            {

                if (m_radioIDFormat != 0)//Direct id value to SJA1000 format
                {
                    //SendID[2] &= 0x07;
                    SendID2[0] = (byte)(SendID[0]<<3);
                    SendID2[1] = (byte)(SendID[1]<<3);
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];

                }
                else
                {
                    //SendID[1] = (byte)(SendID[1] & 0xE0);//the last 5 bits is invalid, clear to 0
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = (byte)(SendID[i] << 3);
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }
            //////////////above changed
            ///////////////////////////////////////
            for (int i = 0; i < datanum; i++)
                sendbuf.Data[i] = buf[i];


            int flag=0;
          //  while (flag != 1)
            //{
                flag = VCI_Transmit(2, 0, 0, ref sendbuf);//CAN DATA SEND
            //}
            PVCI_CAN_OBJ[] TempBuf = new PVCI_CAN_OBJ[50];
            TempBuf[0] = sendbuf;
            showCan(0, 0, TempBuf, 1);
            //DrawBar(TempBuf);
            if (flag != 1)
            {
                if (flag == -1)
                    MessageBox.Show("fail,device not open\n");
                else if (flag == 0)
                   // MessageBox.Show("fail\n");
                return;

            }
            string str = "";
            for (int i = 0; i < sendbuf.DataLen; i++)			//DATA
            {
                uint data = sendbuf.Data[i];
                str1 = Convert.ToString(data, 16);
                str += str1 + " ";
            }
            CurrentBuff=sendbuf;
            //    listView1.Items.Add(str).SubItems.Add("Send");
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="shiliu"></param>
        /// <returns></returns>
        private int HexToInt(string shiliu)
        {
            int shi = 0;
            try
            {
                int Int = Convert.ToInt32(shiliu, 16);
                shi = Int;
            }
            catch (Exception Err)
            {
                MessageBox.Show(Err.Message);
            }
            return shi;
        }
        /// <summary>
        /// HexArr convert to int
        /// </summary>
        /// <param name="arr"></param>
        /// <returns></returns>
        int HexArr2Int(byte[] arr) {
            int sum=0;
            for (int i = 0; i < arr.GetLength(0);i++ ) {
                
                sum+=HexToInt(Convert.ToString(arr[i]))*16^(7-i);

            }
            return sum;
        }
        /// <summary>
        /// timer's tick in which we recive and update the data sent to the PC then update them to the htable
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timerRecieve_Tick(object sender, EventArgs e)
        {
            
            PVCI_CAN_OBJ[] sendbuf = new PVCI_CAN_OBJ[200];
            int NumValue = 0;
            NumValue = VCI_Receive(2, 0, 0, sendbuf);/////////////////Get the msg from the CAN NetWork
            showCan(0, 1, sendbuf, NumValue);
            int len=sendbuf.GetLength(0);

            if (sendbuf[0].DataLen > 0)
            {
                CurrentBuff = sendbuf[0];
                for (int i = 0; i < CurrentBuff.ID.GetLength(0); i++)
                {
                    CurrentBuff.ID[i] = (byte)((CurrentBuff.ID[i])>> 3);
                }
                ////////////////Recive the data from the dsp /////recive data of valid distance and draw on board ////the ID is 00 11 12 19
                if (CurrentBuff.ID[0] == 0x00 && CurrentBuff.ID[1] == 0x11 && CurrentBuff.ID[2] == 0x12 && CurrentBuff.ID[3] == 0x19)
                {
                    Int32[] temp = new Int32[8];
                    CurrentBuff = sendbuf[0];
                    for (int i = 0; i < 6; i++)
                    {
                        temp[i] = CurrentBuff.Data[i];
                    }
                    temp[0] = (temp[0] << 16);
                    temp[1] = (temp[1] << 8);
                    temp[2] = (temp[2]);
                    distance = temp[0] + temp[1] + temp[2];
                    angle = (temp[3]<<16) + (temp[4]<<8) + temp[5];
                    angle = angle * 360 /1152000;
                    valid_coord = new Coor();
                    valid_coord= pollor_to_Descart(distance, angle);
                    file_txt+="<Real X " + valid_coord.X.ToString() + " Y " + valid_coord.Y.ToString() + ">\n\r ";
                    //write_file("E:\\trail.txt","","X "+valid_coord.X.ToString()+"Y "+valid_coord.Y.ToString()+"\n\r");

                    //"D:\\trail.txt"
                    DrawCoorodinates( ORG_X+ (valid_coord.X)/100,ORG_Y+ (valid_coord.Y)/100);

                }

                ////////////////Recive the data from the dsp /////valid aim map point 00 11 12 1a
                if (CurrentBuff.ID[0] == 0x00 && CurrentBuff.ID[1] == 0x11 && CurrentBuff.ID[2] == 0x12 && CurrentBuff.ID[3] == 0x1a)
                {
                    Int32[] temp = new Int32[8];
                    CurrentBuff = sendbuf[0];
                    for (int i = 0; i < 6; i++)
                    {
                        temp[i] = CurrentBuff.Data[i];
                    }
                    temp[0] = (temp[0] << 16);
                    temp[1] = (temp[1] << 8);
                    temp[2] = (temp[2]);
                    x_map = temp[0] + temp[1] + temp[2];
                    y_map = (temp[3] << 16) + (temp[4] << 8) + temp[5];
                    //angle = angle * 360 / 1152000;
                    valid_coord = new Coor();
                    valid_coord = pollor_to_Descart(distance, angle);
                    file_txt += "<Map :X " + valid_coord.X.ToString() + " Y " + valid_coord.Y.ToString() + ">\n\r ";
                    //write_file("E:\\trail.txt","","X "+valid_coord.X.ToString()+"Y "+valid_coord.Y.ToString()+"\n\r");

                    //"D:\\trail.txt"
                    DrawCoorodinates_with_pen(ORG_X + (valid_coord.X) / 100, ORG_Y + (valid_coord.Y) / 100,new Pen(Color.Red));

                }
         
            }
            //  TempVpOnMain.Value = (int)Form1.htable[pressure]/TempVpOnMain.Maximum;
            //DrawBar(sendbuf[sendbuf.GetLength(0)-1]);

        }

        int GetSpeedAngleFromHtable(Hashtable table) {
            if (table.ContainsKey("Speed"))
            {
                int tempSpeed = Convert.ToInt32(table["Speed"].ToString());
                tempSpeed = tempSpeed / MaxSpeed;
                return tempSpeed;
            }
            else {
                return 0;
            }
        }


        private bool write_file(String path, String f_name, String content)
        {
            if (!File.Exists(path + f_name))//"D:\\trail.txt"
            {
                try
                {
                    FileStream fs1 = new FileStream(path + f_name, FileMode.Create, FileAccess.Write);//创建写入文件
                    StreamWriter sw = new StreamWriter(fs1);
                    
                    sw.WriteLine(content);//开始写入值
                    sw.Close();
                    fs1.Close();
                    return true;
                }
                catch (IOException e)
                {
                    MessageBox.Show(e.ToString());
                    return false;
                }

            }
            else
            {
                try
                {
                    FileStream fs = new FileStream(path + f_name, FileMode.Open, FileAccess.Write);
                    StreamWriter sr = new StreamWriter(fs);
                    sr.WriteLine(content);//开始写入值
                    sr.Close();
                    fs.Close();
                    return true;
                }
                catch (IOException e)
                {
                    MessageBox.Show(e.ToString());
                    return false;
                }
            }

        }




        private void showCan(byte byCanIndex,byte byOrientation, PVCI_CAN_OBJ []buf,int nLength)
        {
           DateTime CurrentTime = new DateTime();
            for(uint i=0;i<nLength;i++)
            {
                PVCI_CAN_OBJ temp = buf[i];
                ListViewItem Item = new ListViewItem();
                string strNum = Convert.ToString(m_nNum);
                string strTime = DateTime.Now.ToString();//strHour + ":" + strMinute + "" + strSecond;
                int foundS1 = strTime.IndexOf(" ");
                strTime= strTime.Remove(0, foundS1);

                string strCanIndex = Convert.ToString(byCanIndex);
                string strOrientation;
                if(byOrientation==0)
                    strOrientation ="发送";
                else
                    strOrientation="接收";
                byte a = temp.ID[0];
                string strID0 = Convert.ToString((buf[i]).ID[0]>>3);
                string strID1 = Convert.ToString((buf[i]).ID[1] >> 3);
                string strID2 = Convert.ToString((buf[i]).ID[2] >> 3);
                string strID3 = Convert.ToString((buf[i]).ID[3] >> 3);
                string strID=strID0+" "+strID1+" "+strID2+" "+strID3;
                string strFrame;
                if ((buf[i]).RemoteFlag==1)
                    strFrame="远程帧";
                else
                    strFrame="数据帧";
                string strType;
                if(buf[i].ExternFlag==1)
                    strType="扩展帧";
                else
                    strType="标准帧";
                string strDataLen=Convert.ToString(buf[i].DataLen);
                string strData=Convert.ToString(buf[i].Data[0]);
                for(int j=1;j<8;j++)
                {
                    string strTemp;
                    strTemp=Convert.ToString(buf[i].Data[j]);
                    strData=strData+" "+strTemp;
                }    
                Item.SubItems.Clear();
                Item.SubItems[0].Text = strNum;
                Item.SubItems.Add(strTime);
                Item.SubItems.Add(strCanIndex);
                Item.SubItems.Add(strOrientation);
                Item.SubItems.Add(strID);
                Item.SubItems.Add(strFrame);
                Item.SubItems.Add(strType);
                Item.SubItems.Add(strDataLen);
                Item.SubItems.Add(strData);
                listViewRecieve.Items.Add(Item);
                m_nNum++;
            }
            //DrawBar(buf);/////////////////////////////////
        }

        private void listViewRecieve_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }



        private void button1_Click(object sender, EventArgs e)
        {
            //SpeedToSend+=1;
            //0F000000
            Char[] value = (up_down_value.Text.ToString().Trim().ToCharArray());
            String vertikcal_step = "";
            vertikcal_step=value[0].ToString() + value[1].ToString();
           
            TransmitACanMsg("00 11 12 13","00 00 00 00 00 "+vertikcal_step+" 00 00");
            
        }

        private void init_can(int baud,int work_mode) 
        {
            PVCI_INIT_CONFIG[] config = new PVCI_INIT_CONFIG[1];
            config[0].AccCode = 0x80000008;
            config[0].AccMask = 0xFFFFFFFF;
            config[0].Reserved = 204;
            config[0].Filter = 0;
            config[0].kCanBaud = 15;
            /////////////
            ////////Config the baund rate
            config[0].Timing0 = 0x01;// 0x00;
            config[0].Timing1 = 0x1C;//0x14;
            if (baud==250)
            {
                config[0].Timing0 = 0x01;// 0x00;
                config[0].Timing1 = 0x1C;//0x14;
            }
            else
            {
                if (baud==500)
                {
                    config[0].Timing0 = 0x00;// 0x00;
                config[0].Timing1 = 0x1C;//0x14;
                }   
            }
          
            /////
            config[0].CanRx_IER = 1;
            config[0].Mode =(byte) work_mode;//1~自发自收模式，0~正常工作模式

            if (VCI_InitCAN(2, 0, 0, ref config[0]) != 1)
            {
                MessageBox.Show("Init USB_CAN fail");
                return;
            }

            if (VCI_StartCAN(2, 0, 0) != 1)
            {
                MessageBox.Show("Start USB_CAN fail");
                return;
            }
        }
        private void button2_Click(object sender, EventArgs e)
        {
            Char[] value = (up_down_value.Text.ToString().Trim().ToCharArray());
            String vertikcal_step = "";
            vertikcal_step = value[0].ToString() + value[1].ToString();

            TransmitACanMsg("00 11 12 13", "00 00 00 00 00 " + vertikcal_step + " 00 01");
           
            
        }

        public void TransmitACanMsg(string IDtoSend,string data) {
            PVCI_CAN_OBJ sendbuf = new PVCI_CAN_OBJ();
            sendbuf.ID = new byte[4];
            sendbuf.Data = new byte[8];
            sendbuf.Reserved = new byte[3];
            string[] str5 = new string[32];
            byte[] buf = new byte[50];
            byte[] SendID = new byte[10];
            string strtemp, strtemp1;
           // string str1;
            int len, datanum = 0, IDnum = 0, newflag = 1;
            string strSendID = IDtoSend;//textBoxID.Text.ToString();
            char[] m_strSendID = strSendID.ToCharArray();

            len = m_strSendID.Length;
            for (int i = 0; i < len; i++)
            {
                strtemp = m_strSendID[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {
                    newflag = 0;
                    strtemp = m_strSendID[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[IDnum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendID[i + 1].ToString();

                        if (strtemp1 == " ")
                            str5[IDnum] = "0" + strtemp;
                        else
                            str5[IDnum] = strtemp + strtemp1;
                    }

                    SendID[IDnum] = (byte)Str2Hex(str5[IDnum]);
                    IDnum++;
                    if (IDnum >= 4)
                        break;
                }
            }
            newflag = 1;

            string strSendData =data;
            char[] m_strSendData = strSendData.ToCharArray();
            len = m_strSendData.Length;
            //m_strSendData.GetLength();
            for (int i = 0; i < len; i++)
            {
                
                strtemp = m_strSendData[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {

                    newflag = 0;
                    strtemp = m_strSendData[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[datanum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendData[i + 1].ToString();

                        if (strtemp1 == " ")
                        {
                            str5[datanum] = "0" + strtemp;

                        }
                        else
                            str5[datanum] = strtemp + strtemp1;

                    }
                    buf[datanum] = (byte)Str2Hex(str5[datanum]);
                    datanum++;
                    if (datanum >= 8)
                        break;
                }
            }

            byte m_nSendFrameType = 0;
            byte m_nSendFrameFormat = 0;
            byte m_radioIDFormat = 0;
            sendbuf.ExternFlag = 1;// m_nSendFrameType;//1
            sendbuf.DataLen = (byte)datanum;//8
            sendbuf.RemoteFlag = m_nSendFrameFormat;//0
            if (m_nSendFrameFormat == 1)//if remote frame, data is invalid
                for (int i = 0; i < datanum; i++)
                    buf[i] = 0;
            byte[] SendID2 = new byte[4];

            if ((sendbuf.ExternFlag) == 1)//Extend ID progress
            {
                if (m_radioIDFormat != 0)//将直接ID值进行转换到CAN2.0B格式
                {
                    // SendID[0] &= 0x1F;
                    SendID2[0] = (byte)(SendID[0] << 3);
                    SendID2[1] = (byte)(SendID[1] << 3);
                    SendID2[2] = (byte)(SendID[2] << 3);
                    SendID2[3] = (byte)(SendID[3] << 3);
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //	sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];
                }
                else
                {
                    //  SendID[3] = (byte)(SendID[3] & 0xF8);//the last 3 bits is invalid,clear to 0
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = (byte)(SendID[i] << 3);
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }
            else//Basic CAN
            {

                if (m_radioIDFormat != 0)//Direct id value to SJA1000 format
                {
                    //SendID[2] &= 0x07;
                    SendID2[0] = (byte)(SendID[0] << 3);
                    SendID2[1] = (byte)(SendID[1] << 3);
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];

                }
                else
                {
                    //SendID[1] = (byte)(SendID[1] & 0xE0);//the last 5 bits is invalid, clear to 0
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = (byte)(SendID[i] << 3);
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }

            for (int i = 0; i < datanum; i++)
                sendbuf.Data[i] = buf[i];


            int flag=0;
          
                flag = VCI_Transmit(2, 0, 0, ref sendbuf);//CAN DATA SEND
            
            PVCI_CAN_OBJ[] TempBuf = new PVCI_CAN_OBJ[50];
            for (int i = 0; i < TempBuf.GetLength(0); i++) {
                TempBuf[i] = sendbuf;
            }
                //TempBuf[1] = sendbuf;
                showCan(0, 0, TempBuf, 1);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            meterform = new FormMeters();
            meterform.Show();
        }

        private void button4_Click(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            controlform = new FormControl();
            controlform.Show();
        }

        private void label6_Click(object sender, EventArgs e)
        {

        }
        private void transmitVariableCan(string id, int data,int loc) { 
        PVCI_CAN_OBJ sendbuf = new PVCI_CAN_OBJ();
            sendbuf.ID = new byte[4];
            sendbuf.Data = new byte[8];
            sendbuf.Reserved = new byte[3];
            string[] str5 = new string[32];
            byte[] buf = new byte[50];
            byte[] SendID = new byte[10];
            string strtemp, strtemp1;
            string str1;
            int len, datanum = 0, IDnum = 0, newflag = 1;
            string strSendID =id.ToString();
            char[] m_strSendID = strSendID.ToCharArray();

            len = m_strSendID.Length;
            for (int i = 0; i < len; i++)
            {
                strtemp = m_strSendID[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {
                    newflag = 0;
                    strtemp = m_strSendID[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[IDnum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendID[i + 1].ToString();

                        if (strtemp1 == " ")
                            str5[IDnum] = "0" + strtemp;
                        else
                            str5[IDnum] = strtemp + strtemp1;
                    }

                    SendID[IDnum] = (byte)Str2Hex(str5[IDnum]);
                    IDnum++;
                    if (IDnum >= 4)
                        break;
                }
            }
            newflag = 1;

            string strSendData = TextBoxSend.Text.ToString();
            char[] m_strSendData = strSendData.ToCharArray();
            len = m_strSendData.Length;
            //m_strSendData.GetLength();
            for (int i = 0; i < len; i++)
            {

                strtemp = m_strSendData[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {

                    newflag = 0;
                    strtemp = m_strSendData[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[datanum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendData[i + 1].ToString();

                        if (strtemp1 == " ")
                        {
                            str5[datanum] = "0" + strtemp;

                        }
                        else
                            str5[datanum] = strtemp + strtemp1;

                    }
                    buf[datanum] = (byte)Str2Hex(str5[datanum]);
                    datanum++;
                    if (datanum >= 8)
                        break;

                }

            }
            byte m_nSendFrameType = 0;
            byte m_nSendFrameFormat = 0;
            byte m_radioIDFormat = 0;
            //////////
            /////CHANGED
            sendbuf.ExternFlag = 1;//m_nSendFrameType;//1
            sendbuf.DataLen = (byte)datanum;//8
            sendbuf.RemoteFlag = m_nSendFrameFormat;//0
            if (m_nSendFrameFormat == 1)//if remote frame, data is invalid
                for (int i = 0; i < datanum; i++)
                    buf[i] = 0;
            byte[] SendID2 = new byte[4];

            if ((sendbuf.ExternFlag) == 1)//Extend ID progress
            {
                if (m_radioIDFormat != 0)//将直接ID值进行转换到CAN2.0B格式
                {
                    SendID[0] &= 0x1F;
                    SendID2[0] = (byte)((SendID[0] << 3) | (SendID[1] >> 5));
                    SendID2[1] = (byte)((SendID[1] << 3) | (SendID[2] >> 5));
                    SendID2[2] = (byte)((SendID[2] << 3) | (SendID[3] >> 5));
                    SendID2[3] = (byte)(SendID[3] << 3);
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //	sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];
                }
                else
                {
                    SendID[3] = (byte)(SendID[3] & 0xF8);//the last 3 bits is invalid,clear to 0
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = SendID[i];
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }
            else//Basic CAN
            {

                if (m_radioIDFormat != 0)//Direct id value to SJA1000 format
                {
                    SendID[2] &= 0x07;
                    SendID2[0] = (byte)((SendID[2] << 5) | (SendID[3] >> 3));
                    SendID2[1] = (byte)(SendID[3] << 5);
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];

                }
                else
                {
                    SendID[1] = (byte)(SendID[1] & 0xE0);//the last 5 bits is invalid, clear to 0
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = SendID[i];
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }

            for (int i = 0; i < datanum; i++)
            {
                sendbuf.Data[i] = buf[i];
            }
            sendbuf.Data[loc]=(byte)data;
            int flag=0;
          //  while (flag != 1)
            //{
                flag = VCI_Transmit(2, 0, 0, ref sendbuf);//CAN DATA SEND
            //}
            PVCI_CAN_OBJ[] TempBuf = new PVCI_CAN_OBJ[50];
            TempBuf[0] = sendbuf;
            showCan(0, 0, TempBuf, 1);
            //DrawBar(TempBuf);
            //if (flag != 1)
            //{
            //    if (flag == -1)
            //        MessageBox.Show("fail,device not open\n");
            //    else if (flag == 0)
                   
            //    return;

            //}
            string str = "";
            for (int i = 0; i < sendbuf.DataLen; i++)			//DATA
            {
                uint sdata = sendbuf.Data[i];
                str1 = Convert.ToString(sdata, 16);
                str += str1 + " ";
            } 
        }
        private void transmitVariableCan(string id, int data1, int data2,int loc1,int loc2)
        {
            PVCI_CAN_OBJ sendbuf = new PVCI_CAN_OBJ();
            sendbuf.ID = new byte[4];
            sendbuf.Data = new byte[8];
            sendbuf.Reserved = new byte[3];
            string[] str5 = new string[32];
            byte[] buf = new byte[50];
            byte[] SendID = new byte[10];
            string strtemp, strtemp1;
            string str1;
            int len, datanum = 0, IDnum = 0, newflag = 1;
            string strSendID = id.ToString();
            char[] m_strSendID = strSendID.ToCharArray();

            len = m_strSendID.Length;
            for (int i = 0; i < len; i++)
            {
                strtemp = m_strSendID[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {
                    newflag = 0;
                    strtemp = m_strSendID[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[IDnum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendID[i + 1].ToString();

                        if (strtemp1 == " ")
                            str5[IDnum] = "0" + strtemp;
                        else
                            str5[IDnum] = strtemp + strtemp1;
                    }

                    SendID[IDnum] = (byte)Str2Hex(str5[IDnum]);
                    IDnum++;
                    if (IDnum >= 4)
                        break;
                }
            }
            newflag = 1;

            string strSendData = TextBoxSend.Text.ToString();
            char[] m_strSendData = strSendData.ToCharArray();
            len = m_strSendData.Length;
            //m_strSendData.GetLength();
            for (int i = 0; i < len; i++)
            {

                strtemp = m_strSendData[i].ToString();
                if (strtemp == " ")
                    newflag = 1;
                else if (newflag == 1)
                {

                    newflag = 0;
                    strtemp = m_strSendData[i].ToString();
                    if (i == (len - 1))
                    {
                        str5[datanum] = "0" + strtemp;

                    }
                    else
                    {
                        strtemp1 = m_strSendData[i + 1].ToString();

                        if (strtemp1 == " ")
                        {
                            str5[datanum] = "0" + strtemp;

                        }
                        else
                            str5[datanum] = strtemp + strtemp1;

                    }
                    buf[datanum] = (byte)Str2Hex(str5[datanum]);
                    datanum++;
                    if (datanum >= 8)
                        break;

                }

            }
            byte m_nSendFrameType = 0;
            byte m_nSendFrameFormat = 0;
            byte m_radioIDFormat = 0;
            //////////
            /////CHANGED
            sendbuf.ExternFlag = 1;//m_nSendFrameType;//1
            sendbuf.DataLen = (byte)datanum;//8
            sendbuf.RemoteFlag = m_nSendFrameFormat;//0
            if (m_nSendFrameFormat == 1)//if remote frame, data is invalid
                for (int i = 0; i < datanum; i++)
                    buf[i] = 0;
            byte[] SendID2 = new byte[4];

            if ((sendbuf.ExternFlag) == 1)//Extend ID progress
            {
                if (m_radioIDFormat != 0)//将直接ID值进行转换到CAN2.0B格式
                {
                    SendID[0] &= 0x1F;
                    SendID2[0] = (byte)((SendID[0] << 3) | (SendID[1] >> 5));
                    SendID2[1] = (byte)((SendID[1] << 3) | (SendID[2] >> 5));
                    SendID2[2] = (byte)((SendID[2] << 3) | (SendID[3] >> 5));
                    SendID2[3] = (byte)(SendID[3] << 3);
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //	sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];
                }
                else
                {
                    SendID[3] = (byte)(SendID[3] & 0xF8);//the last 3 bits is invalid,clear to 0
                    for (int i = 0; i < 4; i++)
                        sendbuf.ID[i] = SendID[i];
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }
            else//Basic CAN
            {

                if (m_radioIDFormat != 0)//Direct id value to SJA1000 format
                {
                    SendID[2] &= 0x07;
                    SendID2[0] = (byte)((SendID[2] << 5) | (SendID[3] >> 3));
                    SendID2[1] = (byte)(SendID[3] << 5);
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = SendID2[i];
                    //sendbuf->ID=(SendID2[0]<<24)+(SendID2[1]<<16)+(SendID2[2]<<8)+SendID2[3];

                }
                else
                {
                    SendID[1] = (byte)(SendID[1] & 0xE0);//the last 5 bits is invalid, clear to 0
                    for (int i = 0; i < 2; i++)
                        sendbuf.ID[i] = SendID[i];
                    //	sendbuf->ID=(SendID[0]<<24)+(SendID[1]<<16)+(SendID[2]<<8)+SendID[3];

                }
            }

            for (int i = 0; i < datanum; i++)
            {
                sendbuf.Data[i] = buf[i];
            }
            sendbuf.Data[loc1] = (byte)data1;
            sendbuf.Data[loc2] = (byte)data2;
            int flag = 0;
            //  while (flag != 1)
            //{
            flag = VCI_Transmit(2, 0, 0, ref sendbuf);//CAN DATA SEND
            //}
            PVCI_CAN_OBJ[] TempBuf = new PVCI_CAN_OBJ[50];
            TempBuf[0] = sendbuf;
            showCan(0, 0, TempBuf, 1);
            
            //if (flag != 1)
            //{
            //    if (flag == -1)
            //        MessageBox.Show("fail,device not open\n");
            //    else if (flag == 0)
                   
            //    return;

            //}
            string str = "";
            for (int i = 0; i < sendbuf.DataLen; i++)			//DATA
            {
                uint sdata = sendbuf.Data[i];
                str1 = Convert.ToString(sdata, 16);
                str += str1 + " ";
            }

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }



        Coor pollor_to_Descart(float dis, float angle) 
        {
            Coor tempCoor = new Coor();

            tempCoor.X = (int)(dis *(float)Math.Cos(angle));
            tempCoor.Y = (int)(dis * (float)Math.Sin(angle));
            return tempCoor;
        }


        public int ORG_X { get; set; }

        public int ORG_Y { get; set; }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox1.SelectedItem.ToString()=="250kps")
            {
                BAUD_RATE = 250;
                init_can(250, CAN_WORK_MODE);//zifazishou
            }
            else
            {
                if (comboBox1.SelectedItem.ToString()=="500kps")
                {
                    BAUD_RATE = 500;
                init_can(500, CAN_WORK_MODE);//zifazishou
                }
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox2.SelectedIndex==0)
            {
                CAN_WORK_MODE = 1;
                init_can(0, 1);//self listen
                
            }
            if (comboBox2.SelectedIndex==1)
            {
                CAN_WORK_MODE = 0;
                init_can(0, 0);
            }
        }

        public int CAN_WORK_MODE { get; set; }

        public int BAUD_RATE { get; set; }

        private void TextBoxSend_TextChanged(object sender, EventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            TransmitACanMsg("00 00 00 01", "00 00 00 00 00 00 f0 00");
        }

        private void Form1_Click(object sender, EventArgs e)
        {
            ORG_X=Control.MousePosition.X;
            ORG_Y = Control.MousePosition.Y;
            x_org_label.Text = ORG_X.ToString();
            y_org_label.Text = ORG_Y.ToString();
            DrawCoorodinates_with_pen(ORG_X, ORG_Y,new Pen(Color.Red));
            DrawAxis(ORG_X, ORG_Y, 500, Color.Blue);

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button7_Click(object sender, EventArgs e)
        {
            write_file("E:\\trail.txt","",file_txt);

        }

        public int x_map { get; set; }

        public int y_map { get; set; }

        private void button8_Click(object sender, EventArgs e)
        {
            Char[] value = (up_down_value.Text.ToString().Trim().ToCharArray());
            String vertikcal_step = "";
            vertikcal_step = value[0].ToString() + value[1].ToString();

            TransmitACanMsg("00 11 12 13", "00 " + vertikcal_step + " 00 00 00 00 00 00");
        }

        private void button9_Click(object sender, EventArgs e)
        {
            Char[] value = (up_down_value.Text.ToString().Trim().ToCharArray());
            String vertikcal_step = "";
            vertikcal_step = value[0].ToString() + value[1].ToString();

            TransmitACanMsg("00 11 12 13", "00 " + vertikcal_step + " 00 01 00 00 00 00");
        }

        private void button10_Click(object sender, EventArgs e)
        {
            String vertikcal_step = "";
            String num;
            String X_val;
            String Y_val;
            Char[] value = (POINT_NUM_TEXT.Text.ToString().Trim().ToCharArray());
            if (value.GetLength(0) > 1)
            {
                num = value[0].ToString() + value[1].ToString();
            }
            else 
            {
                num = "0"+value[0].ToString();
            }
            value = (MAP_X_TEXT.Text.ToString().Trim().ToCharArray());
            X_val = value[0].ToString() + value[1].ToString() + " " + value[2].ToString() + value[3].ToString();

            value = (MAP_Y_TEXT.Text.ToString().Trim().ToCharArray());
            Y_val = value[0].ToString() + value[1].ToString() + " " + value[2].ToString() + value[3].ToString();

            TransmitACanMsg("00 11 12 1b", X_val+" " + num+ " "+ Y_val+" "+num+" 00 00");
        }

        private void POINT_NUM_TEXT_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            TransmitACanMsg("00 00 00 01", "00 00 00 00 00 00 f0 00");
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            PathDisplayForm pdForm = new PathDisplayForm();
            pdForm.Show();
        }
    }
}