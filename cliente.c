#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define QTD_MAX_BYTES 1000

int cria_palavra_secreta(char *palavra, int tam)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < tam - 1; i++)
        // sorteia algum caracter visivel, valores ASCII entre 32 e 126
        palavra[i] = 32 + rand() % 40;
    palavra[tam - 1] = '\0';
}

int main()
{
    int sockfd;
    struct sockaddr_in address;
    int result;
    int tamanho_palavra;

    // Lendo dados para enviar
    printf("Digite o tamanho da palavra a ser descoberta: ");
    scanf("%d", &tamanho_palavra);

    // Configurando socket
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // address.sin_family = AF_INET;
    // address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // address.sin_port = htons(9734);

    // result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));

    // if (result == -1)
    // {
    //     perror("oops: não foi possível se conectar via socket");
    //     exit(1);
    // }

    char *palavra_para_descobrir = malloc(sizeof(char) * tamanho_palavra);
    cria_palavra_secreta(palavra_para_descobrir, tamanho_palavra);

    printf("Palavra secreta: %s\n", palavra_para_descobrir);

    // Divide a palavra em vários segmentos de até 1000 bytes.
    int qtd_segmentos = tamanho_palavra / QTD_MAX_BYTES;

    printf("Quantidade de segmentos: %d\n", qtd_segmentos);

    if (qtd_segmentos < 1)
        qtd_segmentos = 1;

    char segmentos[qtd_segmentos][QTD_MAX_BYTES];

    for (int i = 0; i < qtd_segmentos; i++)
    {
        char substring[QTD_MAX_BYTES];
        strncpy(substring, &palavra_para_descobrir[i * QTD_MAX_BYTES], QTD_MAX_BYTES);
        substring[QTD_MAX_BYTES] = '\0';

        printf("Segmento %d: %s\n", i, substring);
        strcpy(segmentos[i], substring);
    }

    // write(sockfd, &palavra_para_descobrir, 1024);

    // read(sockfd, &str, 1024);
    // printf("mensagem do servidor: %s\n", str);
    // close(sockfd);
    // exit(0);
}
