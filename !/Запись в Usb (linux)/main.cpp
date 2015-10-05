#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <sys/types.h>
#include <sys/stat.h>


int fd; /* Файловый дескриптор для порта */
char buf[512];/*размер зависит от размера строки принимаемых данных*/
int outa=0;
int iIn;

int open_port(void);
int main(void)
 {
  //port_set:
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY); /*'open_port()' - Открывает последовательный порт */
      if (fd == -1)
        {
          /*
           * Возвращает файловый дескриптор при успехе или -1 при ошибке.
           */
           printf("error port\n");
           perror("open_port: Unable to open /dev/ttyUSBn - ");
        }
     else
        {
         struct termios options; /*структура для установки порта*/
         tcgetattr(fd, &options); /*читает пораметры порта*/

         cfsetispeed(&options, B115200); /*установка скорости порта*/
         cfsetospeed(&options, B115200); /*установка скорости порта*/

         options.c_cflag &= ~PARENB; /*выкл проверка четности*/
         options.c_cflag &= ~CSTOPB; /*выкл 2-х стобит, вкл 1 стопбит*/
         options.c_cflag &= ~CSIZE; /*выкл битовой маски*/
         options.c_cflag |= CS8; /*вкл 8бит*/
         tcsetattr(fd, TCSANOW, &options); /*сохронения параметров порта*/

        }
 read_port:
  iIn=read(fd,buf,8); /*чтения приходящих данных из порта*/



  printf("%s",buf);
goto read_port;


     printf("stop1\n");
}
