#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define SHOW_PRINTS 0
#define NUM_THREADS 8

char *texto;
char *chute;

unsigned long tam;

int cria_palavra_secreta(char *palavra, int tam)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < tam - 1; i++)
        // sorteia algum caracter visivel, valores ASCII entre 32 e 126
        palavra[i] = 32 + rand() % 94;
    palavra[tam - 1] = '\0';
}

void *descobrir_texto(void *arg)
{
    char *textoParaDescobrir;
    textoParaDescobrir = (char *)arg;

    if (SHOW_PRINTS)
        printf("Thread: %ld - Texto: %s\n", pthread_self(), textoParaDescobrir);

    chute[tam - 1] = '\0';
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < 255; j++)
        {
            chute[i] = j;
            if (chute[i] == textoParaDescobrir[i])
            {
                j = 0;
                break;
            }
        }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    // Variáveis auxiliares
    unsigned long i;
    struct timeval t1, t2;

    // Threads
    pthread_t threads[NUM_THREADS];
    char *textoThread;

    if (argc != 2)
    {
        printf("Favor informar o tamanho da palavra. Por exemplo:\n");
        printf("./reproduz texto 100\n");
        return 0;
    }

    sscanf(argv[1], "%lu", &tam);
    texto = malloc(sizeof(char) * tam);
    chute = malloc(sizeof(char) * tam);
    cria_palavra_secreta(texto, tam);

    if (SHOW_PRINTS)
        printf("Texto inicial: %s\n", texto);

    gettimeofday(&t1, NULL);

    // Cria as threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        textoThread = malloc(sizeof(char) * tam);

        long tamMin = (tam / NUM_THREADS) * i;
        long tamMax = (tam / NUM_THREADS) * (i + 1);

        memset(textoThread, '\0', sizeof(textoThread));
        strncpy(textoThread, &texto[tamMin], tamMax);

        pthread_create(&threads[i], NULL, descobrir_texto, (void *)textoThread);
    }

    // Aguarda a finalização das threads
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&t2, NULL);

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

    printf("%lf+", t_total);

    pthread_exit(NULL);

    return 0;
}
