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

//Bibliotecas do Arduino IDE:
#include <Arduino.h> // Inclui a biblioteca principal do Arduino.
                    // Esta biblioteca fornece as funções essenciais para programar o Arduino,
                    // como pinMode(), digitalWrite(), analogRead(), Serial.begin(), delay(), millis(), etc.
                    // É *obrigatória* em praticamente todos os sketches do Arduino.
#include "gerenciadorComandos.h" // Inclui o cabeçalho desta biblioteca (gerenciador de comandos).

// Declaração das variáveis globais (definidas aqui, declaradas com 'extern' no .h)
bool piscarAtivo = false;            // Flag que indica se o modo de piscar está ativo.
unsigned long tempoAnteriorLigado = 0;   // Armazena o tempo em que o LED foi ligado pela última vez (millis()).
unsigned long tempoAnteriorDesligado = 0; // Armazena o tempo em que o LED foi desligado pela última vez (millis()).
int numPiscadasRestantes = 0;       // Número de piscadas restantes (se definido um número limitado).
int tempoLigadoAtual = 1000;         // Tempo atual em que o LED deve permanecer ligado (em milissegundos).
int tempoDesligadoAtual = 1000;      // Tempo atual em que o LED deve permanecer desligado (em milissegundos).

// Funções de tratamento dos comandos (handlers)

// Trata o comando "status".
void tratarStatus(Comando comando) {
    // Verifica se o número de valores (argumentos) passados para o comando é diferente de zero.
    if (comando.numValores != 0) {
        // Se `comando.numValores` for diferente de 0, significa que parâmetros foram fornecidos, o que é um erro para este comando.
        Serial.println("Erro: O comando 'status' não aceita parâmetros.");
        // Imprime o número de parâmetros fornecidos para auxiliar na depuração.
        Serial.print("Número de parâmetros fornecidos: ");
        Serial.println(comando.numValores);
        // Abandona a execução da função `tratarStatus` imediatamente, pois houve um erro.
        return; // Saída antecipada da função em caso de erro
    }

    // Se não houver parâmetros, imprime "online" na Serial.
    // Isso indica que o sistema está funcionando e a comunicação Serial está ativa.
    Serial.println("online"); // Imprime "online" na Serial, indicando que o sistema está funcionando
}

// Trata o comando "ligarLed".
void tratarLigarLed(Comando comando) {
    // Verifica se o número de valores (argumentos) passados para o comando é diferente de zero.
    if (comando.numValores != 0) {
        // Se `comando.numValores` for diferente de 0, significa que parâmetros foram fornecidos, o que é um erro para este comando.
        Serial.println("Erro: O comando 'ligarLed' não espera nenhum parâmetro.");
        // Imprime o número de parâmetros fornecidos na Serial para auxiliar na depuração.
        Serial.print("Número de parâmetros fornecidos: ");
        Serial.println(comando.numValores);
        // Abandona a execução da função `tratarLigarLed` imediatamente, pois houve um erro.
        return; // Saída antecipada da função em caso de erro
    } 
    piscarAtivo = false;     // Desativa o piscar (Esse comando é uma garantia caso o piscarLed esteja ativo).
    // Se a execução chegou até aqui, significa que `comando.numValores` é igual a 0 (nenhum parâmetro foi fornecido).
    // Define o pino do LED (ledPin) como HIGH, ligando o LED.
    digitalWrite(ledPin, HIGH); // Liga o LED
}

