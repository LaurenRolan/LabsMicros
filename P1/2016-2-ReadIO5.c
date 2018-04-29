#Assumindo que este arquivo seja executado com permissões de super-usuário...
#include<galileo2io.h>
#include<stdio.h>


//Acho que é isso.
int main(int argc,char * argv[])
{
        int fd;
        char data_str[80];
        double scale;
        int raw;

        if((fd=open("/sys/class/gpio/gpio0/value",O_RDONLY)) < 0)
        {
                perror("Opening gpio0:");
                return -1;
        }
        
        pgets(data_str,sizeof data_str,"/sys/class/gpio/gpio0/value");
        
        for(;;)
        {
                lseek(fd,0,SEEK_SET);
                read(fd,data_str,sizeof data_str);
                raw=atoi(data_str);
                
                printf("Value at gpio0 = %d\n",raw);
        }
        
        close(fd);

        return 0;
}
