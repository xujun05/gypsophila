#include <gypsophila.h>

int main(int argc, char *argv[])
{
	char username[32];
	char passwd[32];
	printf("WebCourse@Tsinghua(Console Edition)\n");
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
