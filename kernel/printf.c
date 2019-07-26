#include <stdarg.h>
#include "string.c"

#include "memcpy.c"

/* Simple printf implementation for internal kernel use */

/* Internal buffer for now */
static char str_buf[1024];

static inline void buf_put(char *str)
{
	memcpy(str_buf + strlen(str_buf), str, strlen(str));
}

static inline char* _fmt(int num, int base)
{
	static char str_vals[] = "0123456789ABCDEF";
	static char buf[50];
	char *ptr;

	ptr = buf + 49;
	*ptr = '\0';

	do {
		*--ptr = str_vals[num % base];
		num /= base;
	} while (num != 0);

	return(ptr);
}

static inline int _fmt_single(const char c, va_list arg)
{
	switch (c) {
	case 'd':
	{
		char *str;
		str = _fmt(va_arg(arg, int), 10);
		buf_put(str);
		break;
	}
	case 's':
	{
		char *str = va_arg(arg, char *);
		buf_put(str);
		break;
	}

	default:
		break;
	}
}

static int _printf(const char *fmt, va_list arg)
{
	const char *ptr = fmt;

	do {
		if (*(ptr) == '%')
			_fmt_single(*(++ptr), arg);
		else
			*(str_buf + strlen(str_buf)) = *ptr;
	} while (*++ptr != 0);

	return 0;
}

void printf(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	_printf(fmt, arg);
	va_end(arg);
}

int _start(int argc, char **argv)
{
	printf("This function is tested %d-%d-%d", 27, 07, 2019);

	return 0;
}
