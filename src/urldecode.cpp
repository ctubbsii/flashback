/* from 
http://www.oreillynet.com/pub/a/network/excerpt/spcookbook_chap03/index2.html
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "urldecode.h"

#define SPC_BASE16_TO_10(x) (((x) >= '0' && (x) <= '9') ? ((x) - '0') : \
                             (toupper((x)) - 'A' + 10))

char *spc_decode_url(const char *url, size_t *nbytes)  {
  char       *out, *ptr;
  const char *c;
   
  if (!(out = ptr = strdup(url))) return 0;
  for (c = url;  *c;  c++) {
    if (*c != '%' || !isxdigit(c[1]) || !isxdigit(c[2])) *ptr++ = *c;
    else {
      *ptr++ = (SPC_BASE16_TO_10(c[1]) * 16) + (SPC_BASE16_TO_10(c[2]));
      c += 2;
    }
  }
  *ptr = 0;
  if (nbytes) *nbytes = (ptr - out); /* does not include null byte */
  return out;
}

