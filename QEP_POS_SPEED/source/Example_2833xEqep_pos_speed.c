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
#define ScanMode 1
#define AdjustMode 2
#define FALSE 0
#define TRUE 1
#define baseIndex 0
#define TotalLoopCountH 1152000
#define TotalLoopCountV 400000
#define MapPointCountCount 10
#define x_bais_thr 0x82
#define scan_speed 3500
#define guidance_speed_low 2500
#define guidance_speed PRD/14
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
long scale=7;
long scaleY=10;


long swing_speed=0;
long swing_speed_y=0;
float Arc2Degree(float arc);
float GetCountFromDegree(float deg);
float GetDegreeFromCount(long cnt);
float GetDegreeFromCountY(long cnt);
void Driver2(int D,Uint32 Deg);
void DriveWithDir(float dircount);
typedef struct Coor{
 float x;
 float y;
 float z;
 }Coor;
Coor Get_Position(float a,float d,float angCos);
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
void follow(float targetX,float targetY);

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
float baseX=-2659;
float baseY=0;
float baseZ=630;

float initAngle2Y=0;///


float walkstep=1;

long midY=0;
int shouldTurnOffFlag=FALSE;
int isFirstScan=TRUE;
int AutoMode=ScanMode;
int  OutOfWork=FALSE;
Uint16 MapState;
void main(void)
{ 
	
	int i=0;
	for(i=0;i<10;i++)
	{
			Map[i].x=36000-i*3600;
			//Map[i].y=-800+1600/9*i;
			Map[i].y=400;
	}
	for(i=0;i<10;i=i+2)
	{
			//Map[i].x=36000-i*3600;
			//Map[i].y=-800+1600/9*i;
		//	Map[i].y=-200;
	}
	Map[6].x=5000;
	Map[6].y=300;
	Map[7].x=2500;
	Map[7].y=300;
	
	Map[8].x=900;
	Map[8].y=-2900;
	
	Map[9].x=0;
	Map[9].y=-3000;
	
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
     ECanaRegs.CANME.bit.ME0 = 0;//ʹ������0
     ECanaRegs.CANME.bit.ME1 = 0;//ʹ������1
     ECanaRegs.CANME.bit.ME2 = 0;//ʹ������2
     ECanaRegs.CANME.bit.ME3 = 0;//enable mail box 3
     
    // Mailboxs can be written to 16-bits or 32-bits at a time
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_L =0x1213 ;//��չ֡ID��0x00111213
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_H=0x01;
    ECanaMboxes.MBOX0.MSGID.bit.STDMSGID=0x04;
	ECanaMboxes.MBOX0.MSGID.bit.IDE=1;//��չ֡����Ϊ0
	ECanaMboxes.MBOX0.MSGID.bit.AME=0;//����λ

	 ECanaMboxes.MBOX1.MSGID.bit.EXTMSGID_L =0x1214;//��չ֡ID��0x00111214
     ECanaMboxes.MBOX1.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX1.MSGID.bit.STDMSGID=0x04;//04
     ECanaMboxes.MBOX1.MSGID.bit.IDE=1;//��չ֡����Ϊ0
	 ECanaMboxes.MBOX1.MSGID.bit.AME=0;//����λ
	 
    
     ECanaMboxes.MBOX2.MSGID.bit.EXTMSGID_L =0x0017;//��չ֡ID��0x00111215
     ECanaMboxes.MBOX2.MSGID.bit.EXTMSGID_H=0x00;
     ECanaMboxes.MBOX2.MSGID.bit.STDMSGID=0x00;//04
     ECanaMboxes.MBOX2.MSGID.bit.IDE=1;//��չ֡����Ϊ0
	 ECanaMboxes.MBOX2.MSGID.bit.AME=0;//����λ
	 
	 ECanaMboxes.MBOX3.MSGID.bit.EXTMSGID_L =0x0019;//��չ֡ID��0x00111219
     ECanaMboxes.MBOX3.MSGID.bit.EXTMSGID_H=0x00;
     ECanaMboxes.MBOX3.MSGID.bit.STDMSGID=0x00;//04
     ECanaMboxes.MBOX3.MSGID.bit.IDE=1;//��չ֡����Ϊ0
	 ECanaMboxes.MBOX3.MSGID.bit.AME=0;//����λ
	 
	 ECanaMboxes.MBOX4.MSGID.bit.EXTMSGID_L =0x1210 ;//��չ֡ID��0x00111210 //for adjust 0
     ECanaMboxes.MBOX4.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX4.MSGID.bit.STDMSGID=0x04;
	 ECanaMboxes.MBOX4.MSGID.bit.IDE=1;//��չ֡����Ϊ0
   	 ECanaMboxes.MBOX4.MSGID.bit.AME=0;//����λ
	 
     ECanaRegs.CANMD.bit.MD0 = 1; //����0����Ϊ����
     ECanaRegs.CANMD.bit.MD1 = 1; //����1����Ϊ����
     ECanaRegs.CANMD.bit.MD2 = 1; //����2����Ϊ����
     ECanaRegs.CANMD.bit.MD3 = 1; //����3����Ϊ����
     ECanaRegs.CANMD.bit.MD4 = 1; //����4����Ϊ����
     
     ECanaRegs.CANME.bit.ME0 = 1;//ʹ������0
     ECanaRegs.CANME.bit.ME1 = 1;//ʹ������2
     ECanaRegs.CANME.bit.ME2 = 1;//ʹ������2
     ECanaRegs.CANME.bit.ME3 = 1;//ʹ������2
     ECanaRegs.CANME.bit.ME4 = 1;//ʹ������2
     
     // Specify that 8 bits will be sent/received
     ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
     ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
   	 ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;	
   	 ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;	
   	 ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;	
   	 
/******************************* SEND target Coordinates********************************************/
/* Write to the MSGID field */
  ECanaMboxes.MBOX26.MSGID.bit.EXTMSGID_L =0x1219;//��չ֡ID��0x00111219
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
/******************************* SEND laser Off Command********************************************/
/* Write to the MSGID field */
   ECanaMboxes.MBOX27.MSGID.bit.EXTMSGID_L =0x0002;//��չ֡ID��0x00000002
   ECanaMboxes.MBOX27.MSGID.bit.EXTMSGID_H=0x00;
   ECanaMboxes.MBOX27.MSGID.bit.STDMSGID=0x00;
   ECanaMboxes.MBOX27.MSGID.bit.IDE=1;
/* Configure Mailbox under test as a Transmit mailbox */
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD27 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
/* Enable Mailbox under test */
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME27 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
/* Write to DLC field in Master Control reg */
   ECanaMboxes.MBOX27.MSGCTRL.bit.DLC = 8;
/******************************* SEND map point cooridinate********************************************/
/* Write to the MSGID field */
   ECanaMboxes.MBOX28.MSGID.bit.EXTMSGID_L =0x0003;//��չ֡ID��0x00000003
   ECanaMboxes.MBOX28.MSGID.bit.EXTMSGID_H=0x00;
   ECanaMboxes.MBOX28.MSGID.bit.STDMSGID=0x00;
   ECanaMboxes.MBOX28.MSGID.bit.IDE=1;
/* Configure Mailbox under test as a Transmit mailbox */
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD28 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
/* Enable Mailbox under test */
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME28 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
/* Write to DLC field in Master Control reg */
   ECanaMboxes.MBOX28.MSGCTRL.bit.DLC = 8;
   /******************************* SEND switch base signal********************************************/
/* Write to the MSGID field */
  ECanaMboxes.MBOX29.MSGID.bit.EXTMSGID_L =0x1210;//extended ID��0x00111210
     ECanaMboxes.MBOX29.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX29.MSGID.bit.STDMSGID=0x04;
     ECanaMboxes.MBOX29.MSGID.bit.IDE=1;
/* Configure Mailbox under test as a Transmit mailbox */
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD29 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
/* Enable Mailbox under test */
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME29 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
/* Write to DLC field in Master Control reg */
   ECanaMboxes.MBOX29.MSGCTRL.bit.DLC = 8;
      /******************************* SEND target Z coordinate********************************************/
/* Write to the MSGID field */
   ECanaMboxes.MBOX30.MSGID.bit.EXTMSGID_L =0x1010;//extended ID��0x00111010
   ECanaMboxes.MBOX30.MSGID.bit.EXTMSGID_H=0x01;
   ECanaMboxes.MBOX30.MSGID.bit.STDMSGID=0x04;
   ECanaMboxes.MBOX30.MSGID.bit.IDE=1;
/* Configure Mailbox under test as a Transmit mailbox */
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD30 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
/* Enable Mailbox under test */
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME30 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
/* Write to DLC field in Master Control reg */
   ECanaMboxes.MBOX30.MSGCTRL.bit.DLC = 8;
     /******************************* SEND target coordinates with x and y bias********************************************/
/* Write to the MSGID field */
   ECanaMboxes.MBOX31.MSGID.bit.EXTMSGID_L =0x1011;//extended ID��0x00111011
   ECanaMboxes.MBOX31.MSGID.bit.EXTMSGID_H=0x01;
   ECanaMboxes.MBOX31.MSGID.bit.STDMSGID=0x04;
   ECanaMboxes.MBOX31.MSGID.bit.IDE=1;
/* Configure Mailbox under test as a Transmit mailbox */
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD31 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
/* Enable Mailbox under test */
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME31 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
/* Write to DLC field in Master Control reg */
   ECanaMboxes.MBOX31.MSGCTRL.bit.DLC = 8;
   
   initGpio_pwm();
  while(1)
   {  
   		//float testa=calulate_from_edges((float)3,(float)4,(float)5);
   	 	ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
   	 	/////////////////////////////////
   	 	////commands from upposition machine
   	 	/////////////////////////
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
			
		   long canplusecount=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000��������ת��360��
		   long canplusecount1=CAN_RxBuffer[3]*65536+CAN_RxBuffer[4]*256+CAN_RxBuffer[5];// 1000000��������ת��360��
           int map_dir=CAN_RxBuffer[6]&0x80;////x sign
           int map_dir1=CAN_RxBuffer[6]&0x40;////y sign 
           MapState= CAN_RxBuffer[6]&0x30;
           
            if(MapState==0x0)/////set map
            {
            	int map_index=CAN_RxBuffer[7];
            	if(map_index<10)
            	{
            		if(map_dir==0){
            		
            			Map[map_index].x=-canplusecount;
            		}
            		else
            		{
            			Map[map_index].x=canplusecount;
            		}		
            		if(map_dir1==0){
            		
            			Map[map_index].y=-canplusecount1;
            		}
            		else
            		{
            			Map[map_index].y=canplusecount1;
            		}		
					
            
            	}
           		
            }
            else if(MapState==0x20)////////Return map
            {
            	
	           int map_return_index=CAN_RxBuffer[7];
	           if(map_return_index<10)
	           {
	         		    long temp_current_x=(long)Map[map_return_index].x;
						long temp_current_y=(long)Map[map_return_index].y;
					   
						if(temp_current_x>0)
						{
							ECanaMboxes.MBOX28.MDH.byte.BYTE6=0x01;
							temp_current_x=(Uint32)temp_current_x;
						}
						else
						{
							ECanaMboxes.MBOX28.MDH.byte.BYTE6=0x00;
							temp_current_x=-temp_current_x;
							temp_current_x=(Uint32)temp_current_x;
						}
						if(temp_current_y>0)
						{
							ECanaMboxes.MBOX28.MDH.byte.BYTE7=0x01;  
							temp_current_y=(Uint32)temp_current_y;
						}
						else
						{
							ECanaMboxes.MBOX28.MDH.byte.BYTE7=0x00; 
							temp_current_y=-temp_current_y;
							temp_current_y=(Uint32)temp_current_y;
						}
						ECanaMboxes.MBOX28.MDL.byte.BYTE0=(Uint16)(((Uint32)temp_current_x)>>16);
					    ECanaMboxes.MBOX28.MDL.byte.BYTE1=(Uint16)(((Uint32)temp_current_x)>>8);
					    ECanaMboxes.MBOX28.MDL.byte.BYTE2=(Uint16)((Uint32)temp_current_x);
						ECanaMboxes.MBOX28.MDH.byte.BYTE5=(Uint16)(((Uint32)temp_current_y));
						ECanaMboxes.MBOX28.MDH.byte.BYTE4=(Uint16)(((Uint32)temp_current_y)>>8);
						ECanaMboxes.MBOX28.MDL.byte.BYTE3=(Uint16)(((Uint32)temp_current_y)>>16);
						
					    ECanaShadow.CANTRS.all = 0;
					    ECanaShadow.CANTRS.bit.TRS28 = 1; // Set TRS for mailbox under test
					    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
					    do // Send 00110000
					    {
					      ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
					    } while(ECanaShadow.CANTA.bit.TA28 == 0 );
					    ECanaShadow.CANTA.all = 0;
					    ECanaShadow.CANTA.bit.TA28 = 1; // Clear TA28
					    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
	           }
	           
            }
            else if(MapState==0x30)//Reset Zero
            {
            
            
            }
		 }
		 ///////////////////////////////
		 ///adjust zero point
		 ///////////////////////////////
		 	if (ECanaShadow.CANRMP.bit.RMP4 == 1)//D0  receieve 111210
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP4 = 1;     	 // Clear RMP4
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	            CAN_RxBuffer[0]=ECanaMboxes.MBOX4.MDL.byte.BYTE0;
	            CAN_RxBuffer[1]=ECanaMboxes.MBOX4.MDL.byte.BYTE1;
	            CAN_RxBuffer[2]=ECanaMboxes.MBOX4.MDL.byte.BYTE2;
	            CAN_RxBuffer[3]=ECanaMboxes.MBOX4.MDL.byte.BYTE3;
				CAN_RxBuffer[4]=ECanaMboxes.MBOX4.MDH.byte.BYTE4;
				CAN_RxBuffer[5]=ECanaMboxes.MBOX4.MDH.byte.BYTE5;
				CAN_RxBuffer[6]=ECanaMboxes.MBOX4.MDH.byte.BYTE6;
				CAN_RxBuffer[7]=ECanaMboxes.MBOX4.MDH.byte.BYTE7;
			if(CAN_RxBuffer[7]==AdjustMode)
			{
			   AutoMode=AdjustMode;
			   long canplusecount=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000��������ת��360��
			   long canplusecount1=CAN_RxBuffer[3]*65536+CAN_RxBuffer[4]*256+CAN_RxBuffer[5];// 1000000��������ת��360��
	           int adj_dir=CAN_RxBuffer[6]&0x80;////x sign
	           int adj_dir1=CAN_RxBuffer[6]&0x40;////y sign 
	           //MapState= CAN_RxBuffer[6]&0x30;
	           dir=adj_dir;
	           dir1=adj_dir1;
	           drive(canplusecount*3.14159/180);
	           driveY(canplusecount1*3.14159/180);
			}
			else
			{
				angle=0;
				angleY=0;
				AutoMode=ScanMode;
			}
		 }
		 ///////////////////////////////
		 ///////////From laser distance detector
		 ///////////////////////////////
		 if (ECanaShadow.CANRMP.bit.RMP1 == 1)//D0  receieve 111214 //from distance detector
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP1 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	       CAN_RxBuffer[0]=ECanaMboxes.MBOX1.MDL.byte.BYTE0;
	       CAN_RxBuffer[1]=ECanaMboxes.MBOX1.MDL.byte.BYTE1;
	       CAN_RxBuffer[2]=ECanaMboxes.MBOX1.MDL.byte.BYTE2;
		 // distance=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000��������ת��360��
	       CAN_RxBuffer[3]=ECanaMboxes.MBOX1.MDL.byte.BYTE3;
		   CAN_RxBuffer[4]=ECanaMboxes.MBOX1.MDH.byte.BYTE4;
		   CAN_RxBuffer[5]=ECanaMboxes.MBOX1.MDH.byte.BYTE5;
		   CAN_RxBuffer[6]=ECanaMboxes.MBOX1.MDH.byte.BYTE6;
				if(CAN_RxBuffer[5]==1)//index to indentify base number
				{
					
					distance=CAN_RxBuffer[0]*10000+CAN_RxBuffer[1]*1000+CAN_RxBuffer[2]*100+CAN_RxBuffer[3]*10+CAN_RxBuffer[4];// 9440000��������ת��360��
					//swing_speed = ((float)distance/(float)(PRD/4+distance))*PRD  ;  //CHANGE THE SWINGING VELOCITY
				
				
				}
				//CAN_RxBuffer[7]=ECanaMboxes.MBOX1.MDH.byte.BYTE7;
			 	
          		
      	 
		 }
		 //////////////////////////////////
		 //////////////////target x bias and y bias from wireless channel
		 ///////////////////////////////////
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
				
				distance_valid_flag  =(CAN_RxBuffer[2]);
				
				if(CAN_RxBuffer[1]==1)
				{
					x_bias_dir=1;
				}
				else
				{
					x_bias_dir=0;
				}
				if(CAN_RxBuffer[5]==1)
				{
					y_bias_dir=1;
				}
				else
				{
					y_bias_dir=0;
				}
				
				
				x_bias=CAN_RxBuffer[0];////
				y_bias=CAN_RxBuffer[4];/////0001 1111
				
				if(CAN_RxBuffer[3]==1)
				{
					dir_flag_for_guidence=1;
				}
				else
				{
					dir_flag_for_guidence=0;
				}
				if(CAN_RxBuffer[7]==1)
				{
				
					dir1_flag_for_guidence=1;
				}
				else
				{
					dir1_flag_for_guidence=0;
				}			
 				if(distance_valid_flag==1&&shouldTurnOffFlag==FALSE)/////distance valid and itself should work
		        {
		           		
		           		
				////////////////////////////
				///////send out can msg
				
						ECanaMboxes.MBOX26.MDL.all = 0;
					    ECanaMboxes.MBOX26.MDH.all = 0;
					   ////////
					   ///transformation of coordinates
						long temp_current_x=(long)current_pos.x;
						long temp_current_y=(long)(-current_pos.y);
					   
						if(current_pos.x>0)
						{
							ECanaMboxes.MBOX26.MDH.byte.BYTE6=0x01;
							temp_current_x=(Uint32)temp_current_x;
						}
						else
						{
							ECanaMboxes.MBOX26.MDH.byte.BYTE6=0x00;
							temp_current_x=-temp_current_x;
							temp_current_x=(Uint32)temp_current_x;
						}
						if(temp_current_y>0)
						{
							ECanaMboxes.MBOX26.MDH.byte.BYTE7=0x01;  
							 temp_current_y=(Uint32)temp_current_y;
						}
						else
						{
							ECanaMboxes.MBOX26.MDH.byte.BYTE7=0x00; 
							temp_current_y=-temp_current_y;
							temp_current_y=(Uint32)temp_current_y;
						}
						ECanaMboxes.MBOX26.MDL.byte.BYTE0=(Uint16)(((Uint32)temp_current_x)>>16);
					    ECanaMboxes.MBOX26.MDL.byte.BYTE1=(Uint16)(((Uint32)temp_current_x)>>8);
					    ECanaMboxes.MBOX26.MDL.byte.BYTE2=(Uint16)((Uint32)temp_current_x);
						ECanaMboxes.MBOX26.MDH.byte.BYTE5=(Uint16)(((Uint32)temp_current_y));
						ECanaMboxes.MBOX26.MDH.byte.BYTE4=(Uint16)(((Uint32)temp_current_y)>>8);
						ECanaMboxes.MBOX26.MDL.byte.BYTE3=(Uint16)(((Uint32)temp_current_y)>>16);
						
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
			
		           		
		        }
	            else    /////distance not valid
	            {
	            	distance_valid_flag=FALSE;
	            	swing_speed=guidance_speed;
					EPwm1Regs.TBPRD=swing_speed;
					EPwm1Regs.CMPA.half.CMPA=swing_speed/2;
	            	
	           	}

		 }
		 ////////////////////////////////////
		 /////////target coordinates from  other base
		 //////////////////////////////////
		 if (ECanaShadow.CANRMP.bit.RMP3 == 1)//D0  receieve 111216 //from other base--the target infor
		{
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP3 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	            CAN_RxBuffer[0]=ECanaMboxes.MBOX3.MDL.byte.BYTE0;
	            CAN_RxBuffer[1]=ECanaMboxes.MBOX3.MDL.byte.BYTE1;
	            CAN_RxBuffer[2]=ECanaMboxes.MBOX3.MDL.byte.BYTE2;
	            
		 // distance=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000��������ת��360��
	            CAN_RxBuffer[3]=ECanaMboxes.MBOX3.MDL.byte.BYTE3;
				CAN_RxBuffer[4]=ECanaMboxes.MBOX3.MDH.byte.BYTE4;
			 	//distance=CAN_RxBuffer[0]*10000+CAN_RxBuffer[1]*1000+CAN_RxBuffer[2]*100+CAN_RxBuffer[3]*10+CAN_RxBuffer[4];// 9440000��������ת��360��
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
		 
		 angle=((long)EQep1Regs.QPOSCNT);
		 
		///////////////////////
		 //////Below is states transformations
		 //////////////////////////
	   	if(AutoMode==ScanMode)//automode is on 
	   	{
	   		 
	   	 	scan();
	   	 	if(isFirstScan==TRUE)
	   	 	{
	   	 		scanY();
	   	 	}
	   	 	//scanY();
	   	 	if(distance_valid_flag==TRUE)//if the distance is valid, the target is locked on
		  	{
		  		AutoMode=AutoModeOFF;
		  		isFirstScan=FALSE;
		  		
		 		current_pos=Get_Position(GetDegreeFromCount(angle),distance,cos(3.14*GetDegreeFromCountY(angleY)/180));//translate the pol coordinate to rectangular coordinate
		 		next_map_pos=get_next_point_on_trace(Map,MapPointCountCount);//search which is the next point on the map
				  //first get angle ,get ho many angles should turn;
				  //then drive 
				drive(get_angle(current_pos,next_map_pos,walkstep));
				
			 }
			  else{
			  		
			  		
				 //scan();//if failed to capture the target,then scan for it 
			  	}
	
	   	}
	   	else
	   	{
	   		if(AutoMode==AutoModeOFF)
	   		{
		   		if(distance_valid_flag==TRUE&&shouldTurnOffFlag==FALSE)//if the distance is valid, the target is locked on
			  	{
			  		
				  		if(	TargetInWorkingZone(current_pos)==FALSE)
				  	{
				  		
				  		   ////TURN OFF LASER 
				  		ECanaMboxes.MBOX27.MDH.all=0; 
				  		ECanaMboxes.MBOX27.MDL.all=0; 
				  		ECanaMboxes.MBOX27.MDL.byte.BYTE0=baseIndex;
						ECanaShadow.CANTRS.all = 0;
					    ECanaShadow.CANTRS.bit.TRS27 = 1; // Set TRS for mailbox under test
					    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
					    do // Send 00110000
					    {
					      ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
					    } while(ECanaShadow.CANTA.bit.TA27 == 0 );
					    ECanaShadow.CANTA.all = 0;
					    ECanaShadow.CANTA.bit.TA27 = 1; // Clear TA5
					    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
				  		////HANDLE TO NEXT STATION 
				  		ECanaMboxes.MBOX29.MDH.all=0; 
				  		ECanaMboxes.MBOX29.MDL.all=0; 
				  		ECanaMboxes.MBOX29.MDL.byte.BYTE0=baseIndex;
						ECanaShadow.CANTRS.all = 0;
					    ECanaShadow.CANTRS.bit.TRS29 = 1; // Set TRS for mailbox under test
					    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
					    do // Send 00110000
					    {
					      ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
					    } while(ECanaShadow.CANTA.bit.TA29 == 0 );
					    ECanaShadow.CANTA.all = 0;
					    ECanaShadow.CANTA.bit.TA29 = 1; // Clear TA5
					    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
					    
					    ///////////
					    ////switch state
					    //////////////
					    shouldTurnOffFlag=TRUE;
					    OutOfWork=TRUE;
				  	}
			  		else
			  		{
			  		
				 		current_pos=Get_Position(GetDegreeFromCount(angle),distance,cos(3.14*GetDegreeFromCountY(angleY)/180));//translate the pol coordinate to rectangular coordinate
				 		next_map_pos=get_next_point_on_trace(Map,MapPointCountCount);//search which is the next point on the map
						//first get angle ,get how many angles should turn;
					    //then drive 
						//
						//Just follow on x direction
						
						if(x_bias<=8){
				
							swing_speed=0;
						}
						else
						{
						//swing_speed_y=((float)y_bias/(float)(PRD/4+y_bias))*PRD;
						//swing_speed=((float)x_bias/(float)(PRD+x_bias))*PRD;
							if(x_bias>8&&x_bias<0xA3)
							{
								swing_speed=PRD/10;
							}
							else
							{
								swing_speed=PRD/14;
							}
						}
						EPwm1Regs.TBPRD=swing_speed;
						EPwm1Regs.CMPA.half.CMPA=swing_speed/2;		
								
						if(x_bias_dir==0)
						{
							dir=1;
							
						}
						else
						{
							dir=0;
							
						}
						Driver1(dir,1);
						////////////////////////////////////
						
						if(y_bias<=2)
						{
							swing_speed_y=0;
						}
						else
						{
							//swing_speed_y=((float)y_bias/(float)(PRD/4+y_bias))*PRD;
							swing_speed_y=PRD*((float)y_bias/18);
						
						}
						EPwm2Regs.TBPRD=swing_speed_y;
						EPwm2Regs.CMPA.half.CMPA=swing_speed_y/2;
						if(y_bias_dir==0)
						{
							dir1=0;
							
						}
						else
						{
							dir1=1;
							
						}
						Driver2(dir1,1);
						
			  		}
				  }
				  else if((distance_valid_flag==TRUE&&shouldTurnOffFlag==TRUE))//some laser is spotted by target,But not its own laser
				  {
				  	follow(current_pos.x,current_pos.y);///ACTUALLY should follow
				  }
				  
				  else{//distance not valid so should start scan again
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
				  		AutoMode=ScanMode;
					 //scan();//if failed to capture the target,then scan for it 
				  }
	   		}
		  }
		  ///////////////////////////
		  /////Below codes tell werther target is in its own working area
		  ///////////////////////////
	  	
	  
	  
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
	float gX;
	float gY;
	if(temp_pointer<length-1)
	{
		gX=map[temp_pointer+1].x;
		gY=map[temp_pointer+1].y;
		
	}
	else
	{
		gX=map[MapPointCountCount-1].x;
		gY=map[MapPointCountCount-1].y;
	}
	float dis1=(map[temp_pointer].x-gX)*(map[temp_pointer].x-gX)+(map[temp_pointer].y-gY)*(map[temp_pointer].y-gY);
	float dis2=(current_pos.x-gX)*(current_pos.x-gX)+(current_pos.y-gY)*(current_pos.y-gY);
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
	GpioDataRegs.GPATOGGLE.bit.GPIO1=1;
	Pwm2OneStepFinishFlag++;
	
	FinishStepFlag2=NOTFINISH;
	}
}

