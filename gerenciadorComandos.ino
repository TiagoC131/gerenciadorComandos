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
 
 #include "gerenciadorComandos.h"

gerenciadorComando gerenciador; // Cria um objeto (instância) da classe gerenciadorComando.
                                // Este objeto será usado para acessar as funções da classe, como analisarComando e processarComando.

// Variaveis
const int ledPin = 13; // Define o pino digital 13 como o pino do LED. 'const' significa que este valor não pode ser alterado durante a execução do programa.
                       // Este é o LED embutido na maioria das placas Arduino Uno.

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial com uma taxa de 9600 bauds.
                      // Isso configura o Arduino para se comunicar com o computador (ou outro dispositivo) pela porta serial.
  pinMode(ledPin, OUTPUT); // Configura o pino ledPin (pino 13) como uma saída.
                           // Isso significa que o Arduino pode enviar um sinal elétrico para este pino, ligando ou desligando o LED.
}

void loop() {
  if (Serial.available() > 0) {// Verifica se há dados disponíveis para leitura na porta serial.
                                // Serial.available() retorna o número de bytes disponíveis para leitura. Se for maior que 0, significa que há dados para serem lidos.

    String comandoRecebido = Serial.readStringUntil('\n'); // Lê uma string da porta serial até encontrar um caractere de nova linha ('\n').
                                                           // Este caractere é tipicamente enviado quando você pressiona Enter no monitor serial.
                                                           // A string lida é armazenada na variável comandoRecebido.

    Comando comando = gerenciador.analisarComando(comandoRecebido); // Chama a função analisarComando do objeto gerenciador.
                                                                    // Esta função processa a string recebida e extrai o nome do comando e seus argumentos (se houver).
                                                                    // O resultado é armazenado em um struct Comando (uma estrutura de dados).

    gerenciador.processarComando(comando); // Chama a função processarComando do objeto gerenciador.
                                           // Esta função recebe o struct Comando e executa a ação correspondente ao comando recebido, usando uma tabela de comandos.
    }

  // Esta parte controla o piscar do LED e deve permanecer dentro do loop(), pois precisa ser executada repetidamente para funcionar.
  // Ela não usa diretamente a tabelaComandos, mas depende da variável global piscarAtivo, que é modificada pela função tratarPiscarLed dentro do gerenciadorComandos.
  if (piscarAtivo) { // Verifica se o modo de piscar está ativo.
        if (digitalRead(ledPin) == LOW) { // Verifica se o LED está desligado (nível LOW).
            if (millis() - tempoAnteriorDesligado >= tempoDesligadoAtual) { // Verifica se o tempo desde que o LED foi desligado é maior ou igual ao tempo de desligado configurado (tempoDesligadoAtual).
                digitalWrite(ledPin, HIGH); // Liga o LED (nível HIGH).
                tempoAnteriorLigado = millis(); // Guarda o tempo atual em que o LED foi ligado.
                if (numPiscadasRestantes > 0) { // Se houver um número limitado de piscadas configurado.
                    numPiscadasRestantes--; // Decrementa o contador de piscadas restantes.
                }
            }
        } else { // Se o LED estiver ligado (nível HIGH).
            if (millis() - tempoAnteriorLigado >= tempoLigadoAtual) { // Verifica se o tempo desde que o LED foi ligado é maior ou igual ao tempo de ligado configurado (tempoLigadoAtual).
                digitalWrite(ledPin, LOW); // Desliga o LED (nível LOW).
                tempoAnteriorDesligado = millis(); // Guarda o tempo atual em que o LED foi desligado.
                if (numPiscadasRestantes > 0) { // Se houver um número limitado de piscadas configurado.
                    numPiscadasRestantes--; // Decrementa o contador de piscadas restantes.
                }
            }
        }
        if (numPiscadasRestantes == 0) { // Se o número de piscadas restantes chegar a 0.
            piscarAtivo = false; // Desativa o modo de piscar.
        }
    }
}