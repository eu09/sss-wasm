#include "sss/randombytes.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#if defined(__EMSCRIPTEN__)
static int randombytes_emscripten_randombytes(void *buf, size_t n)
{
	unsigned char *p = (unsigned char *)buf;
	size_t i;

	for (i = (size_t)0U; i < n; i++)
	{
		p[i] = (unsigned char)EM_ASM_INT_V({
			if (typeof window!=="undefined") {
				var crypto = window.crypto || window.msCrypto;
				if (crypto) {
					var buf = new window.Uint8Array(4);
					crypto.getRandomValues(buf);
					return (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]) >>> 0;
				}
			}
			var crypto = require('crypto');
			if (crypto) {
				var buf = crypto.randomBytes(4);
				return (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]) >>> 0;
			}
		});
	}
	return 0;
}
#endif /* defined(__EMSCRIPTEN__) */

int randombytes(void *buf, size_t n)
{
#if defined(__EMSCRIPTEN__)
#pragma message("Using browser window.crypto.getRandomValues")
	/* Use window.crypto.getRandomValues */
	return randombytes_emscripten_randombytes(buf, n);
#else
#error "randombytes(...) is not supported on this platform"
#endif
}
