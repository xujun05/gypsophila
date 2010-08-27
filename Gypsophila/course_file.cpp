#include "course_file.h"

p_file_element init_file_list(p_file_element head)
{
	head = (p_file_element)malloc (sizeof(file_element));
	head->next = NULL;
	head->file_type = NULL;
	head->file_title = NULL;
	head->file_orign_name = NULL;
	head->file_abstract = NULL;
	head->file_up_time = NULL;
	head->file_url = NULL;
	head->file_local_location = NULL;
	return head;
}

// init discusion element
bool  init_file_element (p_file_element p)
{
	p->next = NULL;
	p->file_type = NULL;
	p->file_title = NULL;
	p->file_orign_name = NULL;
	p->file_abstract = NULL;
	p->file_up_time = NULL;
	p->file_url = NULL;
	p->file_local_location = NULL;
	return TRUE;
}

void print_file_element(p_file_element p)
{
	printf("FILE TYPE NAME:%s\n",p->file_type);
	printf("FILE ID:%d\n",p->file_id);
	printf("FILE TITLE: %s\n",p->file_title);
	printf("FILE SERVER NAME: %s\n", p->file_orign_name);
	printf("FILE ABSTRACT:%s\n",p->file_abstract);
	printf("FILE UP TIME: %s\n", p->file_up_time);
	printf("FILE URL:%s\n",p->file_url);
}
// destroy all
bool destroy_all_file_element(p_file_element head)
{
	p_file_element pre = head;
	p_file_element p = pre->next;
	while(p)
	{
		if(p->file_type)
			free(p->file_type);
		if(p->file_title)
			free(p->file_title);
		if(p->file_orign_name)
			free(p->file_orign_name);
		if(p->file_abstract)
			free(p->file_abstract);
		if(p->file_up_time)
			free(p->file_up_time);
		if(p->file_url)
			free(p->file_url);

		pre = p;
		p = p->next;
		free(pre);
	}
	free(head);
	return TRUE;

}


void write_all_file_to_xmlfile(char *filename, p_file_element head)
{
#ifdef WIN32
	char gbk_filename[BUFFER_MAX_SIZE];
	u2g(filename,strlen(filename), gbk_filename, BUFFER_MAX_SIZE);
	msdosify(gbk_filename);
#endif
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, file = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);

	char number_buf[8];
	char url_buf[BUFFER_MAX_SIZE];

	p_file_element p = head->next;
	while(p)
	{
		xmlNodePtr file_id = NULL, file_type = NULL, file_up_time = NULL, file_title = NULL, file_server_name = NULL, file_abstract= NULL,file_url = NULL ,file_local_location = NULL;
		sprintf(number_buf,"%d",p->file_id);
		file = xmlNewChild(root_node, NULL, BAD_CAST "file", BAD_CAST "");
		file_id = xmlNewChild(file, NULL ,BAD_CAST "id" , BAD_CAST number_buf);
		file_type = xmlNewChild(file, NULL ,BAD_CAST "type" , BAD_CAST p->file_type);
		file_up_time = xmlNewChild(file, NULL ,BAD_CAST "uploadTime" , BAD_CAST p->file_up_time);
		if(url_to_xmlurl(url_buf, p->file_title))
			file_title = xmlNewChild(file, NULL ,BAD_CAST "title" , BAD_CAST url_buf);
		file_server_name = xmlNewChild(file, NULL ,BAD_CAST "serverName" , BAD_CAST p->file_orign_name);
		file_abstract = xmlNewChild(file, NULL ,BAD_CAST "abstract" , BAD_CAST p->file_abstract);
		// convert
		if(url_to_xmlurl(url_buf, p->file_url))
			file_url = xmlNewChild(file, NULL ,BAD_CAST "remoteUrl" , (const xmlChar *)url_buf);
		file_local_location = xmlNewChild(file, NULL ,BAD_CAST "localLocation" , BAD_CAST p->file_local_location);
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
