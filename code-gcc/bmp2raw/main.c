/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
///#include <unistd.h>
///#include <pthread.h>

///#include <signal.h>
///#include <sys/time.h>


#include "helper.h"
///#include "tasks.h"
///#include "usertasks.h"


typedef struct _lbheader{
    unsigned short identifier;      // 0x0000
    unsigned int filesize;          // 0x0002
    unsigned int reserved;          // 0x0006
    unsigned int bitmap_dataoffset; // 0x000A
    unsigned int bitmap_headersize; // 0x000E
    unsigned int width;             // 0x0012
    unsigned int height;            // 0x0016
    unsigned short planes;          // 0x001A
    unsigned short bits_perpixel;   // 0x001C
    unsigned int compression;       // 0x001E
    unsigned int bitmap_datasize;   // 0x0022
    unsigned int hresolution;       // 0x0026
    unsigned int vresolution;       // 0x002A
    unsigned int usedcolors;        // 0x002E
    unsigned int importantcolors;   // 0x0032
    unsigned int palette;           // 0x0036
} __attribute__((packed,aligned(1))) lbheader;


int main( int argc, char *argv[] ) {
	int iRet = -1;
	FILE * pFileIn;
	FILE * pFileOut;
	size_t result;
	
	lbheader hbmp;
	struct{
		unsigned short r:5;
		unsigned short g:6;
		unsigned short b:5;
	} rgb16;

	unsigned char rgb24[3];


	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

    if( argc < 3 ) {
		printf("]]] Warning !!\n" );
		printf("]]] Usage: %s infile [outfile]\n", argv[0] );
		printf("]]]     \n" );
		iRet = -1;
		goto _EXIT;
    }

	pFileIn = fopen ( argv[1] , "rb" );
	if( pFileIn == NULL ) {
		printf("]]] %s File open error !!\n", argv[1] );
		iRet = -2;
		goto _EXIT;
	}
/*
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
*/

	result = fread( &hbmp, sizeof(lbheader), 1, pFileIn );
	if( result != 1 ) {
		printf("]]] Reading error !! [%d]\n", (int)result );
		iRet = -3;
		goto _EXIT;
	}

	printf("]]] BMP header::\n" );
	printf("]]] identifier=0x%04X[%c%c]\n", hbmp.identifier, hbmp.identifier>>8, hbmp.identifier );
	printf("]]] filesize=%d bytes\n", hbmp.filesize );
	printf("]]] bitmap_dataoffset=%d bytes\n", hbmp.bitmap_dataoffset );
	printf("]]] bitmap_headersize=%d bytes\n", hbmp.bitmap_headersize );
	printf("]]] width=%d\n", hbmp.width );
	printf("]]] height=%d\n", hbmp.height );
	printf("]]] bits_perpixel=%d\n", hbmp.bits_perpixel );
	printf("]]] compression=%d\n", hbmp.compression );
	printf("]]] bitmap_datasize=%d\n", hbmp.bitmap_datasize );
	printf("]]] hresolution=%d\n", hbmp.hresolution );
	printf("]]] vresolution=%d\n", hbmp.vresolution );
	printf("]]] usedcolors=%d\n", hbmp.usedcolors );
	printf("]]] palette=%d\n", hbmp.palette );

	pFileOut = fopen ( argv[2] , "wb" );
	if( pFileOut == NULL ) {
		printf("]]] %s File open error !!\n", argv[2] );
		iRet = -4;
		goto _EXIT;
	}

	rewind( pFileIn );
	fseek( pFileIn, hbmp.bitmap_dataoffset, SEEK_SET);

	while( fread( rgb24, 1, 3, pFileIn ) ) {
		rgb16.r = rgb24[2]>>3;
		rgb16.g = rgb24[1]>>2;
		rgb16.b = rgb24[0]>>3;
		fwrite( &rgb16, 1, 2, pFileOut );
	}	///while

_EXIT:
	// terminate
	fclose( pFileIn );
	fclose( pFileOut );

	_SPMSG(dINIT, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}



