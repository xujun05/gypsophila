/*
encoding=UTF-8
*/
#ifndef _COURSE_H
#define _COURSE_H
#include <ctype.h>
#include "course_notice.h"
#include "course_list.h"
#include "course_file.h"
#include "course_assignments.h"
#include "course_discussion.h"

const char COURSE_NOTICE_URL_INFIX[] = "/MultiLanguage/public/bbs/getnoteid_student.jsp?course_id=";
const char COURSE_INFO_URL_INFIX[] = "/MultiLanguage/lesson/student/course_info.jsp?course_id=";
const char COURSE_DOCUMENTS_URL_INFIX[] = "/MultiLanguage/lesson/student/download.jsp?course_id=";
const char COURSE_ASSIGNMENTS_URL_INFIX[] = "/MultiLanguage/lesson/student/hom_wk_brw.jsp?course_id=";
const char COURSE_DISCUSSION_URL_INFIX[] = "/MultiLanguage/public/bbs/gettalkid_student.jsp?course_id=";


extern p_course course_head;
// parse course page through course id and type 
//bool parse_each_course_page(p_course course);

char *download_file_content(char *url,char *save_location);

// Get Course Notice
bool get_course_notice(char *path,int course_id);
bool parse_course_notice(char *path,p_notice_element head);
char *get_notice_content(char *url);

// Get Course File
bool get_course_file(char *path,int course_id);
bool parse_course_file(char *path, p_file_element head);


// Get Assignments
bool get_course_assignment(char *path, int course_id);
bool parse_course_assignment(char *path, p_assignment_element head);
bool get_assignment_score(char *url, p_assignment_element cur);
bool get_assignment_detail(char *url, p_assignment_element cur);

// Get Discussion List
bool get_course_discussion(char *path, int course_id);
bool parse_course_discussion(char *path, p_discussion_element head);
// Get Reply List
bool get_reply(char *url, p_discussion_element cur);
#endif
