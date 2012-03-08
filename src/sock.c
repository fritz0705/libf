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

#include <f/sock.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
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

	if (bind(sock, (const struct sockaddr *)a, (socklen_t)sizeof(struct sockaddr_un)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
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
