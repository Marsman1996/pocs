// This fuzz driver is generated for library liblouis, aiming to fuzz the following
// functions: lou_indexTables at metadata.c:895:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

int
LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	if (Size < 1) return 0;

	// Initialize variables
	char *tableList = "./dummy_file";
	FILE *fp = fopen("./dummy_file", "wb");
	fwrite(Data, 1, Size, fp);
	fclose(fp);

	// Call lou_indexTables
	const char *tablesArray[] = { tableList, NULL };
	lou_indexTables(tablesArray);

	lou_free();
	return 0;
}