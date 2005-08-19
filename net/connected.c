/* $Id$ */
/* net/connected.c - Test if a socket is connected
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

/** Test if a socket is connected. */
int socket_connected(int sock)
{
  struct sockaddr sa;
  int dummy;
  char ch;
  
  dummy = sizeof sa;
  if (getpeername(sock, &sa, &dummy) == -1) {
    read(sock,&ch,1);
    return 0;
  }
  return 1;
}
