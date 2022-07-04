#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

char *chute;

char descobrir_texto(char textoParaDescobrir[1024])
{
    chute[sizeof(textoParaDescobrir) - 1] = '\0';
    for (int i = 0; i < sizeof(textoParaDescobrir); i++)
        for (int j = 0; j < 255; j++)
        {
            chute[i] = j;
            if (chute[i] == textoParaDescobrir[i])
            {
                j = 0;
                break;
            }
        }
}

int main()
{
    int server_sockfd, client_sockfd;
    int client_len;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char str_in[1024];
    char str_out[1024];

    // Configurando conexão Socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);

    // Bind na conexão
    bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_sockfd, 5);

    while (1)
    {
        printf("Server waiting\n");

        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, sizeof(client_address));

        read(client_sockfd, &str_in, 1024);

        chute = malloc(sizeof(char) * sizeof(str_in));
        printf(chute);

        sprintf(str_out, "%s cruel\n", str_in);

        write(client_sockfd, &str_out, 1024);

        close(client_sockfd);
    }
}
