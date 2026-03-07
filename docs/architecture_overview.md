# Architecture Overview

## Overview
This document provides a high-level architectural overview of the SIP Intercom Device Simulator MVP.

Its purpose is to describe the main system building blocks, their responsibilities, their interactions, and the overall runtime organization of the simulator.

The architecture is intentionally modular. The system is designed as a Linux-based C++ application composed of multiple cooperating components responsible for signaling, state management, media emulation, external control, event delivery, logging, and diagnostics.

---

## 1. Architectural Goals

The architecture is designed to satisfy the following goals:

- clear separation of responsibilities between components
- maintainable and extensible code structure
- realistic simulation of intercom device communication behavior
- support for diagnostics and troubleshooting
- safe coordination between concurrently running parts of the system
- suitability for incremental extension after MVP

---

## 2. High-Level System View

At a high level, the SIP Intercom Device Simulator consists of:

- a central application core
- internal state and session management
- signaling input/output handling
- RTP stream emulation
- external control interfaces
- real-time event delivery
- structured logging and diagnostics

The simulator runs as a single Linux process containing multiple cooperating modules.  
Some modules may run in separate execution threads depending on implementation needs.

---

## 3. Main Components

### 3.1 Application Core
Responsible for:
- process startup
- configuration loading coordination
- module initialization order
- lifecycle management
- graceful shutdown coordination

The Application Core acts as the top-level orchestrator of the simulator.

### 3.2 Configuration Manager
Responsible for:
- loading external configuration
- validating required configuration fields
- exposing configuration values to other modules

This module centralizes runtime settings such as ports, addresses, and feature flags.

### 3.3 Logging Module
Responsible for:
- structured log generation
- log formatting
- severity level handling
- writing diagnostic information during runtime

The logging subsystem is shared by all major components.

### 3.4 Device State Machine
Responsible for:
- representing the current device state
- validating state transitions
- controlling state-dependent behavior

Expected MVP states include:
- `Idle`
- `Ringing`
- `ActiveSession`
- `Error`

This component is one of the central logic holders of the simulator.

### 3.5 Session Manager
Responsible for:
- tracking the currently active session
- storing session-related metadata
- coordinating session lifecycle events
- connecting signaling events to RTP activity

The Session Manager provides a structured place for call/session context.

### 3.6 SIP Signaling Module
Responsible for:
- receiving SIP messages
- parsing and validating supported SIP messages
- generating SIP responses
- triggering state transitions and session operations

This module implements the MVP signaling behavior needed for session setup and termination.

### 3.7 RTP Stream Module
Responsible for:
- preparing RTP stream parameters
- generating RTP test packets
- starting and stopping RTP transmission
- exposing RTP-related counters or status

This module is activated only when session state allows RTP transmission.

### 3.8 HTTP API Module
Responsible for:
- exposing control endpoints
- handling incoming HTTP requests
- validating request payloads
- triggering internal actions such as door opening
- returning structured responses

This module provides external control access to the simulator.

### 3.9 WebSocket Event Module
Responsible for:
- accepting WebSocket client connections
- serializing internal events
- delivering selected events to connected clients
- handling disconnects and delivery errors

This module provides real-time monitoring capabilities.

### 3.10 Internal Event Dispatcher
Responsible for:
- distributing internal events between modules
- decoupling event producers from event consumers
- forwarding key device and session events to monitoring/output modules

This component helps reduce direct coupling between modules.

### 3.11 Diagnostics Support
Responsible for:
- exposing useful runtime observability points
- supporting packet-level analysis through external capture tools
- providing internal counters, error signals, and troubleshooting visibility

This is a cross-cutting support area rather than a single isolated business module.

---

## 4. Component Interactions

The expected MVP interaction model is as follows:

1. The Application Core starts the simulator.
2. The Configuration Manager loads and validates configuration.
3. The Logging Module is initialized.
4. Core modules are created and connected.
5. The SIP module listens for signaling traffic.
6. On incoming SIP activity, the SIP module parses messages and consults the Device State Machine and Session Manager.
7. If session establishment succeeds, the RTP module is instructed to start RTP transmission.
8. Internal state changes and control actions generate events.
9. The Internal Event Dispatcher forwards events to the Logging Module and WebSocket Event Module.
10. HTTP requests may trigger additional internal actions, such as door opening or status retrieval.
11. On session termination or system shutdown, modules release resources in a controlled order.

---

## 5. External Interfaces

The MVP system exposes the following external interfaces:

### 5.1 SIP Interface
Used for session signaling between the simulator and an external SIP peer.

### 5.2 RTP Interface
Used for RTP packet transmission during an active session.

### 5.3 HTTP API
Used for control and status operations.

### 5.4 WebSocket Interface
Used for real-time event monitoring by external clients.

### 5.5 Logs
Used for human-readable and machine-useful diagnostics.

### 5.6 Network Capture Visibility
Used indirectly through tools such as tcpdump and Wireshark.

---