// Trata o comando "piscarLed".
void tratarPiscarLed(Comando comando) {
    // Ativa a flag `piscarAtivo`, indicando que o modo de piscar está em execução.
    piscarAtivo = true;

    // Lógica para tratar diferentes quantidades de parâmetros para o comando "piscarLed".

    // Se nenhum parâmetro for fornecido (comando.numValores == 0).
    if (comando.numValores == 0) {
        // Define o tempo ligado para 1000 milissegundos (1 segundo).
        tempoLigadoAtual = 1000;
        // Define o tempo desligado para 1000 milissegundos (1 segundo).
        tempoDesligadoAtual = 1000;
        // Define `numPiscadasRestantes` para -1, indicando que o LED deve piscar indefinidamente.
        numPiscadasRestantes = -1;
    } 

    // Senão, se um parâmetro for fornecido (comando.numValores == 1).
    else if (comando.numValores == 1) {
        // Converte o primeiro valor fornecido para um inteiro e armazena em `numPiscadas`.
        int numPiscadas = comando.valores[0].toInt();
        // Verifica se o número de piscadas é menor ou igual a zero.
        if (numPiscadas <= 0) {
            // Se for, imprime uma mensagem de erro na Serial.
            Serial.println("Erro: O número de piscadas deve ser maior que zero.");
            // Desativa o modo de piscar, pois houve um erro.
            piscarAtivo = false;
            // Abandona a execução da função `tratarPiscarLed`.
            return;
        }
        // Define o tempo ligado para 1000 milissegundos.
        tempoLigadoAtual = 1000;
        // Define o tempo desligado para 1000 milissegundos.
        tempoDesligadoAtual = 1000;
        // Calcula o número total de transições (ligado/desligado) multiplicando o número de piscadas por 2.
        numPiscadasRestantes = numPiscadas * 2;
    } 

    // Senão, se dois parâmetros forem fornecidos (comando.numValores == 2).
    else if (comando.numValores == 2) {
        // Converte o primeiro valor para o tempo ligado.
        tempoLigadoAtual = comando.valores[0].toInt();
        // Converte o segundo valor para o tempo desligado.
        tempoDesligadoAtual = comando.valores[1].toInt();
        // Verifica se os tempos ligado e desligado são menores ou iguais a zero.
        if (tempoLigadoAtual <= 0 || tempoDesligadoAtual <= 0) {
            // Se algum tempo for inválido, imprime uma mensagem de erro.
            Serial.println("Erro: Os valores de tempo devem ser maiores que zero.");
            // Desativa o modo de piscar.
            piscarAtivo = false;
            // Abandona a execução da função.
            return;
        }
        // Define `numPiscadasRestantes` para -1, indicando piscar indefinidamente.
        numPiscadasRestantes = -1;
    } 

    // Senão, se três parâmetros forem fornecidos (comando.numValores == 3).
    else if (comando.numValores == 3) {
        // Converte o primeiro valor para o número de piscadas.
        int numPiscadas = comando.valores[0].toInt();
        // Converte o segundo valor para o tempo ligado.
        tempoLigadoAtual = comando.valores[1].toInt();
        // Converte o terceiro valor para o tempo desligado.
        tempoDesligadoAtual = comando.valores[2].toInt();
        // Verifica se o número de piscadas é menor ou igual a zero.
        if (numPiscadas <= 0) {
            // Se for, imprime uma mensagem de erro.
            Serial.println("Erro: O número de piscadas deve ser maior que zero.");
            // Desativa o modo de piscar.
            piscarAtivo = false;
            // Abandona a execução da função.
            return;
        }
        // Verifica se os tempos ligado e desligado são menores ou iguais a zero.
        if (tempoLigadoAtual <= 0 || tempoDesligadoAtual <= 0) {
            // Se algum tempo for inválido, imprime uma mensagem de erro.
            Serial.println("Erro: Os valores de tempo devem ser maiores que zero.");
            // Desativa o modo de piscar.
            piscarAtivo = false;
            // Abandona a execução da função.
            return;
        }
        // Calcula o número total de transições (ligado/desligado).
        numPiscadasRestantes = numPiscadas * 2; // Multiplica numPiscadas por 2 porque uma "piscada" completa consiste em duas transições:
                                                // 1. LED aceso (HIGH)
                                                // 2. LED apagado (LOW)
    } 

    // Senão, se um número diferente de 0, 1, 2 ou 3 parâmetros for fornecido.
    else {
        // Imprime uma mensagem de erro indicando a quantidade incorreta de parâmetros.
        Serial.println("Erro: O comando 'piscarLed' espera 0, 1, 2 ou 3 parâmetros.");
        // Desativa o modo de piscar.
        piscarAtivo = false;
        // Abandona a execução da função.
        return;
    }
    // Armazena o tempo atual em `tempoAnteriorLigado` para controlar o tempo de piscar.
    tempoAnteriorLigado = millis();
}

