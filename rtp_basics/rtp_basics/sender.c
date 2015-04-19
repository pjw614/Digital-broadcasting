#include "rtp.h"
#define DATA_SIZE 100000000
#define PAYLOAD_SIZE 1000

int main(int argc, const char * argv[]){
    int                 sock, ch;
    struct sockaddr_in  local;
    struct sockaddr_in  to;
    u_int32             rtp_stream_address;
    unsigned long       data_length;
    char                *data = malloc(DATA_SIZE);
    
    if (argc != 3) {
        printf("\nUsage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    data_length = 0;
    while ((ch = getchar()) != EOF){ // reading data from standard input, could be redirected from a file
        data[data_length] = (unsigned char) ch;
        data_length++;
    }
    // initialize RTP stream address
    rtp_stream_address = inet_addr(argv[1]);
    
    // if we got a valid RTP stream address
    if (rtp_stream_address != 0) {
        sock = socket(AF_INET, SOCK_DGRAM, 0); // create new socket for sending datagrams
        if (sock >= 0) {
            // prepare local address
            memset(&local, 0, sizeof(local));
            local.sin_family      = AF_INET;
            local.sin_port        = htons(INADDR_ANY);
            local.sin_addr.s_addr = htonl(INADDR_ANY);
            // bind to local address
            if (bind(sock, (struct sockaddr *)&local, sizeof(local)) == 0) {
                // prepare RTP stream address
                memset(&to, 0, sizeof(to));
                to.sin_family      = AF_INET;
                to.sin_port        = htons(atoi(argv[2]));
                to.sin_addr.s_addr = rtp_stream_address;
                // send RTP packets
                rtp_send_packets( sock, &to, data, data_length, PAYLOAD_SIZE);
            }
            close(sock);
        }
    }
    free(data);
    return 0;
}
