# Projeto 1 Computação Visual - Equalização de Histograma

**Professor:** André Kishimoto  
**Instituição:** Universidade Presbiteriana Mackenzie

## Grupo
- **Matheus de Andrade Lourenço** (RA 10419691)
- **Murillo Cardoso Ferreira** (RA 10418082)
- **Pietro Zanaga Neto** (RA 10418574)
- **Lucas Tuon de Matos** (RA 10417987)

## Contexto do Projeto
Este projeto consiste na implementação de um software em linguagem **C** para carregamento, conversão e análise estatística de imagens utilizando a biblioteca **SDL3**. O programa processa arquivos nos formatos PNG, JPG e BMP, realizando a conversão para escala de cinza e permitindo a manipulação de contraste através do algoritmo de equalização de histograma.

## Como o Projeto Funciona

- **Entrada:**
O programa recebe como entrada o caminho de uma imagem (formatos suportados pela SDL3_image como PNG, JPG, BMP) via argumento de linha de comando.

- **Processamento Inicial:**
O software verifica se a imagem possui canais de cor (RGB/RGBA). Caso positivo, aplica a conversão para escala de cinza pixel a pixel utilizando a fórmula de luminância: 
Y = 0.2125 * R + 0.7154 * G + 0.0721 * B.

- **Interface e Visualização:**
A interface é construída sobre a biblioteca SDL3, operando com duas janelas distintas:
1. **Janela Principal:** Renderiza a imagem processada, redimensionando-se automaticamente para as proporções do arquivo original.
2. **Janela de Informações:** Uma área dedicada à plotagem do histograma (distribuição de frequência dos tons de cinza) e à exibição dinâmica de texto via SDL3_ttf. 
Os dados de brilho e contraste são atualizados em tempo real conforme a imagem é manipulada (original vs. equalizada).

- **Interface e Visualização:**
A interface é construída sobre a biblioteca SDL3, operando com duas janelas distintas:
1. **Janela Principal:** Renderiza a imagem processada, redimensionando-se automaticamente para as proporções do arquivo original.
2. **Janela de Informações:** Uma área dedicada à plotagem do histograma (distribuição de frequência dos tons de cinza) e à exibição dinâmica de texto via SDL3_ttf. 
Os dados de brilho e contraste são atualizados em tempo real conforme a imagem é manipulada (original vs. equalizada).

- **Análise Estatística:**
O programa calcula as métricas baseando-se em todos os pixels da imagem:
1. **Brilho (Intensidade Média):** Classificado como Escuro (brilho < 85), Médio (85 a 170) ou Claro (> 170).
2. **Contraste (Desvio Padrão):** Classificado como Baixo (contraste < 40), Médio (40 a 80) ou Alto (> 80).

- **Interatividade e Botão:**
O sistema utiliza o gerenciamento de eventos da SDL3 para detectar a entrada do usuário. Ao pressionar a tecla **'E'** (ou o botão implementado na interface), o programa alterna entre a imagem em tons de cinza padrão e a imagem com o filtro de equalização aplicado.

- **Equalização do Histograma:**
A técnica implementada utiliza a **Função de Distribuição Acumulada (CDF)**. O processo consiste em:
1. Calcular o histograma da imagem original.
2. Gerar a CDF para mapear a probabilidade acumulada de cada tom.
3. Normalizar os valores para a escala [0, 255].
4. Criar uma Look-Up Table (LUT) para remapear os pixels, resultando em uma distribuição tonal mais uniforme e ganho significativo de contraste.

- **Salvando a Imagem:**
Ao pressionar a tecla **'S'**, o programa utiliza a função `SDL_SavePNG` para exportar o estado atual da `SDL_Surface` exibida na janela principal. O arquivo é gerado no diretório raiz com o nome `output_image.png`.

### Compilação
Abra o terminal na pasta raiz e execute:
$ make

### Execução
Execute o binário gerado passando o caminho da imagem como argumento:
$ ./proj8.exe "imagem.jpg"

**Requisitos Importantes:**
1. O arquivo de imagem deve estar no mesmo diretório ou caminho acessível.
2. As DLLs da SDL3 e o arquivo `arial.ttf` devem estar na pasta do executável para que a interface e os textos funcionem.

## Atribuições e Contribuintes

1. **Carregamento de Imagem:** Matheus de Andrade Lourenço
2. **Análise e Conversão de Grayscale:** Matheus de Andrade Lourenço
3. **Interface Gráfica:** Pietro Zanaga Neto
4. **Análise e Exibição do Histograma:** Lucas Tuon de Matos
5. **Equalização do Histograma:**: Pietro Zanaga Neto
6. **Salvar Imagem**: Pietro Zanaga Neto
7. **Gerenciamento do Repositório:** Matheus de Andrade Lourenço
8. **Relatório e Documentação Técnica:** Lucas Tuon de Matos, Matheus de Andrade Lourenço e Murillo Cardoso Ferreira
