#include "ibuf.h"

/** Retrieve a single character from the \c ibuf. */
int ibuf_getc(ibuf* in, char* ch)
{
  iobuf* io;
  io = &(in->io);
  in->count = 0;
  if (ibuf_eof(in) || ibuf_error(in)) return 0;
  if (io->bufstart >= io->buflen && !ibuf_refill(in)) return 0;
  *ch = io->buffer[io->bufstart++];
  in->count = 1;
  return 1;
}
