// TI File $Revision: /main/9 $
// Checkin $Date: April 21, 2008   15:42:18 $
//###########################################################################
//
// FILE:	Example_EpwmSetup.c
//
// TITLE:	Pos speed measurement using EQEP peripheral
//
// DESCRIPTION:
//
// This file contains source for the ePWM initialization for the 
// pos/speed module
//
//###########################################################################
// Original Author: SD
//
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Example_posspeed.h"   // Example specific Include file
#include "DSP2833x_EPwm_defines.h"

#if (CPU_FRQ_150MHZ)
  #define CPU_CLK   150e6
#endif
#if (CPU_FRQ_100MHZ)
  #define CPU_CLK   100e6
#endif

#define PWM_CLK   1000e3              // 5kHz (300rpm) EPWM1 frequency. Freq. can be changed here
#define SP        CPU_CLK/(2*PWM_CLK)
#define TBCTLVAL  0x200E           // up-down count, timebase=SYSCLKOUT

extern Uint16 PRD;
void initEpwm1A()
{  
//	EPwm1Regs.TBSTS.all=0;
//	EPwm1Regs.TBPHS.half.TBPHS =0;
//	EPwm1Regs.TBCTR=0;
//
//	EPwm1Regs.CMPCTL.all=0x50;     // immediate mode for CMPA and CMPB   
//	EPwm1Regs.CMPA.half.CMPA=SP/2;
//	EPwm1Regs.CMPB=0;
//	
//	EPwm1Regs.AQCTLA.all=0x60;     // CTR=CMPA when inc->EPWM1A=1, when dec->EPWM1A=0
//	EPwm1Regs.AQCTLB.all=0x09;     // CTR=PRD ->EPWM1B=1, CTR=0 ->EPWM1B=0  
//	EPwm1Regs.AQSFRC.all=0;
//	EPwm1Regs.AQCSFRC.all=0;
//	
//	EPwm1Regs.TZSEL.all=0;
//	EPwm1Regs.TZCTL.all=0;
//	EPwm1Regs.TZEINT.all=0;
//	EPwm1Regs.TZFLG.all=0;
//	EPwm1Regs.TZCLR.all=0;
//	EPwm1Regs.TZFRC.all=0;
//
//	EPwm1Regs.ETSEL.all=0x0A;      // Interrupt on PRD 
//	EPwm1Regs.ETPS.all=1;	
//	EPwm1Regs.ETFLG.all=0;
//	EPwm1Regs.ETCLR.all=0;
//	EPwm1Regs.ETFRC.all=0;
//	
//	EPwm1Regs.PCCTL.all=0;
//	
//	EPwm1Regs.TBCTL.all=0x0010+TBCTLVAL; // Enable Timer
//	EPwm1Regs.TBPRD=SP;
	// EPWM Module 1 config
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
   EDIS;

   EPwm1Regs.TBPRD = PRD;       // Set timer period   
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                  // Clear counter

   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading   
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;     //WHNE  TBCTR=0,LOAD FROM SHADOW REGS
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
   EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
   

   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2 ;   // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;     //TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV)
//   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;

   // Setup shadow register load on ZERO
   
   
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;   
 
   //Uint16  INTSEL:3;        // 2:0    EPWMxINTn Select  Uint16  INTEN:1; 
   // Set Compare values
   EPwm1Regs.CMPA.half.CMPA =0;    // Set compare A value

   // Set actions
   
   EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;            // Set PWM1A on Zero
   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;          // Clear PWM1A on event A, up count

//   EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // Set PWM1B on Zero
//   EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;          // Clear PWM1B on event B, up count
//   
   EPwm1Regs.ETSEL.bit.INTSEL=2;
   EPwm1Regs.ETSEL.bit.INTEN=1;
   
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
   EDIS;
   
}
void initEpwm1A_WithSpeed(Uint16 prd)
{  

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
   EDIS;

   EPwm1Regs.TBPRD = prd;       // Set timer period   
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                  // Clear counter

   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading   
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;     //WHNE  TBCTR=0,LOAD FROM SHADOW REGS
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
   EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
   

   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2 ;   // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;     //TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV)
//   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;

   // Setup shadow register load on ZERO
   
   
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;   
 
   //Uint16  INTSEL:3;        // 2:0    EPWMxINTn Select  Uint16  INTEN:1; 
   // Set Compare values
   EPwm1Regs.CMPA.half.CMPA =0;    // Set compare A value

   // Set actions
   
   EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;            // Set PWM1A on Zero
   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;          // Clear PWM1A on event A, up count

//   EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // Set PWM1B on Zero
//   EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;          // Clear PWM1B on event B, up count
//   
   EPwm1Regs.ETSEL.bit.INTSEL=4;
   EPwm1Regs.ETSEL.bit.INTEN=1;
   
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
   EDIS;
   
}
void initEpwm2A()
{  
//	EPwm1Regs.TBSTS.all=0;
//	EPwm1Regs.TBPHS.half.TBPHS =0;
//	EPwm1Regs.TBCTR=0;
//
//	EPwm1Regs.CMPCTL.all=0x50;     // immediate mode for CMPA and CMPB   
//	EPwm1Regs.CMPA.half.CMPA=SP/2;
//	EPwm1Regs.CMPB=0;
//	
//	EPwm1Regs.AQCTLA.all=0x60;     // CTR=CMPA when inc->EPWM1A=1, when dec->EPWM1A=0
//	EPwm1Regs.AQCTLB.all=0x09;     // CTR=PRD ->EPWM1B=1, CTR=0 ->EPWM1B=0  
//	EPwm1Regs.AQSFRC.all=0;
//	EPwm1Regs.AQCSFRC.all=0;
//	
//	EPwm1Regs.TZSEL.all=0;
//	EPwm1Regs.TZCTL.all=0;
//	EPwm1Regs.TZEINT.all=0;
//	EPwm1Regs.TZFLG.all=0;
//	EPwm1Regs.TZCLR.all=0;
//	EPwm1Regs.TZFRC.all=0;
//
//	EPwm1Regs.ETSEL.all=0x0A;      // Interrupt on PRD 
//	EPwm1Regs.ETPS.all=1;	
//	EPwm1Regs.ETFLG.all=0;
//	EPwm1Regs.ETCLR.all=0;
//	EPwm1Regs.ETFRC.all=0;
//	
//	EPwm1Regs.PCCTL.all=0;
//	
//	EPwm1Regs.TBCTL.all=0x0010+TBCTLVAL; // Enable Timer
//	EPwm1Regs.TBPRD=SP;
	// EPWM Module 1 config
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
   EDIS;

   EPwm2Regs.TBPRD = PRD;       // Set timer period   
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                  // Clear counter

   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading   
   EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;     //WHNE  TBCTR=0,LOAD FROM SHADOW REGS
   EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
   EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
   

   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2 ;   // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;     //TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV)
//   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;

   // Setup shadow register load on ZERO
   
   
   EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;   
 
   //Uint16  INTSEL:3;        // 2:0    EPWMxINTn Select  Uint16  INTEN:1; 
   // Set Compare values
   EPwm2Regs.CMPA.half.CMPA =100;    // Set compare A value

   // Set actions
   
   EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR;            // Set PWM1A on Zero
   EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;          // Clear PWM1A on event A, up count

//   EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // Set PWM1B on Zero
//   EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;          // Clear PWM1B on event B, up count
//   
   EPwm2Regs.ETSEL.bit.INTSEL=4;
   EPwm2Regs.ETSEL.bit.INTEN=1;
   
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
   EDIS;
   
}

void initEpwm1B()
{  

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
   EDIS;

   EPwm1Regs.TBPRD = 65000;       // Set timer period   
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                  // Clear counter

   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading   
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;     //WHNE  TBCTR=0,LOAD FROM SHADOW REGS
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
   EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
   

   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1 ;   // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;     //TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV)
//   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;

   // Setup shadow register load on ZERO
   
   
   
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;   
 

   EPwm1Regs.CMPB = 0;              // Set Compare B value

   // Set actions
   

   EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // Set PWM1B on Zero
   EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;          // Clear PWM1B on event B, up count
//   
   EPwm1Regs.ETSEL.bit.INTSEL=6;
   EPwm1Regs.ETSEL.bit.INTEN=1;
   
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
   EDIS;
   
}