// Trata o comando "desligarLed".
void tratarDesligarLed(Comando comando) {
    // Verifica se o número de valores (parâmetros) passados para o comando é diferente de zero.
    if (comando.numValores != 0) {
        // Se `comando.numValores` for diferente de 0, significa que parâmetros foram fornecidos, o que é um erro para este comando.
        Serial.println("Erro: O comando 'desligarLed' não espera nenhum parâmetro.");
        // Imprime o número de parâmetros fornecidos na Serial para auxiliar na depuração.
        Serial.print("Número de parâmetros fornecidos: ");
        Serial.println(comando.numValores);
        // Abandona a execução da função `tratarDesligarLed` imediatamente, pois houve um erro.
        return; // Saída antecipada da função em caso de erro
    }
    // Se a execução chegou até aqui, significa que nenhum parâmetro foi fornecido.
    // Define o pino do LED (ledPin) como LOW, desligando o LED.
    digitalWrite(ledPin, LOW); // Desliga o LED
    // Desativa a flag `piscarAtivo`, interrompendo qualquer ciclo de piscar que estivesse em andamento.
    piscarAtivo = false;     // Desativa o piscar
}

// Funções de tratamento dos comandos
// Define a função tratarAjuda que recebe um objeto Comando como parâmetro. Esta função lida com o comando "ajuda".
void tratarAjuda(Comando comando) { 
// Verifica se o número de valores/argumentos fornecidos com o comando é diferente de zero. O comando "ajuda" não deve receber argumentos.
  if (comando.numValores != 0) {
    // Se `comando.numValores` for diferente de 0, significa que parâmetros foram fornecidos, o que é um erro para este comando.
    Serial.println("Erro: A função 'ajuda' não espera nenhum parâmetro.");
    // Imprime o número de parâmetros fornecidos na Serial para auxiliar na depuração.
    Serial.print("Número de parâmetros fornecidos: ");
    Serial.println(comando.numValores);
    // Abandona a execução da função `tratarDesligarLed` imediatamente, pois houve um erro.
    return; // Saída antecipada da função em caso de erro.
  } 

  Serial.println("Lista de Comandos:"); // Imprime na Serial o cabeçalho "Lista de Comandos:".
  Serial.println("------------------"); // Imprime uma linha separadora na Serial para melhorar a legibilidade.
  Serial.println("status: Exibe o estado atual do sistema."); // Imprime na Serial a descrição do comando "status".
  Serial.println("ligarLed: Liga o LED continuamente."); // Imprime na Serial a descrição do comando "ligarLed".
  Serial.println("desligarLed: Desliga o LED."); // Imprime na Serial a descrição do comando "desligarLed".
  Serial.println("piscarLed:"); // Imprime na Serial o cabeçalho para as opções do comando "piscarLed".
  Serial.println("  Pisca o LED com diferentes configurações:"); // Imprime na Serial uma breve explicação sobre as configurações do comando "piscarLed".
  Serial.println("  - Sem parâmetros: Pisca indefinidamente com 1 segundo ligado e 1 segundo desligado."); // Imprime na Serial a descrição do comportamento padrão do "piscarLed" (sem parâmetros).
  Serial.println("  - <numPiscadas>: Pisca o LED o número especificado de vezes, com 1 segundo ligado e 1 segundo desligado."); // Imprime na Serial a descrição do "piscarLed" com um parâmetro (Número de piscadas).
  Serial.println("  - <tempoLigado> <tempoDesligado>: Pisca indefinidamente com os tempos fornecidos (em milissegundos)."); // Imprime na Serial a descrição do "piscarLed" com dois parâmetros (tempo ligado e tempo desligado).
  Serial.println("  - <numPiscadas> <tempoLigado> <tempoDesligado>: Pisca o LED <numPiscadas> vezes com os tempos fornecidos (em milissegundos)."); // Imprime na Serial a descrição do "piscarLed" com três parâmetros (número de piscadas, tempo ligado e tempo desligado).
  Serial.println("ajuda: Exibe esta lista de comandos."); // Imprime na Serial a descrição do próprio comando "ajuda".
  Serial.println("------------------"); // Imprime uma linha separadora na Serial para melhorar a legibilidade.
}


