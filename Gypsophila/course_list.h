#ifndef _COURSE_LIST_H
#define _COURSE_LIST_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <curl/curl.h>
#include "extern.h"
#include "utils.h"

extern p_course course_head;

__declspec(dllexport) p_course get_course_list(CURL *curl_web_handler, course_type type);


// Get the course list
bool parse_course_list_page(cache_memory *cache, course_type type);


// init the course list
p_course init_course_list();

//void get_course_list();

// get course struct pointer by course id.
p_course get_course_by_id(int id);

void find_next_course(p_course current);

void insert_new_course(int course_id, char *course_name,course_type type);

void destroy_all_list();

#endif
