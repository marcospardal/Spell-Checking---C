/*
    Código para spell checking
    Implementado utilizando função de hash e dicionario
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 29717 //numero de buckets

struct Node {
    char palavra[50];
    struct Node * prox;
}; 

typedef struct {
    int size;
    struct Node * prim;
}Lista;

Lista array[MAX]; // cria o dicionario que sera um array de listas. Listas foram utilizadas para resolver o problemas de colisão que serão encontrados durante o tempo de execução

unsigned int hash(char * palavra){ //função para calcular o hash de cada palavra
    unsigned int soma = 0;

    for(int i = 0; i < strlen(palavra); i++){
        if(palavra[i] == '-')
            continue;
        else
            soma += (((int)palavra[i] - 65) * pow(i + 1, 2)); //multiplica o valor inteiro do char pelo quadrado da sua posição na palavra 
    }

    return soma % MAX; //retorna o resto da divisao da soma pela quantidade de buckets
}

void add(int key, char * palavra){
    if(array[key].prim == NULL){ //verifica se ja existe palavra nesse bucket, caso não tenha ele aloca a palavra na primeira posição
        struct Node* noAtual = (struct Node*) malloc (sizeof(struct Node)); //cria um novo nó dinamicamente

        for(int i = 0; i < 50; i++){
            noAtual->palavra[i] = palavra[i];
        }
        noAtual->prox = NULL;
        array[key].prim = noAtual; // faz o ponteiro apontar para o novo nó

    }else{ // se o primeiro nó da lista já estiver ocupado verifica os próximos até encontrar o nó que está vazio
        struct Node * ptr; //cria um ponteiro auxiliar
        ptr = array[key].prim; //faz com que o ponteiro auxiliar aponte para o primeiro

        while(ptr->prox != NULL){ // roda um laço de repetição até encontrar um nó vazio
            ptr = ptr->prox; //faz com que o ponteiro auxiliar aponte para o próximo para que o laço de repetição funcione
        }

        struct Node* noAtual = (struct Node*) malloc (sizeof(struct Node)); //cria um novo nó dinamicamente

        for(int i = 0; i < 50; i++){
            noAtual->palavra[i] = palavra[i];
        }

        noAtual->prox = NULL;

        ptr->prox = noAtual; //faz com que o ponteiro do ultimo nó da lista aponte para o nó novo
    }
}

//função para ler um arquivo qualquer e buscar por palavras que estão no dicionário
void leArquivo(){
    struct Node * ptr; //cria um ponteiro pra nó pra que seja possível comparar as palavras
    char palavra[50];
    unsigned int posicao, encontrados = 0;
    FILE * arquivoASerLido;

    arquivoASerLido = fopen("palavras.txt", "r"); //abre o arquivo que foi escolhido

    if(arquivoASerLido == NULL)
        printf("Arquivo nao encontrado\n");
    else{

        while(fgets(palavra, 50, arquivoASerLido)){ //lê as palavras do arquivo

            posicao = hash(palavra); //encontra o hash da palavra que foi lida do arquivo teste

            ptr = array[posicao].prim; //faz o ponteiro apontar pro bucket que a palavra tirada do arquivo supostamente está

            while(ptr != NULL){ //roda até que o ponteiro aponte para nada sinalizando o final da lista
                if(strcmp(palavra, ptr->palavra) == 0){ //compara as palavras
                    encontrados++; //atualiza a contagem de palavras encontradas
                    break;//para o loop pois não precisa avançar mais na lista já que já encontrou a palavra desejada
                }

                ptr = ptr->prox;//atualiza o ponteiro caso a palavra não tenha sido encontrada
            }

        }
    }

    printf("Foram encontradas %d palavras\n", encontrados); //printa quantas palavras foram encontradas

    fclose(arquivoASerLido); //fecha o arquivo
}

int main(void){
    unsigned int buckets, num, numEmCada[MAX] = {0}, quantPalavras = 0;
    char palavra[50];
    FILE * arquivo;

    arquivo = fopen("palavras.txt", "r"); //abre o arquivo com as palavras

    for(int i = 0; i < MAX; i++)
        array[i].prim = NULL; // seta todos os primeiros ponteiros para null para "zerar" a lista e o computador saber q não há palavras nos buckets

    if(arquivo == NULL){    //verifica se o arquivo foi aberto corretamente
        printf("Arquivo não encontrado\n");
    }else{
        printf("Adicionando palavras\n");
        while(fgets(palavra, 50, arquivo) != NULL){ //laço de repetição que lê todas as palavras até que o arquivo seja terminado e o fgets retorne um ponteiro vazio

            buckets = hash(palavra); //calcula o hash de cada palavra e armazena

            add(buckets, palavra);

            numEmCada[buckets]++; //atualiza o numero de palavras em cada bucket

            quantPalavras++; //atualiza quantas palavras foram lidas do arquivo

        }

        fclose(arquivo); //fecha o arquivo
    }

    printf("Abrindo arquivo para leitura e busca...\n");
    leArquivo();


    return 0;
}
