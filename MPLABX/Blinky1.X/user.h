#include "leds.h"
//#include "lcd.h"
//#include "adc.h"
#include "buttons.h"
//#include "timer_1ms.h"
//#include "print_lcd.h"

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */

typedef struct
{
    /* Variables used by Timer module */
    volatile unsigned char hours ;
    volatile unsigned char minutes ;
    volatile unsigned char seconds ;
    volatile unsigned char hunds ;
    volatile unsigned char tens ;
    volatile unsigned char ones ;
    volatile unsigned char rtc_lcd_update ;
    
    /* variables used in raw ADC data to hex */
    volatile unsigned char adones ;
    volatile unsigned char adtens ;
    volatile unsigned char adhunds ;
    volatile unsigned char adthous ;
    volatile unsigned int  temp1 ;
    volatile unsigned char adc_lcd_update ;

    /*arrays used for Explorer 16 LCD display*/
    char messageLine1[18] ;
    char messageLine2[18]  ;
    char messageTime[18] ;
    char messageADC[18] ;

} APP_DATA ;

extern APP_DATA appData ;

#define __PROGRAM1_BASE 0xC00;
#define __PROGRAM2_BASE