Coor Get_Position(float a,float d,float angCos)//a is in degrees 
{
	Coor tempCoor;
	
	
	tempCoor.x=angCos*d*cos(a/((float)180)*3.141593)+baseX;
	tempCoor.y=angCos*d*sin(a/((float)180)*3.141593)+baseY;
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
	if(Next_Map_Pos.y<Current_Pos.y)
	{
		dir=0;
	}
	else{
	dir=1;
	
	}
	return angle_to_rotate;
}
void drive(float degree)//the degree passed in should be in arcs 
{
	float degree_in_degrees=degree/3.14*180;
	long degree_count=degree_in_degrees/((float)360)*TotalLoopCountH;
	Driver1(dir,degree_count);

}
void driveY(float degree)//the degree passed in should be in arcs 
{
	float degree_in_degrees=degree/3.14*180;
	long degree_count=degree_in_degrees/((float)360)*TotalLoopCountV;
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
   	 	swing_speed=scan_speed;
   	 	EPwm1Regs.TBPRD=swing_speed;
   	 	drive(3.14/180);
}
void scanY()
{
		if(GetDegreeFromCountY(angleY)>scaleY)
   	 	{
   	 		dir1=0;
   	 		
   	 	} 
   	 	else
   	 	{
   	 		if(GetDegreeFromCountY(angleY)<-scaleY)
   	 		{
	   	 		dir1=1;
	   	 		
   	 		}
   	 	}
   	 	
   	 	EPwm2Regs.TBPRD = PRD/10; 
   	 	swing_speed_y=EPwm2Regs.TBPRD/2;
   	 	
   	 	driveY(1);
}

