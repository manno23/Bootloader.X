/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#include <xc.h>
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

WORD frigbot_status_flags __attribute__((space(data),address(0x800)));

WORD __prog1_addr_start = 0xC00;
WORD __prog1_addr_length = 0x2000;
WORD __prog2_addr_start = 0x4C00;
WORD __prog2_addr_length = 0x2000;

void WriteMemRow();
void OverwriteNewFirmware();
void WriteLatch(WORD page, WORD addrLo, WORD dataHi, WORD dataLo);
DWORD ReadLatch(WORD page, WORD addrLo);
void Erase(WORD page, WORD addrLo);
void ReadPM(BYTE buffer[], WORD length, DWORD_VAL sourceAddr);

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int main(void)
{  
    /* Switch to alternate interrupt table for the boot program */
    _ALTIVT = 1;
    
    /* Read in configuration from program data location. */
    frigbot_status_flags = ReadLatch(0x0000, 0x0800);

    if (frigbot_status_flags & 0x0001) 
    {      
        //DANGER!!
        OverwriteNewFirmware();
        frigbot_status_flags = 0;       // Clear new firmware flag
        
        WriteLatch(0x0000, 0x0800, 0x0000, frigbot_status_flags);
        WriteMemWord();
        
    }
    
    /* Use main interrupt table for the main program */
    _ALTIVT = 0;
    
    /* Continue to program location */
    asm("goto 0xC00");
    
}

/********************************************************************
; Function: 	overwrite_new_firmware
;
; PreCondition: We are assuming the program was verified in flash before
;               setting the NEW_FIRMWARE_AVAILABLE flag.
;
; Input:    	None.
;                               
; Output:   	None.
;
; Side Effects: Original program is overwritten.
;
; Overview: 	Reads and writes program2 from reserved flash location 0x2C00 
;               one hex record at a time.
;               Erased and copies in 0x400 size blocks.
;
;*********************************************************************/	
void OverwriteNewFirmware()
{
    DWORD_VAL prog1_location;
    prog1_location.Val = __prog1_addr_start;
    DWORD_VAL prog2_location;
    prog2_location.Val = __prog2_addr_start;
    int erasure_count;
    
    for(erasure_count = 0;
        erasure_count < __prog2_addr_length / 0x400;
        erasure_count++)
    {
        
        prog1_location.Val += 0x400 * erasure_count;
        prog2_location.Val += 0x400 * erasure_count;
        
        Erase(prog1_location.word.HW, prog1_location.word.LW);
        
        int i, j;
        DWORD_VAL instruction;
        for(i=0; i<8; i++) {
            for(j=0; j<64; j++)
            {
 
                instruction.Val = ReadLatch(prog2_location.word.HW, prog2_location.word.LW);
                WriteLatch(prog1_location.word.HW, prog1_location.word.LW,
                   instruction.word.HW, instruction.word.LW);
            
                prog1_location.Val += 2;
                prog2_location.Val += 2;
            }
            WriteMemRow(); // Writes 64 instructions at a time.
        }
  
    }
}


/********************************************************************
* Function:     void ReadPM(WORD length, DWORD_VAL sourceAddr)
*
* PreCondition: None
*
* Input:		length		- number of instructions to read
*				sourceAddr 	- address to read from
*
* Output:		None
*
* Side Effects:	Puts read instructions into buffer.
*
* Overview:		Reads from program memory, stores data into buffer. 
*
* Note:			None
********************************************************************/
void ReadPM(BYTE buffer[], WORD length, DWORD_VAL sourceAddr)
{
	WORD bytesRead = 0;
	DWORD_VAL temp;

	//Read length instructions from flash
	while(bytesRead < length*PM_INSTR_SIZE)
	{
		//read flash
		temp.Val = ReadLatch(sourceAddr.word.HW, sourceAddr.word.LW);

		buffer[bytesRead+5] = temp.v[0];   	//put read data onto 
		buffer[bytesRead+6] = temp.v[1];	//response buffer
		buffer[bytesRead+7] = temp.v[2];
		buffer[bytesRead+8] = temp.v[3];
	
		//4 bytes per instruction: low word, high byte, phantom byte
		bytesRead+=PM_INSTR_SIZE; 

		sourceAddr.Val = sourceAddr.Val + 2;  //increment addr by 2
	}
}


/********************************************************************
; Function: 	DWORD ReadLatch(WORD page, WORD addrLo)
;
; PreCondition: None.
;
; Input:    	page 	- upper byte of address
;				addrLo 	- lower word of address
;                               
; Output:   	data 	- 32-bit data in W1:W0
;
; Side Effects: TBLPAG changed
;
; Overview: 	Read from location in flash memory
;*********************************************************************/
DWORD ReadLatch(WORD page, WORD addrLo)
{
	DWORD_VAL temp;

	TBLPAG = page;

	temp.word.LW = __builtin_tblrdl(addrLo);
	temp.word.HW = __builtin_tblrdh(addrLo);

	return temp.Val;
}

/********************************************************************
; Function: 	void Erase(WORD page, WORD addrLo, WORD cmd);
;
; PreCondition: None.
;
; Input:    	page 	- upper byte of address
;				addrLo 	- lower word of address
;				cmd		- type of memory operation to perform
;                               
; Output:   	None.
;
; Side Effects: TBLPAG changed
;
; Overview: 	Erases page of flash memory at input address
*********************************************************************/	
void Erase(WORD page, WORD addrLo)
{
	WORD temp;	
	temp = TBLPAG;
	TBLPAG = page;

	NVMCON = 0x4042;

	__builtin_tblwtl(addrLo,addrLo);

	__builtin_write_NVM();


	while(NVMCONbits.WR == 1);

	TBLPAG = temp;
}

/********************************************************************
; Function: 	void WriteMem(WORD cmd)
;
; PreCondition: Appropriate data written to latches with WriteLatch
;
; Input:    	cmd - type of memory operation to perform
;                               
; Output:   	None.
;
; Side Effects: 
;
; Overview: 	Write stored registers to flash memory
;*********************************************************************/
void WriteMemRow()
{
	NVMCON = 0x4001;

	__builtin_write_NVM();

	while(NVMCONbits.WR == 1);

}	

/********************************************************************
; Function: 	void WriteMem(WORD cmd)
;
; PreCondition: Appropriate data written to latches with WriteLatch
;
; Input:    	cmd - type of memory operation to perform
;                               
; Output:   	None.
;
; Side Effects: 
;
; Overview: 	Write stored registers to flash memory
;*********************************************************************/
void WriteMemWord()
{
	NVMCON = 0x4003;

	__builtin_write_NVM();

	while(NVMCONbits.WR == 1);

}	

/********************************************************************
; Function: 	void WriteLatch(WORD page, WORD addrLo, 
;				        		WORD dataHi, WORD dataLo)
;
; PreCondition: None.
;
; Input:    	page 	- upper byte of address
;				addrLo 	- lower word of address
;				dataHi 	- upper byte of data
;				addrLo	- lower word of data
;                               
; Output:   	None.
;
; Side Effects: TBLPAG changed
;
; Overview: 	Stores data to write in hardware latches
;*********************************************************************/	
void WriteLatch(WORD page, WORD addrLo, WORD dataHi, WORD dataLo)
{
	TBLPAG = page;

	__builtin_tblwtl(addrLo,dataLo);
	__builtin_tblwth(addrLo,dataHi);
	
}	