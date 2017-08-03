/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

#define LED_DELAY 4000

uint16_t frigbot_status_flags __attribute__((space(data),address(0x800)));


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int main(void)
{

    /* Configure the oscillator for the device */
    // ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();

    while (true) 
    {
        
        LED_Toggle(LED_D3);
        uint16_t i = 0;
        for(i; i<LED_DELAY; i++) 
        {
            ;
        }

        /* Pressing the button here simulates the programs handling of receiving 
         * the new firmware from the server and having it placed at the 
         * specified memory location. */
        if( !BUTTON_IsPressed(BUTTON_S3) ) 
        {
            frigbot_status_flags = 1;   // set new firmware flag
            RCON = 0;                   // clear the reset control flag
            _ALTIVT = 0;
            asm("goto 0x200");          // reset to bootloader
        }
    }
    
    return 0;
    
}
