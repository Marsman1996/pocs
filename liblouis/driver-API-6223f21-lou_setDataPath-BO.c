#include "liblouis.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Structure definition needed by the fuzz driver */

void* AFG_alloc_list[2] = { NULL };
int AFG_alloc_cnt = 0;
FILE* AFG_fopen_list[1] = { NULL };
int AFG_fopen_cnt = 0;

int AFG_func(char* path)
{
	lou_setDataPath(path); 
	return 0;
}

size_t minimum_size = 0;

#ifdef __cplusplus
extern "C" 
#endif
int LLVMFuzzerInitialize(int *argc, char ***argv) {
	printf("Minimum size is %ld\n", minimum_size);
	return 0;
}
#ifdef __cplusplus
extern "C" 
#endif
int LLVMFuzzerTestOneInput(const uint8_t *AFG_Data, size_t Size) {
	size_t AFG_offset = 0;
	size_t pt_size = (Size - minimum_size) / 1;
	if (pt_size < sizeof(char) ) { return -1; }
	char * path;
	path = (char *)malloc(pt_size+1);
	AFG_alloc_list[AFG_alloc_cnt++] = (void*) path;
	memcpy((void *)path, AFG_Data+AFG_offset, pt_size);
	path[pt_size] = '\0';
	AFG_offset+=pt_size;

	AFG_func(path);

AFG_fail:
	for(int AFG_free_i = 0; AFG_free_i < AFG_alloc_cnt; AFG_free_i++)
		free(AFG_alloc_list[AFG_free_i]);
	for(int AFG_free_i = 0; AFG_free_i < AFG_fopen_cnt; AFG_free_i++)
		fclose(AFG_fopen_list[AFG_free_i]);
	AFG_alloc_cnt = 0;
	AFG_fopen_cnt = 0;

	return 0;
}