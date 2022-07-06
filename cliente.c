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
    int servers[] = {9734, 9735};
    int qtd_servers = sizeof(servers) / sizeof(servers[0]);

    struct timeval t1, t2;

    int sockfd[qtd_servers];
    int len[qtd_servers];
    struct sockaddr_in address[qtd_servers];
    int result[qtd_servers];
    int tamanho_palavra;
    char palavra_descoberta[5000];

    // Lendo dados para enviar
    printf("Digite o tamanho da palavra a ser descoberta: ");
    scanf("%d", &tamanho_palavra);

    if (tamanho_palavra > 5000)
    {
        printf("Tamanho de palavra muito grande, o máximo é 5000. Enviando 5000 como parâmetro.\n");
        tamanho_palavra = 5000;
    }

    // palavra_descoberta = malloc(sizeof(char) * tamanho_palavra);

    char *palavra_para_descobrir = malloc(sizeof(char) * tamanho_palavra);
    cria_palavra_secreta(palavra_para_descobrir, tamanho_palavra);

    printf("Palavra secreta: %s\n", palavra_para_descobrir);

    // Divide a palavra em segmentos de acordo com a quantidade de servidores disponíveis
    printf("Quantidade de segmentos: %d\n", qtd_servers);

    int max_bytes = tamanho_palavra / qtd_servers;
    printf("Quantidade máxima de bytes por segmento %d\n", max_bytes);

    char segmentos[qtd_servers][max_bytes];

    for (int i = 0; i < qtd_servers; i++)
    {
        char substring[max_bytes];
        strncpy(substring, &palavra_para_descobrir[i * max_bytes], max_bytes);
        substring[max_bytes] = '\0';

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

    gettimeofday(&t1, NULL);

    // Enviando dados
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Enviando dados para o servidor %d - %d\n", servers[i], i);
        write(sockfd[i], segmentos[i], max_bytes);
    }

    // Recebendo dados
    char str_out[5000];
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Recebendo dados do servidor %d - %d\n", servers[i], i);
        read(sockfd[i], &str_out, 5000);

        printf("Dado recebido do servidor: %s\n", str_out);
        strncat(palavra_descoberta, str_out, strlen(str_out));
        close(sockfd[i]);
    }

    gettimeofday(&t2, NULL);

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

    printf("Mensagem descoberta: %s\n", palavra_descoberta);
    printf("Tempo para execução: %lf+", t_total);
    exit(0);
}
