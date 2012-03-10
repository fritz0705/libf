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

#include <f/_.h>
#include <f/alloc.h>
#include <f/sock.h>
#include <f/str.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <fcntl.h>

int sock_ipv4()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

int sock_ipv6()
{
	return socket(AF_INET6, SOCK_STREAM, 0);
}

int sock_unix()
{
	return socket(AF_UNIX, SOCK_STREAM, 0);
}

int sock_connect(struct sockaddr *a)
{
	switch (a->sa_family)
	{
		case AF_INET:
			return sock_connect_ipv4(a);
		case AF_INET6:
			return sock_connect_ipv6(a);
		case AF_UNIX:
			return sock_connect_unix(a);
	}

	return -1;
}

int sock_listen(struct sockaddr *a, int backlog)
{
	switch (a->sa_family)
	{
		case AF_INET:
			return sock_listen_ipv4(a, backlog);
		case AF_INET6:
			return sock_listen_ipv6(a, backlog);
		case AF_UNIX:
			return sock_listen_unix(a, backlog);
	}

	return -1;
}

int sock_bind(struct sockaddr *a)
{
	switch(a->sa_family)
	{
		case AF_INET:
			return sock_bind_ipv4(a);
		case AF_INET6:
			return sock_bind_ipv6(a);
		case AF_UNIX:
			return sock_bind_unix(a);
	}

	return -1;
}

