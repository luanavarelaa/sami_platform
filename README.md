# S.A.M.I. - Sistema Automatizado de Medicação Inteligente 💊🌐

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![ESP8266](https://img.shields.io/badge/ESP8266-000000?style=for-the-badge&logo=espressif&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Ubidots-00BFFF?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Protótipo_Finalizado-brightgreen?style=for-the-badge)

## 📌 Sobre o Projeto

O **S.A.M.I.** é um dispositivo assistivo de Internet das Coisas (IoT) desenvolvido para mitigar o esquecimento e otimizar a administração de medicamentos de uso contínuo. Focado em engenharia assistiva, o sistema gere horários, emite alertas visuais locais e sincroniza dados em tempo real com a nuvem (Ubidots), enviando notificações para o telemóvel do paciente ou cuidador.

Este projeto foi desenvolvido como trabalho final da disciplina de Saúde 4.0 do curso de Engenharia Biomédica (PUC-Campinas).

## ✨ Principais Funcionalidades

* **Monitorização em Tempo Real:** Sincronização de horário global via NTP.
* **Tolerância a Falhas (Offline Buffer):** Se a ligação Wi-Fi cair, os eventos de abertura das gavetas são armazenados num *Buffer* Circular na memória local e descarregados automaticamente assim que a rede é restabelecida.
* **Controlo Remoto Bilateral:** Leitura periódica da nuvem para atualizar os horários das doses remotamente.
* **Alertas Visuais Locais:**
    * 🟢 **Verde:** Horário exato da medicação.
    * 🔴 **Vermelho:** Atraso/Falha na administração.
    * 🟡 **Amarelo:** Estado do sistema e ligação Wi-Fi.

## 🛠️ Arquitetura de Software

O código foi estruturado em C/C++ adotando o padrão de **Hardware Abstraction Layer (HAL)** e separação de responsabilidades para garantir um sistema não-bloqueante (sem uso de `delay()`):

1.  **Driver Layer (`/driver`):** Interação direta com os registos físicos do microcontrolador (GPIOs e Modem Wi-Fi).
2.  **Control Layer (`/control`):** Middleware que implementa lógicas de periféricos, como *debounce* de botões por software, controlo cruzado de LEDs, requisições HTTP (GET/POST) e ligação NTP.
3.  **Application Layer (`/app`):** Lógica de negócios. O `app_monitor` avalia o agendamento, o `app_comm` gere a fila de dados para a nuvem, e o `app_sami` orquestra a execução de forma assíncrona.

## 🧰 Hardware Utilizado

* Microcontrolador: NodeMCU ESP8266
* Sensores: Chaves de Fim de Curso (Microswitches)
* Atuadores: LEDs de alto brilho (Verde, Vermelho, Amarelo)
* Componentes passivos: Resistores limitadores e placa perfurada.

## 🚀 Como Executar o Projeto

### Pré-requisitos
* [Arduino IDE](https://www.arduino.cc/en/software) ou [PlatformIO](https://platformio.org/)
* Conta ativa na plataforma [Ubidots](https://ubidots.com/) (Stem/Educacional)
* Bibliotecas necessárias (via Library Manager):
    * `ESP8266WiFi`
    * `ESP8266HTTPClient`

### Configuração

**Passo 1:** Clona este repositório:
```bash
git clone [https://github.com/teu-utilizador/SAMI-IoT.git](https://github.com/teu-utilizador/SAMI-IoT.git)
