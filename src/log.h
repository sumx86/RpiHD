#ifndef __RPI_LOG_H
#define __RPI_LOG_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include "conf.h"

// log format prefixes
char *log_inf_prefix = "[RPI-INFO]  - ";
char *log_err_prefix = "[RPI-ERROR] - ";

void init_log( void );
void _rlog( rpi_log_t level, char *err );

#endif