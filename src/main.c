#include "common.h"
#include "vm.h"

int
main(int argc, const char* argv[])
{
	signal(SIGINT, handle_interrupt);
	disable_input_buffering();
	if ( argc < 2 )
	{
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}

	for (int i = 1; i < argc; ++i)
	{
		if (!VM_ReadImage(argv[i]))
		{
			printf("Failed to read image: %s\n", argv[i]);
			exit(1);
		}
	}

	VM_Init();

	VM_Run();

	int res =  VM_Shutdown();
	restore_input_buffering();

	return res;
}