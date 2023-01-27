#include "common.h"
#include "vm.h"

int
main(int argc, const char* argv[])
{
	if ( argc < 2 )
	{
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}

	for (int i = 1; i < argc; ++i)
	{
		if (!read_image(argv[i]))
		{
			printf("Failed to read image: %s\n", argv[i]);
			exit(1);
		}
	}

	VM_Init();

	VM_Run();

	return VM_Shutdown();
}