# Sistema de Separação de Lixo Automatizado

## Descrição
Este projeto implementa um sistema de separação de lixo automatizado utilizando um Raspberry Pi Pico. Ele detecta diferentes tipos de lixo e os encaminha para os compartimentos corretos. O sistema utiliza sensores, um display OLED, LEDs WS2812, motores e um sistema de detecção simulado por câmera.

## Funcionalidades
- **Identifica e separa diferentes tipos de lixo**
- **Indica estados do sistema com LEDs WS2812**
- **Exibe informações no display OLED SSD1306**
- **Detecta abertura da tampa via sensor**
- **Simula a detecção de objetos por câmera**
- **Controla motores para direcionar o lixo**

## Componentes Utilizados
- **Microcontrolador:** Raspberry Pi Pico
- **Display:** OLED 128x64 SSD1306 (I2C, GPIO 14 e 15)
- **LEDs RGB:** WS2812
- **Sensor de tampa:** Entrada ADC
- **Motores:** Servo para direcionamento do lixo
- **Simulação de câmera** para classificação do lixo

## Estrutura do Código
O código principal está em `trash.c`, e suas principais partes são:

- **Configuração de periféricos** (OLED, LEDs, motores, sensor de tampa)
- **Loop principal**:
  - Exibe mensagens no OLED
  - Aguarda interação do usuário
  - Lê o sensor da tampa
  - Simula a detecção do objeto
  - Move o motor conforme a classificação do lixo

## Como Usar
1. **Monte o hardware** conforme o diagrama e conecte os sensores, LEDs e motores ao Raspberry Pi Pico.
2. **Compile e carregue o firmware** no Raspberry Pi Pico usando o VSCode com a extensão Pico SDK.
3. **Ligue o sistema** e siga as instruções no display OLED.
4. **Coloque o lixo na entrada**, aguarde a detecção e veja a separação automática ocorrer.

## Melhorias Futuras
- Implementação de um modelo real de IA para detecção de objetos
- Integração com Wi-Fi ou Bluetooth para monitoramento remoto
- Uso de sensores reais para identificação automática de materiais

## Licença
Este projeto é de código aberto e pode ser modificado livremente para fins educacionais e de pesquisa.

