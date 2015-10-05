#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <termios.h> /* Объявления управления POSIX-терминалом */

//#include <errno.h>   /* Объявления кодов ошибок */
//#include <string.h>  /* Объявления строковых функций */
//#include <sys/types.h>
//#include <sys/stat.h>


int openport(void)
{
    int fd; /* Файловый дескриптор для порта */
    int open_port(void);
    //port_set:
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY); /*'open_port()' - Открывает последовательный порт */
    if (fd == -1)
    {  // Возвращает файловый дескриптор при успехе или -1 при ошибке.
        return -1;
    }
    else
    {
        struct termios options; /*структура для установки порта*/
        tcgetattr(fd, &options); /*читает пораметры порта*/
        cfsetispeed(&options, B57600); /*установка скорости порта*/
        cfsetospeed(&options, B57600); /*установка скорости порта*/
        options.c_cflag &= ~PARENB; /*выкл проверка четности*/
        options.c_cflag &= ~CSTOPB; /*выкл 2-х стобит, вкл 1 стопбит*/
        options.c_cflag &= ~CSIZE; /*выкл битовой маски*/
        options.c_cflag |= CS8; /*вкл 8бит*/
        tcsetattr(fd, TCSANOW, &options); /*сохранение параметров порта*/
        return fd;
    }
}

void readport (int fd, char buf[512] )
{
//    int iIn = read(fd,buf,512); /*чтения приходящих данных из порта*/
    read(fd,buf,512); /*чтения приходящих данных из порта*/
}

void writetoport (int fd, const char buf[512])
{
    /*размер зависит от размера строки принимаемых данных*/
//    int iOut = write(fd,buf,8); /*чтения приходящих данных из порта*/
    write(fd,buf,8); /*чтения приходящих данных из порта*/
}
