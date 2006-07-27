/* $Id$ */
/* Public domain. */

#ifndef CDB_H
#define CDB_H

#include <sysdeps.h>

#define CDB_HASHSTART 5381
extern uint32 cdb_hashadd(uint32,unsigned char);
extern uint32 cdb_hash(const char *,unsigned int);

struct cdb {
  char *map; /* 0 if no map is available */
  int fd;
  uint32 size; /* initialized if map is nonzero */
  uint32 loop; /* number of hash slots searched under this key */
  uint32 khash; /* initialized if loop is nonzero */
  uint32 kpos; /* initialized if loop is nonzero */
  uint32 hpos; /* initialized if loop is nonzero */
  uint32 hslots; /* initialized if loop is nonzero */
  uint32 dpos; /* initialized if cdb_findnext() returns 1 */
  uint32 dlen; /* initialized if cdb_findnext() returns 1 */

  uint32 nextrec; /* initialized if cdb_{first,next}rec() returns 1 */
  /* (uint32)-1 means no next record */
};

extern void cdb_free(struct cdb *);
extern void cdb_init(struct cdb *,int fd);

extern int cdb_read(struct cdb *,unsigned char *,unsigned int,uint32);

extern void cdb_findstart(struct cdb *);
extern int cdb_findnext(struct cdb *,const char *,unsigned int);
extern int cdb_find(struct cdb *,const char *,unsigned int);

#define cdb_datapos(c) ((c)->dpos)
#define cdb_datalen(c) ((c)->dlen)

#endif