ComandoInfo gerenciadorComando::tabelaComandos[] = { // Cria uma tabela chamada tabelaComandos dentro da classe gerenciadorComando. 
                                                     // Essa tabela serve como um "guia" para o programa, associando os nomes dos comandos que 
                                                     // o usuário pode digitar com as funções que devem ser executadas para cada comando. 
                                                     // É como um índice de um livro: o nome do comando é o título e a função é o conteúdo da página.
  {"status", tratarStatus}, // Quando o usuário digitar "status", o programa vai chamar a função tratarStatus.
  {"ligarLed", tratarLigarLed}, // Se o usuário digitar "ligarLed", a função tratarLigarLed será executada, acendendo o LED (a luzinha).
  {"piscarLed", tratarPiscarLed}, // Ao digitar "piscarLed", a função tratarPiscarLed entra em ação, fazendo o LED piscar.
  {"desligarLed", tratarDesligarLed}, // Com "desligarLed", a função tratarDesligarLed é chamada, apagando o LED.
  {"ajuda", tratarAjuda}, // Se o usuário precisar de ajuda e digitar "ajuda", a função tratarAjuda mostrará uma lista com todos os comandos disponíveis e uma breve explicação de cada um. É como um manual de instruções dentro do programa.
  {nullptr, nullptr} // Essa linha é muito importante! Ela marca o final da tabela. 
                     // É como colocar um ponto final em uma frase. O programa usa essa marcação para saber onde a tabela termina. 
                     // Sem ela, o programa pode tentar ler dados errados na memória, causando erros. 
                     // nullptr significa "ponteiro nulo", ou seja, não aponta para lugar nenhum, indicando o fim da lista.
};

