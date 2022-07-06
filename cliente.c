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
    int servers[] = {9734, 9734};
    int qtd_servers = sizeof(servers) / sizeof(servers[0]);

    int sockfd[qtd_servers];
    int len[qtd_servers];
    struct sockaddr_in address[qtd_servers];
    int result[qtd_servers];
    int tamanho_palavra;
    char palavra_descoberta[5000];

    // Lendo dados para enviar
    printf("Digite o tamanho da palavra a ser descoberta: ");
    scanf("%d", &tamanho_palavra);

    // palavra_descoberta = malloc(sizeof(char) * tamanho_palavra);

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

    // Configurando socket
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Conectando-se via socket com o servidor %d - %d\n", servers[i], i);
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        address[i].sin_family = AF_INET;
        address[i].sin_addr.s_addr = inet_addr("127.0.0.1");
        address[i].sin_port = htons(servers[i]);

        len[i] = sizeof(address);
        result[i] = connect(sockfd[i], (struct sockaddr *)&address[i], len[i]);

        if (result[i] == -1)
        {
            perror("oops: não foi possível se conectar via socket\n");
            exit(1);
        }
    }

    // Enviando dados
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Enviando dados para o servidor %d - %d\n", servers[i], i);
        write(sockfd[i], segmentos[0], 1024);
    }

    // Recebendo dados
    for (int i = 0; i < qtd_servers; i++)
    {
        char palavrinha[5000];
        printf("Recebendo dados do servidor %d - %d\n", servers[i], i);
        read(sockfd[i], &palavrinha, 1024);

        printf("Dado recebido do servidor: %s\n", palavrinha);
        close(sockfd[i]);
    }

    // printf("mensagem do servidor: %s\n", palavra_descoberta);
    exit(0);
}
