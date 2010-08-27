#include "course_assignments.h"

p_assignment_element init_assignment_list(p_assignment_element head)
{
	head = (p_assignment_element)malloc (sizeof(assignment_element));
	head->next = NULL;
	head->assignment_name = NULL;
	head->assignment_time = NULL;
	head->assignment_deadline = NULL;
	head->assignment_status = NULL;
	head->assignment_score = NULL;// another page
	head->assignment_comment = NULL;
	head->assignment_description = NULL; //3th page
	head->assignment_attachment_url = NULL; //3th page
	head->assignment_attachment_name = NULL;// 3th page
	head->assignment_attachment_location = NULL;
	head->my_handin_description = NULL;//3th page
	head->my_handin_attachment_url = NULL; //3th page
	head->my_handin_attachment_name = NULL;//3th page
	head->my_handin_attachment_location = NULL;
	return head;
}

bool  init_assignment_element (p_assignment_element p)
{
	if(p)
	{ 
		p->next = NULL;
		p->assignment_name = NULL;
		p->assignment_time = NULL;
		p->assignment_deadline = NULL;
		p->assignment_status = NULL;
		p->assignment_score = NULL;// another page
		p->assignment_comment = NULL;
		p->assignment_description = NULL; //3th page
		p->assignment_attachment_url = NULL; //3th page
		p->assignment_attachment_name = NULL;// 3th page
		p->assignment_attachment_location = NULL;
		p->my_handin_description = NULL;//3th page
		p->my_handin_attachment_url = NULL; //3th page
		p->my_handin_attachment_name = NULL;//3th page
		p->my_handin_attachment_location = NULL;
		return TRUE;
	}
	else return FALSE;
}
// insert operation
// discard!!!
void print_assignment_element(p_assignment_element cur)
{
	printf("ASSIGNMENT NAME: %s\n",cur->assignment_name);
	printf("ASSIGNMENT TIME: %s\n",cur->assignment_time);
	printf("ASSIGNMENT DEADLINE: %s\n", cur->assignment_deadline);
	printf("ASSIGNMENT STATUS: %s\n", cur->assignment_status);
	printf("ASSIGNMENT SCORE: %s\n", cur->assignment_score);
	printf("ASSIGNMENT COMMENT: %s\n", cur->assignment_comment);
	printf("ASSIGNMENT DESCRIPTION: %s\n", cur->assignment_description);
	printf("ASSIGNMENT ATTACHMENT URL: %s\n",cur->assignment_attachment_url);
	printf("ASSIGNMENT ATTACHMENT NAME: %s\n",cur->assignment_attachment_name);
	printf("HAND_IN DESCRIPTION: %s\n",cur->my_handin_description);
	printf("HAND_IN ATTACHMENT URL :%s\n", cur->my_handin_attachment_url);
	printf("HAND_IN ATTACHMENT NAME: %s\n",cur->my_handin_attachment_name);
}


// destroy all
bool destroy_all_assignment_element(p_assignment_element head)
{
	p_assignment_element pre = head;
	p_assignment_element p = pre->next;
	while(p)
	{
		if(p->assignment_name)
			free(p->assignment_name);
		if(p->assignment_time)
			free(p->assignment_time);
		if(p->assignment_deadline)
			free(p->assignment_deadline);
		if(p->assignment_status)
			free(p->assignment_status);
		if(p->assignment_score)
			free(p->assignment_score);
		if(p->assignment_comment)
			free(p->assignment_comment);
		if(p->assignment_description)
			free(p->assignment_description);
		if(p->assignment_attachment_url)
			free(p->assignment_attachment_url);
		if(p->assignment_attachment_name)
			free(p->assignment_attachment_name);
		if(p->assignment_attachment_location)
			free(p->assignment_attachment_location);
		if(p->my_handin_description)
			free(p->my_handin_description);
		if(p->my_handin_attachment_url)
			free(p->my_handin_attachment_url);
		if(p->my_handin_attachment_name)
			free(p->my_handin_attachment_name);
		if(p->my_handin_attachment_location)
			free(p->my_handin_attachment_location);
		pre = p;
		p = p->next;
		free(pre);
	}
	free(head);
	return TRUE;

}


