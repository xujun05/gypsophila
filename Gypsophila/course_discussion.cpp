#include "course_discussion.h"

// init reply list
p_reply_element init_reply_list(p_reply_element head)
{
	head = (p_reply_element)malloc(sizeof(reply_element));
	if(head)
	{  
		head->next = NULL;
		head->reply_author = NULL;
		head->reply_time = NULL;
		head->reply_content = NULL;
		head->reply_attachment_url = NULL;
		head->reply_attachment_name = NULL;
		head->reply_attachment_location = NULL;
	}
	return head;
}

bool init_reply_element ( p_reply_element p)
{
	if(p)
	{
		p->next = NULL;
		p->reply_author = NULL;
		p->reply_time = NULL;
		p->reply_content = NULL;
		p->reply_attachment_url = NULL;
		p->reply_attachment_name = NULL;
		p->reply_attachment_location = NULL;
		return TRUE;
	}
	return FALSE;
}

bool destroy_all_reply_element(p_reply_element head)
{
	p_reply_element pre = head;
	p_reply_element p = pre->next;
	while(p)
	{
		if(p->reply_author)
			free(p->reply_author);
		if(p->reply_time)
			free(p->reply_time);
		if(p->reply_content)
			free(p->reply_content);
		if(p->reply_attachment_url)
			free(p->reply_attachment_url);
		if(p->reply_attachment_name)
			free(p->reply_attachment_name);
		if(p->reply_attachment_location)
			free(p->reply_attachment_location);
		pre = p;
		p = p->next;
		free(pre);
	}
	free(head);
	return TRUE;
}

// Print
void print_reply_element(p_reply_element p)
{
	printf("Reply AUTHOR:%s\n", p->reply_author);
	printf("Reply TIME:%s\n", p->reply_time);
	printf("Reply CONTENT:%s\n", p->reply_content);
	printf("Reply ATTACHMENT URL:%s\n", p->reply_attachment_url);
	printf("Reply ATTACHMENT NAME:%s\n", p->reply_attachment_name);
}

p_discussion_element init_discussion_list(p_discussion_element head)
{
	head = (p_discussion_element)malloc (sizeof(discussion_element));
	if(head)
	{
		head->next = NULL;
		head->discussion_topic = NULL;
		head->discussion_author = NULL;
		head->discussion_reply = 0;
		head->discussion_click = 0;
		head->discussion_handin_time = NULL;// another page
		head->discussion_content = NULL;
		head->discussion_attachment_url = NULL;
		head->discussion_attachment_name = NULL;
		head->discussion_attachment_location = NULL;
		head->reply_head = NULL;
	}
	return head;
}

bool init_discussion_element (p_discussion_element p)
{
	if(p)
	{
		p->next = NULL;
		p->discussion_topic = NULL;
		p->discussion_author = NULL;
		p->discussion_reply = 0;
		p->discussion_click = 0;
		p->discussion_handin_time = NULL;
		p->discussion_content = NULL;
		p->discussion_attachment_url = NULL;
		p->discussion_attachment_location = NULL;
		p->discussion_attachment_name = NULL; 
		p->reply_head = NULL;
		return TRUE;
	}
	else return FALSE;
}


// destroy all
bool destroy_all_discussion_element(p_discussion_element head)
{
	p_discussion_element pre = head;
	p_discussion_element p = pre->next;
	while(p)
	{
		if(p->discussion_topic)
			free(p->discussion_topic);
		if(p->discussion_author)
			free(p->discussion_author);
		if(p->discussion_handin_time)
			free(p->discussion_handin_time);
		if(p->discussion_content)
			free(p->discussion_content);
		if(p->discussion_attachment_url)
			free(p->discussion_attachment_url);
		if(p->discussion_attachment_name)
			free(p->discussion_attachment_name);
		if(p->discussion_attachment_location)
			free(p->discussion_attachment_location);
		if(p->reply_head)
			destroy_all_reply_element(p->reply_head);
		pre = p;
		p = p->next;
		free(pre);
	}
	free(head);
	return TRUE;
}


// Print
void print_discussion_element(p_discussion_element p)
{
	printf("--NEW DISCUSSION--\n");
	printf("TOPIC: %s\n", p->discussion_topic);
	printf("AUTHOR: %s\n", p->discussion_author);
	printf("REPLY NUMBER:%d\n", p->discussion_reply);
	printf("CLICK NUMBER:%d\n", p->discussion_click);
	printf("TIME:%s\n", p->discussion_handin_time);
	printf("CONTENT:%s\n", p->discussion_content);
	printf("URL:%s\n", p->discussion_attachment_url);
	printf("NAME:%s\n", p->discussion_attachment_name);
	p_reply_element q;
	// fix reply print bug
	if(p->discussion_reply > 0)
	{

		q= p->reply_head->next;
		while(q)
		{
			print_reply_element(q);
			q = q->next;
		}
	}
}


