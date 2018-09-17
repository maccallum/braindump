#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <float.h>
#include <math.h>

// #define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
// #define BYTE_TO_BINARY(byte)  \
// 	(byte & 0x80 ? '1' : '0'), \
// 		(byte & 0x40 ? '1' : '0'), \
// 		(byte & 0x20 ? '1' : '0'), \
// 		(byte & 0x10 ? '1' : '0'), \
// 		(byte & 0x08 ? '1' : '0'), \
// 		(byte & 0x04 ? '1' : '0'), \
// 		(byte & 0x02 ? '1' : '0'), \
// 		(byte & 0x01 ? '1' : '0')

#define DEF_NCHANNELS 64
#define DEF_CHANNEL 0

int main(int argc, char **argv)
{
	if(argc == 1){
		printf("braindump <filename> <channel=0> <nchannels=64> <format=0 (0->column of binary values, 1->max coll, 2->column of text values)>\n");
		printf("\nto write an aiff file, use ffmpeg like this: braindump <filename> <channel> <nchannels> 0 | ffmpeg -y -f s16le -ar 44100 -ac 1 -i - <outfilename>.aiff\n");
		exit(0);
	}
	char *filename = argv[1];
	int channel = DEF_CHANNEL;
	if(argc > 2){
		char *endp = NULL;
		channel = strtol(argv[2], &endp, 0);
	}
	int nchannels = DEF_NCHANNELS;
	if(argc > 3){
		char *endp = NULL;
		nchannels = strtol(argv[3], &endp, 0);
	}
	int ofmt = 0;
	if(argc > 4){
		char *endp = NULL;
		ofmt = strtol(argv[4], &endp, 0);
	}

	FILE *fp = fopen(filename, "rb");
	if(!fp){
		printf("couldn't open file\n");
		exit(0);
	}

	fseek(fp, 0L, SEEK_END);
	size_t sz = ftell(fp);
	sz /= 2; // 16 bits	
	fseek(fp, 0L, SEEK_SET);
	int16_t *buf = (int16_t *)malloc(sizeof(int16_t) * sz);
	fread(buf, 2, sz, fp);
	
	switch(ofmt){
	case 0: // binary output for ffmpeg or something
		for(int i = channel; i < sz; i += nchannels){
			fwrite((void *)(buf + i), 2, 1, stdout);
		}
		break;
	case 1: // max coll
		for(int i = channel, j = 0; i < sz; i += nchannels, j++){
			printf("%d, %d;\n", j, buf[i]);
		}
		break;
	case 2: // numbers
		for(int i = channel; i < sz; i += nchannels){
			printf("%d\n", buf[i]);
		}
		break;
		
	}
	

	fclose(fp);
	return 0;
}
