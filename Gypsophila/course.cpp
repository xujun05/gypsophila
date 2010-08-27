#include "course.h"

const char NOTICE_URL_PREFIX[] =  "http://learn.tsinghua.edu.cn/MultiLanguage/public/bbs/";
const char HOMEWORK_URL_PREFIX[] = "http://learn.tsinghua.edu.cn/MultiLanguage/lesson/student/";
const char REPLY_URL_PREFIX[] = "http://learn.tsinghua.edu.cn/MultiLanguage/public/bbs/";


// parse every course notice page
bool parse_course_notice(CURL *curl_web_handler,char *path,p_notice_element head)
{
	extern cache_memory cache;

	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache.mem,"<tr class=\"tr","</tr>");
	// for loop
	p_list_entity p = entity_head->next;


	reset_cache_memory();

	p_notice_element pre = head;
	p_notice_element cur = head->next;

	//variable
	char buf[BUFFER_MAX_SIZE];
	memset (buf, 0, BUFFER_MAX_SIZE);
	while(p)
	{
		cur = (p_notice_element)malloc(sizeof(notice_element));
		cur->next = NULL;

		char *pointer_src = p->entity;


		// Get NOTICE ID
		char *p_start = strchr(pointer_src, '>');
		p_start++;
		char *p_end = strchr(p_start, '<');
		strncpy(buf, p_start, p_end - p_start);
		buf[p_end - p_start] = '\0';
		cur->notice_id = atoi(buf);
		memset (buf, 0, BUFFER_MAX_SIZE);

		// Jump the next </tg> tag
		pointer_src = p_end + 5;
		// COPY NOTICE URL AND NOTICE TITLE
		p_start = strchr(pointer_src, '>');
		p_start++;
		p_end = strstr(pointer_src,"</td>");
		while(p_end - p_start)
		{
			//the first \" is <a href="
			if(*p_start == '\'')
			{
				char *end_of_notice_url = strchr(p_start + 1, '\'');
				memcpy(buf, p_start + 1, (int)(end_of_notice_url - p_start) -1);
				buf[(int)(end_of_notice_url - p_start) - 1] = '\0';

				// p_start jump to end of notice url , it will meet next tag<font> </font>??
				p_start = end_of_notice_url + 2;
				break;
			}
			p_start++;
		}
		str_add_prefix(NOTICE_URL_PREFIX,buf);
		//printf("%s",buf);

		cur->notice_body = get_notice_content(curl_web_handler,buf);
		//    <td width="87%" class="tr_l2
		// CURL DOWNLOAD
		// Get NOTICE Publisher
		// highlight:??
		cur->is_highlight  = (strstr(p_start, "<font") != NULL);
		if(cur->is_highlight)
			p_start = strchr(p_start, '>') + 1;

		while(p_end - p_start)
		{
#ifdef WIN32
			if((*p_start) == ' ' || *p_start == '\n' || *p_start == '\r')
#else
			if(isspace(*p_start)  || *p_start == '\n' || *p_start == '\r')
#endif
			{
				p_start ++;
				continue;
			}
			else
			{
				char *end_of_notice_title = strchr(p_start, '<');
				memcpy(buf, p_start, end_of_notice_title - p_start);
				buf[end_of_notice_title - p_start] = '\0';
				remove_and_convert_html(buf);
				string_trip(buf);
				cur->notice_title = strdup(buf);
				break;
			}
			p_start ++;
		}

		// Jump the next </tg> tag. This is Publisher
		pointer_src = p_end + 5;
		p_start = strchr(pointer_src, '>') + 1;
		p_end = strchr (p_start, '<');
		memcpy(buf,p_start, p_end - p_start);
		buf[p_end - p_start] = 0;
		cur->notice_publisher = strdup(buf);

		// Jump the next <tg> tag.
		pointer_src = p_end + 5;
		p_start = strchr(pointer_src, '>') + 1;
		p_end = strchr (p_start, '<');
		memcpy(buf,p_start, p_end - p_start);
		buf[p_end - p_start] = 0;
		cur->notice_time = strdup(buf);

		//print_notice_element(cur);

		pre->next = cur;
		pre = cur;
		cur = NULL;

		//
		p = p->next;
	}
	sprintf(buf,"%s%c%s", path, PATH_SPILIT_CHAR, "notice.xml");
	write_all_notice_to_xmlfile(buf,head);
	if(!destroy_all_entity_memory(entity_head))
		return FALSE;
	if(!destroy_all_notice_element(head))
		return FALSE;
	return TRUE;
}

