/* Copyright (c) 2012 Fritz Grimpen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#if __STDC_HOSTED__ == 0
#error Socket functions are only available in hosted environment
#endif

#include <sys/socket.h>

int sock_ipv4();
int sock_ipv6();
int sock_unix();

int sock_connect(struct sockaddr *a);
int sock_listen(struct sockaddr *a, int backlog);
int sock_bind(struct sockaddr *a);

int sock_connect_ipv4(struct sockaddr *a);
int sock_connect_ipv6(struct sockaddr *a);
int sock_connect_unix(struct sockaddr *a);

int sock_listen_ipv4(struct sockaddr *a, int backlog);
int sock_listen_ipv6(struct sockaddr *a, int backlog);
int sock_listen_unix(struct sockaddr *a, int backlog);

int sock_bind_ipv4(struct sockaddr *a);
int sock_bind_ipv6(struct sockaddr *a);
int sock_bind_unix(struct sockaddr *a);

int sock_accept(int fd, struct sockaddr **a);

void sock_blocking(int sock);
void sock_nonblocking(int sock);

int sock_family(int fd);

struct sockaddr *sock_addr(int family);
void sock_addr_port(struct sockaddr *a, int port);
char *sock_addr_dump(struct sockaddr *a);

uint32_t sock_if(char *name);
char *sock_if_name(uint32_t id);

