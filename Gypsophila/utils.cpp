#include "utils.h"
#include <windows.h>
#include   <direct.h> 
#include  <io.h>


char *rindex(char *src, char ch)
{
	char * p =NULL;
	int i = 0;
	for(i = 0; i< strlen(src); i++)
	{
		if(src[i] == ch)
			p = src + i;
	}
	return p;
}

size_t echo_off(void *buf, size_t siez, size_t nmemb, void * user_p)
{
  return strlen((const char*)buf);
}

  
size_t print_data(void *buf, size_t size, size_t nmemb, void * user_p)
{
  printf("length:%dsize:%d,nmemb:%d\n",(int)strlen((const char *)buf),(int)size,(int)nmemb);
  printf("%s",(char *)(buf));
  return strlen((const char *)buf);
}

size_t copy_to_buffer(void *buf, size_t size, size_t nmemb, void * user_p)
{
  extern cache_memory cache;

#ifdef WIN32
  char *gbkbuffer  = (char *)malloc ( (size * nmemb + 1) * 4);

  int reture_code = u2g((char *)buf, size *nmemb, gbkbuffer, ( (size * nmemb +1) * 2));

  if(reture_code < 0)
  {
	  /*extern int errno;
	  if(errno == EILSEQ)
	  {
		printf("EILSEQ ERR!\n");
	  }
	  else if (errno == EINVAL)
	  {
		printf("")
	  }*/
	  printf("Error, UTF-8 to GBK EROOR");
	  //exit(1);
  }
#endif

#ifdef LINUX
	while(cache.size < cache.offset + size * nmemb + 1)
#endif

#ifdef VxWorks
	while(cache.size < cache.offset + size * nmemb + 1)
#endif

#ifdef WIN32
	while(cache.size < cache.offset + (size * nmemb + 1) * 2)
#endif
  {
    cache.mem = (char *)my_realloc(cache.mem, cache.size + CACHE_DEC_SIZE);
    if(cache.mem)
      cache.size = cache.size + CACHE_DEC_SIZE;
    else
    {  
      cache.size = 0;
      cache.offset = 0;
      break;
    }
  }
  if(cache.mem){
#ifdef WIN32
	memcpy (&(cache.mem[cache.offset]), gbkbuffer, strlen(gbkbuffer));
	cache.offset += strlen(gbkbuffer);
	free(gbkbuffer);
#endif
#ifdef LINUX
    memcpy (&(cache.mem[cache.offset]), buf, size * nmemb);
	cache.offset += size * nmemb;
#endif
#ifdef VxWorks
	memcpy (&(cache.mem[cache.offset]), buf, size * nmemb);
	cache.offset += size * nmemb;
#endif
    
    cache.mem[cache.offset] ='\0';

#ifdef WIN32
	return size * nmemb;
#endif
#ifdef LINUX
	return size * nmemb;
#endif
#ifdef VxWorks
	return size * nmemb;
#endif
  }
  else
    return 0;
}


void * my_realloc(void * origin_ptr, size_t size)
{
  if(origin_ptr)
    return realloc(origin_ptr, size);
  else
    return malloc(size);
}



size_t write_to_file(void *buf, size_t size, size_t nmemb, void *user_p)
{
  FILE *fp = (FILE*)user_p;
  fwrite(buf,size,nmemb,fp);
  return nmemb;
}

/*
  Give an url and get this web page.
*/
bool  copy_web_page_to_memory(char *url,void *parse_function, void *userdata)
{
  curl_easy_setopt(curl_web_handler, CURLOPT_URL,url);
  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEFUNCTION, parse_function);
  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEDATA, userdata);
  CURLcode code;
  code = curl_easy_perform(curl_web_handler);
  if(CURLE_OK != code)
    return FALSE;
  return TRUE;
}

/*
  Make Directory on 3 Platforms: Mac Linux Windows
 */
bool make_dir(char * path)
{
  // LINUX MAC CODE
  if(access(path,0) == -1)
  {
    if(mkdir(path))
    {
      return FALSE;
    }
  }
  return TRUE;
}

bool make_dir_recusive(char *path)
{
  char path_buf[BUFFER_MAX_SIZE];
  char *p_start = path;
  if( *p_start == PATH_SPILIT_CHAR)
  {
    p_start++;
  }
  while((p_start = strchr(p_start,PATH_SPILIT_CHAR)))
  {
    memcpy(path_buf, path, p_start - path);
    path_buf[p_start - path] = 0;
    make_dir(path_buf);
    p_start++;
    if(*p_start == 0)
      break;
  }
  make_dir(path);
  return TRUE;
}



/*
  Give an url and get this web page.
 */
