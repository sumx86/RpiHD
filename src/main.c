#include "rpi.h"

void rpi_usage( char *prog )
{
    fprintf( stderr, "\nUsage: %s [OPTION] [ARGUMENT]\n", prog );
    fprintf( stderr, "-h Display help\n" );
    fprintf( stderr, "-d [DEVICE]\n" );
    fprintf( stderr, "-s [SERVER]\n" );
    fprintf( stderr, "-p [PORT]\n" );
    fprintf( stderr, "-t [ARP REQUEST TIME DELAY]\n" );
    fprintf( stderr, "-l [ABSOLUTE PATH][LIST OF TRUSTED HOSTS] each line must be the entity's mac address\n" );
    exit( RPI_BAD );
}

int main( int argc, char **argv )
{
    libnet_t *ltag;
    int opt;
    char *delay;
    double rff;
    struct libnet_ether_addr *hw;
    struct rpi_conf c, *conf = &c;

    conf->device = NULL;
    conf->server = NULL;
    conf->port   = NULL;
    conf->tlist  = NULL;
    delay        = NULL;

    while ( (opt = getopt( argc, argv, "d:s:p:t:l:h" )) != -1 )
    {
        switch ( opt ) {
            case 'h':
                rpi_usage( argv[0] );
            case 'd':
                conf->device = optarg; break;
            case 's':
                conf->server = optarg; break;
            case 'p':
                conf->port   = optarg; break;
            case 't':
                delay        = optarg; break;
            case 'l':
                conf->tlist  = optarg; break;
            default:
                rpi_usage( argv[0] );
        }
    }

    if ( !conf->device || !conf->server || !conf->port || !delay ){
        rpi_usage( argv[0] );
    }

    rff = atof( delay );
    if ( (int) rff != 0 )
    {
        fprintf( stderr, "Delay must be in the range [0.0 - 0.9]\n" );
        exit( RPI_BAD );
    }
    conf->delay = (float) rff;

    if ( (ltag = rpi_initialize( conf, err_buff )) == NULL ){
        // this is the only place where _rlog() is not used since the initialization includes
        // creation of log file ( init_log() ), so in case where init_log() fails, error buffer
        // should be piped to `stderr` instead
        fprintf( stderr, "%s\n", err_buff );
        exit( RPI_BAD );
    }

    // device hardware address
    if ((hw = libnet_get_hwaddr( ltag )) == NULL)
    {
        strcpy( err_buff, libnet_geterror(ltag) );
        _rlog( RPI_LOG_ERR, err_buff );
    }

    memcpy( conf->hw, hw, 6 );

    _rlog( RPI_LOG_INFO, "RPI initialized successfully!\n" );
    rpi_start_receiver( conf );
    rpi_arp_initiate( ltag, conf );
    // wait for the arp receiver (thread) to terminate
    pthread_exit( NULL );
}