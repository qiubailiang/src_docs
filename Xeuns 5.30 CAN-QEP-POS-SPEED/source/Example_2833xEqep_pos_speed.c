//###########################################################################
// Original Author S.D.
//
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: May 30, 2013 $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Example_posspeed.h"   // Example specific Include file
#include "math.h"
#define LEVEL GpioDataRegs.GPADAT.bit.GPIO0
#define DIR GpioDataRegs.GPBDAT.bit.GPIO48

#define LEVEL1 GpioDataRegs.GPADAT.bit.GPIO1
#define DIR1 GpioDataRegs.GPBDAT.bit.GPIO52

#define ROUND(x) ((int)( (x >= 0) ? (x + 0.5) : (x - 0.5)))

Uint16    *ExRamStart = (Uint16 *)0x100000;
void initEpwm();

interrupt void prdTick(void);
interrupt void ISRTimer0(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);

POSSPEED qep_posspeed=POSSPEED_DEFAULTS;
POSSPEED1 qep_posspeed1=POSSPEED1_DEFAULTS;

Uint32 Interrupt_Count = 0,flag=0,flag1=0;
long qposcntx=0,qposcnty=0;//’‚¿Ô±ÿ–Î∏ƒ≥…long–Õ£¨∑Ò‘Úœ¬√Ê«ø÷∆¿‡–Õ◊™ªªµƒ ±∫ÚæÕ≥ˆŒ Ã‚¡À°£
float Dirver (Uint32 N,int D,float F);
float Dirver1 (Uint32 N1,int D1,float F1);

Uint16 i,CAN_RxBuffer[10]={0};
Uint32 plusecount=0,plusecount1=0,canplusecount=0,canplusecount1=0,dir=0,dir1=0;

float F[10]={5000,2500,1666.666,1250,1000,833.333,50,0.5,0.25};//…Ë∂®∆µ¬ ÷µ£¨ ˝◊È÷–Œ™◊‘∂Ø◊∞‘ÿ÷µ£¨º¥ «º∆ ˝÷µ£¨∑÷±∂‘”¶∆µ¬ 100°¢200°¢300°¢400°¢500°¢
                                                      //600Hz 10k 100k
