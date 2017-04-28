# motionutil
CLI for controlling the FPGA registers in the DDCSV1.1 CNC controller

Usages:
  * mutil -r[x|b] <register>
  * mutil -w[x] <register> <value>
  * mutil <-a|-i> <file|->
  * mutil <-h|-v>

Commands:
  * -r:   Reads the decimal value from the register specified in decimal
  * -rx:  Reads the hexadecimal value from the register specified in decimal
  * -rb:  Reads the binary value from the register specified in decimal
  * -a:   Reads all registers and writes them to a file or to - (stdout)
		* -i:   Reads the raw interpolation data and writes it to a file or to - (stdout)
  * -w:   Writes the decimal value to the register specified in decimal
  * -wx:  Writes the hexadecimal value to the register specified in decimal
  * -h:   Displays information about the utility and the commands
  * -v:   Displays the version and copyright
