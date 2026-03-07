# Use Cases

## Overview
This document describes the primary system usage scenarios for the MVP version of the SIP Intercom Device Simulator.

The purpose of these use cases is to define how the simulator should behave in realistic communication and control scenarios. These scenarios will later be used as a basis for requirements, architecture, tests, and implementation planning.

---

## UC-01 Device Startup

### Goal
Initialize the simulator and bring the device into a ready state.

### Trigger
The application process is started by the user or runtime environment.

### Preconditions
- A valid configuration file is available.
- Required runtime dependencies are present.
- Required network ports are free.

### Main Flow
1. The application starts.
2. The configuration is loaded.
3. Logging is initialized.
4. Core modules are initialized.
5. Network listeners and internal services are prepared.
6. The device enters the `Idle` or `Ready` state.

### Error Flow
- If configuration loading fails, the application writes an error log and terminates.
- If required ports are unavailable, the application writes an error log and terminates.
- If a critical module fails to initialize, the application writes an error log and terminates.

### Expected Result
The simulator is running and ready to accept signaling, control, and monitoring interactions.

---

## UC-02 Receive Incoming SIP Session

### Goal
Accept an incoming SIP session initiation request and move the device into a ringing state.

### Trigger
The simulator receives a SIP `INVITE` request.

### Preconditions
- The application is running.
- The device is in the `Idle` or `Ready` state.
- SIP listener is active.

### Main Flow
1. The simulator receives a SIP `INVITE`.
2. The SIP message is parsed and validated.
3. The simulator logs the incoming request.
4. The device state changes to `Ringing`.
5. The simulator sends the appropriate SIP provisional response.

### Error Flow
- If the SIP message is malformed, the simulator logs the error and rejects the request.
- If the device is in a non-available state, the simulator logs the reason and rejects the request.

### Expected Result
The simulator recognizes the incoming session request and enters the ringing state.

---

## UC-03 Establish Active Session

### Goal
Complete the basic SIP call setup and transition the device into an active session state.

### Trigger
The simulator continues processing a valid SIP call setup flow after an incoming `INVITE`.

### Preconditions
- A valid SIP session initiation is in progress.
- The device is in the `Ringing` state.

### Main Flow
1. The simulator sends a final successful SIP response for session establishment.
2. The remote peer sends the expected confirmation message.
3. The simulator validates the signaling sequence.
4. The device state changes to `InCall` or `ActiveSession`.
5. Session metadata is stored internally.

### Error Flow
- If the signaling sequence is incomplete or invalid, the simulator logs the error and terminates the session setup.
- If a timeout occurs, the simulator logs the timeout and returns to `Idle`.

### Expected Result
The simulator enters the active session state and is ready to start RTP transmission.

---

## UC-04 Start RTP Stream

### Goal
Start RTP packet transmission when a session becomes active.

### Trigger
A SIP session is successfully established.

### Preconditions
- The device is in the `InCall` or `ActiveSession` state.
- RTP settings are available in configuration or session context.

### Main Flow
1. The simulator initializes RTP stream parameters.
2. RTP packet generation starts.
3. Packets are transmitted to the target endpoint.
4. RTP activity is logged.
5. Internal counters and statistics are updated.

### Error Flow
- If RTP initialization fails, the simulator logs the error and reports stream startup failure.
- If packet transmission fails, the simulator logs the issue and may stop the stream depending on policy.

### Expected Result
A valid RTP test stream is generated and can be observed in traffic captures or logs.

---

## UC-05 Door Open via HTTP API

### Goal
Allow an external client to trigger a door-open action through HTTP.

### Trigger
An external client sends an HTTP request to the configured control endpoint.

### Preconditions
- The simulator is running.
- HTTP API listener is active.
- The request is sent to a valid endpoint.

### Main Flow
1. The simulator receives an HTTP control request.
2. The request is validated.
3. The simulator triggers the internal door-open action.
4. The action is written to the log.
5. A response is returned to the HTTP client.
6. A device event is generated for internal monitoring and WebSocket delivery.

