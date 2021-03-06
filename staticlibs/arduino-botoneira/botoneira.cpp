#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>

#include "arduino-serial.h"
#include "botoneira.h"

#include <stdexcept>

int init_arduino(const char *port){
    int fd;
    fd = serialport_init(port,SERIALRATE);
    return fd;
}

int set_relay(int fd, char relay , int command){
    char buffer[9],buffer_read[256]="";
    bool sucess=false;
    int tries = 0;

    buffer[0]='$';
    buffer[1]='R';
    buffer[2]=relay;
    buffer[7]='\n';
    buffer[8]='\0';
    if(command == COMMAND_ON){
        buffer[3]='o';
        buffer[4]='n';
        buffer[5]='n';
        buffer[6]='n';
    } else {
        buffer[3]='o';
        buffer[4]='f';
        buffer[5]='f';
        buffer[6]='f';
    }

    // response form: $CIXXXX\n
    // $ is a start byte
    // \n is a stop byte

    while (sucess == false){
        serialport_flush(fd);
        if( serialport_write(fd, buffer) == -1){
            throw std::runtime_error("Problem in Arduino");
            return -1;
        }

        if( serialport_read_until(fd, buffer_read, '\n') == -1){
            throw std::runtime_error("Problem in Arduino");
        }
        if (strncmp(buffer,buffer_read,8)==0){
            sucess=true;
        } else {
            // give up after 5 tries;
            tries++;
            if(tries > 5){
                throw std::runtime_error("Problem in Arduino");
            }
        }
    }
    return 0;
}
