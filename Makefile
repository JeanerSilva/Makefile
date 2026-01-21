# Nome do executável final
TARGET = transmissor_retro

# Compiladores e ferramentas
AS = nasm
CC = gcc
LD = gcc

# Flags (opções) de compilação
# -f elf64: Formato de objeto para Linux 64 bits
ASFLAGS = -f elf64
# -Wall: Mostra todos os avisos do C
# -no-pie: Desativa o executável independente de posição (importante para compatibilidade com ASM)
CFLAGS = -Wall -no-pie

# Arquivos objeto
OBJS = main.o led.o

# Regra padrão: compilar tudo
all: $(TARGET)

# Linkagem final: Une o C e o ASM
$(TARGET): $(OBJS)
	$(LD) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compilação do main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Montagem do led.asm
led.o: led.asm
	$(AS) $(ASFLAGS) led.asm -o led.o

# Limpeza dos arquivos temporários
clean:
	rm -f *.o $(TARGET)

# Executar o programa
run: all
	./$(TARGET)