## 6. Concurrency Model

The exact threading model may evolve during implementation, but the MVP architecture assumes that some parts of the system may operate concurrently.

Possible concurrent areas include:
- SIP message handling
- RTP transmission
- HTTP request handling
- WebSocket event delivery

Shared state must be protected and coordinated carefully, especially:
- current device state
- active session metadata
- internal event distribution
- shutdown signaling

The architecture should favor controlled ownership and predictable synchronization over ad hoc shared access.

---

## 7. Architectural Style

The MVP architecture can be described as:

- modular
- event-driven in selected internal interactions
- single-process
- network-oriented
- diagnostic-friendly

It is not intended to be a distributed system or microservice architecture.  
The focus is on clarity, maintainability, and realistic communication flow simulation.

---

## 8. Expected Evolution After MVP

After the MVP, the architecture should support extension in the following areas:

- additional SIP methods and flows
- multiple simultaneous sessions
- more advanced RTP behavior
- richer diagnostics and metrics
- API authentication
- fault injection and network impairment simulation
- compatibility testing with external SIP tools or PBX systems

This is one of the reasons the architecture is modular from the beginning.

---

## Summary
The SIP Intercom Device Simulator MVP is designed as a modular Linux-based C++ application with clearly separated responsibilities for configuration, signaling, session management, RTP emulation, control APIs, event delivery, logging, and diagnostics.

This architecture is intended to keep the first version focused and achievable while still being technically meaningful and extensible.

# Огляд архітектури

## Загальний опис
Цей документ надає високорівневий огляд архітектури MVP-версії SIP Intercom Device Simulator.

Його мета — описати основні будівельні блоки системи, їхню відповідальність, взаємодію між ними та загальну організацію роботи симулятора під час виконання.

Архітектура свідомо робиться модульною. Система проєктується як C++-застосунок для Linux, що складається з кількох взаємодіючих компонентів, відповідальних за сигналізацію, керування станами, емуляцію медіапотоку, зовнішнє керування, доставку подій, логування та діагностику.

---

## 1. Цілі архітектури

Архітектура проєктується для досягнення таких цілей:

- чітке розділення відповідальностей між компонентами
- підтримувана та розширювана структура коду
- реалістична симуляція комунікаційної поведінки домофонного пристрою
- підтримка діагностики та налагодження
- безпечна координація між частинами системи, що працюють паралельно
- придатність до поетапного розширення після MVP

---

## 2. Високорівневе бачення системи

На високому рівні SIP Intercom Device Simulator складається з:

- центрального ядра застосунку
- внутрішнього керування станами та сесіями
- обробки сигнального вводу/виводу
- емуляції RTP-потоку
- зовнішніх інтерфейсів керування
- доставки подій у реальному часі
- структурованого логування та діагностики

Симулятор працює як один Linux-процес, що містить кілька взаємодіючих модулів.  
Деякі модулі можуть працювати в окремих потоках виконання залежно від потреб реалізації.

---

## 3. Основні компоненти

### 3.1 Application Core
Відповідає за:
- запуск процесу
- координацію завантаження конфігурації
- порядок ініціалізації модулів
- керування життєвим циклом
- координацію коректного завершення роботи

Application Core виступає верхньорівневим оркестратором симулятора.

### 3.2 Configuration Manager
Відповідає за:
- завантаження зовнішньої конфігурації
- валідацію обов’язкових полів конфігурації
- надання конфігураційних значень іншим модулям

Цей модуль централізує runtime-параметри, такі як порти, адреси та feature flags.

### 3.3 Logging Module
Відповідає за:
- генерацію структурованих логів
- форматування логів
- обробку рівнів важливості
- запис діагностичної інформації під час виконання

Підсистема логування є спільною для всіх основних компонентів.

### 3.4 Device State Machine
Відповідає за:
- представлення поточного стану пристрою
- перевірку допустимості переходів між станами
- керування поведінкою, що залежить від стану

Очікувані стани для MVP:
- `Idle`
- `Ringing`
- `ActiveSession`
- `Error`

Цей компонент є одним із центральних носіїв логіки симулятора.

### 3.5 Session Manager
Відповідає за:
- відстеження поточної активної сесії
- збереження метаданих, пов’язаних із сесією
- координацію подій життєвого циклу сесії
- зв’язування сигнальних подій із RTP-активністю

Session Manager надає структуроване місце для зберігання контексту виклику/сесії.

### 3.6 SIP Signaling Module
Відповідає за:
- прийом SIP-повідомлень
- розбір і валідацію підтримуваних SIP-повідомлень
- генерацію SIP-відповідей
- ініціювання переходів станів і операцій над сесіями

Цей модуль реалізує MVP-поведінку сигналізації, потрібну для встановлення та завершення сесії.

### 3.7 RTP Stream Module
Відповідає за:
- підготовку параметрів RTP-потоку
- генерацію тестових RTP-пакетів
- запуск і зупинку RTP-передачі
- надання RTP-лічильників або стану

