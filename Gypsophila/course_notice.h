#ifndef _COURSE_NOTICE_H
#define _COURSE_NOTICE_H

#include "extern.h"
#include "utils.h"


typedef struct _notice_element
{
	int notice_id;
	char *notice_title;
	char *notice_publisher;
	bool is_highlight;
	char *notice_time;
	char *notice_body;
	struct _notice_element *next;
}notice_element, *p_notice_element;

// init operation
p_notice_element init_notice_list(p_notice_element head);
bool init_notice_element(p_notice_element p);

// insert operation
bool insert_element(p_notice_element pre, p_notice_element node);

// destroy all
bool destroy_all_notice_element(p_notice_element head);

// print
void print_notice_element(p_notice_element p);

// write it to file
void write_all_notice_to_xmlfile(char *filename, p_notice_element head);
#endif
