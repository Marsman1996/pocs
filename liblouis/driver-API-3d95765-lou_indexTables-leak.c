// This fuzz driver is generated for library liblouis, aiming to fuzz the following
// functions: lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_indexTables at metadata.c:895:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

int
LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	if (Size < 1) return 0;

	// Set up a dummy data path
	lou_setDataPath("./dummy_file");

	// Call lou_indexTables before lou_findTable
	char *tables[] = { "./tables/afr-za-g1.ctb", NULL };
	lou_indexTables(tables);

	// Clean up
	lou_free();	 // Call lou_free without arguments
	return 0;
}