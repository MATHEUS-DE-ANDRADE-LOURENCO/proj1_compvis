# Projeto 1 Computação Visual - Equalização de Histograma

**Professor:** André Kishimoto

## Grupo

- Matheus de Andrade Lourenço (RA 10419691)
- Murillo Cardoso Ferreira (RA 10418082)
- Pietro Zanaga Neto (RA 10418574)
- Lucas Tuon de Matos (RA 10417987)

## Contexto do Projeto

Este projeto consiste na implementação de um programa em C para carregar, converter e analisar estatisticamente imagens utilizando a biblioteca SDL3. O programa processa arquivos nos formatos PNG, JPG e BMP e os converte em escala de cinza. Além de realizar operações e análises tendo como base o histograma de pixels da imagem.

## Como o Projeto Funciona

- **Entrada:**
O programa recebe como entrada o caminho de uma imagem via linha de comando.

- **Processamento:**
Ocorre uma verificação para ver se a imagem é colorido e, se for necessário, aplica uma conversão para escala de cinza utilizando a fórmula: Y = 0.2125 x R + 0.7154 x G + 0.0721 x B

- **Interface:**
O programa, após ser executado, abre uma janela principal adaptada ao tamanho da imagem que contêm a imagem processada e uma janela secundária que exibe o histograma, a intensidade media (brilho) e o desvio padrão (contraste). Os textos foram escritos com a biblioteca SDL3_ttf.

- **Análise Estatística:**
O brilho (intensidade média) é classificado como Claro (brilho > 170), Médio (85 < brilho > 170) ou Escuro (brilho < 85).
O contraste (desvio padrão) é classificado como Alto (contraste < 40), Médio (40 < contraste > 80) ou Baixo (contraste < 80)

- **Equalização do Histograma:**
ADICIONAR AQUI A LOGICA DA EQUALIZAÇÃO

- **Salvando a Imagem:**
Ao pressionar a tecla S do teclado, o programa salva a imagem atualmente exibida na janela principal em um arquivo chamado "output_image.png".

## Processo de Compilação

O projeto utiliza um Makefile para gerenciar as dependências e o processo de compilação com o GCC.

**Comando para compilar:**  Na pasta raiz utilize o comando seguinte
> make

**Comando para a execução:** ./proj8.exe + o nome da imagem assim como mostra abaixo
> ./proj8.exe "imagem.jpg"  

**Atenção:** O arquivo de imagem deve estar na mesma pasta do programa.


## Contribuintes

**1. Carregamento de Imagem:**  
- Matheus de Andrade Lourenço

**2. Análise e Conversão para Escala de Cinza:**  
- Matheus de Andrade Lourenço

**3. Interface Gráfica:**  
- Nome 
- Nome 

**4. Análise e Exibição do Histograma:**  
- Lucas Tuon de Matos
- Nome 

**5. Equalização do Histograma:**  
- Nome 
- Nome 

**6. Salvar Imagem:**  
- Nome 
- Nome 

**7. Relatório:**  
- Lucas Tuon de Matos
- Nome 
