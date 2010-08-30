#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "extern.h"

// HTTP Response code
#define FILE_EXISTS 200
#define PATH_SPILIT_CHAR '\\'

// getpasswd
// GetPassword
#ifndef WIN32
#include <termio.h>
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
int getch(void);
#else
#include <conio.h>
#endif

#define BACKSPACE 8
#define ENTER     13
#define ALARM     7
// return the passwd pointer
#ifdef WIN32
__declspec(dllexport) char *getPasswd(const char *prompt, char *passwd, int MAX_LENGTH);
#else
char *getPasswd(const char *prompt, char *passwd, int MAX_LENGTH);
#endif
// Output Struct
typedef struct _OutStruct{
	char *filename;
	FILE *stream;
} OutStruct;


__declspec(dllexport) typedef enum _course_type
{
	CUR_COURSE,NEXT_COURSE,PRE_COURSE
}course_type;

typedef struct _list_entity
{
	char *entity;
	struct _list_entity* next;
}list_entity, *p_list_entity;


__declspec(dllexport)  typedef struct _cache_memory 
{
	char *mem;
	int offset;
	size_t size;
}cache_memory;

__declspec(dllexport)  typedef struct _course 
{
	int course_id;
	char *course_name;
	char *course_term;
	course_type type;
	struct _course* next;
}course,*p_course;

#ifdef WIN32
/* The following functions are taken with modification from the DJGPP
* port of tar 1.12. They use algorithms originally from DJTAR. */
char *msdosify ( char *file_name);
#endif
char *rindex(char *src, char ch);

// String Operation
bool str_add_prefix(const char *prefix, char *str);

/*
Callback write function.
*/
size_t echo_off(void *buf, size_t size, size_t nmemb, void * user_p);
size_t print_data(void *buf, size_t size, size_t nmemb, void * user_p);
size_t copy_to_buffer(void *buf, size_t size, size_t nmemb, void *user_p);
void * my_realloc(void *origin_ptr, size_t size);
size_t write_to_file(void *buf, size_t size, size_t nmemb, void *user_p);

/*
Some userful Functions
*/
bool copy_web_page_to_memory(CURL * curl_web_handler, char *url,void *parse_function, void *userdata);
//size_t save_course_list(void *buf, size_t size, size_t nmemb, void *user_p);
//size_t save_course_file(void *buf, size_t size, size_t nmemb, void *user_p);
//size_t save_course_file(void *buf, 

/*
Make Directory on 3 Platforms: Mac Linux Windows
*/
bool make_dir(char * path);
__declspec(dllexport) bool make_dir_recusive(char *path);

/*
* download file into save location.
*/
bool download_file(CURL *curl_web_handler, char *url,char *save_name);

/*
Give an url and get this web page.
*/
bool download_web_page(CURL *curl_web_handler,char *url);

/*
Give an url and get the file downloaded.
*/
bool download_file(CURL *curl_web_handler,char *url,char *save_name);

/*
Auth the user and Password.
*/
__declspec(dllexport) bool login_learn_tsinghua(CURL *curl_web_handler, char * username,char *password);

/*
basic parse the web page
*/
p_list_entity basic_parse_page(char *buf, char *prefix,const char *suffix);

/*
*/
char* extract_content_between_tags(char *buf, char *des, const char *prefix, const char *suffix);

// return pointer has moved.....
char *extract_content_between_fix(char *src, char *des, const char *prefix, const char *suffix);

/*
* remove html '\r''\n'' '
*/
bool string_trip(char *src);


// Clear cache, at last main() will free it. 
bool reset_cache_memory();

// destroy all entity memory
bool destroy_all_entity_memory(p_list_entity head);


// char set convert
int   convert(const char *from, const char *to,char *src, int srclen, char* save, int savelen) ;
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen);

__declspec(dllexport) int u2g(char *inbuf,int inlen,char *outbuf,int outlen);


__declspec(dllexport) int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

// Remove HTML tags and convert html label
char *remove_and_convert_html(char *buf);

// file path
bool get_dir_by_path(char *dir, char *path);

bool get_file_by_path(char *file, char *path);

// remove PATH_SPILIT_CHAR
bool remove_path_spilit_char(char *buf);

bool url_to_xmlurl(char *des, char *src);

#endif