bool download_web_page(char *url)
{

  return TRUE;
}

/*
  Give an url and get the file downloaded.
*/
bool download_file(char *url,char *save_name)
{
  extern CURL * curl_web_handler;
  curl_easy_setopt(curl_web_handler, CURLOPT_URL, url);
  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEFUNCTION, &write_to_file);
  FILE * fp = NULL;
  fp = fopen(save_name, "ab+");
  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEDATA, fp);
  fclose(fp);
  CURLcode return_code;
  return_code = curl_easy_perform(curl_web_handler);

  // http response code
  int ret_code = 0;
  return_code = curl_easy_getinfo(curl_web_handler, CURLINFO_RESPONSE_CODE , &ret_code);
  if(CURLE_OK == return_code && ret_code == FILE_EXISTS)
  {
    printf("File download OK!\n");
    fclose(fp);
  }
  else
  {
    printf("File not exists!\n");
  }
  
  return TRUE;
}

/*
  Auth the user and Password.
*/
bool login_learn_tsinghua(char * username,char *password)
{
  char buf[BUFFER_MAX_SIZE];
  memset(buf,0,BUFFER_MAX_SIZE);
  sprintf(buf,"userid=%s&userpass=%s",username,password);

  // set echo off
  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEFUNCTION, &echo_off);
  
  curl_easy_setopt(curl_web_handler,CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_easy_setopt(curl_web_handler,CURLOPT_POSTFIELDS, buf);
  curl_easy_setopt(curl_web_handler, CURLOPT_URL,"https://learn.tsinghua.edu.cn/MultiLanguage/lesson/teacher/loginteacher.jsp");
  // Save cookies from Learn Tsinghua
  curl_easy_setopt(curl_web_handler,CURLOPT_COOKIEJAR,"cookie_user.txt");
  // Auth User and Password
  CURLcode code = curl_easy_perform(curl_web_handler);
  if(CURLE_OK != code)
    return FALSE;

  // Jump to https url
  curl_easy_setopt(curl_web_handler,CURLOPT_URL,"https://learn.tsinghua.edu.cn/MultiLanguage/lesson/teacher/loginteacher_action.jsp");
  code = curl_easy_perform(curl_web_handler);
  if(CURLE_OK != code)
    return FALSE;

  // 2's Jump to http url
  curl_easy_setopt(curl_web_handler,CURLOPT_URL,"http://learn.tsinghua.edu.cn/MultiLanguage/lesson/teacher/mainteacher.jsp");
  code = curl_easy_perform(curl_web_handler);
  if(CURLE_OK != code)
    return FALSE;

  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEFUNCTION, &echo_off);
  curl_easy_setopt(curl_web_handler,CURLOPT_URL,"http://learn.tsinghua.edu.cn/MultiLanguage/lesson/student/mainstudent.jsp");
  code = curl_easy_perform(curl_web_handler);

  // last clear write function to echo_off
  curl_easy_setopt(curl_web_handler, CURLOPT_WRITEFUNCTION, &echo_off);
  if(CURLE_OK != code)
    return FALSE;

  return TRUE;
}


// list. the first element contains no data.
p_list_entity basic_parse_page(char *buf, char *prefix,const char *suffix)
{
  p_list_entity head = (p_list_entity)malloc(sizeof(list_entity));
  head->next = NULL;

  p_list_entity pre = head;
  p_list_entity cur = NULL;
  
  char *p = buf;
  char *q;
  char *r = NULL;
  bool entity_start = FALSE;
  
  while(*p)
  {
    q = prefix;
    r = p;
    if(*p == *prefix)
    {
      while(*r && *q)
      {
        if(*r == *q)
        {
          r++;
          q++;
        }
        else
          break;
      }
    }
    
    // match success
    if(!*q)
    {
      p = r;
      while(p && *p)
      {
        if(strstr(prefix, "<tr"))
        {
          if(*p == '<' && *(p + 1) == 't' && *(p + 2) == 'd')
          {
            entity_start = TRUE;
            // start copy
            char* end_of_copy = strstr(p,suffix);
            cur = (p_list_entity)malloc(sizeof(list_entity));
            cur->entity = (char *)malloc((end_of_copy - p + 1) * sizeof(char));
            memcpy(cur->entity, p, end_of_copy - p);
            *(cur->entity + (end_of_copy - p)) = '\0';
            pre->next = cur;
            pre = cur;
            cur->next = NULL;
            cur = NULL;
            break;
          
            entity_start = FALSE;
            break;
          }
        }
        else
        {
          entity_start = TRUE;
          p = strchr(p, '>') + 1;
          // start copy
          char* end_of_copy = strstr(p,suffix);
          cur = (p_list_entity)malloc(sizeof(list_entity));
          cur->entity = (char *)malloc((end_of_copy - p + 1) * sizeof(char));
          memcpy(cur->entity, p, end_of_copy - p);
          *(cur->entity + (end_of_copy - p)) = '\0';
          pre->next = cur;
          pre = cur;
          cur->next = NULL;
          cur = NULL;
          break;
          
          entity_start = FALSE;
          break;
        }
        p++;
      }
    }
    p++;
  }

  return head;
}

