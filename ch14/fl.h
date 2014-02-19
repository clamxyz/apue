#ifndef FL_H__
#define FL_H__

#include <sys/types.h>

int set_fl(int fd, int flag);
int clr_fl(int fd, int flag);

int lock_reg(int fd, int cmd, int type, off_t start, int whence, off_t len);
int lock_test(int fd, int cmd, int type, off_t start, int whence, off_t len);

#define lock_rdlock(fd, start, whence, len) (lock_reg)((fd), F_SETLK, F_RDLCK, (start), (whence), (len))
#define lock_wrlock(fd, start, whence, len) (lock_reg)((fd), F_SETLK, F_WRLCK, (start), (whence), (len))
#define lockw_rdlock(fd, start, whence, len) (lock_reg)((fd), F_SETLKW, F_RDLCK, (start), (whence), (len))
#define lockw_wrlock(fd, start, whence, len) (lock_reg)((fd), F_SETLKW, F_RDLCK, (start), (whence), (len))
#define lock_unlock(fd, start, whence, len) (lock_reg)((fd), F_SETLK, F_UNLCK, (start), (whence), (len))

int lockfile(int fd);

ssize_t readn(int fd, void *buf, size_t n);
ssize_t writen(int fd, void *buf, size_t n);

#endif /*FL_H__*/

