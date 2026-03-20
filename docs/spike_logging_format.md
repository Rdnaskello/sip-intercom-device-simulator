# Spike: Logging Format

## Overview
This document defines the initial logging format for the MVP version of the SIP Intercom Device Simulator.

The purpose of this spike is to determine what information logs must contain, which events must always be logged, and how log records should be structured so that the simulator remains diagnosable during development and demonstration.

The MVP does not require an advanced logging platform.  
It requires logs that are consistent, readable, and technically useful.

---

## 1. Goals of Logging in MVP

The logging system should:
- make runtime behavior observable
- help diagnose signaling and session problems
- make state transitions traceable
- help correlate RTP and signaling activity
- support troubleshooting during manual testing
- remain simple enough to implement early

---

## 2. Logging Principles

The MVP logging approach should follow these principles:

- every important runtime action should leave a trace
- logs should be structured consistently
- logs should include enough context to understand what happened
- logs should be readable by a human during debugging
- logs should be stable enough to support future automated analysis

The goal is not verbose logging of everything, but reliable logging of meaningful events.

---

## 3. Minimum Log Record Structure

Each important log record should contain at least:

- timestamp
- log level
- module or source
- message text

Recommended optional fields:
- session identifier or `Call-ID`
- device state
- event category

A practical MVP log shape may look like:

`2026-03-20T12:34:56Z INFO SIP Received INVITE Call-ID=abc123 State=Idle`

---

## 4. Recommended Log Levels

The MVP should at minimum support the following levels:

### `DEBUG`
Detailed development-oriented information.

Examples:
- parsed message details
- internal event dispatch details
- RTP packet counters

### `INFO`
Normal important runtime events.

Examples:
- application startup
- state transitions
- session establishment
- RTP start/stop
- HTTP actions
- WebSocket client connect/disconnect

### `WARN`
Unexpected but non-fatal conditions.

Examples:
- invalid request format
- incomplete session flow
- ignored unsupported message
- recoverable delivery issue

### `ERROR`
Failures that prevent correct execution of an operation.

Examples:
- parser failure for a required path
- failed module initialization
- unrecoverable session handling error
- worker failure that affects runtime behavior

---

## 5. Events That Must Be Logged in MVP

The following events should always be logged:

### Application Lifecycle
- startup begin
- configuration loaded
- module initialization success/failure
- shutdown begin
- shutdown complete

### SIP Signaling
- receipt of supported SIP messages
- sending of SIP responses
- unsupported or invalid SIP input
- parse failures
- session establishment progress
- session termination progress

### State Machine
- all state transitions
- rejected transition attempts
- entry into `Error` state

### RTP
- RTP stream start
- RTP stream stop
- RTP initialization failure
- optionally periodic RTP counters at debug level

### HTTP/API
- receipt of control requests
- request validation failures
- successful door-open action
- status request handling

### WebSocket
- client connection
- client disconnection
- event delivery failure if relevant

### Internal Errors
- unexpected runtime exceptions or critical failures
- shutdown-related failures
- synchronization-related failures if detected

---

## 6. Context Fields That Are Especially Valuable

The most useful extra context fields for MVP are:

- `Call-ID`
- current state
- target module
- endpoint or client information for HTTP/WebSocket
- error reason
- transition trigger

These context fields should be included where relevant, not necessarily in every single message.

---

## 7. Logging Output Style

For MVP, the preferred style is:

- single-line log records
- stable ordering of key fields
- readable plain text format
- no unnecessary multiline verbosity for common runtime events

A structured plain-text format is preferable for the first version because:
- it is easy to read in terminal output
- it is easy to redirect to file
- it can later evolve toward machine-parsable structure if needed

---

## 8. What Logging Should Avoid in MVP

The logging design should avoid:
- inconsistent formatting between modules
- missing timestamps
- messages without module/source attribution
- overuse of `ERROR` for normal invalid input cases
- excessive packet-by-packet verbosity at default log levels
- hidden failures that change state without log evidence

The simulator should be diagnosable from logs without drowning in noise.

---

## 9. Relationship Between Logs and Diagnostics

Logs are one of the main diagnostic tools of the MVP and should complement:
- packet captures
- observed state changes
- HTTP/API responses
- WebSocket events

A developer should be able to correlate:
- SIP signaling seen in Wireshark
- state changes inside the simulator
- RTP activation
- API-triggered actions
through logs.

---

## 10. Recommended Next Design Outcome

Based on this spike, the implementation should next define:
- logging API shape
- where logging responsibility lives in each module
- default log level policy
- whether logs go only to stdout or also to file
- how session context is attached to log records

These choices should be made before deep runtime implementation begins.

---

## Summary
The MVP logging format should be:

- consistent
- timestamped
- leveled
- module-aware
- context-rich where needed

The most important requirement is that logs must make signaling, state changes, RTP lifecycle, and external control actions easy to understand and troubleshoot.

# Spike: формат логування

## Загальний опис
Цей документ визначає початковий формат логування для MVP-версії SIP Intercom Device Simulator.

Мета цього spike — визначити, яку інформацію обов’язково мають містити логи, які події повинні логуватися завжди, і якою має бути структура лог-записів, щоб симулятор залишався придатним до діагностики під час розробки та демонстрації.

MVP не потребує складної logging-платформи.  
Йому потрібні логи, які є послідовними, читабельними та технічно корисними.

---

## 1. Цілі логування в MVP