// parse course notice page.
bool get_course_notice(CURL* curl_web_handler, char *path, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	memset(buf, 0 ,BUFFER_MAX_SIZE);

	sprintf(buf,"%s%s%d",tsinghua_prefix,COURSE_NOTICE_URL_INFIX, course_id);
	extern cache_memory cache;
	curl_easy_setopt(curl_web_handler, CURLOPT_FOLLOWLOCATION, 1);

	if(!copy_web_page_to_memory(curl_web_handler, buf, &copy_to_buffer, NULL))
		return FALSE;

	p_notice_element head = NULL;
	head = init_notice_list(head);
	return parse_course_notice(curl_web_handler,path, head);
}

// get notice content
char *get_notice_content(CURL* curl_web_handler, char *URL)
{
	extern cache_memory cache;
	if(!copy_web_page_to_memory(curl_web_handler, URL, &copy_to_buffer, NULL))
		return FALSE;
	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache.mem,"<td width=\"87%\" class=\"tr_l2\" colspan=\"3\"","</td>");
	// I know where the content is.
	p_list_entity content = entity_head->next;
	reset_cache_memory();

	//
	char *pointer = NULL;
	if(content)
	{
		remove_and_convert_html(content->entity);
		string_trip(content->entity);
		pointer = strdup(content->entity);
	}

	if(entity_head)
		destroy_all_entity_memory(entity_head);

	//char buf[BUFFER_MAX_SIZE*32];
	//while(pointer)
	//{
	//sscanf("%s</td>",buf);
	//}
	//buf[BUFFER_MAX_SIZE*32-1] = '\0';
	return pointer;
}


// Start to parse File Download
bool get_course_file(CURL* curl_web_handler, char *path, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	memset(buf, 0 ,BUFFER_MAX_SIZE);

	sprintf(buf,"%s%s%d",tsinghua_prefix,COURSE_DOCUMENTS_URL_INFIX, course_id);
	extern cache_memory cache;
	curl_easy_setopt(curl_web_handler, CURLOPT_FOLLOWLOCATION, 1);

	if(!copy_web_page_to_memory(curl_web_handler, buf, &copy_to_buffer, NULL))
		return FALSE;

	// init list
	p_file_element head = NULL;
	head = init_file_list(head);
	return parse_course_file(curl_web_handler,path, head);
}



