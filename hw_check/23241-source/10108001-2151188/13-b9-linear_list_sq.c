#include <stdio.h>
#include <stdlib.h>		//malloc/reallocº¯Êý
#if (__linux__)
#include <unistd.h>		//exitº¯Êý
#elif (_MSC_VER)||((__GNUC__)&&(!__linux__))
