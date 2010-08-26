#include <gypsophila.h>

int main(int argc, char *argv[])
{
	char username[32];
	char passwd[32];
	printf("WebCourse@Tsinghua(Console Edition)\n");
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
	getPasswd("Please enter your password:", passwd, 32);
	printf("Default path for saving file is .\\Course:");
	scanf("%s", path);
	if(path[0] == NULL)
		strcpy(path, "Course");



	if(!login_learn_tsinghua(curl_web_handler, username,passwd))
	{
		printf("Auth Failed.\n");
		return -1;
	}
	printf("Login OK\n");


	init_cache();

	mirror_course(curl_web_handler, path);
	//mirror_course_frome_course_id(start_id, path);

	printf("Print OK\n");

	clean_cache();

	curl_easy_cleanup(curl_web_handler);

	curl_global_cleanup();

	int k;
	scanf("%d", &k);
	return 0;
}
