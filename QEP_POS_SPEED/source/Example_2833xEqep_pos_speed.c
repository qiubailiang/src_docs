// TI File $Revision: /main/11 $
// Checkin $Date: July 10, 2008   11:06:28 $
//###########################################################################
//
// FILE:	Example_2833xEqep_pos_speed_.c
//
// TITLE:	EQEP Speed and Position measurement
//
// ASSUMPTIONS:
//
//    This program requires the DSP2833x header files.
//
//    Test requires the following hardware connections from EPWM1 and
//    GPIO pins (simulating QEP sensor) to QEP peripheral
//
//       GPIO20/EQEP1A <- GPIO0/EPWM1A (simulates EQEP Phase A signal)
//       GPIO21/EQEP1B <- GPIO1/EPWM1B (simulates EQEP Phase B signal)
//       GPIO23/EQEP1I <- GPIO4 (simulates EQEP Index Signal)
//
//
//    As supplied, this project is configured for "boot to SARAM"
//    operation.  The 2833x Boot Mode table is shown below.
//    For information on configuring the boot mode of an eZdsp,
//    please refer to the documentation included with the eZdsp,
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// DESCRIPTION:
//
//    This test will provide position measurement,  speed measurement using the capture unit, and
//    speed measurement using unit time out. This example uses the IQMath library. It is used
//    merely to simplify high-precision calculations.
//
//    See DESCRIPTION in Example_posspeed.c for more details on the calculations
//    performed in this example.
//
//    In addition to this file, the following files must be included in this project:
//    Example_posspeed.c - includes all eQEP functions
//    Example_EPwmSetup.c - sets up EPWM1A and EPWM1B as simulated QA and QB encoder signals
//    Example_posspeed.h - includes initialization values for pos and speed structure
//
//    Notes:
//    * Maximum speed is configured to 6000rpm(BaseRpm)
//    * Minimum speed is assumed at 10rpm for capture pre-scalar selection
//    * Pole pair is configured to 2 (pole_pairs)
//    * QEP Encoder resolution is configured to 4000counts/revolution (mech_scaler)
//      which means: 4000/4 = 1000 line/revolution quadrature encoder (simulated by EPWM1)
//    * EPWM1 (simulating QEP encoder signals) is configured for 5kHz frequency or 300 rpm
//      (=4*5000 cnts/sec * 60 sec/min)/4000 cnts/rev)
//    * 300 rpm EPWM1 speed will be measured by EQEP.
//
// 	  SPEEDRPM_FR: High Speed Measurement is obtained by counting the QEP input pulses
//              for 10ms (unit timer set to 100Hz).
//
//    SPEEDRPM_FR = { (Position Delta)/10ms } * 60 rpm
//
//
//    SPEEDRPM_PR: Low Speed Measurement is obtained by measuring time period of QEP edges.
//              Time measurement is averaged over 64edges for better results and
//              capture unit performs the time measurement using pre-scaled SYSCLK
//
//              Note that pre-scaler for capture unit clock is selected such that
//              capture timer does not overflow at the required minimum RPM speed
//
//          Watch Variables: qep_posspeed.SpeedRpm_fr - Speed meas. in rpm using QEP position counter
//                           qep_posspeed.SpeedRpm_pr - Speed meas. in rpm using capture unit
//                           qep_posspeed.theta_mech - Motor mechanical angle (Q15)
//                           qep_posspeed.theta_elec - Motor electrical angle (Q15)
//
//###########################################################################
// Original Author S.D.
//
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Example_posspeed.h"   // Example specific Include file
#include "math.h"
//#include "Example_EPwmSetup.c"

#define DIR GpioDataRegs.GPBDAT.bit.GPIO48
#define DIR1 GpioDataRegs.GPBDAT.bit.GPIO52
#define Pwm2_Gpio GpioDataRegs.GPADAT.bit.GPIO1
 

#define FINISH 0
#define NOTFINISH 1
#define WidthOfScreen 100
#define K 10000
#define AutoModeOFF 0
#define AutoModeON 1
#define FALSE 0
#define TRUE 1
#define TotalLoopCount 3200000

Uint16    *ExRamStart = (Uint16 *)0x100000;

void initEpwm();
void Driver1(int D,Uint32 Deg);


interrupt void prdTick(void);
interrupt void cpu_timer2_isr(void);
interrupt  void EPWM1_int(void);
interrupt  void EPWM2_int(void);
POSSPEED qep_posspeed=POSSPEED_DEFAULTS;
POSSPEED1 qep_posspeed1=POSSPEED1_DEFAULTS;


