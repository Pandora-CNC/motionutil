/*
	Motion driver debug application for DDCSV1.1
	(c) 2017 0xbmuessig (bmuessig.eu)
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define VERSION "0.4"

const int IPOLSIZE = 0x100;
const int NUMREGS = 0xFF;

typedef struct {
	unsigned char addr;
	unsigned short data;
} req_data;

int devfp;

int main(unsigned int argc, char* argv[]);
int write_data(unsigned char addr, unsigned short data);
int read_data(unsigned char addr, unsigned short* data);
int read_all_data(unsigned short* buf);
int read_interpolation_data(unsigned char* buf);

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
				req_data buf;
				if(sscanf(argv[2], "%hhu", &buf.addr)) {
					if(read_data(buf.addr, &buf.data) == 0)
						printf("%u\n", buf.data);
					else {
						fprintf(stderr, "ERR: Reading data from address %u failed!\n", buf.addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-rx") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				req_data buf;
				if(sscanf(argv[2], "%hhu", &buf.addr)) {
					if(read_data(buf.addr, &buf.data) == 0)
						printf("0x%x\n", buf.data);
					else {
						fprintf(stderr, "ERR: Reading data from address %u failed!\n", buf.addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-rb") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				req_data buf;
				if(sscanf(argv[2], "%hhu", &buf.addr)) {
					if(read_data(buf.addr, &buf.data) == 0) {
						int i;
						
						printf("0b");
						for(i = (sizeof(buf.data) * 8) - 1; i >= 0 ; i--)
							printf("%c", ((buf.data >> i) & 1) ? '1' : '0');
						printf("\n");
					} else {
						fprintf(stderr, "ERR: Reading data from address %u failed!\n", buf.addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-a") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				FILE* outfile = NULL;
				
				if(strcmp(argv[2], "-") == 0)
					outfile = stdout;
				else
					outfile = fopen(argv[2], "w");
				
				if(!outfile) {
					fprintf(stderr, "ERR: Cannot open file '%s' for writing!\n",
						(strcmp(argv[2], "-") == 0) ? "stdout" : argv[2]);
					programErr = 1;
				} else {
					unsigned short regs[NUMREGS];
					if(read_all_data(regs) != 0) {
						fprintf(stderr, "ERR: Reading the register data failed!\n");
						programErr = 1;
					} else {
						if(!fwrite(regs, NUMREGS, 1, outfile)) {
							fprintf(stderr, "ERR: Writing the register data to file failed!\n");
							programErr = 1;
						}
					}
					
					if(strcmp(argv[2], "-") != 0)
						fclose(outfile);
				}
			}
		} else if(strcmp(argv[1], "-i") == 0) {
			if(argc != 3)
				syntaxErr = 1;
			else {
				FILE* outfile = NULL;
				
				if(strcmp(argv[2], "-") == 0)
					outfile = stdout;
				else
					outfile = fopen(argv[2], "w");
				
				if(!outfile) {
					fprintf(stderr, "ERR: Cannot open file '%s' for writing!\n",
						(strcmp(argv[2], "-") == 0) ? "stdout" : argv[2]);
					programErr = 1;
				} else {
					unsigned char ipoldata[IPOLSIZE];
					if(read_interpolation_data(ipoldata) != 0) {
						fprintf(stderr, "ERR: Reading the interpolation data failed!\n");
						programErr = 1;
					} else {
						if(!fwrite(ipoldata, IPOLSIZE, 1, outfile)) {
							fprintf(stderr, "ERR: Writing the interpolation data to file failed!\n");
							programErr = 1;
						}
					}
					
					if(strcmp(argv[2], "-") != 0)
						fclose(outfile);
				}
			}
		} else if(strcmp(argv[1], "-w") == 0) {
			if(argc != 4)
				syntaxErr = 1;
			else {
				req_data buf;
				if(sscanf(argv[2], "%hhu", &buf.addr) && sscanf(argv[3], "%hu", &buf.data)) {
					if(write_data(buf.addr, buf.data) != 0) {
						fprintf(stderr, "ERR: Writing data %d to address %u failed!\n", buf.data, buf.addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-wx") == 0) {
			if(argc != 4)
				syntaxErr = 1;
			else {
				req_data buf;
				if(sscanf(argv[2], "%hhu", &buf.addr) && sscanf(argv[3], "%hx", &buf.data)) {
					if(write_data(buf.addr, buf.data) != 0) {
						fprintf(stderr, "ERR: Writing data 0x%x to address %u failed!\n", buf.data, buf.addr);
						programErr = 1;
					}
				} else
					syntaxErr = 1;
			}
		} else if(strcmp(argv[1], "-v") == 0) {
			if(argc != 2)
				syntaxErr = 1;
			else {
				puts("Motion Utility (mutil)");
				puts("Version " VERSION);
				puts("(c) 2017 0xbmuessig (bmuessig.eu)");
			}
		} else
			syntaxErr = 1;
	}
	
	if(syntaxErr)
		fprintf(stderr, "Invalid syntax!\n\n");
	if(syntaxErr || showHelp) {
		puts("Usage:");
		puts("mutil -r[x|b] <register>");
		puts("mutil -w[x] <register> <value>");
		puts("mutil <-a|-i> <file|->");
		puts("mutil <-h|-v>\n");
		
		if(showHelp) {
			puts("Synopsis:");
			puts("mutil allows you to read and write the DDCSV1.1 FPGA registers\n");
		}
		
		puts("Commands:");
		puts("\t-r: \tReads the decimal value from the register specified in decimal");
		puts("\t-rx: \tReads the hexadecimal value from the register specified in decimal");
		puts("\t-rb: \tReads the binary value from the register specified in decimal");
		puts("\t-a: \tReads all registers and writes them to a file or to - (stdout)");
		puts("\t-i: \tReads the raw interpolation data and writes it to a file or to - (stdout)");
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

int write_data(unsigned char addr, unsigned short data)
{
	req_data buf;
	buf.addr = addr;
	buf.data = data;
	
	return ioctl(devfp, 0x4620u, &buf);
}

int read_data(unsigned char addr, unsigned short* data)
{
	req_data buf;
	buf.addr = addr;
	
	if(!data)
		return -1;
	
	int ret;
	
	if((ret = ioctl(devfp, 0x4621u, &buf)) != -1)
		*data = buf.data;
	
	return ret;
}

int read_all_data(unsigned short* buf)
{
	unsigned short i;
	
	for(i = 0; i < NUMREGS; i++) {
		if(read_data(i, &buf[i]) != 0)
			return -1;
	}
	
	return 0;
}

int read_interpolation_data(unsigned char* buf)
{
	if(!buf)
		return -1;
	
	return ioctl(devfp, 0x4622u, buf);
}