### Error Flow
- If the request is invalid, the simulator returns an error response and logs the issue.
- If the action cannot be performed, the simulator returns a failure response and logs the reason.

### Expected Result
The simulator performs the door-open action and reports the result through HTTP response and internal events.

---

## UC-06 Monitor Events via WebSocket

### Goal
Provide real-time device event notifications to connected clients.

### Trigger
A client connects to the WebSocket endpoint and the simulator generates events.

### Preconditions
- The simulator is running.
- WebSocket service is active.
- At least one client is connected.

### Main Flow
1. A client connects to the WebSocket endpoint.
2. The simulator registers the client session.
3. Internal device events occur, such as state changes or control actions.
4. The simulator serializes event data.
5. The event is sent to all relevant connected clients.
6. Event delivery is logged if required.

### Error Flow
- If a client disconnects unexpectedly, the simulator removes the client session and logs the disconnect.
- If event serialization or delivery fails, the simulator logs the error and continues operating.

### Expected Result
Connected clients receive real-time notifications about device activity.

---

## UC-07 Terminate Session

### Goal
Correctly terminate an active session and return the simulator to the idle state.

### Trigger
The simulator receives a SIP `BYE` request or session termination is triggered by timeout or internal condition.

### Preconditions
- An active SIP session exists.
- The device is in the `InCall` or `ActiveSession` state.

### Main Flow
1. The simulator receives or initiates session termination.
2. The signaling sequence for session termination is processed.
3. RTP transmission is stopped.
4. Session-related resources are released.
5. The device state returns to `Idle`.
6. The termination event is logged.

### Error Flow
- If the termination sequence is malformed, the simulator logs the error and attempts safe cleanup.
- If cleanup fails partially, the simulator logs the issue and moves the system into a safe state.

### Expected Result
The session is terminated cleanly and the simulator becomes ready for the next session.

---

## UC-08 Handle Invalid Message or Runtime Error

### Goal
Ensure the simulator behaves safely and predictably when invalid input or runtime errors occur.

### Trigger
The simulator receives malformed data or encounters an internal processing error.

### Preconditions
- The simulator is running.

### Main Flow
1. The simulator detects an invalid message, protocol violation, or runtime error.
2. The error is classified.
3. The error is written to the log.
4. The simulator rejects the invalid input or stops the failed operation.
5. If possible, the simulator keeps running in a safe state.

### Error Flow
- If the error is critical and recovery is not possible, the simulator logs the failure and terminates gracefully.

### Expected Result
The simulator either continues operating safely or shuts down in a controlled and diagnosable way.

---

## Summary
The MVP use cases define the minimum meaningful system behavior for the project:
- startup and initialization
- SIP session handling
- RTP stream emulation
- HTTP-based control
- WebSocket-based monitoring
- session termination
- error handling

These use cases form the basis for the next steps:
- functional requirements
- non-functional requirements
- architecture design
- testing strategy

# Варіанти використання

## Загальний опис
Цей документ описує основні сценарії використання системи для MVP-версії SIP Intercom Device Simulator.

Мета цих сценаріїв — визначити, як симулятор повинен поводитися в реалістичних сценаріях зв’язку та керування. Надалі ці сценарії будуть використовуватися як основа для формування вимог, архітектури, тестів і плану реалізації.

---

## UC-01 Запуск пристрою

### Мета
Ініціалізувати симулятор і перевести пристрій у стан готовності до роботи.

### Тригер
Процес застосунку запускається користувачем або середовищем виконання.

### Передумови
- Доступний коректний конфігураційний файл.
- Усі необхідні runtime-залежності присутні.
- Потрібні мережеві порти вільні.

### Основний потік
1. Застосунок запускається.
2. Завантажується конфігурація.
3. Ініціалізується система логування.
4. Ініціалізуються основні модулі.
5. Готуються мережеві слухачі та внутрішні сервіси.
6. Пристрій переходить у стан `Idle` або `Ready`.

