#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned short int vetor_para_palavra_de_8_bits(unsigned short int vetor[8])
{
    unsigned short int palavra = 0;

    for (int i = 0; i < 8; i++)
    {
        if (vetor[i])
        {
            palavra |= 1 << i;
        }
    }

    return palavra;
}

unsigned short int vetor_para_palavra_de_16_bits(unsigned short int vetor[16])
{
    unsigned short int palavra = 0;

    for (int i = 0; i < 16; i++)
    {
        if (vetor[i])
        {
            palavra |= 1 << i;
        }
    }

    return palavra;
}

unsigned short int escrita_hamming(unsigned char palavra_lida)
{

    unsigned short int palavraHamming;                      // armazena palavra hamming 16 bits
    unsigned short int vetorHamming[16];                    // vetor da tabela hamming 16 bits
    unsigned short int vetorM[] = {0, 0, 0, 0, 0, 0, 0, 0}; // vetor da palavra_lida 8 bits
    unsigned short int valor_do_bit = 1;                    // referencia aos bits do vetor de 8 bits
    unsigned short int palavraM = palavra_lida;             // armazena a palavra de 8 bits em 16 bits
    int i;

    // Converte palavra para vetor
    for (int i = 0; i < 8; i++)
    {
        vetorM[i] = (palavra_lida & (1 << i)) != 0;
    }

    // Atribuindo os M
    vetorHamming[3] = vetorM[0];  // M1
    vetorHamming[5] = vetorM[1];  // M2
    vetorHamming[6] = vetorM[2];  // M3
    vetorHamming[7] = vetorM[3];  // M4
    vetorHamming[9] = vetorM[4];  // M5
    vetorHamming[10] = vetorM[5]; // M6
    vetorHamming[11] = vetorM[6]; // M7
    vetorHamming[12] = vetorM[7]; // M8

    // Calculando C
    vetorHamming[1] = vetorM[0] ^ vetorM[1] ^ vetorM[3] ^ vetorM[4] ^ vetorM[6]; // C1
    vetorHamming[2] = vetorM[0] ^ vetorM[2] ^ vetorM[3] ^ vetorM[5] ^ vetorM[6]; // C2
    vetorHamming[4] = vetorM[1] ^ vetorM[2] ^ vetorM[3] ^ vetorM[7];             // C4
    vetorHamming[8] = vetorM[4] ^ vetorM[5] ^ vetorM[6] ^ vetorM[7];             // C8

    // Calculando G
    vetorHamming[0] = vetorHamming[1];
    for (i = 2; i < 13; i++)
    {
        vetorHamming[0] = vetorHamming[0] ^ vetorHamming[i];
    }

    // Atribuindo 0 nas posições não utilizadas
    vetorHamming[13] = 0;
    vetorHamming[14] = 0;
    vetorHamming[15] = 0;

    printf(".wham escrita\n");
    for (i = 0; i < 16; i++)
    {
        printf("%d ", vetorHamming[i]);
    }
    printf("\n");
    
    // Converter vetor da palavr hamming em palavra 16 bits
    palavraHamming = vetor_para_palavra_de_16_bits(vetorHamming);

    printf("palavra hamming: %x\n", palavraHamming);

    // Retorna o hamming
    return palavraHamming;
}

