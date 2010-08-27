#ifndef _COURSE_ASSIGNMENTS_H
#define _COURSE_ASSIGNMENTS_H

#include "extern.h"
#include "utils.h"


typedef struct _assignment_element
{
	char *assignment_name;
	char *assignment_time;
	char *assignment_deadline;
	char *assignment_status;
	char *assignment_score; //3th
	char *assignment_comment; //3th
	char *assignment_description; //2th
	char *assignment_attachment_url;//2th
	char *assignment_attachment_name; //2th
	char *assignment_attachment_location;
	char *my_handin_description; //3th
	char *my_handin_attachment_url;//3th
	char *my_handin_attachment_name; //3th
	char *my_handin_attachment_location;
	struct _assignment_element *next;
}assignment_element, *p_assignment_element;

// init operation
p_assignment_element init_assignment_list(p_assignment_element head);
// init an element
bool  init_assignment_element (p_assignment_element p);


// insert operation
void print_assignment_element(p_assignment_element cur);

// destroy all
bool destroy_all_assignment_element(p_assignment_element head);


// write_all_file_to_xmlfile
void write_all_assignment_to_xmlfile(char *filename, p_assignment_element head);
#endif