int sock_connect_ipv4(struct sockaddr *a)
{
	if (a->sa_family != AF_INET)
		return -1;

	int sock = sock_ipv4();
	if (sock == -1)
		return -1;

	if (connect(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_connect_ipv6(struct sockaddr *a)
{
	if (a->sa_family != AF_INET6)
		return -1;

	int sock = sock_ipv6();
	if (sock == -1)
		return -1;

	if (connect(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_in6)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_connect_unix(struct sockaddr *a)
{
	if (a->sa_family != AF_UNIX)
		return -1;

	int sock = sock_unix();
	if (sock == -1)
		return -1;

	if (connect(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_un)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_listen_ipv4(struct sockaddr *a, int backlog)
{
	int sock = sock_bind_ipv4(a);
	if (sock == -1)
		return -1;

	if (listen(sock, backlog) == -1)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_listen_ipv6(struct sockaddr *a, int backlog)
{
	int sock = sock_bind_ipv6(a);
	if (sock == -1)
		return -1;

	if (listen(sock, backlog) == -1)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_listen_unix(struct sockaddr *a, int backlog)
{
	int sock = sock_bind_unix(a);
	if (sock == -1)
		return -1;

	if (listen(sock, backlog) == -1)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_bind_ipv4(struct sockaddr *a)
{
	if (a->sa_family != AF_INET)
		return -1;

	int sock = sock_ipv4();
	if (sock == -1)
		return -1;

	if (bind(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_bind_ipv6(struct sockaddr *a)
{
	if (a->sa_family != AF_INET6)
		return -1;

	int sock = sock_ipv6();
	if (sock == -1)
		return -1;

	if (bind(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_in6)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_bind_unix(struct sockaddr *a)
{
	if (a->sa_family != AF_UNIX)
		return -1;

	int sock = sock_unix();
	if (sock == -1)
		return -1;

	unlink(((struct sockaddr_un *)a)->sun_path);
	if (bind(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_un)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_accept(int fd, struct sockaddr **a)
{
	struct sockaddr_storage *addr = alloc(sizeof(struct sockaddr_storage));
	*a = (struct sockaddr *)addr;

	if (addr == NULL)
		return -1;

	socklen_t socklen = (socklen_t)sizeof(struct sockaddr_storage);
	int retval = accept(fd, (struct sockaddr * restrict)addr, &socklen);
	if (retval < 0)
	{
		unalloc(addr);
		*a = NULL;
	}

	return retval;
}

void sock_blocking(int sock)
{
	int flags = fcntl(sock, F_GETFL);
	if (flags < 0)
		return;
	if (flags & O_NONBLOCK)
	{
		flags &= ~(O_NONBLOCK);
		fcntl(sock, F_SETFL, flags);
	}
}

void sock_nonblocking(int sock)
{
	int flags = fcntl(sock, F_GETFL);
	if (flags < 0)
		return;
	if (!(flags & O_NONBLOCK))
	{
		flags |= O_NONBLOCK;
		fcntl(sock, F_SETFL, flags);
	}
}

struct sockaddr *sock_addr(int family)
{
	switch (family)
	{
		case AF_INET:
			return alloc(sizeof(struct sockaddr_in));
		case AF_INET6:
			return alloc(sizeof(struct sockaddr_in6));
		case AF_UNIX:
			return alloc(sizeof(struct sockaddr_un));
		case AF_UNSPEC:
			return alloc(sizeof(struct sockaddr_storage));
	}

	return NULL;
}

void sock_addr_port(struct sockaddr *a, int port)
{
	int net_port = htons(port);

	if (a->sa_family == AF_INET)
	{
		struct sockaddr_in *in_a = (struct sockaddr_in *)a;
		in_a->sin_port = net_port;
	}
	else if (a->sa_family == AF_INET6)
	{
		struct sockaddr_in6 *in6_a = (struct sockaddr_in6 *)a;
		in6_a->sin6_port = net_port;
	}
}

uint32_t sock_if(char *name)
{
	int sock = sock_ipv4();
	if (sock == -1)
		return 0;

	struct ifreq req;
	strncpy(req.ifr_name, name, IFNAMSIZ);

	if (ioctl(sock, SIOCGIFINDEX, &req) < 0)
	{
		close(sock);
		return 0;
	}

	close(sock);
	return req.ifr_ifindex;
}

char *sock_if_name(uint32_t index)
{
	int sock = sock_ipv4();
	if (sock == -1)
		return NULL;

	struct ifreq req;
	req.ifr_ifindex = index;

	if (ioctl(sock, SIOCGIFNAME, &req) < 0)
	{
		close(sock);
		return NULL;
	}

	char *result = alloc(IFNAMSIZ);
	if (result == NULL)
	{
		close(sock);
		return NULL;
	}
	memcpy(result, req.ifr_name, IFNAMSIZ);

	close(sock);
	return result;
}

char *sock_addr_dump(struct sockaddr *a)
{
	str_t result = str_new();

	if (result == NULL)
		return NULL;

	switch (a->sa_family)
	{
		case AF_UNIX:
			;
			struct sockaddr_un *un_a = (struct sockaddr_un *)a;
			str_append_cs(result, un_a->sun_path);
			break;
		case AF_INET6:
			;
			struct sockaddr_in6 *in6_a = (struct sockaddr_in6 *)a;

			str_append_c(result, '[');
			{
				char buffer[INET6_ADDRSTRLEN];
				if (inet_ntop(AF_INET6, &in6_a->sin6_addr, buffer, (socklen_t)INET6_ADDRSTRLEN) == NULL)
				{
					str_destroy(result);
					return NULL;
				}
				str_append_cs(result, buffer);
			}

			if (in6_a->sin6_scope_id > 0)
			{
				str_append_c(result, '%');
				str_append_cs(result, sock_if_name(in6_a->sin6_scope_id));
			}
			str_append_c(result, ']');
			str_append_c(result, ':');
			str_append_ui(result, ntohs(in6_a->sin6_port), 10);
			break;
		case AF_INET:
			;
			struct sockaddr_in *in_a = (struct sockaddr_in *)a;

			{
				char buffer[INET_ADDRSTRLEN];
				if (inet_ntop(AF_INET, &in_a->sin_addr, buffer, (socklen_t)INET_ADDRSTRLEN) == NULL)
				{
					str_destroy(result);
					return NULL;
				}
				str_append_cs(result, buffer);
			}

			str_append_c(result, ':');
			str_append_ui(result, ntohs(in_a->sin_port), 10);
			break;
	}

	char *res_str = str_dump(result);
	str_destroy(result);

	return res_str;
}

int sock_family(int fd)
{
#ifdef SO_DOMAIN
	int family;
	socklen_t len = sizeof(int);

	if (getsockopt(fd, SOL_SOCKET, SO_DOMAIN, &family, &len) < 0)
		return -1;

	return family;
#else
	return AF_UNSPEC;
#endif
}

void sock_mcast_join(int fd, struct sockaddr *a)
{
	if (a->sa_family != AF_INET6)
		return;

	struct sockaddr_in6 *ipv6_a = (struct sockaddr_in6 *)a;

	struct ipv6_mreq req;
	memcpy(&req.ipv6mr_multiaddr, &ipv6_a->sin6_addr, sizeof(struct in6_addr));
	req.ipv6mr_interface = ipv6_a->sin6_scope_id;

	setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &req, sizeof(struct ipv6_mreq));
}

void sock_mcast_leave(int fd, struct sockaddr *a)
{
	if (a->sa_family != AF_INET6)
		return;

	struct sockaddr_in6 *ipv6_a = (struct sockaddr_in6 *)a;

	struct ipv6_mreq req;
	memcpy(&req.ipv6mr_multiaddr, &ipv6_a->sin6_addr, sizeof(struct in6_addr));
	req.ipv6mr_interface = ipv6_a->sin6_scope_id;

	setsockopt(fd, IPPROTO_IPV6, IPV6_LEAVE_GROUP, &req, sizeof(struct ipv6_mreq));
}

