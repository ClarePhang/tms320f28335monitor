//###########################################################################
//
// FILE:    main.c
//
// TITLE:   TMS320F28335MONITOR
//
// ASSUMPTIONS:
//
//###########################################################################
// $Release Date: 11, 8, 2008 $		BY  HWANG HA YUN
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


void main(void)
{
	char RcvData;
	
	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the DSP2833x_SysCtrl.c file.
	InitSysCtrl();

	// Step 2. Initalize GPIO:
	// This example function is found in the DSP2833x_Gpio.c file and
	// illustrates how to set the GPIO to it's default state.
	InitGpio();
	InitXintf();
	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;
	
	// Initialize PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the DSP2833x_PieCtrl.c file.
	MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	
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

	InitFlash();
	InitSystem();

	VFDMenu("TMS28335");
	PrintMenu();
	TxPrintf("\nMonitor28x35]#");

	while(TRUE)
	{
		
		RcvData = SCIa_RxChar();
		SCIa_TxChar(RcvData);

		switch(RcvData)
		{
			case 'M':
			case 'm':
				PrintMenu();
				break;
				
			case 'A':
			case 'a':
				DeleteAllFlash();
				break;
				
			case 'O':
			case 'o':
				DeleteSecletFlash();
				break;
				
			case 'D':
			case 'd':
				SCItoRamDownloadPrm();

				break;
				
			case 'G':
			case 'g':
				FlashtoRamDownloadPrm();
				break;

			case 'b':
			case 'B':
				FlashBurnPrm();
				FlashtoRamDownloadPrm();
			case CR:
				break;
				
			default:
				TxPrintf("\n=======Wrong Command!!========\n");
				PrintMenu();
				break;
		}
		
		TxPrintf("\nMonitor28x35]#");


		
	}
	

}


void InitSystem(void)
{
	InitSci();
	InitFlashAPI28235();
	InitVFDBright(VfdBright20);
}

void PrintMenu(void)
{
	TxPrintf("\n");
	TxPrintf("========   TMS320F28335 Monitor V1.0   ========\n");	
	TxPrintf("  M  :	Display Menu.\n");
	TxPrintf("  A  :	Delete All Flash.( Sector B - H )\n");
	TxPrintf("  O  :	Delete Select Sector Flash. ( ex. O B )\n");
	TxPrintf("  D  :	RAM down  User Program ( *.Hex )\n");
	TxPrintf("  B  :	Flash down  User Program ( *.Hex )\n");
	TxPrintf("  G  :	Go User Program\n");
	TxPrintf("========   by Hwang Ha-yun Maze 8th   ========\n");	
	TxPrintf("========  blog.naver.com/hhyjjang.do  ========\n");	
	
}