Система логування повинна:
- робити runtime-поведінку спостережуваною
- допомагати діагностувати проблеми сигналізації та сесій
- робити transitions станів трасованими
- допомагати співвідносити RTP та signaling activity
- підтримувати troubleshooting під час ручного тестування
- залишатися достатньо простою для ранньої реалізації

---

## 2. Принципи логування

Підхід до логування в MVP повинен дотримуватися таких принципів:

- кожна важлива runtime-дія повинна залишати слід
- логи повинні мати узгоджену структуру
- логи повинні містити достатньо контексту, щоб зрозуміти, що сталося
- логи мають бути читабельними для людини під час debugging
- логи мають бути достатньо стабільними для можливої майбутньої автоматизованої обробки

Мета — не логувати все підряд, а надійно логувати змістовні події.

---

## 3. Мінімальна структура лог-запису

Кожен важливий лог-запис повинен містити щонайменше:

- timestamp
- log level
- module або source
- message text

Рекомендовані додаткові поля:
- session identifier або `Call-ID`
- стан пристрою
- категорія події

Практична MVP-форма запису може виглядати так:

`2026-03-20T12:34:56Z INFO SIP Received INVITE Call-ID=abc123 State=Idle`

---

## 4. Рекомендовані рівні логування

MVP повинен щонайменше підтримувати такі рівні:

### `DEBUG`
Детальна інформація для розробки.

Приклади:
- деталі parsed message
- деталі dispatch внутрішніх подій
- RTP packet counters

### `INFO`
Нормальні важливі runtime-події.

Приклади:
- запуск застосунку
- transitions станів
- встановлення сесії
- старт/зупинка RTP
- HTTP-дії
- connect/disconnect WebSocket-клієнтів

### `WARN`
Неочікувані, але нефатальні умови.

Приклади:
- невалідний формат request
- неповний session flow
- ігнорування непідтримуваного повідомлення
- recoverable проблема доставки

### `ERROR`
Збої, які заважають коректному виконанню операції.

Приклади:
- parser failure у критичному шляху
- невдала ініціалізація модуля
- невідновна помилка обробки сесії
- збій worker-а, що впливає на runtime-поведінку

---

## 5. Події, які обов’язково мають логуватися в MVP

Наступні події повинні логуватися завжди:

### Життєвий цикл застосунку
- початок запуску
- конфігурація завантажена
- успіх/збій ініціалізації модулів
- початок завершення роботи
- завершення роботи

### SIP Signaling
- отримання підтримуваних SIP-повідомлень
- надсилання SIP-відповідей
- непідтримуваний або невалідний SIP-ввід
- помилки парсингу
- прогрес встановлення сесії
- прогрес завершення сесії

### State Machine
- усі переходи станів
- спроби недозволених переходів
- вхід у `Error`

### RTP
- запуск RTP-потоку
- зупинка RTP-потоку
- помилка ініціалізації RTP
- за бажанням періодичні RTP-лічильники на debug-рівні

### HTTP/API
- отримання керуючих запитів
- помилки валідації запитів
- успішна дія відкриття дверей
- обробка status request

### WebSocket
- підключення клієнта
- відключення клієнта
- помилка доставки події, якщо релевантно

### Внутрішні помилки
- неочікувані runtime-exceptions або критичні збої
- помилки під час shutdown
- проблеми синхронізації, якщо вони виявлені

---

## 6. Контекстні поля, які особливо цінні

Найкорисніші додаткові контекстні поля для MVP:

- `Call-ID`
- поточний стан
- цільовий модуль
- endpoint або client information для HTTP/WebSocket
- причина помилки
- trigger переходу

Ці поля слід додавати там, де вони релевантні, а не обов’язково в кожен запис.

---

## 7. Стиль виводу логів

Для MVP бажаний такий стиль:

- однорядкові log records
- стабільний порядок ключових полів
- читабельний plain-text формат
- без зайвої multiline-verbosity для звичайних runtime-подій

Структурований plain-text формат кращий для першої версії, тому що:
- його легко читати в терміналі
- його легко перенаправити у файл
- його потім легко еволюціонувати в machine-parsable формат

---

## 8. Чого слід уникати в логуванні MVP

Дизайн логування повинен уникати:
- неузгодженого форматування між модулями
- відсутності timestamp
- повідомлень без module/source attribution
- надмірного використання `ERROR` для звичайних випадків невалідного вводу
- надто шумного packet-by-packet логування на default-рівнях
- прихованих збоїв, які змінюють стан без сліду в логах

Симулятор має бути придатним до діагностики через логи без потопання в шумі.

---

## 9. Зв’язок між логами та діагностикою

Логи — один із головних діагностичних інструментів MVP і повинні доповнювати:
- packet captures
- спостережувані transitions станів
- HTTP/API responses
- WebSocket events

Розробник повинен мати змогу співвіднести:
- SIP signaling у Wireshark
- transitions станів усередині симулятора
- активацію RTP
- API-triggered actions
через логи.

---

## 10. Рекомендований наступний результат дизайну

На основі цього spike реалізація повинна далі визначити:
- форму logging API
- де живе відповідальність за логування в кожному модулі
- політику default log level
- чи пишемо логи лише у stdout, чи також у файл
- як session context прикріплюється до log records

Ці рішення слід ухвалити до початку глибокої runtime-реалізації.

---

## Підсумок
MVP-формат логування повинен бути:

- послідовним
- з timestamp
- з log levels
- з module-awareness
- з додатковим контекстом там, де це потрібно

Найважливіша вимога — логи повинні робити signaling, transitions станів, життєвий цикл RTP і зовнішні керуючі дії легкими для розуміння та troubleshooting.