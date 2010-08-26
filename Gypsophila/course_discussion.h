//<tr class="tr
#ifndef _COURSE_DISCUSSION_H
#define _COURSE_DISCUSSION_H

#include "extern.h"
#include "utils.h"
typedef struct _reply_element{
	char * reply_author;
	char * reply_time;
	char * reply_content;
	char * reply_attachment_url;
	char * reply_attachment_location;
	char * reply_attachment_name;
	struct _reply_element *next;
}reply_element, *p_reply_element;


typedef struct _discussion_element
{
	int id;//
	char *discussion_topic;//**
	char *discussion_author;//**
	int discussion_reply;//
	int discussion_click;//
	char *discussion_handin_time;//**
	char *discussion_content;
	char *discussion_attachment_url;
	char *discussion_attachment_location;
	char *discussion_attachment_name;
	p_reply_element reply_head;// reply list
	struct _discussion_element *next;
}discussion_element, *p_discussion_element;

// init discussion list operation
p_discussion_element init_discussion_list(p_discussion_element head);
// init discusion element
bool  init_discussion_element (p_discussion_element p);

// destroy all
bool destroy_all_discussion_element(p_discussion_element head);

void print_discussion_element(p_discussion_element p);


// init reply list
p_reply_element init_reply_list(p_reply_element head);
bool init_reply_element ( p_reply_element p);
bool destroy_all_reply_element(p_reply_element head);
void print_reply_element(p_reply_element p);

// write_all_file_to_xmlfile
void write_all_discussion_to_xmlfile(char *filename, p_discussion_element head);
void write_all_reply_to_xmlfile_by_discussion_id(char *filename, p_discussion_element p);


void download_reply_attachment(CURL *curl_web_handler,char *path, p_reply_element head);
#endif
