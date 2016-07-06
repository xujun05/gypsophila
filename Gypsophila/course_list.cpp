#include "course_list.h"

extern p_course course_head;

// Get Course List.
p_course get_course_list(CURL *curl_web_handler, course_type type)
{
	char buf[BUFFER_MAX_SIZE];
	memset(buf, 0, BUFFER_MAX_SIZE);

	sprintf(buf,"%s/MultiLanguage/lesson/student/MyCourse.jsp?typepage=%d",tsinghua_prefix,type);
	//printf("URL:%s",buf);

	extern cache_memory cache;

	if(!copy_web_page_to_memory(curl_web_handler, buf, &copy_to_buffer, &type))
		return FALSE;
	parse_course_list_page(&cache,type);
	return course_head;
}

// Get the course list
bool parse_course_list_page(cache_memory *cache, course_type type)
{
	extern p_course course_head;
	course_head = init_course_list();
	//todo
	p_list_entity entity_head = NULL;
	entity_head = basic_parse_page(cache->mem, "<tr class=\"info_tr","</tr>");
	reset_cache_memory();
	p_list_entity entity_pre = entity_head;
	p_list_entity p = entity_pre->next;

	p_course pre = course_head;
	p_course q = course_head->next;

	while(p)
	{
		if (strstr(p->entity, "coursehome") != NULL) { p = p->next; continue; }
		q = (p_course)malloc(sizeof(course));
		char *p_course_id = strstr(p->entity,"course_id=");
		p_course_id += 10;

		// COPY COURSE ID
		char buf[BUFFER_MAX_SIZE / 32];
		memset(buf, 0, BUFFER_MAX_SIZE/32);
		int i = 0;

		while(isdigit(*p_course_id))
		{
			buf[i++] = *p_course_id;
			p_course_id++;
		}
		q->course_id = atoi(buf);
		memset(buf, 0, BUFFER_MAX_SIZE/32);

		// COPY COURSE NAME
		char *p_course_name = strchr(p_course_id + 1,'>') + 1;
#ifdef WIN32
		while((*p_course_name == ' ')|| (*p_course_name == '\n') || (*p_course_name == '\r'))
#else
		while(isspace(*p_course_name))
#endif

		{
			p_course_name++;
		}
		char *end_of_course_name = strchr(p_course_name,'(');
		q->course_name = (char *)malloc(end_of_course_name - p_course_name + 1);
		memset(q->course_name, 0 , end_of_course_name - p_course_name + 1);
		strncpy(q->course_name, p_course_name, end_of_course_name - p_course_name);

		// COPY COURSE TERM
		char *p_course_term = end_of_course_name + 1;
		p_course_term = rindex(p_course_term, '(') + 1 ;
		char *end_of_course_term = strchr(p_course_term, ')');
		q->course_term = (char *)malloc(end_of_course_term - p_course_term + 1);
		memset(q->course_term, 0 , end_of_course_term - p_course_term + 1);
		strncpy(q->course_term,p_course_term, end_of_course_term - p_course_term);

		// COPY COURSE TYPE
		q->type = type;

		//printf("COURSE ID:%d\nCOURSE_NAME:%s\nCOURSE_TERM:%s\nCOURSE_TYPE:%d\n", q->course_id, q->course_name, q->course_term, q->type);

		// init the list
		pre->next = q;
		pre = q;
		q->next = NULL;
		//
		entity_pre = p;
		p = p->next;

		// free list entity
		if(entity_pre->entity)
			free(entity_pre->entity);
		free(entity_pre);
	}

	// entity head: free memory
	free(entity_head);
	return TRUE;
}

//free it


/*
* init head pointer
*/
p_course init_course_list()
{
	course_head = (p_course)malloc (sizeof(course));
	course_head->next = NULL;
	course_head->course_name = NULL;
	course_head->course_term = NULL;
	return course_head;
}

// get course struct pointer by course id.
p_course get_course_by_id(int id)
{
	p_course p = course_head;
	while(p)
	{
		if(p->course_id == id)
			return p;
	}
	return NULL;
}

// discard
p_course insert_course(p_course course, p_course *pre)
{
	(*pre)->next = course;
	(*pre) = course;
	(*pre)->next = NULL;
	return NULL;
}

/*
* destroy all course list
*/
bool destroy_all_course_list()
{
	p_course p = course_head;
	p_course q = p;
	while(p)
	{
		if(p->course_name)
		{
			free(p->course_name);
			p->course_name = NULL;
		}
		free(p);
		p = q->next;
		q = p;
	}
	free(course_head);
	return TRUE;
}
