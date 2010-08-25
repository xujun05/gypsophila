#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <curl/curl.h>
// LINUX MAC CODE
#include <sys/stat.h>
#include <sys/types.h>
// WINDOWS??
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "extern.h"

// HTTP Response code
#define FILE_EXISTS 200
#define PATH_SPILIT_CHAR '\\'

// Output Struct
typedef struct _OutStruct{
	char *filename;
	FILE *stream;
} OutStruct;
/*
  Boolean Type
*/
typedef enum _course_type
{
  CUR_COURSE,NEXT_COURSE,PRE_COURSE
}course_type;

typedef struct _list_entity
{
  char *entity;
  struct _list_entity* next;
}list_entity, *p_list_entity;


typedef struct _cache_memory 
{
  char *mem;
  int offset;
  size_t size;
}cache_memory;

typedef struct _course 
{
  int course_id;
  char *course_name;
  char *course_term;
  course_type type;
  struct _course* next;
}course,*p_course;

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
bool copy_web_page_to_memory(char *url,void *parse_function, void *userdata);
//size_t save_course_list(void *buf, size_t size, size_t nmemb, void *user_p);
//size_t save_course_file(void *buf, size_t size, size_t nmemb, void *user_p);
//size_t save_course_file(void *buf, 

/*
  Make Directory on 3 Platforms: Mac Linux Windows
 */
bool make_dir(char * path);
bool make_dir_recusive(char *path);

/*
 * download file into save location.
 */
bool download_file(char *url,char *save_name);

/*
  Give an url and get this web page.
 */
bool download_web_page(char *url);

/*
  Give an url and get the file downloaded.
*/
bool download_file(char *url,char *save_name);

/*
  Auth the user and Password.
*/
bool login_learn_tsinghua(char * username,char *password);

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

int u2g(char *inbuf,int inlen,char *outbuf,int outlen);


int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

// Remove HTML tags and convert html label
char *remove_and_convert_html(char *buf);

// file path
bool get_dir_by_path(char *dir, char *path);

bool get_file_by_path(char *file, char *path);

// remove PATH_SPILIT_CHAR
bool remove_path_spilit_char(char *buf);

bool url_to_xmlurl(char *des, char *src);

#endif
