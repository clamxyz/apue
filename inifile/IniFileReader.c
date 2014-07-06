#include "IniFileReader.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
/**************************************Type Defination************************************************/
/* Error Message Map */
typedef struct 
	{ 
		int error_code; 
		char *error_message;
	} emmap;
	
	/* Indicator for INIFILE's status */
typedef enum {Opened = 1, Closed = 2} open_status_t;
	
	/* Tree Node */
struct node
{
	char *key;
	char *value;
	struct node *children; 
	struct node *brother;
};

/*INIFILE struct*/
struct INIFILE{
	FILE *fp;
	long cookie;
	int err_code;
	open_status_t open_status;
	struct node *root;
	void (*pfn_free)(void *);
	struct node* (*pfn_create_tree)();
	struct node* (*pfn_add_child)(struct node *parent, char *key, char *value);
	struct node* (*pfn_add_brother)(struct node *elder_brother, char *key, char *value);
};

/***********************************End Type Defination***********************************************/

/***********************************Global Variables and Macro *****************************************/
#define ERR_MAP_LEN	(sizeof(msg_map) / sizeof(msg_map[0]) - 1)

#define NO_ERROR						0
#define NULL_VALUE					1
#define MALLOC_FAILED				2
#define INVALID_COOKIE			3
#define INVALID_LINE				4	

static int err_code = -1;

static emmap msg_map[] = {
		{NO_ERROR,				"No Error."},
		{NULL_VALUE,			"Null Pointer Refrence."},
		{MALLOC_FAILED,		"Malloc Failed."},
		{INVALID_COOKIE,	"Invalid Cookie."},
		{INVALID_LINE,		"Invalid line."},
		{-1, NULL}
};
/***********************************End Global Variables and Macro************************************/

/**********************************Private Function Declaration****************************************/
static int internal_ifr_open(struct INIFILE *ifr);
static void internal_tree_free(void *ifr);
struct node* internal_create_tree();
static struct node* internal_add_child(struct node *parent, char *key, char *value);
static struct node* internal_add_brother(struct node *elder_brother, char *key, char *value);
static void print_node(struct node *root);
/******************************End Private Function Declaration***************************************/

/*
*	Function: open a ini file
*	Parameter: pfile_name	- Initialization File Name.
*	Return:
*		if success return NOT NULL INITFILE pointer.
*		if failed return NULL and you can get error code by IfrGetLastError
*/
struct INIFILE* IfrOpen(const char *pfile_name)
{
		INIFILE *ifr;
		FILE *fp = NULL;
	 if (pfile_name == NULL)
	 {
	 		err_code = NULL_VALUE;
	 		return NULL;
	 }
		ifr = (INIFILE *)malloc(sizeof(struct INIFILE));
		if (ifr == NULL)
		{
			err_code = MALLOC_FAILED;
			return NULL;
		}
		fp = fopen(pfile_name, "r");
		if (fp == NULL)
		{
				printf("Open file failed[%d][%s]\n", errno, strerror(errno));
				free(ifr);
				return NULL;
		}
		ifr->cookie = (long)ifr;
		ifr->err_code = 0;
		ifr->open_status = Closed;
		ifr->fp = fp;
		ifr->root = NULL;
		ifr->pfn_free = internal_tree_free;
		ifr->pfn_create_tree = internal_create_tree;
		ifr->pfn_add_child = internal_add_child;
		ifr->pfn_add_brother = internal_add_brother;
		
	if (internal_ifr_open(ifr))
	{
			err_code = ifr->err_code;
			free(ifr);
			fclose(fp);
			return NULL;
	}
	fclose(fp);
	return ifr;
}

/*
*	Function: close a opened file. if pfile do not assocate a valid INIFILE, the operate will failed.
* Parameter: pfile - a valid INITFILE pointer.
* Return:
*		if success return 0, others return -1.
*/
int IfrClose(struct INIFILE *pfile)
{
	if (pfile == NULL || pfile->open_status == Closed)
		return 0;
	if (pfile->open_status == Closed)
		return 0;
	pfile->pfn_free(pfile->root);
	free(pfile);
	return 0;
}

/*
*	Function: get last error code
* Parameter: pfile - pointer to a open ini file.
* Return:
*		return error code, if previous operator didn't occur a error, the error is 0.
*/
int IfrGetLastError(struct INIFILE *pfile)
{
	if (pfile == NULL)
			return err_code;
	if (pfile->cookie != (long)pfile)
		return INVALID_COOKIE;
	return pfile->err_code;
}

/*
*	Function: get last error message
* Parameter: pfile - pointer to a open ini file.
* Return:
*		return error message. if previous operator didn't occur a error, the message is "No Error." with a linefeed.
*/
char *IfrGetLastErrMsg(struct INIFILE *pfile)
{
		return pfile == NULL ? msg_map[err_code].error_message : msg_map[pfile->err_code].error_message;
}

