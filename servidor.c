#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *chute;

char descobrir_texto(char textoParaDescobrir[1024])
{
    int texto_size = strlen(textoParaDescobrir);
    chute[texto_size - 1] = '\0';
    for (int i = 0; i < texto_size; i++)
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
    int server_len, client_len;

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
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);

    while (1)
    {
        printf("Server waiting\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        read(client_sockfd, &str_in, 1024);

        printf("Texto recebido: %s\n", str_in);

        chute = malloc(sizeof(char) * sizeof(str_in));
        descobrir_texto(str_in);
        printf("chute: %s\n", chute);

        write(client_sockfd, chute, 1024);

        close(client_sockfd);
    }
}