long qposcntx=0,qposcnty=0;
Uint32 Receive_PWM=400;
Uint32 Interrupt_Count = 0,flag=0,flag1=0;
Uint16 i,CAN_RxBuffer[10]={0};
Uint32 nn=0,dir=0,dir1=0;
Uint32 StartFlg=1,EndFlg1=0,EndFlg2=0;
int   D1=1;
Uint32 N=0;
Uint32 N2=0;
Uint32 code_d,code_r=0,code_r1=0,code,code_flag;
Uint32 codebuffer[1]={0}; // array for coding
Uint16 PRD=65000/2;
struct ECAN_REGS ECanaShadow;

void initEpwm1A();
void initEpwm1B();
void initEpwm2A();
void initGpio_pwm();

long angle=0;  //horizatal degree counter 
long angle_sendout=0;

long angleY=0;  //vertical degree counter

long distance;
long polar_angle_count;
long scale=5;
long scaleY=2;
long midY=0;
long swing_speed=0;
long swing_speed_y=0;
float Arc2Degree(float arc);
float GetCountFromDegree(float deg);
float GetDegreeFromCount(long cnt);
void Driver2(int D,Uint32 Deg);
void DriveWithDir(float dircount);
typedef struct Coor{
 float x;
 float y;
 float z;
 }Coor;
Coor Get_Position(float a,float d);
Coor get_next_point_on_trace(struct Coor map [],int length);

float get_angle(Coor Current_Pos,Coor Next_Map_Pos,float Step);
float calulate_from_edges(float a,float b,float c);
int x_bias=0;
int y_bias=0;

int  x_bias_dir;
int y_bias_dir;

void drive(float degree);
void driveY(float degree);
void scan();//x scaning
void scanY();//y scaning
int TargetInWorkingZone(Coor c);

int PwmOneStepFinishFlag=0;
int Pwm2OneStepFinishFlag=0;
int FinishStepFlag2=FINISH;
int FinishStepFlag=FINISH;// 0 =finish ;1= not finish
float stepcount=0;//used to identify when to change direction
int stepLength=1;
int pwm2=0;
float DegreeOfSwing=200;
int distance_valid_flag=FALSE;

int dir_flag_for_guidence;
int dir1_flag_for_guidence;
Coor current_pos;
Coor next_map_pos;
struct Coor Map [10]; 
float baseX=50;
float baseY=23000;

float initAngle2Y=0;///

float Hx=50;
float Hy=23000;


float walkstep=1;

