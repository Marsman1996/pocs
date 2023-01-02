#include <stdint.h>
#include <stdio.h>

#include "ming.h"

/* Structure definition needed by the fuzz driver */
struct SWFShape_s
{
	char character[104];
	void *records;
	int nRecords;
	void *out;
	int xpos;
	int ypos;
	void *lines;
	void *fills;
	byte nLines;
	byte nFills;
	short lineWidth;
	unsigned char isMorph;
	unsigned char isEnded;
	int useVersion;
	unsigned char flags;
	void *edgeBounds;
	void *gcnode;
};

void* AFG_alloc_list[2] = { NULL };
int AFG_alloc_cnt = 0;
FILE* AFG_fopen_list[1] = { NULL };
int AFG_fopen_cnt = 0;

int AFG_func(struct SWFShape_s* shape,double bx,double by,double cx,double cy,double dx,double dy)
{
	SWFShape_drawCubic(shape, bx, by, cx, cy, dx, dy); 
	return 0;
}

#ifdef __cplusplus
extern "C" 
#endif
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{
	size_t AFG_offset = 0, minimum_size = sizeof(double)*6+sizeof(struct SWFShape_s)*1;
	if(Size <= minimum_size) {
		// printf("[Error]: Cannot pass size check %ld < %ld\n", Size, minimum_size);
		return 0;
	}

	struct SWFShape_s * shape;
	shape = (struct SWFShape_s*)malloc(sizeof(struct SWFShape_s));
	AFG_alloc_list[AFG_alloc_cnt++] = (void*) shape;
		memcpy((void *)&shape->nRecords, Data+AFG_offset, sizeof(int));
		AFG_offset+=sizeof(int);
		memcpy((void *)&shape->useVersion, Data+AFG_offset, sizeof(int));
		AFG_offset+=sizeof(int);
		memcpy((void *)&shape->xpos, Data+AFG_offset, sizeof(int));
		AFG_offset+=sizeof(int);
		memcpy((void *)&shape->ypos, Data+AFG_offset, sizeof(int));
		AFG_offset+=sizeof(int);
		memcpy((void *)&shape->lineWidth, Data+AFG_offset, sizeof(short));
		AFG_offset+=sizeof(short);
		memcpy((void *)&shape->flags, Data+AFG_offset, sizeof(unsigned char));
		AFG_offset+=sizeof(unsigned char);
		memcpy((void *)&shape->isEnded, Data+AFG_offset, sizeof(unsigned char));
		AFG_offset+=sizeof(unsigned char);
		memcpy((void *)&shape->isMorph, Data+AFG_offset, sizeof(unsigned char));
		AFG_offset+=sizeof(unsigned char);
		memcpy((void *)&shape->nFills, Data+AFG_offset, sizeof(unsigned char));
		AFG_offset+=sizeof(unsigned char);
		memcpy((void *)&shape->nLines, Data+AFG_offset, sizeof(unsigned char));
		AFG_offset+=sizeof(unsigned char);
	double  bx;
	memcpy((void *)&bx, Data+AFG_offset, sizeof(double));
	AFG_offset+=sizeof(double);
	double  by;
	memcpy((void *)&by, Data+AFG_offset, sizeof(double));
	AFG_offset+=sizeof(double);
	double  cx;
	memcpy((void *)&cx, Data+AFG_offset, sizeof(double));
	AFG_offset+=sizeof(double);
	double  cy;
	memcpy((void *)&cy, Data+AFG_offset, sizeof(double));
	AFG_offset+=sizeof(double);
	double  dx;
	memcpy((void *)&dx, Data+AFG_offset, sizeof(double));
	AFG_offset+=sizeof(double);
	double  dy;
	memcpy((void *)&dy, Data+AFG_offset, sizeof(double));
	AFG_offset+=sizeof(double);

shape->nRecords=0;
shape->records=NULL;
shape->isEnded=1;

	AFG_func(shape, bx, by, cx, cy, dx, dy);

	for(int AFG_free_i = 0; AFG_free_i < AFG_alloc_cnt; AFG_free_i++)
		free(AFG_alloc_list[AFG_free_i]);
	for(int AFG_free_i = 0; AFG_free_i < AFG_fopen_cnt; AFG_free_i++)
		fclose(AFG_fopen_list[AFG_free_i]);
	AFG_alloc_cnt = 0;
	AFG_fopen_cnt = 0;

	return 0;
}