Comando gerenciadorComando::analisarComando(String comandoRecebido) {
  /*
   * Objetivo: Esta função analisa uma string de comando recebida, separando o nome do comando e seus valores numéricos.
   * Parâmetro: comandoRecebido - A string contendo o comando e seus valores. Ex: "piscarLed 10 200 300"
   * Retorno: Um struct Comando contendo o nome do comando e um array de até o maximo de valores numéricos (representados como strings, para posterior conversão).
   */

  Comando comando; // Cria uma variável chamada comando do tipo struct Comando. Essa variável armazenará as informações do comando que será analisado.
  comando.nome = ""; // Limpa o nome do comando, colocando uma string vazia. Isso evita que restos de comandos anteriores causem erros.
  comando.numValores = 0; // Zera o contador de valores. Assim como o nome, isso evita que valores antigos interfiram na análise do novo comando.
  for (int i = 0; i < Comando::maxValores; i++) { // Este loop percorre o array de valores dentro do struct Comando.
    comando.valores[i] = ""; // Inicializa cada posição do array de valores com uma string vazia. Importante para evitar "lixo" na memória.
  }
  comandoRecebido.trim(); // Remove espaços em branco extras no início e no final da string recebida. Por exemplo: "   piscarLed 10 " se torna "piscarLed 10".

  if (comandoRecebido.length() == 0) return comando; // Se a string estiver vazia (só espaços ou nada), retorna o comando vazio. Isso evita erros ao tentar processar uma string sem conteúdo.

  int pos = comandoRecebido.indexOf(' '); // Procura o primeiro espaço em branco na string. Este espaço separa o nome do comando dos seus valores.
                                          // Se não houver espaço, pos será -1.

  if (pos != -1) { // Verifica se encontrou um espaço. Se sim, significa que o comando tem pelo menos um valor.

    comando.nome = comandoRecebido.substring(0, pos); // Extrai o nome do comando. É a parte da string que vem antes do primeiro espaço.
                                                      // Ex: Se comandoRecebido é "piscarLed 10", comando.nome será "piscarLed".
    comandoRecebido = comandoRecebido.substring(pos + 1); // Remove o nome do comando e o espaço da string original, deixando apenas os valores.
                                                          // Ex: Se antes era "piscarLed 10 200", agora será "10 200".
    comandoRecebido.trim(); // Remove espaços extras entre os valores, como múltiplos espaços entre "10" e "200".

    int i = 0; // Cria um contador para controlar quantos valores foram lidos.
    while (i < Comando::maxValores && comandoRecebido.length() > 0) { // Loop que lê os valores. Ele continua enquanto não atingir o limite máximo de valores (Comando::maxValores) e enquanto ainda houver texto na string comandoRecebido.
                                                                    // O limite maximo de valores protege contra erros de acessar posições inválidas na memória (estouro de buffer) em comando.valores.
      pos = comandoRecebido.indexOf(' '); // Procura o próximo espaço em branco.

      if (pos != -1) { // Se encontrou um espaço, significa que ainda há mais valores a serem lidos.
        comando.valores[i] = comandoRecebido.substring(0, pos); // Extrai o valor atual (até o próximo espaço).
        comandoRecebido = comandoRecebido.substring(pos + 1); // Remove o valor lido e o espaço da string.
        comandoRecebido.trim(); // Remove espaços extras após o valor lido.
      } else { // Se não encontrou espaço, significa que este é o último valor.
        comando.valores[i] = comandoRecebido; // Extrai o último valor restante.
        comandoRecebido = ""; // Limpa a string comandoRecebido para encerrar o loop.
      }
      i++; // Incrementa o contador de valores lidos.
    }
    comando.numValores = i; // Armazena a quantidade total de valores numéricos que foram lidos.
  } else { // Se não encontrou nenhum espaço na string original, significa que o comando não tem valores.

    comando.nome = comandoRecebido; // O nome do comando é a string inteira. Ex: "piscarLed".
  }
  return comando; // Retorna o struct Comando preenchido com o nome do comando e seus valores.
}

void gerenciadorComando::processarComando(Comando comando) {
  // Esta função recebe um struct Comando (que contém o nome do comando e seus valores) e procura na tabela de comandos a função que deve ser executada para esse comando.

  for (int i = 0; tabelaComandos[i].nome != nullptr; i++) {  // Loop que percorre a tabela de comandos 'tabelaComandos'.
    // A tabela 'tabelaComandos' é uma lista de pares: nome do comando e a função que o executa.
    // O loop continua enquanto não chegar ao final da tabela, que é marcado por um 'nullptr' no campo 'nome'.
    // 'i' é o índice que indica a posição atual na tabela.

    if (comando.nome == tabelaComandos[i].nome) { // Verifica se o nome do comando que foi recebido ('comando.nome') é igual ao nome de um comando que está na tabela ('tabelaComandos[i].nome').
      // Esta é a parte principal da função: encontrar o comando correto na tabela.

      tabelaComandos[i].funcao(comando); // Se encontrou o comando na tabela, esta linha chama a função correspondente para executar o comando.
      // 'tabelaComandos[i].funcao' é um "ponteiro para função". Isso significa que ele armazena o endereço da função que deve ser executada.
      // O 'comando' é passado como argumento para a função de tratamento, para que a função tenha acesso aos valores que foram enviados junto com o comando.

      return; // Após executar a função do comando, a função 'processarComando' termina. Isso evita que o loop continue procurando, o que seria desnecessário.
    }
  }
    // Se o loop terminar sem encontrar o comando:
  Serial.print("ERRO: Comando inválido: "); // Imprime uma mensagem indicando que o comando é inválido.
  Serial.println(comando.nome);       // Imprime o nome do comando que foi digitado incorretamente.
  Serial.println("Digite 'ajuda' para listar os comandos disponíveis.");
}