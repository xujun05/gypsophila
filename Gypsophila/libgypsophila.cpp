#include "course_notice.h"
#include "course_assignments.h"
#include "course_discussion.h"
#include "course.h"

__declspec(dllexport) char tsinghua_prefix[]="http://learn.tsinghua.edu.cn";
cache_memory cache;
p_course course_head;

__declspec(dllexport) void mirror_course_notice( CURL *curl_web_handler, char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程公告",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_notice(curl_web_handler,buf, course_id);
}

__declspec(dllexport) void mirror_course_file( CURL *curl_web_handler, char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程文件",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_file(curl_web_handler,buf, course_id);
}

__declspec(dllexport) void mirror_course_assignment(CURL *curl_web_handler, char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程作业",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_assignment(curl_web_handler,buf, course_id);
}

__declspec(dllexport) void mirror_course_discussion(CURL *curl_web_handler, char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程讨论",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_discussion(curl_web_handler,buf, course_id);
}

__declspec(dllexport) void mirror_course(CURL *curl_web_handler,  char *prefix);

__declspec(dllexport) void mirror_course_frome_course_id(CURL *curl_web_handler,  int course_id, char *prefix)
{
	make_dir_recusive(prefix);
	//printf("Create Pahth %s, DONE!\n", prefix);

	if(!get_course_list(curl_web_handler, PRE_COURSE))
	{
		printf("COURSE LIST ERROR\n");
	}

	char buf[BUFFER_MAX_SIZE];

	p_course p = course_head->next;
	bool isStart = FALSE;
	while(p)
	{

		if(p->course_id == course_id)
		{

			isStart = TRUE;
		}
		if(!isStart)
		{
			printf("Jump %s\n", p->course_name); 
			p = p->next;
			continue;
		}


		sprintf(buf,"%s%c%s%c%s",prefix,PATH_SPILIT_CHAR,p->course_term,PATH_SPILIT_CHAR,p->course_name);
		make_dir_recusive(buf);

		mirror_course_notice(curl_web_handler,(char *)buf, p->course_id);

		mirror_course_file(curl_web_handler,(char *)buf, p->course_id);	


		mirror_course_assignment(curl_web_handler,(char *)buf, p->course_id);


		mirror_course_discussion(curl_web_handler,(char *)buf, p->course_id);
		p = p->next;
	}
}

// type means 0(CUR),1(NEXT),2(PREV)
__declspec(dllexport) void mirror_course(CURL *curl_web_handler,  char *prefix, int type)
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

	if(!get_course_list(curl_web_handler, (course_type)type))
	{
		printf("COURSE LIST ERROR\n");
	}

	char buf[BUFFER_MAX_SIZE];
	p_course p = NULL;
	if(course_head)
		p = course_head->next;
#ifdef WIN32
	int i = 0;
	while(p)
	{
		sprintf(origin_buff, "课程:%s 所在学期为%s", p->course_name,p->course_term);
		u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
		printf("%s\n", print_gbk_buff);
		p = p->next;
		i++;
	}
	sprintf(origin_buff, "课程总数: %d",i);
	u2g(origin_buff, strlen(origin_buff), print_gbk_buff, BUFFER_MAX_SIZE);
	printf("%s\n\n\n", print_gbk_buff);
	if(course_head)
		p = course_head->next;
#endif
	while(p)
	{
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


__declspec(dllexport) bool init_cache()
{
	cache.mem = (char *)malloc(CACHE_INIT_SIZE);
	memset(cache.mem, 0 ,CACHE_INIT_SIZE);
	cache.offset = 0;
	cache.mem[cache.offset] = '\0';
	cache.size = CACHE_INIT_SIZE;
	return true;
}

__declspec(dllexport) bool clean_cache()
{
	// free cache memory.
	if(cache.mem)
	{
		free(cache.mem);
		cache.size = 0;
		return true;
	}
	return true;
}
/*int main(int argc, char *argv[])
{
	printf("WebCourse@Tsinghua(Console Edition)\n");
	CURLcode return_code;
	return_code = curl_global_init(CURL_GLOBAL_ALL);

	// check if init right
	if(CURLE_OK != return_code){
		printf("init the libcurl error\n");
		return -1;
	}

	curl_web_handler = curl_easy_init();
	if( NULL == curl_web_handler)
	{
		printf("Web Handle init error!\n");
		curl_global_cleanup();
		return -1;
	}

	char path[BUFFER_MAX_SIZE];

	printf("Enter your username:");
	scanf("%s",username);
	getPasswd("Please enter your password:", passwd, 32);
	printf("Default path for saving file is .\\Course:");
	scanf("%s", path);
	if(path[0] == NULL)
		strcpy(path, "Course");



	if(!login_learn_tsinghua(username,passwd))
	{
		printf("Auth Failed.\n");
		return -1;
	}
	printf("Login OK\n");

	cache.mem = (char *)malloc(CACHE_INIT_SIZE);
	memset(cache.mem, 0 ,CACHE_INIT_SIZE);
	cache.offset = 0;
	cache.mem[cache.offset] = '\0';
	cache.size = CACHE_INIT_SIZE;


	mirror_course(path);
	//mirror_course_frome_course_id(start_id, path);

	printf("Print OK\n");


	// free cache memory.
	if(cache.mem)
		free(cache.mem);

	curl_easy_cleanup(curl_web_handler);

	curl_global_cleanup();

	int k;
	scanf("%d", &k);
	return 0;
}
*/