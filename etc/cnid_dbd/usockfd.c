/*
 * Copyright (C) Joerg Lenneis 2003
 * All Rights Reserved.  See COPYING.
 */

#include "config.h"

#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <atalk/logger.h>

#include "usockfd.h"

/* ---------------
 * create a tcp socket
 */
int tsockfd_create(char *host, char *port, int backlog)
{
    int sockfd, flag _U_, ret;
    struct addrinfo hints, *servinfo, *p;

    /* Prepare hint for getaddrinfo */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((ret = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        LOG(log_error, logtype_cnid, "tsockfd_create: getaddrinfo: %s\n", gai_strerror(ret));
        return -1;
    }

    /* create a socket */
    /* loop through all the results and bind to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            LOG(log_info, logtype_cnid, "tsockfd_create: socket: %s", strerror(errno));
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            LOG(log_info, logtype_cnid, "tsockfd_create: bind: %s\n", strerror(errno));
            continue;
        }

        if (listen(sockfd, backlog) < 0) {
            close(sockfd);
            LOG(log_info, logtype_cnid, "tsockfd_create: listen: %s\n", strerror(errno));
            continue;
        }

        /* We got a socket */
        break;
    }

    if (p == NULL)  {
        LOG(log_error, logtype_cnid, "tsockfd_create: no suitable network config %s:%s", host, port);
        freeaddrinfo(servinfo);
        return -1;
    }

    freeaddrinfo(servinfo);
    return sockfd;
}

/* --------------------- */
int usockfd_check(int sockfd, const sigset_t *sigset)
{
    int fd;
    socklen_t size;
    fd_set readfds;
    int ret;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    if ((ret = pselect(sockfd + 1, &readfds, NULL, NULL, NULL, sigset)) < 0) {
        if (errno == EINTR)
            return 0;
        LOG(log_error, logtype_cnid, "error in select: %s",
            strerror(errno));
        return -1;
    }

    if (ret) {
        size = 0;
        if ((fd = accept(sockfd, NULL, &size)) < 0) {
            if (errno == EINTR)
                return 0;
            LOG(log_error, logtype_cnid, "error in accept: %s",
                strerror(errno));
            return -1;
        }
        return fd;
    } else
        return 0;
}
