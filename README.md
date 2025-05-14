Nome do Aluno: Samuel Guedes Canário
Polo: Bom Jesus da Lapa
Data: 06/05/2025
Projeto: Estação IoT com Controle Inteligente de Umidificador e Feedback Visual
🎯 Objetivo Geral
Desenvolver uma estação IoT embarcada com sensores de temperatura, umidade e oxigenação, integrada a uma interface web que permita o monitoramento remoto dessas variáveis e o acionamento de um umidificador inteligente. O projeto utiliza tarefas do FreeRTOS na placa BitDogLab com RP2040 e oferece feedback visual por meio de uma matriz de LEDs WS2812B.

⚙️ Descrição Funcional
O sistema será dividido em duas fases:

✅ Fase Atual (Etapa 1 - Projeto Conceitual e Estrutura de Código)
Estruturação de tarefas com FreeRTOS para coleta de dados, controle e interface.

Definição das variáveis globais e fluxo de funcionamento.

Organização do código modular e simulações lógicas do comportamento do sistema.

Interface web funcional com botões para:

Atualizar leituras de sensores.

Ativar/desativar o umidificador.

🔧 Fase Futuras (Etapa 2 - Integração com Periféricos)
Será realizada a implementação física e integração dos periféricos com o sistema embarcado:

Sensores ambientais (temperatura, umidade, oxigenação).

Relé ou driver para controle do umidificador.

Matriz de LEDs WS2812B para sinalização visual.

Display OLED para exibição local das leituras.

🔩 Periféricos Planejados
Componente	GPIO	Função Esperada
Sensor de Temperatura e Umidade	GPIO 2	Leitura periódica das condições ambientais.
Sensor de Oxigenação	GPIO 3 (ADC)	Monitoramento da qualidade do ar ou oxigênio.
Matriz de LEDs WS2812B (5x5)	GPIO 7	Exibição de estados visuais (alertas, status, ativação).
Umidificador	GPIO 10	Controle de ativação via relé/MOSFET.
Botão físico (opcional)	GPIO 5	Alternância local do estado do umidificador.
Display OLED I2C (SSD1306)	SDA: 14, SCL: 15	Exibição local dos valores e estado do sistema.
Wi-Fi (via ESP externo)	UART/I2C/SPI	Comunicação com a interface web.

🌐 Interface Web (já funcional na Etapa 1)
Uma página web simples hospedada localmente permitirá:

Visualizar valores atualizados de temperatura, umidade e oxigênio.

Ativar ou desativar o umidificador remotamente.

Visualizar o status do sistema (por exemplo: "Normal", "Alerta", "Umidificador Ativado").

🧠 Organização com FreeRTOS
Tarefas estruturadas para modularidade e expansão futura:

vSensorTask() – Simulada nesta fase; futura leitura real dos sensores.

vWebServerTask() – Já em desenvolvimento; gerencia a interface web.

vControlTask() – Controla a flag de ativação do umidificador.

vMatrizTask() – Simula resposta visual da matriz de LEDs.

vDisplayTask() – Simulada para exibição em display OLED.

Uso de variáveis globais como temperature, humidity, oxygen e humidifier_on para comunicação entre tarefas. Não há necessidade de filas ou mutexes nesta fase.

📘 Destaques Técnicos
Estrutura baseada em FreeRTOS para controle eficiente e escalável.

Interface web acessível por navegador, sem necessidade de backend externo.

Projeto modular com foco em expansão futura para ambientes reais.

Pronto para integrar sensores físicos na próxima fase.

🔗 Links Importantes

Vídeo de Demonstração: https://www.youtube.com/watch?v=KLQc-AOT414

📝 Observação Final
Ainda não foi realizada a implementação física dos periféricos. A leitura de sensores, controle do umidificador e sinalização visual por LEDs serão integrados na segunda fase do projeto, prevista para as próximas semanas.
