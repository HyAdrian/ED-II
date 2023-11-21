# Indexer README

## Autores
- Hygor Adriano Tristão GRR20223811
- Fernando Guilherme Gorski GRR20223817
- Lucas Massayuki Gohara GRR20221097

## Descrição
Este é um programa em C que realiza operações de indexação e busca em documentos de texto. Ele inclui funcionalidades como contagem de frequência de palavras, busca por palavras específicas e geração de índices TF-IDF.

## Como usar

1. **Clone o repositório na sua máquina:**
2. **Compile o arquivo:**
`gcc indexer.c -o indexer -lm`

3. **Execute o programa:**
./indexer.exe [opções] [arquivo1.txt arquivo2.txt ...]

## Opções disponíveis

- **--freq:**
Conta a frequência das `num_palavras` palavras mais frequentes nos documentos fornecidos.

Exemplo:
./indexer.exe --freq 10 documento1.txt

- **--freq-word:**
Conta a frequência da palavra específica nos documentos fornecidos.

Exemplo:
./indexer.exe --freq-word exemplo documento1.txt

- **--search:**
Realiza uma busca utilizando a técnica TF-IDF, considerando os documentos fornecidos e uma lista de termos.

Exemplo:
./indexer.exe --search "termos de exemplo" documento1.txt documento2.txt...

## Descrição das Funcionalidades

### --freq
O programa realiza a abertura do arquivo especificado e calcula dinamicamente o tamanho da tabela hash com base no tamanho do arquivo. Utilizando um buffer, o programa coleta partes do arquivo, as divide em palavras e, para cada palavra, a insere na tabela hash. Caso uma palavra já exista na tabela hash, o programa atualiza a contagem "qtd" dessa palavra no nó incrementando em +1. Após a conclusão da leitura do arquivo, o programa percorre a tabela hash e identifica as *n* palavras mais frequentes.

### --freq-word
O programa abre o arquivo especificado e, utilizando um buffer, coleta partes do arquivo, as divide em palavras e, para cada palavra, compara-a com a palavra especificada no comando. Se a palavra no arquivo corresponder à palavra fornecida no comando, o programa atualiza o contador associado a essa palavra incrementando em +1. Ao final da leitura do arquivo, o programa exibe o contador na tela.

### --search
O programa cria uma tabela hash para as palavras especificadas no termo. Em seguida, para cada arquivo, ele abre o arquivo e, utilizando um buffer, coleta partes do arquivo, as divide em palavras e, para cada palavra, busca na tabela hash do termo. Se a palavra estiver presente, o programa atualiza o contador "qtd" associado a essa palavra em +1. Após a leitura de cada arquivo, o programa calcula o Term Frequency (TF) de cada arquivo.

Posteriormente, após calcular o TF de todos os arquivos, o programa passa a calcular o Inverse Document Frequency (IDF) de cada arquivo. Em seguida, calcula o TF-IDF para cada documento. Cada documento, juntamente com seu respectivo TF-IDF, é inserido em uma árvore binária de busca.

É importante observar que, no caso de termos compostos por mais de uma palavra, o programa calcula a média dos TF-IDF de cada palavra no arquivo.

Finalmente, o programa imprime na tela os arquivos que possuem maior relevância em relação ao termo, ordenados de forma decrescente com base nos valores de TF-IDF.
