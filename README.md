Bootloader for Frigbot
www.frigbot.com

This is a custom created bootloader for a PIC24FJ308 device that served as the means for remote updates.
I made use of the secondary interrupt table here so as to seperate it from the main programs.
All the logic for writing and reading from flash memory is included here also.

Also included is the hexup.py script I made that was run against the program installer binaries to realign the
position they would be installed to in memory.