int shouldTurnOffFlag=FALSE;
int isFirstScan=TRUE;
int AutoMode=AutoModeON;
void main(void)
{ 
	
	int i=0;
	for(i=0;i<10;i++)
	{
			Map[i].x=300;
			//Map[i].y=-800+1600/9*i;
			Map[i].y=-200*i;
	}
	for(i=0;i<10;i=i+2)
	{
			//Map[i].x=36000-i*3600;
			//Map[i].y=-800+1600/9*i;
			Map[i].x=+200;
	}
	//Map[9].x=2410;
	//Map[9].y=500;
	
   InitSysCtrl();
   InitECan();
   
   InitEQep1Gpio();
   POSSPEED_Init();
   
   InitXintf16Gpio();	
   InitECanGpio();
   InitEPwm1Gpio();
   DINT;
  
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();                  
 
   EALLOW;  
   PieVectTable.TINT2 = &cpu_timer2_isr;
   PieVectTable.EQEP1_INT= &prdTick;
   PieVectTable.EPWM1_INT=&EPWM1_int;
   PieVectTable.EPWM2_INT=&EPWM2_int;
   EDIS;    // This is needed to disable write to EALLOW protected registers
  
   initEpwm1A();  // This function exists in Example_EPwmSetup.c
   initEpwm2A();
   InitCpuTimers();   // For this example, only initialize the Cpu Timers
   ConfigCpuTimer(&CpuTimer2, 150, 100000);//sample period 100ms
   IER |= M_INT1;
   IER |= M_INT3;
   IER |= M_INT6;
   IER |= M_INT13;
   IER |= M_INT14;

//   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;//EPWM1 INT
   PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
   PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
   
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;//enable epwm1
   PieCtrlRegs.PIEIER3.bit.INTx2 = 1;//enable epwm2  
   PieCtrlRegs.PIEIER3.bit.INTx3 = 1;//enable epwm3
   
   PieCtrlRegs.PIEIER1.bit.INTx3 = 1;
   PieCtrlRegs.PIEIER6.bit.INTx1 =1;
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM
    /******************************* receive ********************************************/
     ECanaRegs.CANME.bit.ME0 = 0;//使能邮箱0
     ECanaRegs.CANME.bit.ME1 = 0;//使能邮箱1
     ECanaRegs.CANME.bit.ME2 = 0;//使能邮箱2
     ECanaRegs.CANME.bit.ME3 = 0;//enable mailbox 3 for reciving msg from other base for target location infor 

    // Mailboxs can be written to 16-bits or 32-bits at a time
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_L =0x1213 ;//扩展帧ID：0x00111213
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_H=0x01;
    ECanaMboxes.MBOX0.MSGID.bit.STDMSGID=0x04;
	ECanaMboxes.MBOX0.MSGID.bit.IDE=1;//扩展帧，如为0
	ECanaMboxes.MBOX0.MSGID.bit.AME=0;//屏蔽位

	 ECanaMboxes.MBOX1.MSGID.bit.EXTMSGID_L =0x1214;//扩展帧ID：0x00111214
     ECanaMboxes.MBOX1.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX1.MSGID.bit.STDMSGID=0x04;//04
     ECanaMboxes.MBOX1.MSGID.bit.IDE=1;//扩展帧，如为0
	 ECanaMboxes.MBOX1.MSGID.bit.AME=0;//屏蔽位
	 
    
     ECanaMboxes.MBOX2.MSGID.bit.EXTMSGID_L =0x1215;//扩展帧ID：0x00111215
     ECanaMboxes.MBOX2.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX2.MSGID.bit.STDMSGID=0x04;//04
     ECanaMboxes.MBOX2.MSGID.bit.IDE=1;//扩展帧，如为0
	 ECanaMboxes.MBOX2.MSGID.bit.AME=0;//屏蔽位

	   ECanaMboxes.MBOX3.MSGID.bit.EXTMSGID_L =0x1216;//扩展帧ID：0x00111216
     ECanaMboxes.MBOX3.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX3.MSGID.bit.STDMSGID=0x04;//04
     ECanaMboxes.MBOX3.MSGID.bit.IDE=1;//扩展帧，如为0
	 ECanaMboxes.MBOX3.MSGID.bit.AME=0;//屏蔽位
	 
     ECanaRegs.CANMD.bit.MD0 = 1; //邮箱0设置为接收
     ECanaRegs.CANMD.bit.MD1 = 1; //邮箱1设置为接收
     ECanaRegs.CANMD.bit.MD2 = 1; //邮箱2设置为接收
     ECanaRegs.CANMD.bit.MD3 = 1; //Mail box3 set as recive box
     
     ECanaRegs.CANME.bit.ME0 = 1;//使能邮箱0
     ECanaRegs.CANME.bit.ME1 = 1;//使能邮箱2
     ECanaRegs.CANME.bit.ME2 = 1;//使能邮箱2
     ECanaRegs.CANME.bit.ME3 = 1;//使能邮箱3
     
     // Specify that 8 bits will be sent/received
     ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
     ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
   	 ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;	
   	 ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;	
   	 
   	 
   	 
/******************************* SEND limitx 30********************************************/
/* Write to the MSGID field */
  ECanaMboxes.MBOX26.MSGID.bit.EXTMSGID_L =0x1219;//扩展帧ID：0x00110000
     ECanaMboxes.MBOX26.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX26.MSGID.bit.STDMSGID=0x04;
     ECanaMboxes.MBOX26.MSGID.bit.IDE=1;
/* Configure Mailbox under test as a Transmit mailbox */
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD26 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
/* Enable Mailbox under test */
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME26 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
/* Write to DLC field in Master Control reg */
   ECanaMboxes.MBOX26.MSGCTRL.bit.DLC = 8;
////////////////////////////
     //Driver1(1,4);
      //	DriveWithDir(stepcount);
   initGpio_pwm();
  while(1)
   {  
   		//float testa=calulate_from_edges((float)3,(float)4,(float)5);
   	 	ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
   	if (ECanaShadow.CANRMP.bit.RMP0 == 1)//D0  receieve 111213
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP0 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	            CAN_RxBuffer[0]=ECanaMboxes.MBOX0.MDL.byte.BYTE0;
	            CAN_RxBuffer[1]=ECanaMboxes.MBOX0.MDL.byte.BYTE1;
	            CAN_RxBuffer[2]=ECanaMboxes.MBOX0.MDL.byte.BYTE2;
	            CAN_RxBuffer[3]=ECanaMboxes.MBOX0.MDL.byte.BYTE3;
				CAN_RxBuffer[4]=ECanaMboxes.MBOX0.MDH.byte.BYTE4;
				CAN_RxBuffer[5]=ECanaMboxes.MBOX0.MDH.byte.BYTE5;
				CAN_RxBuffer[6]=ECanaMboxes.MBOX0.MDH.byte.BYTE6;
				CAN_RxBuffer[7]=ECanaMboxes.MBOX0.MDH.byte.BYTE7;
			
			long canplusecount=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000个脉冲电机转动360°
			long canplusecount1=CAN_RxBuffer[4]*65536+CAN_RxBuffer[5]*256+CAN_RxBuffer[6];// 1000000个脉冲电机转动360°
           int dir=CAN_RxBuffer[3];
           int dir1=CAN_RxBuffer[7];
            if(canplusecount==0x1234)/////switch work mode
            {
           		AutoMode=AutoModeON;
            }
            else{
            	////////mannual mode
	           
	            	AutoMode=AutoModeOFF;
	            
	            
	            Driver1(dir,canplusecount);
	            
	            N2=canplusecount1;
	            Driver2(dir1,canplusecount1);
	            
            
            }
             
            //Dirver (2*canplusecount,dir,F[7]);
            //Dirver1 (2*canplusecount1,dir1,F[7]);
		 }
		 
		 if (ECanaShadow.CANRMP.bit.RMP1 == 1)//D0  receieve 111214 //from distance detector
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP1 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	       CAN_RxBuffer[0]=ECanaMboxes.MBOX1.MDL.byte.BYTE0;
	       CAN_RxBuffer[1]=ECanaMboxes.MBOX1.MDL.byte.BYTE1;
	       CAN_RxBuffer[2]=ECanaMboxes.MBOX1.MDL.byte.BYTE2;
		 // distance=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000个脉冲电机转动360°
	       CAN_RxBuffer[3]=ECanaMboxes.MBOX1.MDL.byte.BYTE3;
		   CAN_RxBuffer[4]=ECanaMboxes.MBOX1.MDH.byte.BYTE4;
		   CAN_RxBuffer[5]=ECanaMboxes.MBOX1.MDH.byte.BYTE5;
		   CAN_RxBuffer[6]=ECanaMboxes.MBOX1.MDH.byte.BYTE6;
				if(CAN_RxBuffer[5]==1)
				{
					
					distance=CAN_RxBuffer[0]*10000+CAN_RxBuffer[1]*1000+CAN_RxBuffer[2]*100+CAN_RxBuffer[3]*10+CAN_RxBuffer[4];// 9440000个脉冲电机转动360°
					swing_speed = ((float)distance/(float)(PRD/4+distance))*PRD  ;  //CHANGE THE SWINGING VELOCITY
					EPwm1Regs.TBPRD=swing_speed;
				
				}
				//CAN_RxBuffer[7]=ECanaMboxes.MBOX1.MDH.byte.BYTE7;
			 	
          
      	 
		 }
		 
		  if (ECanaShadow.CANRMP.bit.RMP2 == 1)//D0  receieve 111215 
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP2 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	            CAN_RxBuffer[0]=ECanaMboxes.MBOX2.MDL.byte.BYTE0;
	            CAN_RxBuffer[1]=ECanaMboxes.MBOX2.MDL.byte.BYTE1;
	            CAN_RxBuffer[2]=ECanaMboxes.MBOX2.MDL.byte.BYTE2;
		
	            CAN_RxBuffer[3]=ECanaMboxes.MBOX2.MDL.byte.BYTE3;
				CAN_RxBuffer[4]=ECanaMboxes.MBOX2.MDH.byte.BYTE4;
				CAN_RxBuffer[5]=ECanaMboxes.MBOX2.MDH.byte.BYTE5;
				CAN_RxBuffer[6]=ECanaMboxes.MBOX2.MDH.byte.BYTE6;
				CAN_RxBuffer[7]=ECanaMboxes.MBOX2.MDH.byte.BYTE7;
				
				distance_valid_flag  =(CAN_RxBuffer[1]>>7);
				x_bias_dir=CAN_RxBuffer[1]&0x40;
				y_bias_dir=CAN_RxBuffer[1]&0x20;
				
				x_bias=CAN_RxBuffer[7];////
				y_bias=CAN_RxBuffer[1]&(0x1f);/////0001 1111
				
	          if(distance_valid_flag==1)/////distance valid then do the guidence stuff
	            {
	           		distance_valid_flag=TRUE;
	           		
			////////////////////////////
			///////send out can msg
			
					ECanaMboxes.MBOX26.MDL.all = 0;
				    ECanaMboxes.MBOX26.MDH.all = 0;
				    
				    ECanaMboxes.MBOX26.MDL.byte.BYTE2=distance;
				    ECanaMboxes.MBOX26.MDL.byte.BYTE1=distance>>8;
				    ECanaMboxes.MBOX26.MDL.byte.BYTE0=distance>>16;
				    
				    if(angle<0)
				    {
				   		angle_sendout=angle+TotalLoopCount;
					
				    }
				    else
				    {
				    	angle_sendout=angle;
				    }
				    
					ECanaMboxes.MBOX26.MDH.byte.BYTE5=angle_sendout;
					ECanaMboxes.MBOX26.MDH.byte.BYTE4=angle_sendout>>8;
					ECanaMboxes.MBOX26.MDL.byte.BYTE3=angle_sendout>>16;
					
					ECanaMboxes.MBOX26.MDH.byte.BYTE6=0xf1;
					ECanaMboxes.MBOX26.MDH.byte.BYTE7=0x1f;   
				    ECanaShadow.CANTRS.all = 0;
				    ECanaShadow.CANTRS.bit.TRS26 = 1; // Set TRS for mailbox under test
				    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
				    do // Send 00110000
				    {
				      ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
				    } while(ECanaShadow.CANTA.bit.TA26 == 0 );
				    ECanaShadow.CANTA.all = 0;
				    ECanaShadow.CANTA.bit.TA26 = 1; // Clear TA5
				    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
			/////////////////
			////////////////          		
	           		
	           		
	           		
	            }
	            else    /////distance not valid
	            {
	            	distance_valid_flag=FALSE;
	            	//x_bais_dir=CAN_RxBuffer[1]&0x40;
					//y_bais_dir=CAN_RxBuffer[1]&0x20;
	            	if(x_bias_dir!=0)////lost  on the left should turn right
	            	{
	            		dir_flag_for_guidence=1;/////
	            		
	            	}
	            	else
	            	{
		            	
		            		dir_flag_for_guidence=0;/////
		            		
		            	
	            	}
	            	if(y_bias_dir!=0)////lost  on the top should turn down
	            	{
	            		dir1_flag_for_guidence=1;/////
	            		
	            	}
	            	else
	            	{
		            	
		           		dir1_flag_for_guidence=0;/////
		            		
	            	}
	            	
	            	
	           	}
          

		 }
		 
		  if (ECanaShadow.CANRMP.bit.RMP3 == 1)//D0  receieve 111216 //from other base--the target infor
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP3 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	            CAN_RxBuffer[0]=ECanaMboxes.MBOX3.MDL.byte.BYTE0;
	            CAN_RxBuffer[1]=ECanaMboxes.MBOX3.MDL.byte.BYTE1;
	            CAN_RxBuffer[2]=ECanaMboxes.MBOX3.MDL.byte.BYTE2;
	            
		 // distance=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000个脉冲电机转动360°
	            CAN_RxBuffer[3]=ECanaMboxes.MBOX3.MDL.byte.BYTE3;
				CAN_RxBuffer[4]=ECanaMboxes.MBOX3.MDH.byte.BYTE4;
			 	//distance=CAN_RxBuffer[0]*10000+CAN_RxBuffer[1]*1000+CAN_RxBuffer[2]*100+CAN_RxBuffer[3]*10+CAN_RxBuffer[4];// 9440000个脉冲电机转动360°
				CAN_RxBuffer[5]=ECanaMboxes.MBOX3.MDH.byte.BYTE5;
				CAN_RxBuffer[6]=ECanaMboxes.MBOX3.MDH.byte.BYTE6;
				CAN_RxBuffer[7]=ECanaMboxes.MBOX3.MDH.byte.BYTE7;
				if(CAN_RxBuffer[6]>0)
				{
					current_pos.x=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];
				}
				else
				{
					current_pos.x=-(CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2]);
				}
				if(CAN_RxBuffer[7]>0)
				{
					current_pos.y=CAN_RxBuffer[3]*65536+CAN_RxBuffer[4]*256+CAN_RxBuffer[5];
				}
				else
				{
					current_pos.y=-(CAN_RxBuffer[3]*65536+CAN_RxBuffer[4]*256+CAN_RxBuffer[5]);
				}
			 	shouldTurnOffFlag=TRUE;
          
      	 
		 }
		 if(shouldTurnOffFlag==TRUE)
		 {
		 //tell its own laser to turn off
		 }
		 else
		 {
		 	
		 }
		 
		// angle=((long)EQep1Regs.QPOSCNT)*360/(4*TotalLoopCount);
		 
	
		 
   	if(AutoMode==AutoModeON&&shouldTurnOffFlag==FALSE)//automode is on AND no base has capture the target
   	{
   		 
   	 	scan();
   	 	if(isFirstScan==TRUE)
   	 	{
	   	 	scanY();
   	 	}
   	 	if(distance_valid_flag==TRUE)//if the distance is valid, the target is locked on
	  	{
	  		AutoMode=AutoModeOFF;
	 		current_pos=Get_Position(GetDegreeFromCount(angle),distance);//translate the pol coordinate to rectangular coordinate
	 		next_map_pos=get_next_point_on_trace(Map,10);//search which is the next point on the map
			  //first get angle ,get ho many angles should turn;
			  //then drive 
			drive(get_angle(current_pos,next_map_pos,walkstep));
			
		 }
		  else{
		  		
		  		
			 //scan();//if failed to capture the target,then scan for it 
		  		}

   	}
   	else
   		if(distance_valid_flag==TRUE&&shouldTurnOffFlag==FALSE)//if the distance is valid,and no other base capture, the target is locked on
	  	{
	  		isFirstScan=FALSE;
	  		//AutoMode=AutoModeOFF;
	 		current_pos=Get_Position(GetDegreeFromCount(angle),distance);//translate the pol coordinate to rectangular coordinate
	 		next_map_pos=get_next_point_on_trace(Map,10);//search which is the next point on the map
			//first get angle ,get ho many angles should turn;
		    //then drive 
			if(x_bias>0x3f)
			{
			swing_speed=0;
			}
			drive(get_angle(current_pos,next_map_pos,walkstep));
			float turning = ((float)y_bias)/1000;
			turning=180*turning/3.14159;
			if(y_bias<=2){
				swing_speed_y=0;
			}else{
			//swing_speed_y=((float)y_bias/(float)(PRD/4+y_bias))*PRD;
			swing_speed_y=PRD*((float)y_bias/18);
			
			}
			EPwm2Regs.TBPRD=swing_speed_y;
			EPwm2Regs.CMPA.half.CMPA=swing_speed_y/20;
			if(y_bias_dir==0)
			{
				
				Driver2(0x01,1);
			}
			else
			{
		
				Driver2(0x00,1);
				
			}
			midY=angleY;
		  }
		  else if((distance_valid_flag==TRUE&&shouldTurnOffFlag==TRUE))
		  {
		  			scan();///ACTUALLY should follow
		  			
		  }
		  else //////distance not valid 
		  {
		  	shouldTurnOffFlag=FALSE;
		  	if(dir_flag_for_guidence==0)
		  	{
		  		dir=1;
		  	}
		  	else
		  	{
			  	if(dir_flag_for_guidence==1)
			  	{
			  		dir=0;
			  	}
		  	
		  	}/////
		  	if(dir1_flag_for_guidence==0)
		  	{
		  		dir=1;
		  	}
		  	else
		  	{
			  	if(dir1_flag_for_guidence==1)
			  	{
			  		dir=0;
			  	}
		  	
		  	}/////
		  	
		  		AutoMode=AutoModeON;
			 //scan();//if failed to capture the target,then scan for it 
		  }
	  
	  
	  
	  
   }
}
int get_nearest_point(struct Coor map [],int length)//the current position we refferenced here is a global variable
{
	int i;
	float temp;
	float tempdistance=(map[0].x-current_pos.x)*(map[0].x-current_pos.x)+(map[0].y-current_pos.y)*(map[0].y-current_pos.y);
	int pointer_to_return;
	for(i=0;i<length;i++)
	{
		temp=(map[i].x-current_pos.x)*(map[i].x-current_pos.x)+(map[i].y-current_pos.y)*(map[i].y-current_pos.y);
		if(temp<=tempdistance)
		{
			pointer_to_return=i;
			tempdistance=temp;
		}	
	}
	
	return pointer_to_return;
}
Coor get_next_point_on_trace(struct Coor map [],int length)
{ 
	//int i;
	int temp_pointer=get_nearest_point(map,length);
	float dis1=(map[temp_pointer].x-baseX)*(map[temp_pointer].x-baseX)+(map[temp_pointer].y-baseY)*(map[temp_pointer].y-baseY);
	float dis2=(current_pos.x-baseX)*(current_pos.x-baseX)+(current_pos.y-baseY)*(current_pos.y-baseY);
	if(dis1>=dis2&&temp_pointer<length-1)
	{
		temp_pointer=temp_pointer+1;
	}else
	{
	
	}
    return map[temp_pointer];
}

