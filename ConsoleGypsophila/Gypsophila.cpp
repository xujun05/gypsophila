#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gypsophila.h>
#include "iniparser.h"

char * get_course_ignore_list(char * filename)
{
	dictionary	*ini ;
	bool is_ignore;
	char *list;

	ini = iniparser_load(filename);
	if (ini==NULL) {
		fprintf(stderr, "cannot parse file: %s\n", filename);
		return NULL;
	}
	iniparser_dump(ini, NULL);

	is_ignore = iniparser_getboolean(ini, "Course:ignore", -1);
	if(!is_ignore)
	{
		//不忽略任何课程
		iniparser_freedict(ini);
		return NULL;
	}

	/* Get ignore attributes */
	list = strdup(iniparser_getstring(ini, "Course:ignore_list", NULL));


	iniparser_freedict(ini);
	return list ;
}

// type means 0(CUR),1(NEXT),2(PREV)
void mirror_course(CURL *curl_web_handler,  char *prefix, int type)
{
	make_dir_recusive(prefix);
	//printf("Create Path %s, DONE!\n", prefix);
#ifdef WIN32
	char print_gbk_buff[BUFFER_MAX_SIZE];
	char origin_buff[BUFFER_MAX_SIZE];
#endif
	if(type > 2)
		type = 2;
	if(type < 0)
		type = 0;

	p_course course_list = NULL;
	if((course_list = get_course_list(curl_web_handler, (course_type)type)) == NULL)
	{
		printf("COURSE LIST ERROR\n");
	}

	char buf[BUFFER_MAX_SIZE];
	p_course p = NULL;
	if(course_list)
		p = course_list->next;
#ifdef WIN32
	int i = 0;
    printf("\n\n");
    
	while(p)
	{
		sprintf(origin_buff, "课程编号:%d 课程:%s 所在学期为%s ", p->course_id, p->course_name,p->course_term);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n", print_gbk_buff);
		p = p->next;
		i++;
	}
	sprintf(origin_buff, "课程总数: %d",i);

	u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
	printf("%s\n\n\n", print_gbk_buff);

	// ignore some courses.
	printf("Please edit the global.ini file to ignore courses. If you have finished, press any number to continue!\n");
	scanf("%s", buf);

	char *ignore_list = get_course_ignore_list("global.ini");
	if(course_list)
		p = course_list->next;
#endif
	while(p)
	{
		// check the course id is in the course ignore list;
		if(ignore_list)
		{
			char course_no_buf[32];
			itoa(p->course_id, course_no_buf, 10);
			char *the_first_pos = strstr(ignore_list, course_no_buf);
			// 存在
			if(the_first_pos)
			{
				if(*(the_first_pos + strlen(course_no_buf)) == '\0' || *(the_first_pos + strlen(course_no_buf)) == ' ')
				{
					//忽略课程
					sprintf(origin_buff, "忽略课程: %s -- %s",p->course_term, p->course_name);
					u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
					printf("%s\n", print_gbk_buff);
					p = p->next;
					continue;
				}
			}
		}

		sprintf(buf,"%s%c%s%c%s",prefix,PATH_SPILIT_CHAR,p->course_term,PATH_SPILIT_CHAR,p->course_name);
		make_dir_recusive(buf);
#ifdef WIN32
		sprintf(origin_buff, "正在镜像%s课程--%s",p->course_term, p->course_name);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n", print_gbk_buff);
		sprintf(origin_buff, "\t\t正在镜像(%s)过程I:课程公告",p->course_name);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s    ", print_gbk_buff);
#endif
		mirror_course_notice(curl_web_handler,buf, p->course_id);
#ifdef WIN32
		sprintf(origin_buff, "课程公告镜像完毕");
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n", print_gbk_buff);
		sprintf(origin_buff, "\t\t正在镜像(%s)过程II:课程文件",p->course_name);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s    ", print_gbk_buff);
#endif
		mirror_course_file(curl_web_handler,buf, p->course_id);
#ifdef WIN32
		sprintf(origin_buff, "课程文件镜像完毕");
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n", print_gbk_buff);
		sprintf(origin_buff, "\t\t正在镜像(%s)过程III:课程作业",p->course_name);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s    ", print_gbk_buff);
#endif
		mirror_course_assignment(curl_web_handler,buf, p->course_id);
#ifdef WIN32
		sprintf(origin_buff, "课程作业镜像完毕");
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n", print_gbk_buff);
		sprintf(origin_buff, "\t\t正在镜像(%s)过程IV:课程讨论",p->course_name);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s    ", print_gbk_buff);
#endif
		mirror_course_discussion(curl_web_handler,buf, p->course_id);
#ifdef WIN32
		sprintf(origin_buff, "课程讨论镜像完毕");
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n\n", print_gbk_buff);
#endif
		p = p->next;
	}
}

int main(int argc, char *argv[])
{
	char username[32];
	char passwd[32];
	printf("Gypsophila:WebCourse@Tsinghua(Console Edition)\n");
	printf("Default path for saving course is here: .\\Course\n\n\n");
	CURLcode return_code;
	return_code = curl_global_init(CURL_GLOBAL_ALL);

	// check if init right
	if(CURLE_OK != return_code){
		printf("init the libcurl error\n");
		return -1;
	}

	CURL *curl_web_handler = curl_easy_init();
	if( NULL == curl_web_handler)
	{
		printf("Web Handle init error!\n");
		curl_global_cleanup();
		return -1;
	}

	char path[BUFFER_MAX_SIZE];

	printf("Enter your username:");
	scanf("%s",username);
	getPasswd("Enter your password:", passwd, 32);


	strcpy(path, "Course");



	if(!login_learn_tsinghua(curl_web_handler, username,passwd))
	{
		printf("Auth Failed.\n\n\n");
		return -1;
	}
	printf("Server response: login is OK!\n\n\n");


	init_cache();

	int type = 2;
	printf("What type course you want to save?(numbers only, such as 1) \n\
1. This semester courses\n\
2. Next semester courses\n\
3. Previous courses(ecommended!!!)\n\
Please enter your choice(1-3):");
	scanf("%d", &type);
	type--;

	mirror_course(curl_web_handler, path, type);

	printf("Mirror process is over, press any number to exit!\n");

	clean_cache();

	curl_easy_cleanup(curl_web_handler);

	curl_global_cleanup();

	int k;
	scanf("%d", &k);
	return 0;
}