float a=0,b=0;                                                  //F[]µƒº∆À„π´ ΩŒ™1000000/2x£¨x «∆µ¬ ÷µHz
Uint32 distance=0,code_d=0,code_flag=0;
long code_a=0,code_b=0;
Uint32 codebuffer[2]={0,0}; // ±‡¬Î ˝◊È
struct ECAN_REGS ECanaShadow;
void main(void)
{ 
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();
   InitECan();
// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
   InitXintf16Gpio();	//zq
   InitECanGpio();
// For this case only init GPIO for eQEP1 and ePWM1
// This function is found in DSP2833x_EQep.c
    // InitEQep2Gpio();
//   InitEPwm1Gpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;
  

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();                  

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.TINT0 = &ISRTimer0;
   PieVectTable.XINT13 = &cpu_timer1_isr;
   PieVectTable.TINT2 = &cpu_timer2_isr;
   PieVectTable.EQEP1_INT= &prdTick;
   EDIS;    // This is needed to disable write to EALLOW protected registers
   


// Step 4. Initialize all the Device Peripherals:
//  initEpwm();  // This function exists in Example_EPwmSetup.c

// Step 5. User specific code, enable interrupts:
// Enable CPU INT1 which is connected to CPU-Timer 0:
// CPU int13 which is connected to CPU-Timer 1,
// and CPU int 14, which is connected to CPU-Timer 2:
   InitCpuTimers();   // For this example, only initialize the Cpu Timers
//   ConfigCpuTimer(&CpuTimer2, 150, 100000);//100ms»°—˘“ª¥Œ
   IER |= M_INT1;
   IER |= M_INT3;
   IER |= M_INT6;
   IER |= M_INT13;
   IER |= M_INT14;

// Enable TINT0 in the PIE: Group 3 interrupt 1
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
// Enable TINT0 in the PIE: Group 4 interrupt1
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
	// Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	// Enable TINT0 in the PIE: Group 1 interrupt 6
    PieCtrlRegs.PIEIER6.bit.INTx1 =1;
// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM
   i=0;
    /******************************* receive ********************************************/
     ECanaRegs.CANME.bit.ME0 = 0;// πƒ‹” œ‰0
     ECanaRegs.CANME.bit.ME1 = 0;// πƒ‹” œ‰1
    // Mailboxs can be written to 16-bits or 32-bits at a time
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_L =0x1213 ;//¿©’π÷°ID£∫0x00111213
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_H=0x01;
    ECanaMboxes.MBOX0.MSGID.bit.STDMSGID=0x04;
	ECanaMboxes.MBOX0.MSGID.bit.IDE=1;//¿©’π÷°£¨»ÁŒ™0

	 ECanaMboxes.MBOX1.MSGID.bit.EXTMSGID_L =0x1214 ;//¿©’π÷°ID£∫0x00111214
     ECanaMboxes.MBOX1.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX1.MSGID.bit.STDMSGID=0x04;
     ECanaMboxes.MBOX1.MSGID.bit.IDE=1;//¿©’π÷°£¨»ÁŒ™0
    
     ECanaRegs.CANMD.bit.MD0 = 1; //” œ‰0…Ë÷√Œ™Ω” ’
     ECanaRegs.CANMD.bit.MD1 = 1; //” œ‰1…Ë÷√Œ™Ω” ’
     
     ECanaRegs.CANME.bit.ME0 = 1;// πƒ‹” œ‰0
     ECanaRegs.CANME.bit.ME1 = 1;// πƒ‹” œ‰0
    // Specify that 8 bits will be sent/received
     ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
     ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
	 /******************************* SEND ********************************************/
/* Write to the MSGID field  */

	 ECanaMboxes.MBOX25.MSGID.bit.EXTMSGID_L =0x1215 ;//¿©’π÷°ID£∫0x00111215
     ECanaMboxes.MBOX25.MSGID.bit.EXTMSGID_H=0x01;
     ECanaMboxes.MBOX25.MSGID.bit.STDMSGID=0x04;
     ECanaMboxes.MBOX25.MSGID.bit.IDE=1;

/* Configure Mailbox under test as a Transmit mailbox */

   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD25 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

/* Enable Mailbox under test */

   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME25 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;

/* Write to DLC field in Master Control reg */

   ECanaMboxes.MBOX25.MSGCTRL.bit.DLC = 8;

/* Write to the mailbox RAM field */

  while(1)
   { 
     	     
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
			
			canplusecount=CAN_RxBuffer[0]*65536+CAN_RxBuffer[1]*256+CAN_RxBuffer[2];// 9440000∏ˆ¬ˆ≥ÂµÁª˙◊™∂Ø360°„
			canplusecount1=CAN_RxBuffer[4]*65536+CAN_RxBuffer[5]*256+CAN_RxBuffer[6];// 1000000∏ˆ¬ˆ≥ÂµÁª˙◊™∂Ø360°„
            dir=CAN_RxBuffer[3];
            dir1=CAN_RxBuffer[7];
            Dirver (canplusecount,dir,F[7]);
            Dirver1 (canplusecount1,dir1,F[7]);
 //           code_flag=1;
		
		 }
 if (ECanaShadow.CANRMP.bit.RMP1 == 1)// æ‡¿Î–≈œ¢¥¶¿Ìreceieve 111214
		 {
		  	
	       ECanaShadow.CANRMP.all = 0;
	       ECanaShadow.CANRMP.bit.RMP1 = 1;     	 // Clear RMP20
	       ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
	       
	            CAN_RxBuffer[0]=ECanaMboxes.MBOX1.MDL.byte.BYTE0;
	            CAN_RxBuffer[1]=ECanaMboxes.MBOX1.MDL.byte.BYTE1;
	            CAN_RxBuffer[2]=ECanaMboxes.MBOX1.MDL.byte.BYTE2;
	            CAN_RxBuffer[3]=ECanaMboxes.MBOX1.MDL.byte.BYTE3;
				CAN_RxBuffer[4]=ECanaMboxes.MBOX1.MDH.byte.BYTE4;
				CAN_RxBuffer[5]=ECanaMboxes.MBOX1.MDH.byte.BYTE5;
				CAN_RxBuffer[6]=ECanaMboxes.MBOX1.MDH.byte.BYTE6;
				CAN_RxBuffer[7]=ECanaMboxes.MBOX1.MDH.byte.BYTE7;
		 distance=CAN_RxBuffer[0]*10000+CAN_RxBuffer[1]*1000+CAN_RxBuffer[2]*100+CAN_RxBuffer[3]*10+CAN_RxBuffer[4];//æ‡¿Îµ•ŒªŒ™mm		 
	    	code_d=CAN_RxBuffer[0]*1000+CAN_RxBuffer[1]*100+CAN_RxBuffer[2]*10+CAN_RxBuffer[3];// ±‡¬Î÷–»°≤‚æ‡∑÷±Ê¬ Œ™1cm
			a=qposcntx*3.6*36/(944*5);     //∂‘ÀÆ∆ΩΩ«∂»Ω¯––±‡¬Î£¨“‘5Ω«√ÎŒ™µ•Œªqposcntx*360*3600/(9440000*5)
			b=qposcnty*3.6*36/(100*5);     //∂‘∏©—ˆΩ«∂»Ω¯––±‡¬Î£¨“‘5Ω«√ÎŒ™µ•Œªqposcnty*360*3600/(1000000*5)
            code_a=ROUND(a);
     		code_b=ROUND(b);
				code_d=code_d<<16;
				code_b=code_b<<16;
				codebuffer[0]=code_d+code_a;
				codebuffer[1]=code_b;

			if(flag==1&&flag1==1)
			{
	 	  			ECanaMboxes.MBOX25.MDL.all = codebuffer[0];
	      			ECanaMboxes.MBOX25.MDH.all = codebuffer[1];
	       
	      			 ECanaShadow.CANTRS.all = 0;
	      			 ECanaShadow.CANTRS.bit.TRS25 = 1;     // Set TRS for mailbox under test
	      			 ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

					    do                        // Send 111215 
					   {
					       ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
				       } while(ECanaShadow.CANTA.bit.TA25 == 0 );

				       ECanaShadow.CANTA.all = 0;
				       ECanaShadow.CANTA.bit.TA25 = 1;     	 // Clear TA5
				       ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
              flag=0;
              flag1=0;
			}

		}
            

   }

   
           

}
                                       
