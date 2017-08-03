"""
Readdresses hex file some given offset
"""
import sys


def twos_comp(val):
    val = val ^ 0xff
    val = val + 1
    return val

def chksum(w):
    s = 0
    for i in xrange(len(w)/2):
        s += int(w[2*i : 2*i+2], 16)
    sum_lsb = format(s, '02x')[-2:]       # Get hex format of the sum so to grab the LSB
    cval = twos_comp(int(sum_lsb, 16))    # Take the 2's comp of the LSB
    output = format(cval, '02x')[-2:]     # Convert 2's comp int into hex string
    return output



fin = file('b.hex', 'r')
fout = file('out.hex', 'w')

offset = int('4000' ,16)*2   # offset argument in hex, *2 for bytes instead of words

new_word = ''

for line in fin:
    if int(line[7:9], 16) is 0:
        addr = int(line[3:7], 16)
    
        # Addr(h>d) += offset(h>d)
        addr += offset
        addr = format(addr, '04x')
    
        # Write out modified line to new file
        new_word = line[1:3] + addr + line[7:-3]
    
        cs = chksum(new_word)

        fout.write(':' + new_word + cs + '\n')
    else:
        fout.write(line)

	
fin.close()
fout.close()


