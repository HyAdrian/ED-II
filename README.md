# Indexer README

## Autores
- Hygor Adriano Tristão
- Fernando Guilherme Gorski
- Lucas Massayuki Gohara

## Descrição
Este é um programa em C que realiza operações de indexação e busca em documentos de texto. Ele inclui funcionalidades como contagem de frequência de palavras, busca por palavras específicas e geração de índices TF-IDF.

## Como usar

1. **Clone o repositório na sua máquina:**
2. **Compile o arquivo:**
gcc indexer.c -o indexer.exe

3. **Execute o programa:**
./indexer.exe [opções] [arquivo1.txt arquivo2.txt ...]

## Opções disponíveis

- **--freq num_palavras:**
Conta a frequência das `num_palavras` palavras mais frequentes nos documentos fornecidos.

Exemplo:
./indexer.exe --freq 10 documento1.txt documento2.txt

- **--freq-word palavra:**
Conta a frequência da palavra específica nos documentos fornecidos.

Exemplo:
./indexer.exe --freq-word "exemplo" documento1.txt documento2.txt

- **--search stopwords.txt documento1.txt documento2.txt ...:**
Realiza uma busca utilizando a técnica TF-IDF, considerando os documentos fornecidos e uma lista de stopwords.

Exemplo:
./indexer.exe --search stopwords.txt documento1.txt documento2.txt
## Contribuições
Contribuições são bem-vindas! Sinta-se à vontade para abrir problemas (issues) ou enviar pull requests para melhorar este programa.

Obrigado por usar o Indexer!
