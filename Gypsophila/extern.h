#include <stdio.h>
#include <curl/curl.h>
#define BUFFER_MAX_SIZE 1024
#define CACHE_INIT_SIZE 1024 * 32
#define CACHE_DEC_SIZE 1024 * 16

__declspec(dllexport) extern char tsinghua_prefix[];

