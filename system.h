/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        8000000L
#define FCY             SYS_FREQ/2

/* Device Flash Memory Configuration */
#define PM_INSTR_SIZE 		4		//bytes per instruction 
#define PM_ROW_SIZE 		256  	//user flash row size in bytes 
#define PM_PAGE_SIZE 		2048 	//user flash page size in bytes


/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

