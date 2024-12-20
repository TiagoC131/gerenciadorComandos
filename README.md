# gerenciadorComandos

[![Version](https://img.shields.io/badge/version-1.0-blue.svg)](https://github.com/TiagoC131/gerenciadorComandos/releases/tag/v1.0)

Uma biblioteca Arduino para gerenciar e processar comandos textuais recebidos pela Serial (ou outra interface de comunicação).

## Descrição

Esta biblioteca define uma estrutura robusta para processar comandos enviados via Serial para o seu Arduino. Ela utiliza uma tabela de despacho (dispatch table) para associar nomes de comandos a funções específicas (handlers), permitindo a execução dinâmica de diferentes ações com base nos comandos recebidos. Isso torna o código mais organizado, modular e fácil de manter, especialmente para projetos com muitos comandos.

## Funcionalidades Principais

A biblioteca `gerenciadorComandos` permite:

*   Receber comandos textuais pela Serial no formato `nome_comando valor1 valor2 ...`.
*   Analisar e extrair o nome do comando e seus argumentos.
*   Suportar múltiplos parâmetros para um único comando, permitindo o controle preciso de diferentes aspectos da ação.
*   Executar a função correspondente ao comando através de uma tabela de despacho.
*   Tratar comandos inválidos com mensagens de erro informativas.
*   Tornar a manutenção do código mais fácil, pois as funções de tratamento de cada comando são isoladas.
*   Facilitar a adição e remoção de comandos sem alterar a estrutura principal do código.

Isso permite a criação de um sistema de comandos robusto e extensível, onde novos comandos podem ser facilmente adicionados sem modificar o código principal de processamento.

 **Exemplos de uso no Monitor Serial:**

*   `ligarLed`: Liga o LED.
*   `desligarLed`: Desliga o LED.
*   `piscarLed`: Pisca o LED indefinidamente com 1 segundo ligado e 1 segundo desligado.
*   `piscarLed 5`: Pisca o LED 5 vezes com os tempos padrão (1 segundo ligado, 1 segundo desligado).
*   `piscarLed 500 250`: Pisca o LED indefinidamente, com 500ms ligado e 250ms desligado.
*   `piscarLed 3 500 250`: Pisca o LED 3 vezes, com 500ms ligado e 250ms desligado.

<div align="center">
  Se você gostou deste projeto e deseja contribuir para o seu desenvolvimento contínuo, você pode fazer uma doação via Pix: <strong>tiago.c1@hotmail.com</strong> <br>
  Sua ajuda é fundamental para o desenvolvimento contínuo deste projeto!
</div>

## Licença

Este projeto é licenciado sob a [Licença MIT](LICENSE). Consulte o arquivo `LICENSE` para obter mais informações.

MIT License / Copyright (c) 2024 TiagoC131