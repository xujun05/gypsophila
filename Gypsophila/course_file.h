#ifndef _COURSE_FILE_H
#define _COURSE_FILE_H

#include "extern.h"
#include "utils.h"


typedef struct _file_element
{
	int file_id;
	// so difficult
	char *file_type;
	char *file_title;
	char *file_orign_name;
	char *file_abstract;
	char *file_up_time;
	char *file_url;
	char *file_local_location;
	struct _file_element *next;
}file_element, *p_file_element;

// init operation
p_file_element init_file_list(p_file_element head);
// init discusion element
bool  init_file_element (p_file_element p);

// insert operation
void print_file_element(p_file_element p);

// destroy all
bool destroy_all_file_element(p_file_element head);

// write_all_file_to_xmlfile
void write_all_file_to_xmlfile(char *filename, p_file_element head);
#endif