// Reset Cache
bool reset_cache_memory()
{
  extern cache_memory cache;
  memset(cache.mem, 0 ,cache.size);
  cache.offset = 0;
  return TRUE;
}

// String Operation, ignore str's length
bool str_add_prefix(const char *prefix, char *str)
{
  int i = 0;
  str[strlen(str) + strlen(prefix)] ='\0';
  
  for(i = strlen(str) - 1; i >=0;i--)
  {
    str[i + strlen(prefix)] = str[i];
  }
  memcpy(str, prefix, strlen(prefix));
  
  return TRUE;
}

// destroy all entity memory
bool destroy_all_entity_memory(p_list_entity head)
{
  p_list_entity p = head;
  p_list_entity q = p->next;
  while(q)
  {
    if(q->entity)
      free(q->entity);
    p = q;
    q = q->next;
    free(p);
  }
  free(head);
  return TRUE;
}

/*
  Extract Content Between Tags.<td...> </td>
  * Prefix: <td....
  * suffix: </td>
  retrun the first content.
*/
char * extract_content_between_tags(char *src, char *des, const char *prefix, const char *suffix)
{
  char *p_start = strstr(src,prefix);
  if(!p_start)
    return NULL;
  p_start = strchr(p_start + 1, '>') + 1;
  if(!p_start)
    return NULL;
  char *p_end = strstr(p_start, suffix);
  if(p_end)
  {
    strncpy(des, p_start, p_end - p_start);
    des[p_end - p_start] = 0;
    return p_end + strlen(suffix);
  }
  else
    return NULL;
}

// return pointer has moved.....
char *extract_content_between_fix(char *src, char *des, const char *prefix, const char *suffix)
{
  char *p_start = strstr(src,prefix);
  if(!p_start)
    return NULL;
  p_start += strlen(prefix);
  if(!p_start)
    return NULL;
  char *p_end = strstr(p_start, suffix);
  if(p_end)
  {
    strncpy(des, p_start, p_end - p_start);
    des[p_end - p_start] = 0;    
    return p_end + strlen(suffix);
  }
  else
    return NULL;
}


// Trip String
bool string_trip(char *src)
{
  char *p_start = src;
  char *p_end = src + strlen(src) - 1;
  char *p = src;
  
  while(p_start)
  {
    if((*p_start == ' ') || (*p_start == '\r') || (*p_start == '\n') || (*p_start == '\t')) 
      p_start++;
    else
      break;
  }

  while(p_end)
  {
    if((*p_end == ' ') || (*p_end == '\r') || (*p_end == '\n') || (*p_end == '\t')) 
      p_end--;
    else break;
  }
  p_end++;
  while( p_end - p_start > 0)
  {
    *p = *p_start;
    p_start++;
    p++;
  }
  *p = 0;
  
  return TRUE;
}


int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen)
{
  iconv_t handle;
  size_t in_len = (size_t) inlen;
  size_t out_len = (size_t) outlen;
  const char **p_in = (const char **)&inbuf;
  char **p_out = &outbuf;
 
  handle = iconv_open(to_charset, from_charset);

  int one = 1;
  iconvctl(handle,ICONV_SET_DISCARD_ILSEQ,&one);

  if(handle == NULL)
    return -1;

  memset(outbuf, 0 ,outlen);
  if( iconv(handle, p_in, &in_len, p_out, &out_len) == -1)
    return -1;

  iconv_close(handle);

  return 0;
}


int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
  return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
  return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}


/* Remove HTML tags and convert html label
 * 
 * &nbsp;
 * &ldquo; ->"""
 * &rdquo;->"""
 * <br/> ->"\n"
 * &hellip;гн>"..."
 * &amp;->"&"
 * &lt; ->"<"
 * &gt;->">"
 * &quot;->"""
 * else remove it
 */
