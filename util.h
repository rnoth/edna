#ifndef _util_
#define _util_
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/* unsafe macros */
#define MAX(X,Y)	((X) > (Y) ? (X) : (Y))
#define MIN(X,Y)	((X) < (Y) ? (X) : (Y))
#define callq(func, ...) ((func) ? func(__VA_ARGS__) : 0)
#define callr(func, ...) ((func) ? func(__VA_ARGS__) : -1)


/* safe macros */
#define bit(off)	(1UL << (off))
#define die(blame) do { perror(blame); abort(); } while (0);

#define eat(ret, buf, expr)			\
do {						\
        char *_buf = buf;			\
        int _ext;				\
        size_t _len = strlen(buf);		\
        size_t *_ret = &(ret);			\
        wchar_t wc;				\
						\
	*_ret = 0;				\
	while ((_ext = mbtowc(&wc, _buf, _len))	\
		&& (expr)) {			\
                if (_ext == -1) {		\
			*_ret = -1;		\
			break;			\
		}				\
		*_ret += (size_t)_ext;		\
		 _buf += (size_t)_ext;		\
		 _len -= (size_t)_ext;		\
	}					\
} while (0)

#endif /* _util_ */
