/**
 * @file serial.cpp
 * @author alex
 * @brief 串口通信
 * @version 0.1
 * @date 2021-10-13
 * 
 * alex all rights reserved
 * 
 */

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main()
{
    int fd;
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        std::cout << "open ttyUSB0 fail" << std::endl;
    }
    else
    {
        std::cout << "open ttyUSB0 success" << std::endl;
    }
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= CLOCAL | CREAD;
    //设置数据位为8
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    //设置停止位为１
    options.c_cflag &= ~CSTOPB;
    //关闭流动控制
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    //奇校验
    options.c_cflag |= PARENB;
    options.c_cflag |= PARODD;
    options.c_iflag |= (INPCK | ISTRIP);
    tcflush(fd, TCOFLUSH);

    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        std::cout << "setting ttyUSB0 fail" << std::endl;
    }
    else
    {
        std::cout << "setting ttyUSB0 success" << std::endl;
    }
    char buffer[] = "serial test success";
    int data = -1;

    data = write(fd, buffer, strlen(buffer));
    if (data < 0)
    {
        std::cout << "writting error" << std::endl;
    }
    else
    {
        std::cout << "writting success" << std::endl;
    }

    close(fd);
    return 0;
}