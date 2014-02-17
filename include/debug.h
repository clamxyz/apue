#ifndef DEBUG_H__
#define DEBUG_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define dp(s, err) do { printf("%s[%d]\t%s:[%d][%s]\n", __FILE__,__LINE__,s, err, strerror(err));} while(0)
#define err_ret(s, err) do { dp(s, err); return err; } while(0)
#define err_sys(s, err) do { dp(s, err); exit(err) ;} while(0)
#else
#define dp(s, err) 
#define err_ret(s, err)
#define err_sys(s, err)
#endif

#ifdef __cplusplus
}
#endif

#endif /*DEBUG_H__*/