### Потік помилок
- Якщо завантаження конфігурації завершується помилкою, застосунок записує помилку в лог і завершує роботу.
- Якщо потрібні порти недоступні, застосунок записує помилку в лог і завершує роботу.
- Якщо критичний модуль не вдалося ініціалізувати, застосунок записує помилку в лог і завершує роботу.

### Очікуваний результат
Симулятор працює і готовий приймати сигнальні, керуючі та моніторингові взаємодії.

---

## UC-02 Отримання вхідної SIP-сесії

### Мета
Прийняти вхідний запит на ініціацію SIP-сесії та перевести пристрій у стан виклику.

### Тригер
Симулятор отримує SIP-запит `INVITE`.

### Передумови
- Застосунок запущений.
- Пристрій перебуває у стані `Idle` або `Ready`.
- SIP listener активний.

### Основний потік
1. Симулятор отримує SIP `INVITE`.
2. SIP-повідомлення розбирається та перевіряється.
3. Симулятор записує вхідний запит у лог.
4. Стан пристрою змінюється на `Ringing`.
5. Симулятор надсилає відповідну проміжну SIP-відповідь.

### Потік помилок
- Якщо SIP-повідомлення некоректне, симулятор записує помилку в лог і відхиляє запит.
- Якщо пристрій перебуває в недоступному стані, симулятор записує причину в лог і відхиляє запит.

### Очікуваний результат
Симулятор розпізнає вхідний запит на сесію і переходить у стан виклику.

---

## UC-03 Встановлення активної сесії

### Мета
Завершити базове встановлення SIP-виклику та перевести пристрій у стан активної сесії.

### Тригер
Симулятор продовжує обробку коректного SIP-сценарію після отримання `INVITE`.

### Передумови
- Коректний процес ініціації SIP-сесії вже розпочато.
- Пристрій перебуває у стані `Ringing`.

### Основний потік
1. Симулятор надсилає фінальну успішну SIP-відповідь для встановлення сесії.
2. Віддалена сторона надсилає очікуване підтвердження.
3. Симулятор перевіряє послідовність сигналізації.
4. Стан пристрою змінюється на `InCall` або `ActiveSession`.
5. Метадані сесії зберігаються всередині системи.

### Потік помилок
- Якщо послідовність сигналізації неповна або некоректна, симулятор записує помилку в лог і припиняє встановлення сесії.
- Якщо виникає timeout, симулятор записує це в лог і повертається в стан `Idle`.

### Очікуваний результат
Симулятор переходить у стан активної сесії та готовий розпочати RTP-передачу.

---

## UC-04 Запуск RTP-потоку

### Мета
Розпочати передачу RTP-пакетів після успішного встановлення сесії.

### Тригер
SIP-сесія успішно встановлена.

### Передумови
- Пристрій перебуває у стані `InCall` або `ActiveSession`.
- RTP-параметри доступні в конфігурації або контексті сесії.

### Основний потік
1. Симулятор ініціалізує параметри RTP-потоку.
2. Запускається генерація RTP-пакетів.
3. Пакети передаються на цільову кінцеву точку.
4. RTP-активність записується в лог.
5. Внутрішні лічильники та статистика оновлюються.

### Потік помилок
- Якщо ініціалізація RTP не вдається, симулятор записує помилку в лог і повідомляє про збій запуску потоку.
- Якщо передача пакетів завершується помилкою, симулятор записує проблему в лог і може зупинити потік залежно від політики.

### Очікуваний результат
Коректний тестовий RTP-потік генерується і може спостерігатися в мережевих захопленнях або логах.

---

## UC-05 Відкриття дверей через HTTP API

### Мета
Дозволити зовнішньому клієнту ініціювати дію відкриття дверей через HTTP.

### Тригер
Зовнішній клієнт надсилає HTTP-запит до налаштованої керуючої точки доступу.

### Передумови
- Симулятор запущений.
- HTTP API listener активний.
- Запит надісланий на коректний endpoint.

### Основний потік
1. Симулятор отримує HTTP-запит керування.
2. Запит перевіряється.
3. Симулятор запускає внутрішню дію відкриття дверей.
4. Дія записується в лог.
5. HTTP-клієнту повертається відповідь.
6. Генерується подія пристрою для внутрішнього моніторингу та доставки через WebSocket.

