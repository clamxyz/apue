#ifndef MKT_H__
#define MKT_H__
#ifdef __cplusplus
extern "C"
{
#endif

int makethread(void *(*thr_fn)(void *), void *arg);

#ifdef __cplusplus
}
#endif

#endif