Цей модуль активується лише тоді, коли стан сесії дозволяє RTP-передачу.

### 3.8 HTTP API Module
Відповідає за:
- надання керуючих endpoint-ів
- обробку вхідних HTTP-запитів
- валідацію payload
- запуск внутрішніх дій, таких як відкриття дверей
- повернення структурованих відповідей

Цей модуль надає зовнішній доступ до керування симулятором.

### 3.9 WebSocket Event Module
Відповідає за:
- прийом підключень WebSocket-клієнтів
- серіалізацію внутрішніх подій
- доставку вибраних подій підключеним клієнтам
- обробку відключень і помилок доставки

Цей модуль забезпечує можливість моніторингу в реальному часі.

### 3.10 Internal Event Dispatcher
Відповідає за:
- розподіл внутрішніх подій між модулями
- зменшення прямої зв’язаності між продуцентами та споживачами подій
- пересилання ключових подій пристрою і сесій до модулів моніторингу/виводу

Цей компонент допомагає зменшити прямі залежності між модулями.

### 3.11 Diagnostics Support
Відповідає за:
- надання корисних точок спостереження під час виконання
- підтримку аналізу на рівні пакетів через зовнішні інструменти захоплення трафіку
- надання внутрішніх лічильників, сигналів помилок і видимості для налагодження

Це швидше наскрізна зона підтримки, а не окремий ізольований бізнес-модуль.

---

## 4. Взаємодія компонентів

Очікувана модель взаємодії для MVP така:

1. Application Core запускає симулятор.
2. Configuration Manager завантажує та перевіряє конфігурацію.
3. Ініціалізується Logging Module.
4. Створюються й з’єднуються основні модулі.
5. SIP module починає слухати сигнальний трафік.
6. При надходженні SIP-подій SIP module розбирає повідомлення та взаємодіє з Device State Machine і Session Manager.
7. Якщо встановлення сесії успішне, RTP module отримує команду почати RTP-передачу.
8. Внутрішні зміни стану й керуючі дії генерують події.
9. Internal Event Dispatcher пересилає події до Logging Module і WebSocket Event Module.
10. HTTP-запити можуть ініціювати додаткові внутрішні дії, наприклад відкриття дверей або отримання статусу.
11. Під час завершення сесії або вимкнення системи модулі звільняють ресурси у контрольованому порядку.

---

## 5. Зовнішні інтерфейси

MVP-система надає такі зовнішні інтерфейси:

### 5.1 SIP Interface
Використовується для сигнального встановлення сесії між симулятором і зовнішнім SIP-peer.

### 5.2 RTP Interface
Використовується для передачі RTP-пакетів під час активної сесії.

### 5.3 HTTP API
Використовується для керування та отримання статусу.

### 5.4 WebSocket Interface
Використовується для моніторингу подій у реальному часі зовнішніми клієнтами.

### 5.5 Logs
Використовуються для людиночитної та машинно-корисної діагностики.

### 5.6 Network Capture Visibility
Використовується опосередковано через інструменти на кшталт tcpdump і Wireshark.

---

## 6. Модель конкурентності

Точна модель потоків може уточнюватися під час реалізації, але MVP-архітектура передбачає, що деякі частини системи можуть працювати паралельно.

Можливі конкурентні області:
- обробка SIP-повідомлень
- RTP-передача
- обробка HTTP-запитів
- доставка WebSocket-подій

Спільний стан має бути захищений і скоординований особливо для:
- поточного стану пристрою
- метаданих активної сесії
- розподілу внутрішніх подій
- сигналізації завершення роботи

Архітектура повинна віддавати перевагу контрольованому володінню станом і передбачуваній синхронізації, а не хаотичному спільному доступу.

---

## 7. Архітектурний стиль

MVP-архітектуру можна описати як:

- модульну
- частково event-driven у внутрішніх взаємодіях
- single-process
- network-oriented
- diagnostic-friendly

Вона не призначена бути розподіленою системою або мікросервісною архітектурою.  
Фокус — на ясності, підтримуваності та реалістичній симуляції комунікаційних потоків.

---

## 8. Очікувана еволюція після MVP

Після MVP архітектура повинна підтримувати розширення в таких напрямах:

- додаткові SIP-методи та сценарії
- кілька одночасних сесій
- більш просунута RTP-поведінка
- багатша діагностика та метрики
- автентифікація API
- fault injection і симуляція погіршення мережі
- перевірка сумісності зі сторонніми SIP-інструментами або PBX-системами

Це одна з причин, чому архітектура з самого початку робиться модульною.

---

## Підсумок
MVP-версія SIP Intercom Device Simulator проєктується як модульний C++-застосунок для Linux з чітко розділеними відповідальностями за конфігурацію, сигналізацію, керування сесіями, RTP-емуляцію, API керування, доставку подій, логування та діагностику.

Ця архітектура покликана зробити першу версію сфокусованою та досяжною, але водночас технічно змістовною і придатною до розширення.