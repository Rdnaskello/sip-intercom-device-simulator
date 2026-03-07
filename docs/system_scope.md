# System Scope

## Overview
This document defines the boundaries of the SIP Intercom Device Simulator MVP.

Its purpose is to clearly state what the system is expected to include in the first meaningful version, what is intentionally excluded, and which future directions remain outside the current implementation scope.

---

## 1. In Scope for MVP

The MVP version of the SIP Intercom Device Simulator includes the following areas:

### 1.1 Core Application
- Linux-based standalone simulator process
- External configuration loading
- Core module initialization and controlled shutdown
- Internal device lifecycle handling

### 1.2 Device State Management
- Internal state machine for device behavior
- Support for at least the following states:
  - `Idle`
  - `Ringing`
  - `ActiveSession`
  - `Error` or equivalent safe error state

### 1.3 SIP Signaling
- Basic SIP listener
- Basic parsing and validation of SIP messages required for MVP flows
- Support for the minimum signaling scenario needed for session setup and termination
- Generation of corresponding SIP responses
- Logging of signaling events and failures

### 1.4 RTP Stream Emulation
- Test RTP packet generation
- RTP stream startup after successful session establishment
- RTP stream shutdown after session termination
- Basic RTP-related logging and counters

### 1.5 HTTP Control API
- HTTP endpoint for retrieving current device state
- HTTP endpoint for at least one control action, such as door opening
- Basic request validation and response generation
- Logging of HTTP control actions

### 1.6 WebSocket Monitoring
- WebSocket endpoint for real-time device events
- Delivery of selected internal events to connected clients
- Logging of connection and delivery issues where appropriate

### 1.7 Logging and Diagnostics
- Structured logging for startup, shutdown, signaling, RTP, API actions, state transitions, and errors
- Support for troubleshooting through logs
- Network behavior observable with external tools such as tcpdump and Wireshark

### 1.8 Multithreaded Execution
- Use of multiple execution threads where needed
- Safe coordination of shared state between modules
- Controlled shutdown without thread hangs or corrupted internal state

### 1.9 Documentation and Engineering Structure
- Repository documentation describing project purpose, scope, and architecture
- Structured project layout suitable for further development
- Initial engineering-oriented documentation for MVP

---

## 2. Out of Scope for MVP

The following items are intentionally excluded from the MVP version:

### 2.1 Full SIP Stack Completeness
- Full SIP RFC coverage
- Advanced SIP edge-case handling
- Broad interoperability guarantees with all third-party SIP systems

### 2.2 Real Audio Handling
- Audio capture from microphone
- Audio playback through speaker
- Real codec processing
- Full media pipeline implementation

### 2.3 Production-Grade Security
- Advanced authentication and authorization
- TLS/SRTP support
- Full hardening against malicious input
- Production-level security policies

### 2.4 Hardware Integration
- Real relay control
- Camera integration
- Physical sensors
- Direct access-control hardware interfaces

### 2.5 Graphical User Interface
- Desktop GUI
- Web front-end UI
- User-facing dashboard

### 2.6 Enterprise Deployment Features
- High-availability deployment
- Distributed scaling
- Centralized fleet management
- Persistent telemetry backend
- Production monitoring stack

### 2.7 Broad Multi-Session Scalability
- High concurrency optimization
- Large-scale stress handling
- Full performance tuning for production traffic patterns

---

## 3. Deferred / Future Scope

The following areas may be considered after the MVP is completed:

- broader SIP method coverage
- more complete session handling logic
- richer RTP behavior and timing control
- multiple concurrent sessions
- fault injection and network impairment simulation
- authentication for API endpoints
- metrics and observability extensions
- test automation expansion
- compatibility experiments with external SIP tools or PBX platforms

---

## 4. Scope Rationale

The scope is intentionally limited to ensure that the first version of the project is:
- technically meaningful
- architecturally coherent
- demonstrable end-to-end
- feasible to implement without uncontrolled complexity growth

The MVP is not intended to be a production-ready intercom platform.  
Its purpose is to serve as a realistic engineering simulator focused on communication logic, diagnostics, modular design, and demonstrable technical depth.

---

## Summary
The MVP scope of the SIP Intercom Device Simulator includes the core communication, control, monitoring, logging, and lifecycle features needed to demonstrate a realistic intercom-device simulation.

Everything outside that core is intentionally excluded or postponed in order to keep the first version focused, coherent, and achievable.

# Межі системи

## Загальний опис
Цей документ визначає межі MVP-версії SIP Intercom Device Simulator.

Його мета — чітко зафіксувати, що саме повинно входити до першої змістовної версії системи, що свідомо виключається, а які напрями розвитку залишаються поза межами поточної реалізації.

---

## 1. Що входить у MVP

MVP-версія SIP Intercom Device Simulator включає такі області:

