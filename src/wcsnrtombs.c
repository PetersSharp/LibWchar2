/* 
 * This code was written by Rich Felker in 2010; no copyright is claimed.
 * This code is in the public domain. Attribution is appreciated but
 * unnecessary.
 */

#include "libwchar.h"

size_t _wcsnrtombs(char *restrict dst, const wchar_t **restrict wcs, size_t wn, size_t n, mbstate_t *restrict st)
{
    (void) st;
	size_t l, cnt = 0, n2;
	char *s, buf[256];
	const wchar_t *ws = *wcs;

	if (!dst) { s = buf; n = sizeof buf; }
	else s = dst;

	while ((ws) && (n) && (((n2 = wn) >= n) || (n2 > 32))) {
		if (n2 >= n) n2 = n;
		wn -= n2;
		if (!(l = _wcsrtombs(s, &ws, n2, 0))) {
			cnt = l;
			n = 0;
			break;
		}
		if (s != buf) {
			s += l;
			n -= l;
		}
		cnt += l;
	}
	if (ws) while (n && wn) {
		if (!(l = _wcrtomb(s, *ws, 0))) {
			if (!l) ws = 0;
			else cnt = l;
			break;
		}
		ws++; wn--;
		/* safe - this loop runs fewer than sizeof(buf) times */
		s+=l; n-=l;
		cnt++;
	}
	if (dst) *wcs = ws;
	return cnt;
}
