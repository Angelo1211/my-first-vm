#include "common.h"

#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

struct termios original_tio;

void 
handle_interrupt(int signal)
{
	restore_input_buffering();
	printf("\n");
	exit(-2);
}

void 
disable_input_buffering()
{
	tcgetattr(STDIN_FILENO, &original_tio);
	struct termios new_tio = original_tio;
	new_tio.c_lflag &= ~ICANON & ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void 
restore_input_buffering()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

u16 
check_key()
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

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