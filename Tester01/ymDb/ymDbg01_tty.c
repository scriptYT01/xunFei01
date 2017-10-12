#include "ymList01.h"

int _setTTY_ymDB01(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes */
#if 0
    // blocking read
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;
#else
    // non-blocking read
    tty.c_cc[VMIN] = 0;         // read doesn't block
    tty.c_cc[VTIME] = 1;        // 0.1 seconds read timeout
#endif

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }

    return 0;
} /* _setTTY_ymDB01 */

void _printTTYinfo_ymDB01(char *portname, int speed)
{
    switch (speed) {
    case B9600:
        printf("port: %s, speed: B9600\n", portname);
        break;
    case B19200:
        printf("port: %s, speed: B19200\n", portname);
        break;
    case B38400:
        printf("port: %s, speed: B38400\n", portname);
        break;
    case B115200:
        printf("port: %s, speed: B115200\n", portname);
        break;
    default:
        printf("port: %s, speed_t: %d(0x%x)\n", portname, speed, speed);
        break;
    }
} /* _printTTYinfo_ymDB01 */
