#ifndef INI_FILE_READER_H
#define INI_FILE_READER_H

#define MAX_ERR_MSG_LEN		1024

#ifdef __cplusplus
extern "C"{
#endif

typedef struct INIFILE INIFILE;
/*
*	Function: open a ini file
*	Parameter: pfile_name	- Initialization File Name.
*	Return:
*		if success return NOT NULL INITFILE pointer.
*		if failed return NULL and you can get return code by ifr_get_errno & ifr_get_errmsg.
*/
INIFILE* IfrOpen(const char *pfile_name);
/*
*	Function: close a opened file. if pfile do not assocate a valid INIFILE, the operate will failed.
* Parameter: pfile - a valid INITFILE pointer.
* Return:
*		if success, return 0. others, return -1.
*/
int IfrClose(INIFILE *pfile);
/*
*	Function: get last error code
* Parameter: pfile - pointer to a open ini file.
* Return:
*		return error code, if previous operator didn't occur a error, the error is 0.
*/
int IfrGetLastError(INIFILE *pfile);

/*
*	Function: get last error message
* Parameter: pfile - pointer to a open ini file.
* Return:
*		return error message. if previous operator didn't occur a error, the message is "No Error." with a linefeed.
*/
char *IfrGetLastErrMsg(INIFILE *pfile);

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
char *IfrGetValueBySection(INIFILE *pfile, const char *section, const char *key);
/*
*	Function: get a value by section and key.
* Parameter: 
*			path - the path of section, for example /section1/key1
* Return:
*			if success ,return the value; if failed, return NULL.				
*/
char *IfrGetValueByPath(INIFILE *pf, const char *path);

void IfrDump(INIFILE *pf);
#ifdef __cplusplus
}
#endif

#endif /* INI_FILE_READER_H */