void epwm(float x_bisa,float distance)
{
	int t=2;
	if(distance<1000)
		{
		swing_speed=PRD/(4*t);
		}
	if(distance>=1000&&distance<5000)
	{
		if(x_bisa<=20)
			{swing_speed=PRD/(4*t);}
		if(x_bisa>20&&x_bisa<=100)
			{swing_speed=PRD/(5*t);}
		if(x_bisa>100&&x_bisa<=170)
			{swing_speed=PRD/(6*t);}
	}
	if(distance>=5000&&distance<10000)
		{
		if(x_bisa<=20)
			{swing_speed=PRD/(5*t);}
		if(x_bisa>20&&x_bisa<=100)
			{swing_speed=PRD/(6*t);}
		if(x_bisa>100&&x_bisa<=170)
			{swing_speed=PRD/(7*t);}
		}

	if(distance>=10000&&distance<15000)
	{
		if(x_bisa<=20)
			{swing_speed=PRD/(6*t);}
		if(x_bisa>20&&x_bisa<=100)
			{swing_speed=PRD/(7*t);}
		if(x_bisa>100&&x_bisa<=170)
			{swing_speed=PRD/(8*t);}
	}
	if(distance>=15000&&distance<25000)
	{
		if(x_bisa<=20)
			{swing_speed=PRD/(7*t);}
		if(x_bisa>20&&x_bisa<=100)
			{swing_speed=PRD/(8*t);}
		if(x_bisa>100&&x_bisa<=170)
			{swing_speed=PRD/(9*t);}
	}
	if(distance>=25000)
		{
		swing_speed=PRD/(10*t);
		}
	EPwm1Regs.TBPRD=swing_speed;
	EPwm1Regs.CMPA.half.CMPA=swing_speed/2;
}


float Arc2Degree(float arc)
{

return arc/3.1415927*180;
}
float GetDegreeFromCount(long cnt)
{
	return (float)cnt/((float)TotalLoopCountH)*360;
}
float GetDegreeFromCountY(long cnt)
{
	return (float)cnt/((float)TotalLoopCountV)*360;
}
int TargetInWorkingZone(Coor c)
{
	float tX,tY;
	tX=c.x;
	tY=c.y;
	float gX=Map[MapPointCountCount-1].x;
	float gY=Map[MapPointCountCount-1].y;
	if((tX-gX)*(tX-gX)+(tY-gY)*(tY-gY)<=8500000)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
//	if((tX<900)&&(tY<-700))
//	{
//		return FALSE;
//	}
//	else
//	{
//		return TRUE;
//	}

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