### 1.1 Основний застосунок
- автономний процес симулятора для Linux
- завантаження зовнішньої конфігурації
- ініціалізація основних модулів і контрольоване завершення роботи
- внутрішня обробка життєвого циклу пристрою

### 1.2 Керування станами пристрою
- внутрішній автомат станів пристрою
- підтримка щонайменше таких станів:
  - `Idle`
  - `Ringing`
  - `ActiveSession`
  - `Error` або еквівалентний безпечний стан помилки

### 1.3 SIP-сигналізація
- базовий SIP listener
- базовий розбір і валідація SIP-повідомлень, потрібних для MVP-сценаріїв
- підтримка мінімального сигнального сценарію для встановлення і завершення сесії
- генерація відповідних SIP-відповідей
- логування сигнальних подій і збоїв

### 1.4 Емуляція RTP-потоку
- генерація тестових RTP-пакетів
- запуск RTP-потоку після успішного встановлення сесії
- зупинка RTP-потоку після завершення сесії
- базове логування RTP-активності та лічильники

### 1.5 HTTP API для керування
- HTTP endpoint для отримання поточного стану пристрою
- HTTP endpoint щонайменше для однієї дії керування, наприклад відкриття дверей
- базова валідація запитів і формування відповідей
- логування HTTP-керуючих дій

### 1.6 WebSocket-моніторинг
- WebSocket endpoint для подій пристрою в реальному часі
- доставка вибраних внутрішніх подій підключеним клієнтам
- логування проблем із підключенням і доставкою там, де це доречно

### 1.7 Логування та діагностика
- структуроване логування запуску, зупинки, сигналізації, RTP, API-дій, змін стану та помилок
- підтримка діагностики через логи
- мережева поведінка, придатна для аналізу зовнішніми інструментами, такими як tcpdump і Wireshark

### 1.8 Багатопотокове виконання
- використання кількох потоків виконання там, де це потрібно
- безпечна координація спільного стану між модулями
- контрольоване завершення роботи без зависання потоків і пошкодження внутрішнього стану

### 1.9 Документація та інженерна структура
- документація в репозиторії, що описує призначення, межі та архітектуру проєкту
- структурована організація проєкту, придатна для подальшої розробки
- початкова інженерна документація для MVP

---

## 2. Що не входить у MVP

Наступні елементи свідомо виключаються з MVP-версії:

### 2.1 Повноцінний SIP stack
- повне покриття SIP RFC
- розширена обробка edge-case сценаріїв SIP
- широкі гарантії сумісності з усіма сторонніми SIP-системами

### 2.2 Реальна робота з аудіо
- захоплення аудіо з мікрофона
- відтворення аудіо через динамік
- реальна обробка кодеків
- повноцінна реалізація медіа-пайплайну

### 2.3 Production-рівень безпеки
- розширена автентифікація та авторизація
- підтримка TLS/SRTP
- повне hardening-захист від шкідливого вводу
- production-рівень security-політик

### 2.4 Інтеграція з обладнанням
- реальне керування реле
- інтеграція з камерою
- фізичні сенсори
- прямі апаратні інтерфейси системи доступу

### 2.5 Графічний інтерфейс користувача
- desktop GUI
- web front-end
- користувацька панель керування

### 2.6 Enterprise-функції розгортання
- high-availability розгортання
- розподілене масштабування
- централізоване керування парком пристроїв
- backend для постійної телеметрії
- production monitoring stack

### 2.7 Широка масштабованість по кількості сесій
- оптимізація під високу конкуренцію
- обробка великого навантаження
- повне performance tuning під production-трафік

---

## 3. Відкладений / майбутній scope

Наступні напрями можуть розглядатися після завершення MVP:

- ширше покриття SIP-методів
- повніша логіка обробки сесій
- багатша поведінка RTP та контроль таймінгів
- кілька одночасних сесій
- fault injection і симуляція погіршення мережі
- автентифікація для API endpoint-ів
- розширення метрик та observability
- розширення автоматизації тестування
- експерименти із сумісністю зі сторонніми SIP-інструментами або PBX-платформами

---

## 4. Обґрунтування меж

Межі свідомо обмежені, щоб перша версія проєкту була:
- технічно змістовною
- архітектурно цілісною
- придатною для end-to-end демонстрації
- реальною для реалізації без неконтрольованого зростання складності

MVP не призначений бути production-ready платформою домофона.  
Його мета — бути реалістичним інженерним симулятором, сфокусованим на логіці комунікації, діагностиці, модульному дизайні та демонстрації технічної глибини.

---

## Підсумок
MVP-обсяг SIP Intercom Device Simulator включає основні функції комунікації, керування, моніторингу, логування та життєвого циклу, необхідні для демонстрації реалістичної симуляції домофонного пристрою.

Усе, що виходить за межі цього ядра, свідомо виключається або відкладається, щоб перша версія залишалася сфокусованою, цілісною та досяжною.