bool parse_course_file(CURL *curl_web_handler,char *path,p_file_element head)
{
	extern cache_memory cache;

	// temp buffer
	char buf[BUFFER_MAX_SIZE];
	memset(buf, 0 ,BUFFER_MAX_SIZE);

	// saved the file type name
	char file_type_name[6][128];

	// how many of types
	int i = 0;
	int no_of_file_type = 0;

	// pointer to the cache.
	char *p_cache = cache.mem;
	for(i = 0;i < 5; i++)
	{
		memset(file_type_name[i], 0, 128);
		p_cache = extract_content_between_tags(p_cache,file_type_name[i],"<td class=\"textTD\"","</td>");
		if(!p_cache)
			break;
	}
	no_of_file_type = i;

	p_cache = cache.mem;

	char *p_cache_start[6];
	p_cache_start[0] = p_cache;

	// discard p_cache_start[0], it has no use.
	for(i = 1; i<= no_of_file_type;i++)
	{
		p_cache_start[i] = strstr(p_cache_start[i-1] + 1, "<table  id=\"table_box\"");
		memset(p_cache_start[i] - 1, 0, 1);
	}

	// because 0 not use.
	p_file_element pre = head;
	p_file_element cur = head->next;

	for(i = 1;i <= no_of_file_type;i++)
	{
		char file_save_prefix[BUFFER_MAX_SIZE];
		sprintf(file_save_prefix, "%s%c%s%c",path, PATH_SPILIT_CHAR,file_type_name[i-1],PATH_SPILIT_CHAR);
		make_dir_recusive(file_save_prefix);

		p_list_entity entity_head = NULL;
		entity_head = basic_parse_page(p_cache_start[i],"<tr class=\"tr","</tr>");

		// for loop
		p_list_entity p = entity_head->next;

		while(p)
		{
			cur = (p_file_element)malloc(sizeof(file_element));
			if(!init_file_element(cur))
			{
				printf("Init Error!\n");
				return FALSE;
			}


			// SET FILE TYPE
			cur->file_type = strdup(file_type_name[i-1]);

			// SET DOCUMENT ID
			char *pointer_src = p->entity;
			pointer_src = extract_content_between_tags(pointer_src, buf, "<td width=\"80\">","</td>");
			cur->file_id = atoi(buf);

			// GET DOCUMENT ORIGIN_NAME

			pointer_src = extract_content_between_fix(pointer_src, buf, "getfilelink=", "&");
			cur->file_orign_name = strdup(buf);

			// GET FILE URL 
			extract_content_between_fix(pointer_src, buf, "href=\"","\" >");
			str_add_prefix(tsinghua_prefix,buf);
			cur->file_url = strdup(buf);

			// GET FILE TITLE && MOVE POINTER
			pointer_src = extract_content_between_tags(pointer_src, buf, "<a target=\"_top\"", "</a>");
			remove_and_convert_html(buf);
			if(string_trip(buf))
			{
				remove_path_spilit_char(buf);
				cur->file_title = strdup(buf);
			}
			else
				cur->file_title = NULL;

			// GET FILE ABSTRACT
			pointer_src = extract_content_between_tags(pointer_src, buf, "<td width=\"300\"", "</td>");
			cur->file_abstract = strdup(buf);

			// GET FILE UP TIME
			pointer_src = extract_content_between_tags(pointer_src, buf, "<td width=\"100\"", "</td>");
			cur->file_up_time = strdup(buf);
			sprintf(buf, "%s%s_%s",file_save_prefix,cur->file_title,cur->file_orign_name);
			cur->file_local_location = strdup(buf);

			download_file(curl_web_handler,cur->file_url, cur->file_local_location);

			//print_file_element(cur);

			pre->next = cur;
			pre = cur;
			cur = NULL;
			p = p->next;
		}
		if( !destroy_all_entity_memory(entity_head))
		{
			printf("FREE EROOR @ LINE %d FILE %s",__LINE__, __FILE__);
			return FALSE;
		}
	}
	reset_cache_memory();
	sprintf(buf, "%s%c%s",path, PATH_SPILIT_CHAR,"file.xml");
	write_all_file_to_xmlfile(buf,head);
	if(!destroy_all_file_element(head))
	{
		printf("FREE EROOR @ LINE %d FILE %s",__LINE__, __FILE__);  
		return FALSE;
	}
	return TRUE;
}




// Assignment

// Get Assignments
bool get_course_assignment(CURL* curl_web_handler, char *path, int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	memset(buf, 0 ,BUFFER_MAX_SIZE);

	sprintf(buf,"%s%s%d",tsinghua_prefix,COURSE_ASSIGNMENTS_URL_INFIX, course_id);
	//printf("%s\n",buf);

	extern cache_memory cache;

	if(!copy_web_page_to_memory(curl_web_handler, buf, &copy_to_buffer, NULL))
		return FALSE;

	// init list
	p_assignment_element head = NULL;
	head = init_assignment_list(head);
	return parse_course_assignment(curl_web_handler,path, head);
}

