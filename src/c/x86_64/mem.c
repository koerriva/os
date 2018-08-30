#include "include/type.h"

void *mem_copy(void *d, const void *s, size_t n) {
	size_t i;
	uint8_t *dst = (uint8_t*) d;
	uint8_t *src = (uint8_t*) s;

	for (i = 0; i < n; i++) {
		dst[i] = src[i];
	}

	return dst;
}