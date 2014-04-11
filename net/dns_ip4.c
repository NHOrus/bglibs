#include <string.h>

#include "dns.h"

static int getit(str* out, const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char header[4];
  if (datalen == 4) {
    if (!dns_packet_copy(buf,len,pos,header,4)) return -1;
    if (!str_catb(out,(char*)header,4)) return -1;
  }
  return 1;
  (void)datalen;
}

/** Extract IPv4 address (A) records from a DNS response packet. */
int dns_ip4_packet(str *out,const char *buf,unsigned int len)
{
  if (dns_packet_extract(out, buf, len, DNS_T_A, DNS_C_IN, getit) < 0)
    return -1;
  dns_rotateipv4((ipv4addr*)out->s,out->len/4);
  return 0;
}

/** Request the IPv4 address (A) records for a domain name. */
int dns_ip4_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  char *q = 0;
  ipv4addr ip;
  const char* x;

  if ((x = ipv4_scan(fqdn, &ip)) != 0 && *x == 0) {
    if (!str_copyb(out, (char*)ip.addr, 4)) return -1;
    return 0;
  }

  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_A) == -1) { free(q); return -1; }
  free(q);
  if (dns_ip4_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_ip4(str*, const char*)
    Request the IPv4 address (A) records for a domain name.
*/
DNS_R_FN_WRAP2(dns_ip4, str*, const char*)

#ifdef SELFTEST_MAIN
str out = {0};
void doit(const char* fqdn)
{
  unsigned int i;

  dns_ip4(&out, fqdn);
  obuf_putf(&outbuf, "s{ }d{:}", fqdn, out.len);
  for (i = 0; i+4 <= out.len; i += 4) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, ipv4_format((ipv4addr*)out.s + i));
  }
  NL();
}
MAIN
{
  doit("1.2.3.4");
  doit("untroubled.org");
}
#endif
#ifdef SELFTEST_EXP
1.2.3.4 4: 1.2.3.4
untroubled.org 4: 69.5.1.51
#endif
