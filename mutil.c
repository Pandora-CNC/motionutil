/*
	Motion driver debug application for DDCSV1.1
	(c) 2017 0xbmuessig (bmuessig.eu)
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define VERSION "0.3"

typedef struct {
	unsigned int address;
	unsigned int data;
} req_data;

int devfp;

int main(unsigned int argc, char* argv[]);
int write_data(unsigned int addr, unsigned int data);
int read_data(unsigned int addr, unsigned int* data);

int main(unsigned int argc, char* argv[])
{
	if((devfp = open("/dev/motion", O_RDWR)) == -1) {
		fprintf(stderr, "ERR: Failed to open the motion driver!\n");
		exit(EXIT_FAILURE);
	}
	
	int syntaxErr = 0, programErr = 0, showHelp = 0;
	if(argc < 2)
		syntaxErr = 1;
	else {
		if(strcmp(argv[1], "-h") == 0) {
			if(argc != 2)
				syntaxErr = 1;
			showHelp = 1;
		} else if(strcmp(argv[1], "-r") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				unsigned int addr, data;
				if(sscanf(argv[2], "%u", &addr)) {
					if(read_data(addr, &data) == 0)
						printf("%u\n", data);
					else {
						fprintf(stderr, "ERR: Reading data from address %u failed!\n", addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-rx") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				unsigned int addr, data;
				if(sscanf(argv[2], "%u", &addr)) {
					if(read_data(addr, &data) == 0)
						printf("0x%x\n", data);
					else {
						fprintf(stderr, "ERR: Reading data from address %u failed!\n", addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-rb") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				unsigned int addr, data;
				if(sscanf(argv[2], "%u", &addr)) {
					if(read_data(addr, &data) == 0) {
						int i;
						
						printf("0b");
						for(i = (sizeof(data) * 8) - 1; i >= 0 ; i--)
							printf("%c", ((data >> i) & 1) ? '1' : '0');
						printf("\n");
					} else {
						fprintf(stderr, "ERR: Reading data from address %u failed!\n", addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-w") == 0) {
			if(argc != 4)
				syntaxErr = 1;
			else {
				unsigned int addr, data;
				if(sscanf(argv[2], "%u", &addr) && sscanf(argv[3], "%u", &data)) {
					if(write_data(addr, data) != 0) {
						fprintf(stderr, "ERR: Writing data %d to address %u failed!\n", data, addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-wx") == 0) {
			if(argc != 4)
				syntaxErr = 1;
			else {
				unsigned int addr, data;
				if(sscanf(argv[2], "%u", &addr) && sscanf(argv[3], "%x", &data)) {
					if(write_data(addr, data) != 0) {
						fprintf(stderr, "ERR: Writing data 0x%x to address %u failed!\n", data, addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-v") == 0) {
			if(argc != 2)
				syntaxErr = 1;
			else {
				puts("mutil v" VERSION);
				puts("(c) 2017 0xbmuessig (bmuessig.eu)");
			}
		} else
			syntaxErr = 1;
	}
	
	if(syntaxErr)
		fprintf(stderr, "Invalid syntax!\n\n");
	if(syntaxErr || showHelp) {
<<<<<<< HEAD
		puts("Usage:");
		puts("mutil -r[x|b] <register>");
		puts("mutil -w[x] <register> <value>");
		puts("mutil <-a|-i> <file|->");
		puts("mutil <-h|-v>\n");
=======
		puts("mutil <-r[x|b]|-w[x]|-h|-v> [register] [value]\n");
>>>>>>> parent of 2b6a7ba... v.0.4, Added new features and parameters
		
		if(showHelp) {
			puts("Synopsis:");
			puts("mutil allows you to read and write the DDCSV1.1 FPGA registers\n");
		}
		
		puts("Commands:");
		puts("\t-r: \tReads the decimal value from the register specified in decimal");
		puts("\t-rx: \tReads the hexadecimal value from the register specified in decimal");
		puts("\t-rb: \tReads the binary value from the register specified in decimal");
		puts("\t-w: \tWrites the decimal value to the register specified in decimal");
		puts("\t-wx: \tWrites the hexadecimal value to the register specified in decimal");
		puts("\t-h: \tDisplays this help");
		puts("\t-v: \tDisplays the version and copyright");
	}
	
	close(devfp);
	
	if(syntaxErr || programErr)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int write_data(unsigned int addr, unsigned int data)
{
	req_data buf;
	buf.address = addr;
	buf.data = data;
	
	return ioctl(devfp, 0x4620u, &buf);
}

int read_data(unsigned int addr, unsigned int* data)
{
	req_data buf;
	buf.address = addr;
	
	int ret;
	
	if((ret = ioctl(devfp, 0x4621u, &buf)) != -1)
		*data = buf.data;
	
	return ret;
}
