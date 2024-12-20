/*
 * gerenciadorComandos.h
 *
 * Descrição:
 * Esta biblioteca define uma estrutura para gerenciar e processar comandos
 * textuais recebidos pela Serial (ou outra interface de comunicação).
 * Ela utiliza uma tabela de despacho (dispatch table) para associar nomes
 * de comandos a funções específicas (handlers), permitindo a execução
 * dinâmica de diferentes ações com base nos comandos recebidos.
 *
 * Funcionalidade Principal:
 * A biblioteca recebe comandos formatados como "nome_comando valor1 valor2 ...",
 * analisa a string, extrai o nome do comando e seus valores, e então
 * procura o comando na tabela de despacho. Se o comando for encontrado,
 * a função associada a ele é executada.
 *
 * Utilização:
 * 1. Inclua "gerenciadorComandos.h" no seu sketch Arduino.
 * 2. Defina as funções de tratamento (handlers) para cada comando.
 * 3. Popule a tabela 'tabelaComandos' no arquivo .cpp com os nomes dos
 *    comandos e os ponteiros para suas respectivas funções de tratamento.
 * 4. Use as funções 'analisarComando' e 'processarComando' para processar
 *    os comandos recebidos pela Serial.
 *
 * Exemplo de Comando:
 * "ligarLed" (liga o LED)
 * "piscarLed 5 1000 500" (pisca o LED 5 vezes, ligado por 1000ms e desligado por 500ms)
 *
 * Autor: Tiago Carvalho Pontes
 * Data: 12/12/2024
 * Versão: 1.0
 */



#ifndef GERENCIADOR_COMANDOS_H
#define GERENCIADOR_COMANDOS_H
      
// Estrutura para armazenar as informações de um comando individual.
struct Comando {
    String nome;                    // Nome do comando. Ex: "ligarLed".
    static const int maxValores = 5; // Número máximo de valores que um comando pode ter. 
                                     // O limite maximo de valores protege contra erros de acessar posições inválidas na memória (estouro de buffer) em comando.valores.
    String valores[maxValores];     // Array para armazenar até o limite maximo (maxValores) de valores (argumentos) do comando.
    int numValores;                // Número de valores presentes no comando.
};

// Estrutura para a tabela de comandos.
// Associa um nome de comando (string C) a um ponteiro para uma função que trata esse comando.
struct ComandoInfo {
    const char* nome;        // Nome do comando (string C). Ex: "ligarLed".
    void (*funcao)(Comando); // Ponteiro para a função que processa o comando.
};

// Declaração das variáveis globais que controlam o piscar do LED.
// O uso de 'extern' indica que a definição real dessas variáveis está em outro arquivo (.cpp).
extern bool piscarAtivo;            // Indica se o modo de piscar está ativo.
extern unsigned long tempoAnteriorLigado; // Armazena o tempo em que o LED foi ligado pela última vez (usado para controlar o tempo de ligado).
extern unsigned long tempoAnteriorDesligado;// Armazena o tempo em que o LED foi desligado pela última vez (usado para controlar o tempo de desligado).
extern int numPiscadasRestantes;    // Número de piscadas restantes (se o modo de piscar tiver um número limitado de piscadas).
extern int tempoLigadoAtual;        // Tempo atual em que o LED deve permanecer ligado (em milissegundos).
extern int tempoDesligadoAtual;     // Tempo atual em que o LED deve permanecer desligado (em milissegundos).
extern const int ledPin;            // Declaração do pino do LED (definido no .ino).

// Classe gerenciadorComando.
// Encapsula a lógica para analisar e processar comandos.
class gerenciadorComando {
public:
    // Analisa uma string de comando recebida, extraindo o nome do comando e seus valores.
    Comando analisarComando(String comandoRecebido);

    // Processa um comando, buscando-o na tabela de comandos e executando a função correspondente.
    void processarComando(Comando comando);

    // Tabela de despacho (dispatch table) que associa nomes de comandos a funções de tratamento.
    // 'static' significa que esta tabela é compartilhada por todas as instâncias da classe.
    static ComandoInfo tabelaComandos[];
};

#endif