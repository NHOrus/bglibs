/* $Id$ */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "iobuf/iobuf.h"
#include "err.h"

static pid_t pid = 0;

void err_message(const char* type,
		 const char* a, const char* b, const char* c,
		 const char* d, const char* e, const char* f,
		 int showsys)
{
  if (!pid) pid = getpid();
  obuf_puts(&errbuf, program);
  obuf_putc(&errbuf, '[');
  obuf_putu(&errbuf, pid);
  obuf_puts(&errbuf, "]: ");
  obuf_puts(&errbuf, type);
  obuf_puts(&errbuf, ": ");
  if (a) obuf_puts(&errbuf, a);
  if (b) obuf_puts(&errbuf, b);
  if (c) obuf_puts(&errbuf, c);
  if (d) obuf_puts(&errbuf, d);
  if (e) obuf_puts(&errbuf, e);
  if (f) obuf_puts(&errbuf, f);
  if (showsys) {
    obuf_puts(&errbuf, ": ");
    obuf_puts(&errbuf, strerror(errno));
  }
  obuf_putsflush(&errbuf, "\n");
}
