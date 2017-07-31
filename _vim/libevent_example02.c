/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_LINE 16384

void do_read(evutil_socket_t __fd, short events, void *___arg);
void do_write(evutil_socket_t __fd, short events, void *___arg);

    char
_rot13_char(char ___c)
{
    /* We don't want to use isalpha here; setting the locale would change
     * * which characters are considered alphabetical. */
    if ((___c >= 'a' && ___c <= 'm') || (___c >= 'A' && ___c <= 'M'))
        return ___c + 13;
    else if ((___c >= 'n' && ___c <= 'z') || (___c >= 'N' && ___c <= 'Z'))
        return ___c - 13;
    else
        return ___c;
} // _rot13_char

    void
_readcb(struct bufferevent *___bev, void *___ctx)
{
    struct evbuffer *__input, *__output;
    char *__line;
    size_t __n;
    int i;
    __input = bufferevent_get_input(___bev);
    __output = bufferevent_get_output(___bev); // _readcb

    while ((__line = evbuffer_readln(__input, &__n, EVBUFFER_EOL_LF))) {
        for (i = 0; i < __n; ++i)
            __line[i] = _rot13_char(__line[i]);
        evbuffer_add(__output, __line, __n); // _readcb
        evbuffer_add(__output, "\n", 1);
        free(__line);
    }

    if (evbuffer_get_length(__input) >= MAX_LINE) { // _readcb
        /* Too long; just process what there is and go on so that the buffer
         * * doesn't grow infinitely long. */
        char __buf[1024];
        while (evbuffer_get_length(__input)) {
            int __n = evbuffer_remove(__input, __buf, sizeof(__buf)); // _readcb
            for (i = 0; i < __n; ++i)
                __buf[i] = _rot13_char(__buf[i]);
            evbuffer_add(__output, __buf, __n); // _readcb
        }
        evbuffer_add(__output, "\n", 1);
    }
} // _readcb

    void
_errorcb(struct bufferevent *___bev, short ___error, void *___ctx)
{
    if (___error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (___error & BEV_EVENT_ERROR) { // _errorcb
        /* check errno to see what ___error occurred */
        /* ... */
    } else if (___error & BEV_EVENT_TIMEOUT) { // _errorcb
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(___bev); // _errorcb
} // _errorcb

    void
_do_accept(evutil_socket_t ___listener, short event, void *___arg)
{
    struct event_base *__base = ___arg;
    struct sockaddr_storage __ss;
    socklen_t __slen = sizeof(__ss);
    int __fd = accept(___listener, (struct sockaddr*)&__ss, &__slen);
    if (__fd < 0) {
        perror("accept");
    } else if (__fd > FD_SETSIZE) { // _do_accept
        close(__fd);
    } else {
        struct bufferevent *___bev;
        evutil_make_socket_nonblocking(__fd);
        ___bev = bufferevent_socket_new(__base, __fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(___bev, _readcb, NULL, _errorcb, NULL); // _do_accept
        bufferevent_setwatermark(___bev, EV_READ, 0, MAX_LINE);
        bufferevent_enable(___bev, EV_READ|EV_WRITE);
    } // _do_accept
} // _do_accept

    void
_run(void)
{
    evutil_socket_t __listener;
    struct sockaddr_in __sin;
    struct event_base *__base;
    struct event *__listener_event;

    __base = event_base_new();
    if (!__base)
        return; /*XXXerr*/

    __sin.sin_family = AF_INET;
    __sin.sin_addr.s_addr = 0;
    __sin.sin_port = htons(40713); // _run()

    __listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(__listener);

#ifndef WIN32
    {
        int one = 1;
        setsockopt(__listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)); // _run()
    }
#endif

    if (bind(__listener, (struct sockaddr*)&__sin, sizeof(__sin)) < 0) {
        perror("bind");
        return;
    }

    if (listen(__listener, 16)<0) { // _run()
        perror("listen");
        return;
    }

    __listener_event = event_new(__base, __listener, EV_READ|EV_PERSIST, _do_accept, (void*)__base);
    /*XXX check it */
    event_add(__listener_event, NULL);

    event_base_dispatch(__base);
} // _run()

    int
main(int ___argc, char **___argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    _run();
    return 0;
} // main








