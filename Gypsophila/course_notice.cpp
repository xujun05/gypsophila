#include "course_notice.h"

p_notice_element init_notice_list(p_notice_element head)
{
	head = (p_notice_element)malloc (sizeof(notice_element));
	head->next = NULL;
	head->notice_title = NULL;
	head->notice_publisher = NULL;
	head->notice_time = NULL;
	head->notice_body = NULL;
	head->is_highlight =  FALSE;
	return head;
}

bool init_notice_element(p_notice_element p)
{
	p->next = NULL;
	p->notice_title = NULL;
	p->notice_publisher = NULL;
	p->notice_time = NULL;
	p->notice_body = NULL;
	p->is_highlight =  FALSE;
	return TRUE;
}

// insert operation
// discard!!!
bool insert_element(p_notice_element pre, p_notice_element node)
{
	pre->next = node;
	node->next = NULL;
	return TRUE;
}


// destroy all
bool destroy_all_notice_element(p_notice_element head)
{
	p_notice_element pre = head;
	p_notice_element p = pre->next;
	while(p)
	{
		//printf("%d\n",p->notice_id);
		if(p->notice_title)
			free(p->notice_title);
		if(p->notice_publisher)
			free(p->notice_publisher);
		if(p->notice_time)
			free(p->notice_time);
		if(p->notice_body)
			free(p->notice_body);
		pre = p;
		p = p->next;
		free(pre);
	}
	free(head);
	return TRUE;
}

// Print
void print_notice_element(p_notice_element p)
{
	printf("ID:%d\n",p->notice_id);
	printf("PUBLISHER:%s\n",p->notice_publisher);
	printf("HIGHLIGHT:%d\n",p->is_highlight);
	printf("TIME:%s\n",p->notice_time);
	printf("TITLE:%s\n",p->notice_title);
	printf("CONTENT:%s\n",p->notice_body);
}

// write it to file
void write_all_notice_to_xmlfile(char *filename, p_notice_element head)
{
#ifdef WIN32
	char gbk_filename[BUFFER_MAX_SIZE];
	u2g(filename,strlen(filename), gbk_filename, BUFFER_MAX_SIZE);
	msdosify(gbk_filename);
#endif
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, notice = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);

	char number_buf[8];
	p_notice_element p = head->next;
	while(p)
	{
		xmlNodePtr notice_id = NULL, notice_publisher = NULL, notice_time = NULL,notice_title = NULL ,notice_content = NULL;
		sprintf(number_buf,"%d",p->notice_id);
		notice = xmlNewChild(root_node, NULL, BAD_CAST "notice", BAD_CAST "");
		xmlNewProp(notice, BAD_CAST "isHighLight", p->is_highlight == TRUE? BAD_CAST "true":BAD_CAST "false");
		notice_id = xmlNewChild(notice, NULL ,BAD_CAST "id" , BAD_CAST number_buf);
		notice_publisher = xmlNewChild(notice, NULL ,BAD_CAST "publisher" , BAD_CAST p->notice_publisher);
		notice_time = xmlNewChild(notice, NULL ,BAD_CAST "time" , BAD_CAST p->notice_time);
		notice_title = xmlNewChild(notice, NULL ,BAD_CAST "title" , BAD_CAST p->notice_title);
		notice_content = xmlNewChild(notice, NULL ,BAD_CAST "content" , BAD_CAST p->notice_body);
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

