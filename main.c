#include <stdio.h>
#include <unistd.h> // Para usleep
#include <string.h>
#include <ctype.h>

/* * Declaração da função externa em Assembly.
 * O Linker buscará o rótulo 'asm_io_signal' no arquivo led.o
 */
extern void asm_io_signal(int state);

// Definição dos tempos em microssegundos (1s = 1.000.000us)
#define UNIT 100000          // Unidade base: 100ms
#define DOT_TIME UNIT        // Ponto = 1 unidade
#define DASH_TIME (UNIT * 3) // Traço = 3 unidades
#define SYMBOL_SPACE UNIT    // Espaço entre símbolos (. ou -) = 1 unidade
#define LETTER_SPACE (UNIT * 3) // Espaço entre letras = 3 unidades
#define WORD_SPACE (UNIT * 7)   // Espaço entre palavras = 7 unidades

void disparar_sinal(int duracao) {
    asm_io_signal(1);        // Liga o sinal (Inverte a tela via ASM)
    fflush(stdout);          // Força a atualização imediata no terminal
    usleep(duracao);         // Mantém ligado pelo tempo necessário
    
    asm_io_signal(0);        // Desliga o sinal (Volta ao normal via ASM)
    fflush(stdout);          // Força a atualização
    usleep(SYMBOL_SPACE);    // Silêncio obrigatório após cada símbolo
}

void transmitir_morse(const char* msg) {
    // Tabela Morse: A-Z e 0-9
    const char *tabela[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // A-J
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",    // K-T
        "..-", "...-", ".--", "-..-", "-.--", "--..",                          // U-Z
        "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
    };

    for (int i = 0; msg[i] != '\0'; i++) {
        char c = toupper(msg[i]);

        if (c == ' ') {
            usleep(WORD_SPACE);
            printf(" / ");
            fflush(stdout);
            continue;
        }

        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            const char *codigo;
            if (c >= 'A' && c <= 'Z') codigo = tabela[c - 'A'];
            else codigo = tabela[c - '0' + 26];

            printf("%c", c);
            fflush(stdout);

            for (int j = 0; codigo[j] != '\0'; j++) {
                if (codigo[j] == '.') {
                    disparar_sinal(DOT_TIME);
                } else if (codigo[j] == '-') {
                    disparar_sinal(DASH_TIME);
                }
            }
            usleep(LETTER_SPACE); // Espaço após terminar uma letra
            printf(" ");
            fflush(stdout);
        }
    }
    printf("\n[Transmissão concluída]\n");
}

int main() {
    char mensagem[128];

    // Limpa a tela antes de começar (estilo DOS)
    printf("\033[H\033[J"); 
    printf("==========================================\n");
    printf("   TRANSMISSOR MORSE HÍBRIDO (C & ASM)    \n");
    printf("==========================================\n\n");

    printf("Digite a mensagem para modular: ");
    if (fgets(mensagem, sizeof(mensagem), stdin)) {
        // Remove o \n do final da string
        mensagem[strcspn(mensagem, "\n")] = 0;
        transmitir_morse(mensagem);
    }

    return 0;
}