bool parse_course_assignment(CURL *curl_web_handler,char *path, p_assignment_element head)
{
	extern cache_memory cache;

	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache.mem,"<tr class=\"tr","</tr>");
	// for loop
	p_list_entity p = entity_head->next;

	reset_cache_memory();

	p_assignment_element pre = head;
	p_assignment_element cur = head->next;

	char buf[BUFFER_MAX_SIZE];


	while(p)
	{
		cur = (p_assignment_element)malloc(sizeof(assignment_element));
		// init all char *
		init_assignment_element(cur);
		char *src = p->entity;

		// GET NAME
		extract_content_between_tags(p->entity, buf, "<a href","</a>");
		remove_and_convert_html(buf);
		string_trip(buf);
		cur->assignment_name = strdup(buf);

		// GET DETAIL URL
		extract_content_between_fix(p->entity, buf, "href=\"","\">");
		str_add_prefix(HOMEWORK_URL_PREFIX,buf);

		// GET DETAIL
		get_assignment_detail(curl_web_handler,buf,cur);

		// GET TIME
		src = extract_content_between_fix( src, buf, "<td width=\"10%\">", "</td>");
		cur->assignment_time = strdup(buf);

		// GET DEADLINE
		src = extract_content_between_fix(src, buf, "<td width=\"10%\">", "</td>");
		cur->assignment_deadline = strdup(buf);

		// GET STAUTS
		src = extract_content_between_tags(src, buf, "<td width=\"15%\"", "</td>");
		if(string_trip(buf))
		{  
			cur->assignment_status = strdup(buf);
		}
		// GET SCORE URL
		src = extract_content_between_fix (src, buf, "javascript:window.location.href='","';");
		extract_content_between_fix (src, buf,"javascript:window.location.href='","'\"");
		if(str_add_prefix(HOMEWORK_URL_PREFIX, buf))
			get_assignment_score(curl_web_handler,buf,cur);


		//print_assignment_element(cur);

		// save it
		if(cur->assignment_attachment_url)
		{  
			sprintf(buf,"%s%c%s%c%s%c",path, PATH_SPILIT_CHAR, cur->assignment_name, PATH_SPILIT_CHAR, "教师作业附件", PATH_SPILIT_CHAR);
			make_dir_recusive(buf);
			sprintf(buf, "%s%s", buf, cur->assignment_attachment_name);
			cur->assignment_attachment_location = strdup(buf);
			download_file(curl_web_handler,cur->assignment_attachment_url, cur->assignment_attachment_location);
		}

		if(cur->my_handin_attachment_url)
		{
			sprintf(buf,"%s%c%s%c%s%c",path, PATH_SPILIT_CHAR, cur->assignment_name, PATH_SPILIT_CHAR, "我的提交", PATH_SPILIT_CHAR);
			make_dir_recusive(buf);
			sprintf(buf, "%s%s", buf, cur->my_handin_attachment_name);
			cur->my_handin_attachment_location = strdup(buf); 
			download_file(curl_web_handler,cur->my_handin_attachment_url, cur->my_handin_attachment_location);
		}


		pre->next = cur;
		pre = cur;
		cur = NULL;

		p = p->next;
	}

	sprintf(buf, "%s%c%s", path, PATH_SPILIT_CHAR, "assignment.xml");
	write_all_assignment_to_xmlfile(buf,head);
	if(!destroy_all_entity_memory(entity_head))
		return FALSE;
	if(!destroy_all_assignment_element(head))
		return FALSE;
	return TRUE;
}

bool get_assignment_score(CURL* curl_web_handler, char *url, p_assignment_element cur)
{
	extern cache_memory cache;
	if(!copy_web_page_to_memory(curl_web_handler, url, &copy_to_buffer, NULL))
		return FALSE;
	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache.mem,"<td colspan=\"3\" class=\"tr_","</td>");
	reset_cache_memory();

	p_list_entity p = entity_head->next;
	char buf[BUFFER_MAX_SIZE];
	// I know where the content is.
	int i = 0;
	while(p)
	{
		switch(i){
	case 0:
		// homework title
		break;
	case 1:
		//my_handin_description
		//cur->my_handin_description = strdup(p->entity);
		break;
	case 2:
		//
		/*if(extract_content_between_fix(p->entity, buf, "href=\"","\">"))
		{
		cur->my_handin_attachment_url = strdup(buf);
		if(extract_content_between_tags(p->entity, buf, "<a target=\"_top\"","</a>"))
		cur->my_handin_attachment_name = strdup(buf);
		else
		cur->my_handin_attachment_name = strdup("");
		}

		else
		{
		strcpy(buf,p->entity);
		remove_and_convert_html(buf);
		string_trip(buf);
		cur->my_handin_attachment_name = strdup(buf);
		cur->my_handin_attachment_url = strdup("");
		}*/

		break;
	case 3:
		strcpy(buf, p->entity);
		remove_and_convert_html(buf);
		string_trip(buf);
		cur->assignment_score = strdup(buf);
		break;
	case 4:
		strcpy(buf, p->entity);
		remove_and_convert_html(buf);
		string_trip(buf);
		cur->assignment_comment = strdup(buf);

		break;
	case 5:
		break;
	default:
		break;
		}
		i++;
		p = p->next;
	}
	if(entity_head)
		destroy_all_entity_memory(entity_head);
	return TRUE;
}