/*«˝∂ØµÁª˙≥Ã–Ú*/
float Dirver (Uint32 N,int D,float F)
{   
    InitEQep1Gpio();
	qep_posspeed.init(&qep_posspeed);
    plusecount=N;
    EALLOW;
	DIR=D;
    EDIS;
	ConfigCpuTimer(&CpuTimer0, 150,F);   
	StartCpuTimer0();//±ÿ–Î÷ÿ–¬∆Ù∂Ø∂® ±∆˜0≤≈ø…“‘£¨∑Ò‘Ú∂® ±∆˜≤ªƒ‹π§◊˜
	return 0;
}
float Dirver1 (Uint32 N1,int D1,float F1)
{    
    InitEQep2Gpio();
	qep_posspeed1.init(&qep_posspeed1);
    plusecount1=N1;
    EALLOW;
   	DIR1=D1;
	EDIS;
   	ConfigCpuTimer(&CpuTimer1, 150,F1);   
	StartCpuTimer1();//±ÿ–Î÷ÿ–¬∆Ù∂Ø∂®±∆˜0≤≈ø…“‘£¨∑Ò‘Ú∂® ±∆˜≤ªƒ‹π§◊˜
	return 0;
}

interrupt void ISRTimer0(void)
{
    CpuTimer0.InterruptCount++;
    
   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    CpuTimer0Regs.TCR.bit.TIF=1;
    CpuTimer0Regs.TCR.bit.TRB=1;
	
	if(plusecount==0)
    
      { 
       	
	    qposcntx=(long)EQep1Regs.QPOSCNT;
        qposcnty=(long)EQep2Regs.QPOSCNT;

		flag=1;
	   	StopCpuTimer0();
	  }
	  else
	  {
		 LEVEL=~LEVEL;
		 plusecount--;
	  }
}

interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;

    CpuTimer1Regs.TCR.bit.TIF=1;
    CpuTimer1Regs.TCR.bit.TRB=1;

    
	if(plusecount1==0)
    {
	
		qposcntx=(long)EQep1Regs.QPOSCNT;
		qposcnty=(long)EQep2Regs.QPOSCNT;

		flag1=1;
	   	StopCpuTimer1();
	}
	else
	{
		LEVEL1=~LEVEL1;
		plusecount1--;
	}
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
       for (i=0; i<700; i++){
       }
       GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
	   Interrupt_Count = 0;                   // Reset count
	   EDIS;
   }
   

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
   EPwm1Regs.ETCLR.bit.INT=1;
}