unsigned char leitura_palavra_hamming(unsigned short int palavraHamming)
{
    int i;

    unsigned short int vetorHamming[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // vetor palavra hamming
    unsigned short int palavraM;                                                          // palavra 8 bits
    unsigned short int novoC[4];                                                          // C linha
    unsigned short int xorC[4];                                                           // armazena xor entre C e C linha
    unsigned short int vetorM[8];                                                         // vetor para a palavra de 8 bits
    unsigned short int bit_errado = 0;                                                    // indice do bit errado
    unsigned short int valor_do_bit = 1;                                                  // referencia a posicao do bit
    int G_antigo;

    // Converte palavraHamming 16 bits para vetorHamming de 16 posicoes
    for (i = 0; i < 16; i++)
    {
        vetorHamming[i] = (palavraHamming & valor_do_bit) != 0 ? 65535 : 0;
        valor_do_bit <<= 1;
    }

    // Atribuindo 0 nas posições não utilizadas
    vetorHamming[13] = 0;
    vetorHamming[14] = 0;
    vetorHamming[15] = 0;

    // Salva G antigo
    G_antigo = vetorHamming[0];

    // Calculando novo C
    novoC[0] = vetorHamming[3] ^ vetorHamming[5] ^ vetorHamming[7] ^ vetorHamming[9] ^ vetorHamming[11];  // C1
    novoC[1] = vetorHamming[3] ^ vetorHamming[6] ^ vetorHamming[7] ^ vetorHamming[10] ^ vetorHamming[11]; // C2
    novoC[2] = vetorHamming[5] ^ vetorHamming[6] ^ vetorHamming[7] ^ vetorHamming[12];                    // C4
    novoC[3] = vetorHamming[9] ^ vetorHamming[10] ^ vetorHamming[11] ^ vetorHamming[12];                  // C8

    // XOR do novo C com o velho C
    xorC[0] = vetorHamming[8] ^ novoC[3];
    xorC[1] = vetorHamming[4] ^ novoC[2];
    xorC[2] = vetorHamming[2] ^ novoC[1];
    xorC[3] = vetorHamming[1] ^ novoC[0];

    // Pega o bit errado de acordo com o XOR do C
    bit_errado = 0;
    for (i = 0; i < 4; i++)
    {
        if (xorC[i] != 0)
        {
            bit_errado += (1 << i);
        }
    }

    printf(".wham leitura\n");
    for (i = 0; i < 16; i++)
    {
        printf("%d ", vetorHamming[i]);
    }
    printf("\n");

    // Verificando se o arquivo está intacto
    if (bit_errado == 0)
    {
        printf("Palavra intacta.\n");

        vetorM[0] = vetorHamming[3];
        vetorM[1] = vetorHamming[5];
        vetorM[2] = vetorHamming[6];
        vetorM[3] = vetorHamming[7];
        vetorM[4] = vetorHamming[9];
        vetorM[5] = vetorHamming[10];
        vetorM[6] = vetorHamming[11];
        vetorM[7] = vetorHamming[12];

        palavraM = vetor_para_palavra_de_8_bits(vetorM);
        printf("palavra: %x\n", palavraM);
        // Retorna o M sem precisar executar os próximos passos
        return palavraM;
    }

    printf("Bit errado na posição: %d\n", bit_errado + 1);
    printf("bit errado: %d\n", vetorHamming[15 - bit_errado]);
    
    // Inverte o bit errado
    vetorHamming[15 - bit_errado] = ~vetorHamming[15 - bit_errado];
  
    printf("bit errado corrigido: %u\n", vetorHamming[15 - bit_errado]);

    // Calculando G novamante
    vetorHamming[0] = vetorHamming[1];
    for (i = 1; i < 13; i++)
        vetorHamming[0] = vetorHamming[0] ^ vetorHamming[i];

    printf("G: %d\n", G_antigo);
    printf("G linha: %d\n", vetorHamming[0]);

    // Verificando G novo com G antigo
    if ((vetorHamming[0] ^ G_antigo) == 0)
    {
        printf("Palavra recuperada.\n");

        vetorM[0] = vetorHamming[3];
        vetorM[1] = vetorHamming[5];
        vetorM[2] = vetorHamming[6];
        vetorM[3] = vetorHamming[7];
        vetorM[4] = vetorHamming[9];
        vetorM[5] = vetorHamming[10];
        vetorM[6] = vetorHamming[11];
        vetorM[7] = vetorHamming[12];

        palavraM = vetor_para_palavra_de_8_bits(vetorM);
    }
    else
    {
        printf("Palavra irrecuperável.\n");
        exit(1);
    }
    
    printf(".wham corrigido\n");
    for (i = 0; i < 16; i++)
    {
        printf("%d ", vetorHamming[i]);
    }
    printf("palavra corrigida: %x\n", palavraM);

    return palavraM;
}

void escrita_arquivo_hamming(char nome[])
{
    char novo_nomeArquivo[200];         // nome do arquivo .wham
    unsigned short int palavra_Hamming; // armazena a palavra hamming
    unsigned char leitura_palavra_M;    // leitura_palavra_M da palavra original
    int tamanho_arquivo = 0;            // tamanho do arquivo original
    int i = 0;

    // copia o nome do arquivo original para o novo nome
    strcpy(novo_nomeArquivo, nome);
    // concatena a extensao .wham
    strcat(novo_nomeArquivo, ".wham");

    // abre arquivo originall para leitura_palavra_M
    FILE *arquivo_normal = fopen(nome, "rb");
    // abre arquivo .wham para escrita
    FILE *arquivo_hamming = fopen(novo_nomeArquivo, "wb");

    if (arquivo_normal == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // movimenta ponteiro para o final do arquivo
    fseek(arquivo_normal, 0, SEEK_END);
    // calculo do tamanho do arquivo
    tamanho_arquivo = ftell(arquivo_normal);
    // movimenta ponteiro para o inicio do arquivo
    fseek(arquivo_normal, 0, SEEK_SET);

    for (i = 0; i < tamanho_arquivo; i++)
    {
        // leitura palavra-palavra 8 bits do arquivo normal
        fread(&leitura_palavra_M, 1, 1, arquivo_normal);
        // calculo da palavra hamming
        palavra_Hamming = escrita_hamming(leitura_palavra_M);
        // armazena palavra hamming 16 bits no arquivo .wham
        fwrite(&palavra_Hamming, 2, 1, arquivo_hamming);
    }

    fclose(arquivo_normal);
    fclose(arquivo_hamming);
}

void leitura_arquivo_hamming(char nome[])
{
    char novo_nome[200];
    unsigned short int leitura_palavra_H; // palavra hamming 16 bits
    unsigned char palavraM;               // palavra 8 bits
    int i;

    // retirna .wham do arquivo e adiciona final de string
    strncpy(novo_nome, nome, strlen(nome) - 5);
    novo_nome[strlen(nome) - 5] = '\0';

    // abre arquivo original para escrita
    FILE *arquivo_normal = fopen(novo_nome, "wb");
    // abre arquivo .wham para leitura
    FILE *arquivo_hamming = fopen(nome, "rb");

    if (arquivo_hamming == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(-1);
    }

    // move ponteiro para o final do arquivo
    fseek(arquivo_hamming, 0, SEEK_END);
    // tamanho do arquivo em bytes
    int tamanho = ftell(arquivo_hamming);
    // move ponteiro para o inicio do arquivo
    fseek(arquivo_hamming, 0, SEEK_SET);

    for (i = 0; i < tamanho; i++)
    {
        leitura_palavra_H = 0;
        // leitura palavra hamming 16 bits
        fread(&leitura_palavra_H, 2, 1, arquivo_hamming);
        // decodificacao palavra hamming 16 bits para original 8 bits
        palavraM = leitura_palavra_hamming(leitura_palavra_H);
        // armazena palavra original 8 bits no arquivo
        fwrite(&palavraM, 1, 1, arquivo_normal);
        i++;
    }

    fclose(arquivo_normal);
    fclose(arquivo_hamming);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("[ERRO] - Argumentos inválidos!\n");
        exit(1);
    }

    if (strcmp(argv[2], "-r") == 0)
    {
        printf("[MODO] - Leitura do arquivo .wham\n");
        leitura_arquivo_hamming(argv[1]);
    }

    else if (strcmp(argv[2], "-w") == 0)
    {
        printf("[MODO] - Escrita do arquivo Hamming.\n");
        escrita_arquivo_hamming(argv[1]);
    }

    else
    {
        printf("[ERRO] - Argumentos inválidos!\n");
        exit(1);
    }

    return 0;
}