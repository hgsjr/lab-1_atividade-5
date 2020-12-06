#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//ponteiro para inteiro, global, que irá apontar para o início do vetor com intuito de ser enxergado por ambas as threads
int * vetor;

typedef struct {
    int inicio, fim; //demarca intervalo no qual a thread vai trabalhar
    int ident; //identificação da thread que será usada para observar melhor o que está ocorrendo
} intervalo;

void * incrementa (void * arg) {
    intervalo *args = (intervalo *) arg;
    for (int i = args->inicio; i <= args->fim; i++) {
        *(vetor + i) += 1;
        printf("A thread %d incrementou a posição %d do vetor em uma unidade.\n", args->ident, i);
    }
    pthread_exit(NULL);
}

void printa_vetor_int(int * vet, int n) {
    printf("[");
    for(int i = 0; i < n; i++) {
        if(i != n - 1)
            printf("%d ", *(vet + i));
        else
            printf("%d]\n\n", *(vet + i));
    }
}

int main() {

    //bloco que cria um vetor com n elementos
    int n;
    while(1) {
        printf("Digite o número (10 < n < 100) de elementos do vetor: ");
        scanf("%d", &n);
        if(10 < n && n < 100)
            break;
        else
            printf("\nVocê digitou um número fora do intervalo, por favor tente novamente.\n\n");
    }
    int v[n];
    vetor = v;
    for(int i = 0; i < n; i++) {
        v[i] = rand() % 100;
    }

    //bloco que exibe o vetor criado
    printf("\nVetor de %d elementos gerado: ", n);
    printa_vetor_int(vetor, n);

    //bloco que cria as threads
    pthread_t tid[2]; //vetor de identificadores das threads
    int meio = n / 2;
    intervalo * args[2];
    args[0] = malloc(sizeof(intervalo *)); //aloca memória para o argumento (struct) da thread 1
    if(args[0] == NULL) {
        printf("ERRO ao executar: malloc();"); exit(-1);
    }
    args[0] -> inicio = 0;
    args[0] -> fim = meio;
    args[0] -> ident = 1;
    args[1] = malloc(sizeof(intervalo)); //aloca memória para o argumento (struct) da thread 2
    if(args[1] == NULL) {
        printf("ERRO ao executar: malloc();"); exit(-1);
    }
    args[1] -> inicio = meio + 1;
    args[1] -> fim = n - 1;
    args[1] -> ident = 2;
    for(int thread = 0; thread < 2; thread++) {
        printf("Cria a thread %d.\n", thread + 1);
        //é passado como argumento da função das threads uma struct que delimita o intervalo de ação de cada thread
        //é usada uma struct, pois, assim é possível passar mais de uma "informação" como argumento
        if(pthread_create(&tid[thread], NULL, incrementa, (void *) args[thread])) {
            printf("ERRO ao executar: pthread_create();\n");
            exit(-1);
        }
    }

    //espera todas as threads terminarem a execução
    for(int thread = 0; thread < 2; thread++) {
        if (pthread_join(tid[thread], NULL)) {
            printf("ERRO ao executar: pthread_join();\n");
            exit(-1);
        }
    }
    printf("\n");

    //bloco que exibe o novo vetor com suas posições incrementadas
    printf("Vetor incrementado: ", n);
    printa_vetor_int(vetor, n);

    return 0;
}
