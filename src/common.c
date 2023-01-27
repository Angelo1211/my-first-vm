#include "common.h"

char *
Read_Entire_File(char *file_name, size_t *size)
{
	char *result = 0;

	FILE *file = fopen(file_name, "r");
	if ( file )
	{
		// Seek to the end of the file
		fseek(file, 0, SEEK_END);

		// Tell me where that is and store it
		size_t file_size = ftell(file);

		// Seek back to the front of the file
		fseek(file, 0, SEEK_SET);

		// Extra space for the null terminator
		result = malloc(file_size + 1);

		if ( result )
		{
			size_t read = fread(result, sizeof(char), file_size, file);
			if ( read )
			{
				*size = read;
				result[read] = 0;
			}
		}

		fclose(file);
	}

	return result;
}