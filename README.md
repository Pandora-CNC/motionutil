# motionutil
CLI for controlling the FPGA registers in the DDCSV1.1 CNC controller

Usage:  mutil <-r[x|b]|-w[x]|-h|-v> [register] [value]

  * -r:   Reads the decimal value from the register specified in decimal
  * -rx:  Reads the hexadecimal value from the register specified in decimal
  * -rb:  Reads the binary value from the register specified in decimal
  * -w:   Writes the decimal value to the register specified in decimal
  * -wx:  Writes the hexadecimal value to the register specified in decimal
  * -h:   Displays this help
  * -v:   Displays the version and copyright
