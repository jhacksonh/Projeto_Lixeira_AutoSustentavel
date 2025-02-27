# Passo a Passo para Simulação da Lixeira AutoSustentável no BitDogLab (RP2040)

Este documento descreve o passo a passo para simular o funcionamento da **Lixeira AutoSustentável** usando a placa **BitDogLab (RP2040)**. O projeto é dividido em módulos funcionais para facilitar a implementação e testes.

---

## 1. Configuração do Ambiente
- Instale o **SDK do Raspberry Pi Pico** (para RP2040) no seu ambiente de desenvolvimento.
- Configure o BitDogLab no seu computador e conecte-o via USB.
- Crie um projeto em C no seu editor preferido (VS Code, PlatformIO, etc.) e configure o ambiente para compilar e enviar o código para a placa.

---

## 2. Módulos do Projeto

### 2.1 Módulo de Entrada (Sensor de Tampa)
- Use um **sensor de proximidade** ou **botão** para simular a abertura da tampa.
- Configure um pino GPIO como entrada para detectar quando a tampa é aberta.
- Quando o sinal for detectado, inicie o fluxo principal do programa.

### 2.2 Módulo de Captura de Imagem (Câmera Simulada)
- Simule a câmera usando um array de dados pré-definidos.
- Crie um array com valores que representem diferentes tipos de resíduos (ex: 1 = Papel, 2 = Plástico, 3 = Metal, etc.).
- Use uma função para "capturar" um valor aleatório ou fixo desse array.

### 2.3 Módulo de Classificação
- Crie uma função para classificar o resíduo com base no valor "capturado" pela câmera simulada.
- Use uma estrutura `switch-case` ou `if-else` para determinar o tipo de resíduo.
- **Melhoria:** Adicione um modelo de machine learning embarcado (TensorFlow Lite) para classificação mais precisa (opcional).

### 2.4 Módulo de Atuadores (Compartimentos)
- Use LEDs ou servos para simular a abertura dos compartimentos.
- Configure pinos GPIO como saída para controlar os LEDs (cada LED representa um compartimento).
- Quando o resíduo for classificado, acenda o LED correspondente ao compartimento correto.

### 2.5 Módulo de Sensor de Volume
- Simule o sensor de volume usando um potenciômetro ou um valor aleatório.
- Leia o valor do potenciômetro usando o ADC (Conversor Analógico-Digital) da placa.
- Exiba o valor do volume no terminal ou use um LED para indicar quando o compartimento estiver cheio.

### 2.6 Módulo de Botão de Emergência
- Configure um botão em outro pino GPIO para simular o botão de emergência.
- Quando pressionado, interrompa o fluxo principal e acione um LED ou exiba uma mensagem de erro.

### 2.7 Módulo de Hibernação
- Após a classificação e abertura do compartimento, coloque o sistema em modo de baixo consumo.
- Use a função `sleep_ms()` para simular a hibernação até que a tampa seja aberta novamente.

### 2.8 Módulo de Comunicação (Wi-Fi/Bluetooth)
- Adicione um módulo Wi-Fi (ex: ESP8266/ESP32) ou Bluetooth para comunicação.
- Envie dados sobre o status da lixeira (volume dos compartimentos, tipo de resíduo coletado, etc.) para um servidor ou aplicativo.
- **Exemplo:** Envie um alerta quando um compartimento estiver cheio.

### 2.9 Módulo de Interface do Usuário
- Adicione um display LCD ou OLED para mostrar informações como o tipo de resíduo detectado, volume dos compartimentos e status do sistema.
- Use um buzzer para dar feedback sonoro ao usuário.

### 2.10 Módulo de Autonomia Energética
- Simule a energia solar usando uma entrada analógica para representar a carga da bateria.
- Implemente um modo de baixo consumo usando o modo "sleep" do RP2040.

---

## 3. Fluxo Principal do Programa
1. Inicialize todos os módulos (GPIO, ADC, comunicação, display, etc.).
2. Aguarde o sinal do sensor de tampa (ou botão) para iniciar o fluxo.
3. Capture o valor da câmera simulada.
4. Classifique o resíduo com base no valor capturado.
5. Abra o compartimento correspondente (acenda o LED correto).
6. Verifique o sensor de volume e exiba o status no display.
7. Envie os dados para um servidor ou aplicativo via Wi-Fi/Bluetooth.
8. Hiberne o sistema até que a tampa seja aberta novamente.
9. Repita o processo.

---

## 4. Testes e Simulação
- Conecte LEDs aos pinos GPIO correspondentes para simular os compartimentos.
- Use um botão para simular a abertura da tampa.
- Conecte um potenciômetro ao pino ADC para simular o sensor de volume.
- Adicione um display e um buzzer para a interface do usuário.
- Monitore a saída no terminal para verificar o funcionamento do sistema.

---

## 5. Próximos Passos
- Implementar o código para cada módulo.
- Realizar testes em diferentes cenários.
- Documentar o projeto detalhadamente para reprodução.
- Expandir o projeto com novas funcionalidades (ex: machine learning, integração IoT).

---

Este passo a passo serve como guia para desenvolver a simulação da **Lixeira AutoSustentável** no BitDogLab. Adapte conforme necessário e avance para a implementação! 😊