/*
*	Function: get a value by section and key.
* Parameter: 
*			section - the section name
*			key - the key name
* Return:
*			if success ,return the value; if failed, return NULL.
* Note:
*			if section is NULL, get the default section.					
*/
char *IfrGetValueBySection(struct INIFILE *ifr, const char *section, const char *key)
{
		struct node *brother;
	
		if (ifr->cookie != (long)ifr)
			return NULL;
		if (section == NULL && key == NULL || key == NULL )
			return NULL;
		if (section == NULL)
		{
			brother = ifr->root->children;
			while (brother && strcmp(brother->key, key))
				brother = brother->brother;
			return brother == NULL ? NULL : brother->value;
		}
		brother = ifr->root->children;
		while(brother && strcmp(brother->key, section))
			brother = brother->brother;
		if (brother)
		{
			brother = brother->children;
			while(brother && strcmp(brother->key, key))
				brother = brother->brother;
			if (brother) return brother->value;
		}
		return NULL;
}

/*
*	Function: get a value by section and key.
* Parameter: 
*			path - the path of section, for example /section1/key1
* Return:
*			if success ,return the value; if failed, return NULL.				
*/
char *IfrGetValueByPath(struct INIFILE *ifr, const char *path)
{
		struct node *root = ifr->root;
		struct node *children, *brother;
		char *section, *key, *value, szPath[1024];
		char *saveptr;
		if (root == NULL)
		{
			printf("root is null\n");
			return NULL;
		}
		if (*path != '/')
		{
			printf("invalid path[%s]\n", path);
			return NULL;
		}
		strcpy(szPath, path+1);
		section = strtok_r(szPath, "/", &saveptr);
		if (section == NULL)
		{
			printf("It's a dir[%s]\n", szPath);
			return NULL;
		}
		
		if (NULL == (key = strtok_r(NULL, "/", &saveptr)))
		{
			key = szPath + strlen(section);
		}
		
		brother = root->children;
		while(brother && strcmp(brother->key, section))
		{
			brother = brother->brother;
		}
		if (brother == NULL)
		{
			printf("invalid section[%s]\n", path);
			return NULL;
		}
		/*path not contains key*/
		if (brother->value == NULL && key[0] == 0)
		{
			printf("it's a dir[%s]\n", path);
			return NULL;
		}
		else if (brother->value != NULL)
		{
			return brother->value;
		}
		brother = brother->children;
		while(brother && strcmp(brother->key, key))
		{
			brother = brother->brother;
		}
			if (brother == NULL)
		{
			printf("invalid section[%s]\n", path);
			return NULL;
		}
		if (brother->value == NULL)
		{
			printf("it's a dir[%s]\n", path);
			return NULL;
		}
		return brother->value;
}

static void internal_free_node(struct node * root)
{
	struct node *pchild, *pbrother;
	if (root == NULL) return;
	pchild = root->children;
	if (pchild != NULL)
		internal_free_node(pchild);
	pbrother = root->brother;
	if (pbrother != NULL)
		internal_free_node(pbrother);
	if (root->key != NULL)
	{
		printf("free key[%s]\n", root->key);
		free(root->key);
	}
	if (root->value != NULL)
	{
		printf("free value[%s]\n", root->value);
		free(root->value);
	}
	free(root);	
}
static void internal_tree_free(void *ifr)
{
		INIFILE *pfile = (INIFILE *)ifr;
		
		if (pfile == NULL || pfile->root == NULL) return ;
			
		internal_free_node(pfile->root);
		free(pfile);
		
		return ;
}
struct node* internal_create_tree(void)
{
	struct node *root;
	
	root = (struct node *)malloc(sizeof(struct node));
	if (root == NULL)
	{
		printf("Malloc root failed.\n");
		return NULL;
	}
	root->key = NULL;
	root->value = NULL;
	root->brother = NULL;	/*default, all key-value pair is root's brother*/
	root->children = NULL;
	
	return root;
}
static struct node* internal_add_child(struct node *parent, char *key, char *value)
{		
		struct node *child;
		if (parent == NULL)
		{
			printf("parent is null\n");
			return NULL;
		}
		if (parent->children == NULL)
		{
			parent->children = (struct node *)malloc(sizeof(struct node));
			if (parent->children == NULL)
			{
				printf("Malloc parent->children failed.\n");
				return NULL;
			}
			parent->children->key = key;
			parent->children->value = value;
			parent->children->children = NULL;
			parent->children->brother = NULL;
			return parent->children;
		}
		else
		{
			return internal_add_brother(parent->children, key, value);
		}
		
}
static struct node* internal_add_brother(struct node *elder_brother, char *key, char *value)
{
		if (elder_brother == NULL)
		{
			printf("parent is null\n");
			return NULL;
		}
		while(elder_brother->brother)
		{
			elder_brother = elder_brother->brother;
		}
		elder_brother->brother = (struct node *)malloc(sizeof(struct node));
		if (elder_brother->brother == NULL)
		{
			printf("Malloc parent->children failed.\n");
			return NULL;
		}
		elder_brother->brother->key = key;
		elder_brother->brother->value = value;
		elder_brother->brother->children = NULL;
		elder_brother->brother->brother = NULL;
		