// write_all_file_to_xmlfile
void write_all_assignment_to_xmlfile(char *filename, p_assignment_element head)
{
#ifdef WIN32
	char gbk_filename[BUFFER_MAX_SIZE];
	u2g(filename,strlen(filename), gbk_filename, BUFFER_MAX_SIZE);
	msdosify(gbk_filename);
#endif
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, assignment = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);

	char url_buf[BUFFER_MAX_SIZE];

	p_assignment_element p = head->next;
	while(p)
	{
		xmlNodePtr name = NULL, time = NULL, deadline = NULL,status = NULL,score = NULL, comment = NULL, description = NULL, attachment_url = NULL, attachment_name = NULL,attachment_location = NULL, hw_des = NULL, hw_name = NULL, hw_location = NULL;
		assignment = xmlNewChild(root_node, NULL, BAD_CAST "assignment", BAD_CAST "");
		if(p->assignment_name)
			name = xmlNewChild(assignment, NULL ,BAD_CAST "name" , BAD_CAST p->assignment_name);
		if(p->assignment_time)
			time = xmlNewChild(assignment, NULL ,BAD_CAST "time" , BAD_CAST p->assignment_time);
		if(p->assignment_deadline)
			deadline = xmlNewChild(assignment, NULL ,BAD_CAST "deadline" , BAD_CAST p->assignment_deadline);
		if(p->assignment_status)
			status = xmlNewChild(assignment, NULL ,BAD_CAST "status" , BAD_CAST p->assignment_status);
		if(p->assignment_score)
			score = xmlNewChild(assignment, NULL ,BAD_CAST "score" , BAD_CAST p->assignment_score);
		if(p->assignment_comment)
			comment = xmlNewChild(assignment, NULL ,BAD_CAST "comment" , BAD_CAST p->assignment_comment);
		if(p->assignment_description)
			description = xmlNewChild(assignment, NULL ,BAD_CAST "description" , BAD_CAST p->assignment_description);
		if(p->assignment_attachment_url)
		{
			if(url_to_xmlurl(url_buf, p->assignment_attachment_url))
				attachment_url = xmlNewChild(assignment, NULL ,BAD_CAST "attachmentUrl" , BAD_CAST url_buf);
		}

		if(p->assignment_attachment_name)
			attachment_name = xmlNewChild(assignment, NULL ,BAD_CAST "attachmentName" , BAD_CAST p->assignment_name);
		if(p->assignment_attachment_location)
			attachment_location = xmlNewChild(assignment, NULL ,BAD_CAST "attachmentLocation" , BAD_CAST p->assignment_attachment_location);
		if(p->my_handin_description)
			hw_des = xmlNewChild(assignment, NULL ,BAD_CAST "MyHomeworkDescription" , BAD_CAST p->my_handin_description);
		if(p->my_handin_attachment_name)
			hw_name = xmlNewChild(assignment, NULL ,BAD_CAST "MyHomeworkAttachmentName" , BAD_CAST p->my_handin_attachment_name);
		if(p->my_handin_attachment_url)
		{
			if(url_to_xmlurl(url_buf, p->my_handin_attachment_url))
				hw_name = xmlNewChild(assignment, NULL ,BAD_CAST "MyHomeworkAttachmentUrl" , BAD_CAST url_buf);
		}

		if(p->my_handin_attachment_location)
			hw_location = xmlNewChild(assignment, NULL ,BAD_CAST "MyHomeworkAttachmentLocation" , BAD_CAST p->my_handin_attachment_location);
		//file_url = xmlNewChild(file, NULL ,BAD_CAST "remoteUrl" , BAD_CAST p->file_url);
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

