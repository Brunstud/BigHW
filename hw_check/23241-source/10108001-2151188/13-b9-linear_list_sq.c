#include <stdio.h>
#include <stdlib.h>		//malloc/realloc����
#if (__linux__)
#include <unistd.h>		//exit����
#elif (_MSC_VER)||((__GNUC__)&&(!__linux__))
