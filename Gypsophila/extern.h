#include <stdio.h>
#include <curl/curl.h>
#define BUFFER_MAX_SIZE 1024
#define CACHE_INIT_SIZE 1024 * 32
#define CACHE_DEC_SIZE 1024 * 16

extern CURL *curl_web_handler;
extern char username[];
extern char passwd[];
extern char tsinghua_prefix[];

