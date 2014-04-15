using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace USBCAN
{
    public partial class Form1 : Form
    {
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

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
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
                config[0].Timing0 = 0x00;
                config[0].Timing1 = 0x14;
                config[0].CanRx_IER = 1;
                config[0].Mode = 1;//1~自发自收模式，0~正常工作模式

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

        private void buttonSend_Click(object sender, EventArgs e)
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
            sendbuf.ExternFlag = m_nSendFrameType;//1
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
                sendbuf.Data[i] = buf[i];


            int flag;

            flag = VCI_Transmit(2, 0, 0, ref sendbuf);//CAN DATA SEND
            PVCI_CAN_OBJ[] TempBuf = new PVCI_CAN_OBJ[50];
            TempBuf[0] = sendbuf;
            showCan(0, 0, TempBuf, 1);
            if (flag != 1)
            {
                if (flag == -1)
                    MessageBox.Show("fail,device not open\n");
                else if (flag == 0)
                    MessageBox.Show("fail\n");
                return;

            }
            string str = "";
            for (int i = 0; i < sendbuf.DataLen; i++)			//DATA
            {
                uint data = sendbuf.Data[i];
                str1 = Convert.ToString(data, 16);
                str += str1 + " ";

            }
        //    listView1.Items.Add(str).SubItems.Add("Send");
        }
        
        private void timerRecieve_Tick(object sender, EventArgs e)
        {
            PVCI_CAN_OBJ[] sendbuf = new PVCI_CAN_OBJ[200];
            int NumValue = 0;
            NumValue = VCI_Receive(2, 0, 0, sendbuf);
            showCan(0, 1, sendbuf, NumValue);
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
                string strID0 = Convert.ToString((buf[i]).ID[0]);
                string strID1 = Convert.ToString((buf[i]).ID[1]);
                string strID2 = Convert.ToString((buf[i]).ID[2]);
                string strID3 = Convert.ToString((buf[i]).ID[3]);
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
        }
     }
}