bool get_assignment_detail( CURL* curl_web_handler, char *url, p_assignment_element cur)
{
	extern cache_memory cache;
	if(!copy_web_page_to_memory(curl_web_handler, url, &copy_to_buffer, NULL))
		return FALSE;
	p_list_entity entity_head = NULL;
	// utf8 cache buffer covert it 
	// convert buffer to UTF-8
	// This is a GBK coding page.
	char *utf8_buffer = (char *)malloc(sizeof(cache.size) + CACHE_INIT_SIZE);
	if(g2u(cache.mem, strlen(cache.mem), utf8_buffer, sizeof(cache.size) + CACHE_INIT_SIZE) == 0)
	{
		//printf("Length:%d", strlen(cache.mem));
		//printf("Convert OK:\n");
		//puts(utf8_buffer);
	}
	else
	{
		printf("May be memory broken, check CACHE_INIT_SIZE, GBK code convert to UTF8 code failure!@line %d, file %s\n",__LINE__, __FILE__ );
		return FALSE;
	}
	if(!utf8_buffer)
		return FALSE;
	// get entity
	entity_head = basic_parse_page(utf8_buffer,"<td class=\"tr_2","</td>");
	reset_cache_memory();
	p_list_entity p = entity_head->next;
	int i = 0;

	char buf[BUFFER_MAX_SIZE * 10];

	while(p)
	{
		// convert buffer to UTF-8
		// This is a GBK coding page.

		switch(i){
	  case 0:
		  // homework title
		  break;
	  case 1:
		  //homework_description
		  if(extract_content_between_tags(p->entity, buf, "<textarea","</textarea>"))
		  {
			  remove_and_convert_html(buf);
			  string_trip(buf);
			  cur->assignment_description = strdup(buf);
		  }

		  else
			  cur->assignment_description = strdup("");
		  break;
	  case 2:
		  //
		  //puts("-------------------");
		  //puts(p->entity);
		  if(extract_content_between_fix(p->entity, buf, "href=\"","\">"))
		  {
			  str_add_prefix(tsinghua_prefix, buf);
			  cur->assignment_attachment_url = strdup(buf);
			  //printf("IN URL:%s\nADDR:%x\n",cur->assignment_attachment_url,cur->assignment_attachment_url);
			  if(extract_content_between_tags(p->entity, buf, "<a target=\"_top\"","</a>"))
			  {
				  cur->assignment_attachment_name = strdup(buf);
				  //printf("IN NAME:%s\nADDR:%x\n",cur->assignment_attachment_name,cur->assignment_attachment_name);
			  }
			  else
				  cur->assignment_attachment_name  = strdup("");
		  }
		  else
		  {
			  strcpy(buf,p->entity);
			  remove_and_convert_html(buf);
			  string_trip(buf);
			  cur->assignment_attachment_name = strdup(buf);
			  cur->assignment_attachment_url = NULL;
		  }
		  break;
	  case 3:
		  if(extract_content_between_tags(p->entity, buf, "<textarea","</textarea>"))
		  {
			  remove_and_convert_html(buf);
			  string_trip(buf);
			  cur->my_handin_description = strdup(buf);
		  }
		  else
			  cur->my_handin_description = strdup("");
		  break;
	  case 4:
		  if(extract_content_between_fix(p->entity, buf, "href=\"","\">"))
		  {
			  str_add_prefix(tsinghua_prefix, buf);
			  cur->my_handin_attachment_url = strdup(buf);
			  if(extract_content_between_tags(p->entity, buf, "<a target=\"_top\"","</a>"))
				  cur->my_handin_attachment_name = strdup(buf);
			  else
				  cur->my_handin_attachment_name  = strdup("");
		  }
		  else
		  {
			  strcpy(buf,p->entity);
			  remove_and_convert_html(buf);
			  string_trip(buf);
			  cur->my_handin_attachment_name = strdup(buf);
			  cur->my_handin_attachment_url = NULL;
		  }
		  break;
	  default:
		  break;
		}
		i++;
		p = p->next;
	}
	if(utf8_buffer)
		free(utf8_buffer);

	if(entity_head)
		destroy_all_entity_memory(entity_head);
	return TRUE;
}

