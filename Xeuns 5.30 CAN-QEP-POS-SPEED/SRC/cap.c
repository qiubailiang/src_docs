#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define EQEP2B GpioDataRegs.GPADAT.bit.GPIO25
#define LEVEL GpioDataRegs.GPADAT.bit.GPIO0
#define DIR GpioDataRegs.GPBDAT.bit.GPIO48

#define LEVEL1 GpioDataRegs.GPADAT.bit.GPIO1
#define DIR1 GpioDataRegs.GPBDAT.bit.GPIO52

Uint16    *ExRamStart = (Uint16 *)0x100000;
Uint32 t1=0,t2=0,t3=0,t4=0,T1=0,T2=0,t5,t6,t7,t8,T3,T4;
interrupt void ISRCap1(void);
interrupt void ISRCap2(void);
interrupt void ISRTimer0(void);
interrupt void cpu_timer1_isr(void);
Uint16  i;
Uint32 plusecount=0,plusecountcap=0,n=0,plusecount1=0,n1=0;
extern Uint16 dir=0,dir1=0;
float F[10]={5000,2500,1666.666,1250,1000,833.333,50,0.5};//设定频率值，数组中为自动装载值，即是计数值，分别对应频率100、200、300、400、500、
                                                      //600Hz 10k 100k
                                                  //F[]的计算公式为1000000/2x，x是频率值Hz
float N[10]={400,600,800,1000,1200,1400};//对应频率下的脉冲个数

double F1[10]={5000,2500,1666.666,1250,1000,833.333};//设定频率值，数组中为自动装载值，即是计数值，分别对应频率100、200、300、400、500、600Hz
                                                  //F[]的计算公式为1000000/2x，x是频率值Hz
float N1[10]={400,600,800,1000,1200,1400};//对应频率下的脉冲个数
float Dirver (Uint32 N,int D,float F);
float Dirver1 (float N1,int D1,float F1);
void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
   InitXintf16Gpio();	//zq

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
   PieVectTable. ECAP1_INT = &ISRCap1;
   //PieVectTable. ECAP2_INT = &ISRCap2; 
    PieVectTable.TINT0 = &ISRTimer0;
    PieVectTable.XINT13 = &cpu_timer1_isr;
   //PieVectTable.TINT2 = &cpu_timer2_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize the Device Peripheral. This function can be
//         found in DSP2833x_CpuTimers.c

  InitCpuTimers();   // For this example, only initialize the Cpu Timers
  IER |= M_INT1;
// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
// 150MHz CPU Freq, 1 second Period (in uSeconds)
   //ConfigCpuTimer(&CpuTimer0, 150, 5000);   //5ms中断一次
   //ConfigCpuTimer(&CpuTimer0, 150, 1000000);
   //ConfigCpuTimer(&CpuTimer1, 150, 1000000);
   //ConfigCpuTimer(&CpuTimer2, 150, 1000000);
 // StartCpuTimer0();
   //Dirver(2360000,0,F[0]);
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK=0;//disenable clock to Cap1
// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
// cap1 is connected to CPU-int 4, and CPU int 14, which is connected
// to CPU-Timer 2:
    /*IER |= M_INT4;
	IER |= M_INT13;
   IER |= M_INT14;

// Enable TINT0 in the PIE: Group 4 interrupt1
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
	// Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
// Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
	*/
	i = 0;
    Dirver(18800000,0,F[7]);
	//InitCapl();
	    IER |= M_INT4;
	IER |= M_INT13;
   IER |= M_INT14;

// Enable TINT0 in the PIE: Group 4 interrupt1
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
	// Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	// Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
    for(; ;)
    {  
     //Dirver(9440000,0,F[7]);
    }

}
float Dirver (Uint32 N,int D,float F)
{   
    InitCapl();
    plusecount=N;
	DIR=D;
   	ConfigCpuTimer(&CpuTimer0, 150,F);   
	StartCpuTimer0();//必须重新启动定时器0才可以，否则定时器不能工作
	return 0;
}
float Dirver1 (float N1,int D1,float F1)
{   
    
    plusecount1=N1;
	DIR1=D1;
   	ConfigCpuTimer(&CpuTimer1, 150,F1);   
	StartCpuTimer1();//必须重新启动定时器0才可以，否则定时器不能工作
	return 0;
}

interrupt void ISRCap1(void)
{
   
   	plusecountcap++;
   // plusecount1++;
   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
    ECap1Regs.ECCLR.all=0xFFFF;//clare all flag
     ECap1Regs.ECCTL2.bit.REARM = 1; //该句要不要，在上面已经分析过，取决于系统设计要求

    



   /* if(EQEP2B==1)
	{
	dir=0;//正转
    dir1=0;//正转
	}
	else if(EQEP2B==0)
    {dir=1;//反转
     dir1=1;//反转
	 }
    plusecount=plusecount*100;//因为手轮转动一格，不足以驱动步进电机走一步，所以，为了观察明显起见做了这种运算
	plusecount1=plusecount1*100;//因为手轮转动一格，不足以驱动步进电机走一步，所以，为了观察明显起见做了这种运算
    Dirver(plusecount,dir,F[5]);
	Dirver1(plusecount1,dir1,F1[5]);
	*/
    
}
interrupt void ISRTimer0(void)
{
    CpuTimer0.InterruptCount++;
    
   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    CpuTimer0Regs.TCR.bit.TIF=1;
    CpuTimer0Regs.TCR.bit.TRB=1;
	LEVEL=~LEVEL;
    //LEVEL=1;
	plusecount--;
	if(plusecount==0)//N0个脉冲以装载值为F1的频率下转动结束之后，启动Dirver(N[1],1,F[1]);
    {

	SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK=0;//disenable clock to Cap1
    ECap1Regs.ECCTL2.bit.REARM = 0; //不再捕获脉冲
   	StopCpuTimer0();
	}
}



interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;

    CpuTimer1Regs.TCR.bit.TIF=1;
    CpuTimer1Regs.TCR.bit.TRB=1;

    LEVEL1=~LEVEL1;
	plusecount1--;
	if(plusecount1==0)//N0个脉冲以装载值为F1的频率下转动结束之后，启动Dirver(N[1],1,F[1]);
    {
    ECap1Regs.ECCTL2.bit.REARM = 1; //重新开启捕获功能，继续捕获脉冲
   	StopCpuTimer1();
	}
}
/*
interrupt void ISRCap2(void)
{
   

   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
    ECap2Regs.ECCLR.all=0xFFFF;//clare all flag
	t5= ECap2Regs.CAP1;
	t6= ECap2Regs.CAP2;
	t7= ECap2Regs.CAP3;
  	t8= ECap2Regs.CAP4;  
    T3=t6-t5;T4=t8-t7;

    
}
*/

//===========================================================================
// No more.
//===========================================================================
