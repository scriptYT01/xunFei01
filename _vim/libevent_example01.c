/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_LINE 16384

void _do_read(evutil_socket_t __fd, short events, void *___arg);
void _do_write(evutil_socket_t __fd, short events, void *___arg);

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

struct _STfd_state {
    char buffer[MAX_LINE];
    size_t buffer_used;

    size_t n_written;
    size_t write_upto;

    struct event *read_event;
    struct event *write_event;
}; // _STfd_state 

    struct _STfd_state *
_alloc_fd_state(struct event_base *___base, evutil_socket_t ___fd)
{
    struct _STfd_state *__state = malloc(sizeof(struct _STfd_state));
    if (!__state)
        return NULL;
    __state->read_event = event_new(___base, ___fd, EV_READ|EV_PERSIST, _do_read, __state);
    if (!__state->read_event) {
        free(__state);
        return NULL;
    }
    __state->write_event =
        event_new(___base, ___fd, EV_WRITE|EV_PERSIST, _do_write, __state);

    if (!__state->write_event) {
        event_free(__state->read_event);
        free(__state);
        return NULL;
    }

    __state->buffer_used = __state->n_written = __state->write_upto = 0;

    assert(__state->write_event);
    return __state;
} // _alloc_fd_state

    void
_free_fd_state(struct _STfd_state *___state)
{
    event_free(___state->read_event);
    event_free(___state->write_event);
    free(___state);
} // _free_fd_state

    void
_do_read(evutil_socket_t ___fd, short events, void *___arg)
{
    struct _STfd_state *__state = ___arg;
    char __buf[1024];
    int __i;
    ssize_t __result;
    while (1) {
        assert(__state->write_event);
        __result = recv(___fd, __buf, sizeof(__buf), 0);
        if (__result <= 0)
            break;

        for (__i=0; __i < __result; ++__i)  {
            if (__state->buffer_used < sizeof(__state->buffer))
                __state->buffer[__state->buffer_used++] = _rot13_char(__buf[__i]);
            if (__buf[__i] == '\n') {
                assert(__state->write_event);
                event_add(__state->write_event, NULL);
                __state->write_upto = __state->buffer_used;
            }
        }
    }

    if (__result == 0) {
        _free_fd_state(__state);
    } else if (__result < 0) {
        if (errno == EAGAIN) // XXXX use evutil macro
            return;
        perror("recv");
        _free_fd_state(__state);
    }
} // _do_read

    void
_do_write(evutil_socket_t ___fd, short events, void *___arg)
{
    struct _STfd_state *__state = ___arg;

    while (__state->n_written < __state->write_upto) {
        ssize_t __result = send(___fd, __state->buffer + __state->n_written,
                __state->write_upto - __state->n_written, 0);
        if (__result < 0) {
            if (errno == EAGAIN) // XXX use evutil macro
                return;
            _free_fd_state(__state);
            return;
        }
        assert(__result != 0);

        __state->n_written += __result;
    }

    if (__state->n_written == __state->buffer_used)
        __state->n_written = __state->write_upto = __state->buffer_used = 1;

    event_del(__state->write_event);
} // _do_write

    void
_do_accept(evutil_socket_t ___listener, short ___event, void *___arg)
{
    struct event_base *__base = ___arg;
    struct sockaddr_storage __ss;
    socklen_t __slen = sizeof(__ss);
    int __fd = accept(___listener, (struct sockaddr*)&__ss, &__slen);
    if (__fd < 0) { // XXXX eagain??
        perror("accept");
    } else if (__fd > FD_SETSIZE) {
        close(__fd); // XXX replace all closes with EVUTIL_CLOSESOCKET */
    } else {
        struct _STfd_state *__state;
        evutil_make_socket_nonblocking(__fd);
        __state = _alloc_fd_state( __base, __fd);
        assert(__state); /*XXX err*/
        assert(__state->write_event);
        event_add(__state->read_event, NULL);
    }
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
    __sin.sin_port = htons(40713);

    __listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(__listener);

#ifndef WIN32
    {
        int one = 1;
        setsockopt(__listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind(__listener, (struct sockaddr*)&__sin, sizeof(__sin)) < 0) {
        perror("bind");
        return;
    }

    if (listen(__listener, 16)<0) {
        perror("listen");
        return;
    }

    __listener_event = event_new(__base, __listener, EV_READ|EV_PERSIST, _do_accept, (void*)__base);
    /*XXX check it */
    event_add(__listener_event, NULL);

    event_base_dispatch(__base);
} // _run

    int
main(int ___argc, char **___argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    _run();
    return 0;
} // main