// Get Discussion List, only download the html page
bool get_course_discussion(CURL* curl_web_handler, char *path,int course_id)
{
	char buf[BUFFER_MAX_SIZE];
	memset(buf, 0 ,BUFFER_MAX_SIZE);

	// Get discussion url
	sprintf(buf,"%s%s%d",tsinghua_prefix,COURSE_DISCUSSION_URL_INFIX, course_id);

	extern cache_memory cache;
	// AUTOMATIC HTTP FORWARD
	curl_easy_setopt(curl_web_handler, CURLOPT_FOLLOWLOCATION, 1);

	if(!copy_web_page_to_memory(curl_web_handler, buf, &copy_to_buffer, NULL))
		return FALSE;

	// init list
	p_discussion_element head = NULL;
	head = init_discussion_list(head);
	return parse_course_discussion(curl_web_handler,path, head);
}

// Get discussion list, main function
bool parse_course_discussion(CURL *curl_web_handler,char *path, p_discussion_element head)
{
	extern cache_memory cache;

	// split the memory and copy it into entity list
	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache.mem,"<tr class=\"tr","</tr>");
	// for loop
	p_list_entity p = entity_head->next;
	// reset cache memory and waiting for next download.
	reset_cache_memory();

	// init 
	p_discussion_element pre = head;
	p_discussion_element cur = head->next;
	// BUFFER
	char buf[BUFFER_MAX_SIZE];
	char num_buf[16];

	// loop
	char *reply_url = NULL;

	//
	int discussion_id = 0;
	while(p)
	{
		cur = (p_discussion_element)malloc(sizeof(discussion_element));
		// clear cur pointer memory
		init_discussion_element(cur);
		cur->id = discussion_id;
		// save a char pointer and move it, for performance
		char *src = p->entity;

		// GET TOPIC
		extract_content_between_tags(p->entity, buf,"<a href","</a>");
		remove_and_convert_html(buf);
		cur->discussion_topic = strdup(buf);

		// GET REPLY URL WAITING, SAVE IT
		extract_content_between_fix(p->entity, buf, "href='","'>");
		str_add_prefix(REPLY_URL_PREFIX, buf);
		reply_url = strdup(buf);

		// GET AUTHOR
		src = extract_content_between_tags( src, buf, "<td width=\"15%\"", "</td>");
		cur->discussion_author = strdup(buf);

		// GET reply and click
		src = extract_content_between_fix( src, buf, "<td width=\"15%\">", "</td>");
		memset (num_buf, 0, 16);
		int i = 0,j = 0;
		for(i = 0;i < strlen(buf); i++)
		{
			if(buf[i] == '/')
			{
				// how many reply
				cur->discussion_reply = atoi(num_buf);
				break;
			}
			num_buf[j++] = buf[i];
		}
		cur->discussion_click = atoi(buf+i+1);

		// Get handin time
		extract_content_between_fix(src, buf, "<td width=\"20%\">", "</td>");
		cur->discussion_handin_time = strdup(buf);

		// Get content...etc
		if(!get_reply(curl_web_handler,reply_url, cur))
			printf("Get Reply Error, Check Network.\n");


		// print
		//print_discussion_element(cur);

		// write to file
		if(cur->discussion_attachment_url)
		{
			sprintf(buf,"%s%c%d%c%s%c",path, PATH_SPILIT_CHAR,cur->id, PATH_SPILIT_CHAR, "附件", PATH_SPILIT_CHAR);
			make_dir_recusive(buf);
			sprintf(buf, "%s%s", buf, cur->discussion_attachment_name);
			cur->discussion_attachment_location = strdup(buf);
			download_file(curl_web_handler,cur->discussion_attachment_url, cur->discussion_attachment_location);
		}
		if(cur->reply_head)
		{
			sprintf(buf,"%s%c%d%c%s%c",path, PATH_SPILIT_CHAR, cur->id, PATH_SPILIT_CHAR, "回复附件", PATH_SPILIT_CHAR);
			download_reply_attachment(curl_web_handler,buf, cur->reply_head);

		}


		pre->next = cur;
		pre = cur;
		cur = NULL;

		p = p->next;
		discussion_id ++;
	}

	sprintf(buf, "%s%c%s", path, PATH_SPILIT_CHAR, "discussion.xml");
	write_all_discussion_to_xmlfile(buf,head);
	if(!destroy_all_entity_memory (entity_head))
		return FALSE;
	if(!destroy_all_discussion_element (head))
		return FALSE;
	return TRUE;
}