		return elder_brother->brother;
}

static int get_key_value(char *buf, char **ppkey, char **ppval)
{
	char *saveptr, *key, *value, *line;
	int keylen, vallen;
	
	line = buf;
	*ppkey = *ppval = NULL;
	if (line == NULL) 
	{
		printf("buf is null\n");
		return -1;
	}
	for (;*line && isspace(*line); line++);
	/*blank line or comment line*/
	if (*line == 0 || *line == '#')
	{
		return 0;
	}	
	/*section*/
	if (*line == '[')
	{
		key = ++line;
		while (*line && *line != ']') line++;
		if (*line == ']') *line = 0;
		else
		{
			printf("invalid line, line[%s]\n", buf);
			return -1;
		}
		keylen = strlen(key);
		*ppkey = (char *)malloc(sizeof(keylen) + 1);
		if (*ppkey == NULL)
		{
			printf("malloc key failed.\n");
			return -1;
		}
		strcpy(*ppkey, key);
		return 0;
	}
	/*key-value pair*/
	/*parse key*/
	key = line;
	while (*line && isalnum(*line)) line++;
	if (*line == 0)
	{
		printf("invalid line[%s]\n", buf);
		return -1;
	}
	keylen = line - key;
	/*parse equal mark*/
	while (*line && isspace(*line)) line++;
	if (*line++ != '=')
	{
		printf("invalid line[%s]\n", buf);
		return -1;
	}
	/*parse value*/
	while (*line && isspace(*line)) line++;
	if (!isalnum(*line) || *line == '#')
	{
		printf("invalid line[%s]\n", buf);
		return -1;
	}
	value = line;
	while(*line && isalnum(*line) && *line != '#') line++;
	vallen = line - value;
	while(*line && isspace(*line)) line++;
	if (*line != 0 && *line != '#')
	{
		printf("invalid line[%s]\n", buf);
		return -1;
	}
	*ppkey = (char *)malloc(keylen + 1);
	if (*ppkey == NULL)
	{
		printf("malloc ppkey failed.\n");
		return -1;
	}
	*ppval = (char *)malloc(vallen + 1);
	if (*ppval == NULL)
	{
		free(*ppkey);
		*ppkey = NULL;
		printf("malloc ppval failed.\n");
		return -1;
	}
	memcpy(*ppkey, key, keylen);
	memcpy(*ppval, value, vallen);
	*(*ppkey + keylen) = 0;
	*(*ppval + vallen) = 0;
	
	return 0;
}
static int internal_ifr_open(struct INIFILE *ifr)
{
	char buf[1024] = {0};
	char *key, *value;
	struct node *pcur;
	int ret, default_section = 1;
	int buf_size = sizeof(buf);
	FILE *fp = ifr->fp;
	
	ifr->root = internal_create_tree();
	if (ifr->root == NULL)
	{
		printf("create tree failed.\n");
		return -1;
	}
	while (fgets(buf, buf_size, fp) != NULL)
	{
			ret = get_key_value(buf, &key, &value);
			if (ret != 0)
			{
				printf("get_key_value failed[%d].\n", ret);
				return -1;
			}
			
			if (key == NULL)
			{
				printf("Comment\n");
				continue;
			}
			if (value == NULL || default_section)
			{
				printf("Section\n");
				default_section = 0;
				pcur = internal_add_child(ifr->root, key, value);
				if (pcur == NULL)
				{
					printf("internal_add_child failed,Section[%s]\n", key, value);
					internal_free_node(ifr->root);
					return -1;
				}
				printf("inernal_add_children success,Section[%s]\n", key);
				continue;
			}
			if (NULL == internal_add_child(pcur, key, value))
			{
				printf("internal_add_brother failed.key[%s], value[%s]\n", key, value);
				internal_free_node(ifr->root);
				return -1;
			}
			printf("internal_add_brother success.key[%s], value[%s]\n", key, value);
			key = value = NULL;
	}
	if (ferror(fp))
	{
		printf("read file occured a error.\n");
		internal_free_node(ifr->root);
		return -1;
	}
	ifr->open_status = Opened;
	return 0;
}

void IfrDump(struct INIFILE *ifr)
{
	if ((long)ifr == ifr->cookie)
		print_node(ifr->root);
	else
		printf("ifr != ifr->cookie\n");
}

static void print_node(struct node *root)
{
	if (!root) return;
	if (root->key && root->value)
		printf("%s=%s\n", root->key, root->value);
	else if (root->key)
		printf("[%s]\n", root->key);
	if (root->children != NULL)
		print_node(root->children);
	if (root->brother != NULL)
		print_node(root->brother);
	
		return;
}
