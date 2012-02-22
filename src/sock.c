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

#include <arpa/inet.h>

int sock_connect(char *addr)
{
	return sock_connect_unix(addr);
}

int sock_listen(char *addr)
{
	return sock_listen_unix(addr);
}

int sock_connect_ipv6(char *addr, int port)
{
	int sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == -1)
		return -1;

	struct sockaddr_in6 saddr = {
		.sin6_family = AF_INET6
	};
	saddr.sin6_port = htons(port);

	if (inet_pton(AF_INET6, addr, &saddr.sin6_addr) != 1)
		return -1;

	if (connect(sock, (const struct socaddr *)&saddr, (socklen_t)sizeof(struct sockaddr_in6)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_connect_unix(char *addr)
{
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1)
		return -1;

	struct sockaddr_un saddr = {
		.sun_family = AF_UNIX
	};

	strncpy(saddr.sun_path, addr, 108);

	if (connect(sock, (const struct sockaddr *)&saddr, (socklen_t)sizeof(struct sockaddr_un)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_bind_unix(char *addr)
{
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1)
		return -1;

	struct sockaddr_un saddr = {
		.sun_family = AF_UNIX
	};

	strncpy(saddr.sun_path, addr, 108);

	if (bind(sock, (const struct sockaddr *)&saddr, (socklen_t)sizeof(struct sockaddr_un)) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int sock_listen_unix(char *addr)
{
	int sock = sock_bind_unix(addr);
	if (sock == -1)
		return -1;

	if (listen(sock, 128) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