// Get replys
bool get_reply(CURL* curl_web_handler, char *url, p_discussion_element cur)
{
	extern cache_memory cache;
	if(!copy_web_page_to_memory(curl_web_handler, url, &copy_to_buffer, NULL))
		return FALSE;

	// split the memory and copy it into entity list
	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache.mem,"<table id","</table>");
	// for loop
	p_list_entity p = entity_head->next;
	// reset cache memory and waiting for next download.
	reset_cache_memory();

	// BUFFER
	char buf[BUFFER_MAX_SIZE];
	// The first element is the author , who hand in the discussion
	{
		char *src = p->entity;
		// TOPIC, IGNORE IT
		src = extract_content_between_fix(src, buf, "<p align=\"left\">", "</p>");

		// GET CONTENT
		{
			src = extract_content_between_tags(src, cache.mem, "<td colspan=\"4\" class=\"tr_2", "</td>");
			remove_and_convert_html(cache.mem);
			string_trip(cache.mem);
			cur->discussion_content = strdup(cache.mem);
		}

		// GET ATTACHMENT NAME AND URL
		if(extract_content_between_fix(src, buf, "href=\"","\">"))
		{
			str_add_prefix(tsinghua_prefix,buf);
			cur->discussion_attachment_url = strdup(buf);
			if(extract_content_between_tags (src, buf, "<a target=\"_top\"","</a>"))
			{
				cur->discussion_attachment_name = strdup(buf);
			}
			else
				cur->discussion_attachment_name = strdup("");
		}
		else
		{
			src = extract_content_between_fix(src, buf, "<p align=\"left\">", "</p>");
			remove_and_convert_html(buf);
			string_trip(buf);
			cur->discussion_attachment_name = strdup(buf);
			cur->discussion_attachment_url = NULL;
		}
	}
	p = p->next;

	// NO REPLY
	if(cur->discussion_reply == 0)
		return TRUE;

	// INIT
	cur->reply_head = init_reply_list(cur->reply_head);
	p_reply_element reply_pre = cur->reply_head;
	p_reply_element reply_cur = reply_pre->next;

	while(p)
	{
		reply_cur = (p_reply_element)malloc(sizeof (reply_element));
		init_reply_element(reply_cur);

		// for performance
		char *src = p->entity;

		// GET AUTHOR
		src = extract_content_between_tags(src, buf, "<td width=\"33%\" class=\"tr", "</td>");
		reply_cur->reply_author = strdup(buf);

		// GET TIME
		src = extract_content_between_tags(src, buf, "<td width=\"20%\" class=\"tr", "</td>");
		if(string_trip(buf))
			reply_cur->reply_time = strdup(buf);

		// GET CONTENT
		{
			src = extract_content_between_tags(src, cache.mem, "<td colspan=\"4\" class=\"tr_2\" align=\"left\"", "</td>");
			remove_and_convert_html(cache.mem);
			if(string_trip(cache.mem))
				reply_cur->reply_content = strdup(cache.mem);
		}

		// GET ATTACHMENT NAME AND URL
		if(extract_content_between_fix(src, buf, "href=\"","\">"))
		{
			// print src
			//printf("----DEBUG---\n%s",src);
			if(string_trip(buf))
			{
				str_add_prefix(tsinghua_prefix,buf);
				reply_cur->reply_attachment_url = strdup(buf);
			}

			if(extract_content_between_tags (src, buf, "<a target=\"_top\"","</a>"))
			{
				if(string_trip(buf))
					reply_cur->reply_attachment_name = strdup(buf);
			}
			else
				reply_cur->reply_attachment_name = strdup("");
		}
		else
		{
			// print src
			//printf("----DEBUG_NO_URL_REPLY_PART---\n%s\n",src);
			src = extract_content_between_tags(src, buf, "<td colspan=\"4\" class=\"tr_", "</td>");
			if(string_trip(buf))
				reply_cur->reply_attachment_name = strdup(buf);
			reply_cur->reply_attachment_url = NULL;
		}
		// Create a reply list
		reply_pre ->next = reply_cur;
		reply_pre = reply_cur;
		reply_cur = NULL;
		p = p->next;
	}
	// memory will be destroy in caller function
	return TRUE;
}