### Потік помилок
- Якщо запит некоректний, симулятор повертає відповідь з помилкою і записує проблему в лог.
- Якщо дію неможливо виконати, симулятор повертає повідомлення про помилку і записує причину в лог.

### Очікуваний результат
Симулятор виконує дію відкриття дверей і повідомляє результат через HTTP-відповідь та внутрішні події.

---

## UC-06 Моніторинг подій через WebSocket

### Мета
Надавати підключеним клієнтам події пристрою в режимі реального часу.

### Тригер
Клієнт підключається до WebSocket endpoint, а симулятор генерує події.

### Передумови
- Симулятор запущений.
- WebSocket service активний.
- Підключений щонайменше один клієнт.

### Основний потік
1. Клієнт підключається до WebSocket endpoint.
2. Симулятор реєструє клієнтську сесію.
3. Усередині системи виникають події пристрою, наприклад зміни стану або керуючі дії.
4. Симулятор серіалізує дані події.
5. Подія надсилається всім відповідним підключеним клієнтам.
6. Доставка події логуються за потреби.

### Потік помилок
- Якщо клієнт неочікувано відключається, симулятор видаляє його сесію і записує це в лог.
- Якщо серіалізація або доставка події завершується помилкою, симулятор записує це в лог і продовжує роботу.

### Очікуваний результат
Підключені клієнти отримують сповіщення в реальному часі про активність пристрою.

---

## UC-07 Завершення сесії

### Мета
Коректно завершити активну сесію і повернути симулятор у стан очікування.

### Тригер
Симулятор отримує SIP-запит `BYE` або завершення сесії ініціюється через timeout чи внутрішню умову.

### Передумови
- Існує активна SIP-сесія.
- Пристрій перебуває у стані `InCall` або `ActiveSession`.

### Основний потік
1. Симулятор отримує або ініціює завершення сесії.
2. Обробляється сигнальна послідовність завершення сесії.
3. RTP-передача зупиняється.
4. Ресурси, пов’язані із сесією, звільняються.
5. Стан пристрою повертається до `Idle`.
6. Подія завершення записується в лог.

### Потік помилок
- Якщо послідовність завершення некоректна, симулятор записує помилку в лог і намагається виконати безпечне очищення.
- Якщо очищення частково не вдалося, симулятор записує проблему в лог і переводить систему в безпечний стан.

### Очікуваний результат
Сесія коректно завершена, а симулятор знову готовий до наступної сесії.

---

## UC-08 Обробка некоректного повідомлення або runtime-помилки

### Мета
Забезпечити безпечну та передбачувану поведінку симулятора у випадку некоректного вводу або внутрішніх помилок виконання.

### Тригер
Симулятор отримує пошкоджені дані або стикається з внутрішньою помилкою під час обробки.

### Передумови
- Симулятор запущений.

### Основний потік
1. Симулятор виявляє некоректне повідомлення, порушення протоколу або runtime-помилку.
2. Помилка класифікується.
3. Помилка записується в лог.
4. Симулятор відхиляє некоректний ввід або зупиняє невдалу операцію.
5. Якщо це можливо, симулятор продовжує працювати у безпечному стані.

### Потік помилок
- Якщо помилка є критичною і відновлення неможливе, симулятор записує збій у лог і завершує роботу контрольованим способом.

### Очікуваний результат
Симулятор або продовжує безпечно працювати, або завершується у контрольований спосіб з можливістю діагностики.

---

## Підсумок
Сценарії використання MVP визначають мінімально необхідну змістовну поведінку системи:
- запуск та ініціалізація
- обробка SIP-сесії
- емуляція RTP-потоку
- керування через HTTP
- моніторинг через WebSocket
- завершення сесії
- обробка помилок

Ці сценарії є основою для наступних кроків:
- функціональні вимоги
- нефункціональні вимоги
- проєктування архітектури
- стратегія тестування