#include <reent.h>

int _close_r(struct _reent *r,int a) { return 0; }
int _execve_r(struct _reent *r,const char *a,char *const *b,char *const *c) { return 0; }
int _fcntl_r(struct _reent *r,int a,int b,int c) { return 0; }
int _fork_r(struct _reent *r) { return 0; }
int _fstat_r(struct _reent *r,int a,struct stat *b) { return 0; }
int _getpid_r(struct _reent *r) { return 0; }
int _isatty_r(struct _reent *r,int a) { return 0; }
int _kill_r(struct _reent *r,int a,int b) { return 0; }
int _link_r(struct _reent *r,const char *a, const char *b) { return 0; }
_off_t _lseek_r(struct _reent *r,int a,_off_t b,int c) { return 0; }
int _mkdir_r(struct _reent *r,const char *a,int b) { return 0; }
int _open_r(struct _reent *r,const char *a,int b,int c) { return 0; }
_ssize_t _read_r(struct _reent *r,int a,void *b,size_t c) { return 0; }
int _rename_r(struct _reent *r,const char *a, const char *b) { return 0; }
void *_sbrk_r(struct _reent *r,ptrdiff_t a) { return 0; }
int _stat_r(struct _reent *r,const char *a, struct stat *b) { return 0; }
_CLOCK_T_ _times_r(struct _reent *r,struct tms *a) { return 0; }
int _unlink_r(struct _reent *r,const char *a) { return 0; }
int _wait_r(struct _reent *r,int *a) { return 0; }
_ssize_t _write_r(struct _reent *r,int a,const void *b,size_t c) { return 0; }

void _exit()
{
	for(;;);
}