// write_all_file_to_xmlfile
// this is a index file
void write_all_discussion_to_xmlfile(char *filename, p_discussion_element head)
{
#ifdef WIN32
	char gbk_filename[BUFFER_MAX_SIZE];
	u2g(filename,strlen(filename), gbk_filename, BUFFER_MAX_SIZE);
	msdosify(gbk_filename);
#endif
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, discussion = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);
	p_discussion_element p = head->next;

	char number_buf[8];
	char path[128];
	while(p)
	{
		xmlNodePtr id = NULL,topic = NULL, author = NULL, reply  = NULL, click = NULL, time = NULL;
		discussion = xmlNewChild(root_node, NULL, BAD_CAST "discussion", BAD_CAST "");
		sprintf(number_buf, "%d", p->id);
		id = xmlNewChild(discussion, NULL ,BAD_CAST "id" , BAD_CAST number_buf);
		topic = xmlNewChild(discussion, NULL ,BAD_CAST "topic" , BAD_CAST p->discussion_topic);
		author = xmlNewChild(discussion, NULL ,BAD_CAST "author" , BAD_CAST p->discussion_author);
		sprintf(number_buf, "%d", p->discussion_reply);
		reply = xmlNewChild(discussion, NULL ,BAD_CAST "reply" , BAD_CAST number_buf);
		sprintf(number_buf, "%d", p->discussion_click);
		click = xmlNewChild(discussion, NULL ,BAD_CAST "click" , BAD_CAST number_buf);
		if(p->discussion_handin_time)
			time =  xmlNewChild(discussion, NULL ,BAD_CAST "time" , BAD_CAST p->discussion_handin_time);
		if(get_dir_by_path(path, filename))
		{
			// create new path to path
			sprintf(path,"%s%d.xml", path, p->id);
			write_all_reply_to_xmlfile_by_discussion_id(path, p);
		}  
		p = p->next;
	}
#ifdef WIN32
	xmlSaveFormatFileEnc(gbk_filename, doc, "UTF-8", 1);
#endif
#ifndef WIN32
	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
#endif

	/*free the document */
	xmlFreeDoc(doc);

	xmlCleanupParser();

	xmlMemoryDump();//debug memory for regression tests*/
}

void write_all_reply_to_xmlfile_by_discussion_id(char *filename, p_discussion_element cur)
{
#ifdef WIN32
	char gbk_filename[BUFFER_MAX_SIZE];
	u2g(filename,strlen(filename), gbk_filename, BUFFER_MAX_SIZE);
	msdosify(gbk_filename);
#endif
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, discussion = NULL, reply = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);
	// write discussion
	xmlNodePtr topic = NULL, author = NULL,  time = NULL, content = NULL;

	char url_buf[BUFFER_MAX_SIZE];
	p_reply_element p = NULL;

	if(cur->reply_head)
		p = cur->reply_head->next;

	{
		discussion = xmlNewChild(root_node, NULL, BAD_CAST "discussion", BAD_CAST "");
		topic = xmlNewChild(discussion, NULL ,BAD_CAST "topic" , BAD_CAST cur->discussion_topic);
		author = xmlNewChild(discussion, NULL ,BAD_CAST "author" , BAD_CAST cur->discussion_author); 
		if(cur->discussion_handin_time)
			time =  xmlNewChild(discussion, NULL ,BAD_CAST "time" , BAD_CAST cur->discussion_handin_time);
		if(cur->discussion_content)
			content =  xmlNewChild(discussion, NULL ,BAD_CAST "content" , BAD_CAST cur->discussion_content);
		if(cur->discussion_attachment_name)
			xmlNewChild(discussion, NULL ,BAD_CAST "attachmentName" , BAD_CAST cur->discussion_attachment_name);
		if(cur->discussion_attachment_url)
		{
			if(url_to_xmlurl(url_buf, cur->discussion_attachment_url)) 
				xmlNewChild(discussion, NULL ,BAD_CAST "attachmentUrl" , BAD_CAST url_buf);
		}

		if(cur->discussion_attachment_location)
			xmlNewChild(discussion, NULL ,BAD_CAST "attachmentLocation" , BAD_CAST cur->discussion_attachment_location);
	}
	// reply
	while(p)
	{
		reply = xmlNewChild(root_node, NULL, BAD_CAST "reply", BAD_CAST "");
		if(p->reply_author)
			xmlNewChild(reply, NULL ,BAD_CAST "author" , BAD_CAST p->reply_author);
		if(p->reply_time)
			xmlNewChild(reply, NULL ,BAD_CAST "time" , BAD_CAST p->reply_time);
		if(p->reply_content)
			xmlNewChild(reply, NULL ,BAD_CAST "content" , BAD_CAST p->reply_content);
		if(p->reply_attachment_name)
			xmlNewChild(reply, NULL ,BAD_CAST "attachmentName" , BAD_CAST p->reply_attachment_name);
		if(p->reply_attachment_url)
		{
			if(url_to_xmlurl(url_buf, p->reply_attachment_url)) 
				xmlNewChild(reply, NULL ,BAD_CAST "attachmentUrl" , BAD_CAST url_buf);
		}

		if(p->reply_attachment_location)
			xmlNewChild(reply, NULL ,BAD_CAST "attachmentLocation" , BAD_CAST p->reply_attachment_location);
		p = p->next;
	}
#ifdef WIN32
	xmlSaveFormatFileEnc(gbk_filename, doc, "UTF-8", 1);
#endif
#ifndef WIN32
	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
#endif
	/*free the document */
	xmlFreeDoc(doc);

	xmlCleanupParser();

	xmlMemoryDump();//debug memory for regression tests*/
}


void download_reply_attachment(CURL *curl_web_handler,char *path, p_reply_element head)
{
	p_reply_element p = head->next;
	char buf[BUFFER_MAX_SIZE];
	bool first_reply_attachment = TRUE;
	while(p)
	{
		if(p->reply_attachment_url)
		{
			if(first_reply_attachment)
			{
				make_dir_recusive(path);
				first_reply_attachment = FALSE;
			}
			sprintf(buf,"%s%s",path, p->reply_attachment_name);
			p->reply_attachment_location = strdup(buf);
			download_file(curl_web_handler,p->reply_attachment_url, p->reply_attachment_location);
		}
		p = p->next;
	}
}