float GetCountFromDegree(float deg)
{
	return K*deg;
}
void initGpio_pwm()  ////CONFIG THE OUTPUT FOR PWM2
{
   	EALLOW;

	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0; //config as digital io

	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1; //CONFIG AS OUTPUT
	
	EDIS;
}
void DriveWithDir(float dircount)
{
	float Amplify=1;
	float d=Amplify*sin(dircount/180*3.14);
	if(d>0)
	{
		Driver1(0,stepLength);
	}
	else
	{
		Driver1(1,stepLength);
	}


}   

void Driver1(int D,Uint32 Deg)//D stand for direction Deg stands for  degrees to rotate 
{
	//int i=0;
	
	N=Deg;
	EALLOW;
	DIR=D;
	EDIS;
	EPwm1Regs.CMPA.half.CMPA =(swing_speed/2);

}
void Driver2(int D,Uint32 Deg)//D stand for direction Deg stands for  degrees to rotate 
{
	//int i=0;
	
	N2=Deg;
	EALLOW;
	DIR1=D;
	EDIS;
	EPwm2Regs.CMPA.half.CMPA =swing_speed_y/2;

}


interrupt void cpu_timer2_isr(void)
{
   
	CpuTimer2.InterruptCount++;

    CpuTimer2Regs.TCR.bit.TIF=1;
    CpuTimer2Regs.TCR.bit.TRB=1;

}