char *remove_and_convert_html(char *src)
{
  int i = 0;
  int buf_index = 0;
  int length = strlen(src);
  // Bug need to fixed
  //char *buf = malloc (strlen(src));
  char buf[CACHE_INIT_SIZE];
  
  bool tag_start = FALSE;
  
  char save_tag_prefix_3[4];
  save_tag_prefix_3[3] = 0;
  int pointer_tag_prefix = 0;
  bool is_new_line = FALSE;

  char some_special_symbol_buffer[9];
  int k = 0;
  
  for(i = 0;i < length;i++)
  {
    if(tag_start)
    {
      if( src[i] == '>')
      {
        tag_start = FALSE;
        pointer_tag_prefix = 0;
        memset(save_tag_prefix_3, 0 ,4);
        if(is_new_line)
        {
          buf[buf_index++] = '\n';
          is_new_line = FALSE;
        }
        continue;
      }
      if(pointer_tag_prefix == 3 && !is_new_line)
      {
        is_new_line = (strcmp(save_tag_prefix_3, "br ") == 0 || strcmp(save_tag_prefix_3, "br/") == 0);
        pointer_tag_prefix++;
        continue;
      }
      else if(pointer_tag_prefix < 3)
        save_tag_prefix_3[pointer_tag_prefix++] = src[i];
      continue;
    }
    else
    {
      if(src[i] == '<')
      {
        tag_start = TRUE;
        continue;
      }
      else if(src[i] == '&')
      {
        k = i;
        while( k <= i + 8 && *(src + k) )
        {
          some_special_symbol_buffer[k - i] = src[k];
          k++;
        }
        if(strstr(some_special_symbol_buffer,"&nbsp;"))
        {
          buf[buf_index++] = ' ';
          i += strlen("&nbsp;") - 1;
        }
          
        else if(strstr(some_special_symbol_buffer, "&ldquo;"))
        {
          buf[buf_index++] = '\"';
          i += strlen("&ldquo;") - 1;
        }
        else if(strstr(some_special_symbol_buffer, "&rdquo;"))
        {
          buf[buf_index++] = '\"';
          i += strlen("&rdquo;") - 1;
        }
        else if(strstr(some_special_symbol_buffer, "&hellip;"))
        {
          buf[buf_index++] = '.';
          buf[buf_index++] = '.';
          buf[buf_index++] = '.';
          i += strlen("&hellip;") - 1;
        }
        else if(strstr(some_special_symbol_buffer, "&amp;"))
        {
          buf[buf_index++] = '&';
          i += strlen("&amp;") - 1;
        }
        else if(strstr(some_special_symbol_buffer, "&lt;"))
        {
          buf[buf_index++] = '<';
          i += strlen("&lt;") - 1;
        }
        else if(strstr(some_special_symbol_buffer, "&gt;"))
        {
          buf[buf_index++] = '>';
          i += strlen("&gt;") - 1;
        }
        else if(strstr(some_special_symbol_buffer, "&quot;"))
        {
          buf[buf_index++] = '"';
          i += strlen("&quot;") - 1;
        }
        else if(strchr(some_special_symbol_buffer, ';'))
        {
          buf[buf_index++] = ' ';
          i += (strchr(some_special_symbol_buffer, ';') - some_special_symbol_buffer);
        }
        else
          buf[buf_index++] = src[i];
      }
      else
        buf[buf_index++] = src[i];
    }
  }
  buf[buf_index] = 0;
  strcpy(src,buf);
  
  //free(buf);
  return src;
}


bool url_to_xmlurl(char *des, char *src)
{
  unsigned int i = 0;
  int des_index = 0;
  
  for(i = 0;i < strlen(src); i++)
  {
    if(src[i] != '&' && src[i] != '<')
      des[des_index++] = src[i];
    else
    {
      if(src[i] == '&')
      {
        des[des_index++] = '&';
        des[des_index++] = 'a';
        des[des_index++] = 'm';
        des[des_index++] = 'p';
        des[des_index++] = ';'; 
      }
      else
      {
        des[des_index++] = '&';
        des[des_index++] = 'l';
        des[des_index++] = 't';
        des[des_index++] = ';';
      }
    }
  }
  des[des_index] = 0;
  return TRUE;  
}

// file path
bool get_dir_by_path(char *dir, char *path)
{
  char *p_last = rindex(path, PATH_SPILIT_CHAR);
  if(!*p_last)
    return FALSE;
  else
  {
    memcpy(dir, path, p_last - path + 1);
    dir[p_last - path + 1] = '\0';
  }
  return TRUE;
}


bool get_file_by_path(char *file, char *path)
{
  char *p_last = rindex(path, PATH_SPILIT_CHAR);
  if(!*p_last)
    return FALSE;
  else
  {
    strcpy(file, p_last);
  }
  return TRUE;
}


bool remove_path_spilit_char(char *buf)
{
  char * p = buf;
  p = strchr(buf, PATH_SPILIT_CHAR);
  // NO SPILIT CHAR
  if(p == NULL)
    return TRUE;
  buf[p-buf] = 0;
  return TRUE;
}