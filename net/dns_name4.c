#include <string.h>

#include "uint16.h"
#include "dns.h"

static char *q = 0;

int dns_name4_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  char header[12];
  uint16 numanswers;
  uint16 datalen;

  if (!str_copys(out,"")) return -1;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb((unsigned char*)header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  pos += 4;

  while (numanswers--) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb((unsigned char*)header + 8);
    if (memcmp(header,DNS_T_PTR,2) == 0)
      if (memcmp(header + 2,DNS_C_IN,2) == 0) {
	if (!dns_packet_getname(buf,len,pos,&q)) return -1;
	if (!dns_domain_todot_cat(out,q)) return -1;
	return 0;
      }
    pos += datalen;
  }

  return 0;
}

int dns_name4(str *out,const ipv4addr *ip)
{
  char name[DNS_NAME4_DOMAIN];

  dns_name4_domain(name,ip);
  if (dns_resolve(name,DNS_T_PTR) == -1) return -1;
  if (dns_name4_packet(out,dns_resolve_tx.packet,dns_resolve_tx.packetlen) == -1) return -1;
  dns_transmit_free(&dns_resolve_tx);
  dns_domain_free(&q);
  return 0;
}