interrupt void prdTick(void)                  // EPWM1 Interrupts once every 4 QCLK counts (one period)
{  Uint16 i;
   // Position and Speed measurement
   qep_posspeed.calc(&qep_posspeed);
   qep_posspeed1.calc(&qep_posspeed1);
   // Control loop code for position control & Speed contol
   Interrupt_Count++;
   if (Interrupt_Count==10000)                 // Every 1000 interrupts(4000 QCLK counts or 1 rev.)
   {       EALLOW;
       GpioDataRegs.GPASET.bit.GPIO4 = 1;     // Pulse Index signal  (1 pulse/rev.)
       for (i=0; i<700; i++);
       GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
	   Interrupt_Count = 0;                   // Reset count
	   EDIS;
   }
   

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
   EPwm1Regs.ETCLR.bit.INT=1;
}

interrupt  void EPWM1_int(void)
{
	
	EPwm1Regs.ETCLR.bit.INT=1;///////////////////////////////////
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;////the two statments are used to clear the flags of interrupt
	if(EPwm1Regs.CMPA.half.CMPA !=0)
	{
		FinishStepFlag=NOTFINISH;
		PwmOneStepFinishFlag++;
		if(dir==0)
			{
				angle--;	
			}
			else
			{
				if(dir==1)
				{
					angle++;
				}
			}
	}
	
	
	if(PwmOneStepFinishFlag>N)
	{
	PwmOneStepFinishFlag=0;	
	FinishStepFlag=FINISH;
	  EPwm1Regs.CMPA.half.CMPA =0; 
	}
}

