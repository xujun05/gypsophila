#include "course_notice.h"
#include "course_assignments.h"
#include "course_discussion.h"
#include "course.h"


CURL *curl_web_handler = NULL;
char username[32];
char passwd[32];
char tsinghua_prefix[]="http://learn.tsinghua.edu.cn";
cache_memory cache;
cache_memory cache_gbk;
p_course course_head;

void mirror_course_notice(char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程公告",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_notice(buf, course_id);
}

void mirror_course_file(char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程文件",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_file(buf, course_id);
}

void mirror_course_assignment(char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程作业",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_assignment(buf, course_id);
}

void mirror_course_discussion(char *prefix, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	sprintf(buf,"%s%c%s%c",prefix, PATH_SPILIT_CHAR,"课程讨论",PATH_SPILIT_CHAR);
	make_dir_recusive(buf);
	get_course_discussion(buf, course_id);
}

void mirror_course(char *prefix);

void mirror_course_frome_course_id(int course_id, char *prefix)
{
  make_dir_recusive(prefix);
  printf("Create Pahth %s, DONE!\n", prefix);

  if(!get_course_list(PRE_COURSE))
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
    
    mirror_course_notice((char *)buf, p->course_id);

	mirror_course_file((char *)buf, p->course_id);	


    mirror_course_assignment((char *)buf, p->course_id);


	mirror_course_discussion((char *)buf, p->course_id);
    p = p->next;
  }
}


void mirror_course(char *prefix)
{
  make_dir_recusive(prefix);
  printf("Create Pahth %s, DONE!\n", prefix);

  if(!get_course_list(PRE_COURSE))
  {
    printf("COURSE LIST ERROR\n");
  }
  
  char buf[BUFFER_MAX_SIZE];
  p_course p = NULL;
  if(course_head)
	p = course_head->next;
  while(p)
  {
    sprintf(buf,"%s%c%s%c%s",prefix,PATH_SPILIT_CHAR,p->course_term,PATH_SPILIT_CHAR,p->course_name);
    make_dir_recusive(buf);
    
    //mirror_course_notice(buf, p->course_id);

	mirror_course_file(buf, p->course_id);


	//mirror_course_assignment(buf, p->course_id);


	//mirror_course_discussion(buf, p->course_id);
    p = p->next;
  }
}



int main(int argc, char *argv[])
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
  printf("Enter your passwd:");
  scanf("%s",passwd);
  printf("Enter the path for saving course:");
  scanf("%s", path);



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
