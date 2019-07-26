#include <stddef.h>

size_t strlen(const char *str)
{
	const char *ptr = str;
	for (ptr; *ptr; ptr++);

	return  ptr - str;
}