interrupt  void EPWM2_int(void)
{
	
	EPwm2Regs.ETCLR.bit.INT=1;///////////////////////////////////
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;////the two statments are used to clear the flags of interrupt
	if(EPwm2Regs.CMPA.half.CMPA !=0)
	{
		
		//PwmOneStepFinishFlag++;
		if(dir1==0)
			{
				angleY--;	
			}
			else
			{
				if(dir1==1)
				{
					angleY++;
				}
			}
	}
	if(Pwm2OneStepFinishFlag>=N2)
	{
	Pwm2OneStepFinishFlag=0;	
	FinishStepFlag2=FINISH;
	EPwm2Regs.CMPA.half.CMPA =0; 
	N2=0;
	}
	else
	{
	if( EPwm2Regs.CMPA.half.CMPA!=0)
	{
		GpioDataRegs.GPATOGGLE.bit.GPIO1=1;
		Pwm2OneStepFinishFlag++;
		FinishStepFlag2=NOTFINISH;
	}
	}
}

Coor Get_Position(float a,float d)//a is in degrees 
{
	Coor tempCoor;
	a=(float)a;
	d=(float)d;
	tempCoor.x=d*sin(initAngle2Y+a/((float)180)*3.141593)+Hx;
	tempCoor.y=d*cos(initAngle2Y+a/((float)180)*3.141593)+Hy;
	return tempCoor;
}
float calulate_from_edges(float a,float b,float c)
{
	float temp=acos((a*a+b*b-c*c)/(2*a*b));
	return temp;
}
float get_angle(Coor Current_Pos,Coor Next_Map_Pos,float Step)////return in arcs
{
	float a,b,c;
	float angle_B;
	a=sqrt((Current_Pos.x-baseX)*(Current_Pos.x-baseX)+ (Current_Pos.y-baseY)*(Current_Pos.y-baseY));
	b=sqrt((Next_Map_Pos.x-baseX)*(Next_Map_Pos.x-baseX)+ (Next_Map_Pos.y-baseY)*(Next_Map_Pos.y-baseY));
	c=sqrt((Next_Map_Pos.x-Current_Pos.x)*(Next_Map_Pos.x-Current_Pos.x)+ (Next_Map_Pos.y-Current_Pos.y)*(Next_Map_Pos.y-Current_Pos.y));
	angle_B=calulate_from_edges(a,c,b);
	float step_to_base=sqrt(a*a+Step*Step-2*a*Step*cos(angle_B));
	// Current_Pos.x Current_Pos.x
	float angle_to_rotate=acos((step_to_base*step_to_base+a*a-Step*Step)/(2*a*step_to_base));
	///////////////////////////////////////////////
	if(Next_Map_Pos.x<Current_Pos.x)/////here should be differet because target goes alone y,shouls compare x
	{
		dir=1;///turn right
	}
	else{
	dir=0;
	
	}
	return angle_to_rotate;
}
void drive(float degree)//the degree passed in should be in arcs 
{
	float degree_in_degrees=degree/3.14*180;
	long degree_count=degree_in_degrees/((float)360)*TotalLoopCount;
	Driver1(dir,degree_count);

}
void driveY(float degree)//the degree passed in should be in arcs 
{
	float degree_in_degrees=degree/3.14*180;
	long degree_count=degree_in_degrees/((float)360)*TotalLoopCount;
	Driver2(dir1,degree_count);

}
void scan()
{
		if(GetDegreeFromCount(angle)>scale)
   	 	{
   	 		dir=0;
   	 		
   	 	} 
   	 	else
   	 	{
   	 		if(GetDegreeFromCount(angle)<-scale)
   	 		{
	   	 		dir=1;
	   	 		
   	 		}
   	 	}
   	 	drive(3.14/180);
}
void scanY()
{
	
		if(GetDegreeFromCount(angleY-midY)>scaleY)
   	 	{
   	 		dir1=0;
   	 		
   	 	} 
   	 	else
   	 	{
   	 		if(GetDegreeFromCount(angleY-midY)<-scaleY)
   	 		{
	   	 		dir1=1;
	   	 		
   	 		}
   	 	}
   	 	EPwm2Regs.TBPRD = PRD/5; 
   	 	swing_speed_y=PRD/10;
   	 	
   	 	driveY(1);
}
float Arc2Degree(float arc)
{

return arc/3.1415927*180;
}
float GetDegreeFromCount(long cnt)
{
return (float)cnt/((float)TotalLoopCount)*360;
}
int TargetInWorkingZone(Coor c)
{
	float tX,tY;
	tX=c.x;
	tY=c.y;
	
	if(tX>10||tY>0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}
void follow(float targetX,float targetY)
{
    float vtX =targetX-baseX;
    float vtY =targetY-baseY;
    float aim_angle_degree=180*(atan(vtX/vtY))/3.14159;
    if(initAngle2Y+GetDegreeFromCount(angle)>aim_angle_degree)
    {
	    dir=1;

    }
    else
    {
    	dir=0;
    }
    
    drive(1);
    
}

