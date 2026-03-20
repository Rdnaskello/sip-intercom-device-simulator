# Spike: Threading Model

## Overview
This document defines the initial threading model for the MVP version of the SIP Intercom Device Simulator.

The goal of this spike is to determine how concurrent activities should be organized in the simulator while keeping the design understandable, safe, and suitable for incremental implementation.

The MVP does not require a highly optimized concurrency model.  
It requires a predictable and maintainable one.

---

## 1. Purpose of the Threading Model

The threading model should:
- support concurrent network-related activities
- keep shared state under control
- make shutdown behavior predictable
- avoid unnecessary complexity in the first implementation
- provide a foundation for future extension

The model should be simple enough for MVP, but structured enough to avoid redesign after the first working version.

---

## 2. Main Concurrent Activities in MVP

The MVP simulator may need to handle the following concurrent activities:

- SIP message reception and processing
- RTP packet generation and transmission
- HTTP request handling
- WebSocket connection handling and event delivery
- logging output
- shutdown coordination

Not all of these must necessarily run in completely separate threads from the beginning, but they represent logically separate runtime activities.

---

## 3. Recommended MVP Threading Approach

The recommended MVP approach is a small multi-threaded design with clearly bounded responsibilities.

A reasonable starting point is:

### 3.1 Main Thread
Responsible for:
- process startup
- configuration loading
- module initialization
- wiring components together
- shutdown coordination

### 3.2 SIP Handling Thread
Responsible for:
- listening for SIP traffic
- receiving SIP messages
- passing validated signaling events into the internal logic

### 3.3 RTP Worker Thread
Responsible for:
- RTP timing loop
- packet generation
- packet transmission during active session
- RTP stop on session termination

### 3.4 HTTP/WebSocket Service Thread
Responsible for:
- handling HTTP requests
- maintaining WebSocket connections
- delivering event notifications

This may later be split if needed, but one service thread is sufficient for MVP planning.

### 3.5 Optional Logging Worker
For MVP, logging may initially remain synchronous.  
If later needed, a dedicated logging thread may be introduced.

---

## 4. Shared State Areas

The following shared state areas require careful ownership and synchronization:

### 4.1 Device State
Shared between:
- SIP handling
- session logic
- HTTP status access
- WebSocket event generation
- shutdown flow

### 4.2 Session Context
Shared between:
- SIP handling
- RTP worker
- logging
- diagnostics

### 4.3 Internal Event Delivery
Shared between:
- event producers
- WebSocket delivery
- logging

### 4.4 Shutdown Signals
Shared between:
- main thread
- SIP thread
- RTP thread
- HTTP/WebSocket thread

---

## 5. Ownership Principles

To keep the MVP safe and understandable, the threading model should follow these principles:

- shared state should have clear ownership
- cross-thread communication should be minimized
- direct unsynchronized access to shared state should be avoided
- state transitions should happen in a controlled place
- shutdown should be driven centrally

The design should prefer:
- explicit ownership
- guarded access
- simple coordination primitives
over ad hoc thread interactions.

---

## 6. Synchronization Strategy for MVP

The exact implementation may evolve, but the MVP should likely use only a small set of standard synchronization tools, such as:

- `std::mutex`
- `std::lock_guard`
- `std::unique_lock`
- `std::condition_variable`
- `std::atomic<bool>` for stop flags where appropriate

The MVP should avoid:
- over-engineered asynchronous frameworks
- too many lock layers
- implicit thread ownership rules
- complex actor-style systems unless later justified

---

## 7. Recommended Coordination Model

A practical coordination model for MVP is:

- SIP thread receives a message
- SIP thread performs parsing and initial validation
- SIP thread updates state/session in a controlled way
- SIP thread emits an internal event
- RTP thread reacts only when session state permits RTP
- HTTP/WebSocket thread reads state in a controlled way and publishes events
- main thread coordinates stop signals and joins worker threads

This keeps the signaling path central while allowing RTP and external services to remain concurrent.

---

## 8. Shutdown Behavior

Controlled shutdown is one of the most important reasons to define a threading model early.

The shutdown model should ensure:
- stop signal is propagated to all worker threads
- blocking loops can exit
- RTP transmission stops cleanly
- SIP listening stops cleanly
- HTTP/WebSocket service stops cleanly
- logs are flushed if needed
- worker threads are joined before process exit

The system should avoid:
- detached worker threads
- silent thread termination
- shutdown races over shared state

---

## 9. Why This Model Is Intentionally Small

The MVP threading model is intentionally limited because:
- the first goal is correctness, not high throughput
- only one canonical session flow is supported
- the simulator should remain understandable
- debugging concurrent behavior is already non-trivial even in small systems

Future versions may introduce:
- finer-grained service separation
- session-specific workers
- improved event queues
- dedicated diagnostics thread
- performance-oriented concurrency refinements

These are intentionally postponed.

---

## 10. Recommended Next Design Outcome

Based on this spike, the implementation should next define:
- which component owns device state
- which component owns session state
- whether internal events are delivered synchronously or through a queue
- the exact stop/exit contract for each worker thread

These decisions should be made before core runtime implementation begins.

---

## Summary
The recommended MVP threading model is a small, controlled multi-threaded design with:

- one main coordination thread
- one SIP handling thread
- one RTP worker thread
- one HTTP/WebSocket service thread

The design emphasizes predictability, controlled ownership, and clean shutdown over concurrency complexity.

# Spike: модель потоків

## Загальний опис
Цей документ визначає початкову модель потоків для MVP-версії SIP Intercom Device Simulator.

Мета цього spike — визначити, як слід організувати паралельні активності в симуляторі так, щоб дизайн залишався зрозумілим, безпечним і придатним до поетапної реалізації.

MVP не потребує високооптимізованої моделі конкурентності.  
Йому потрібна передбачувана й підтримувана модель.

---

## 1. Призначення моделі потоків

Модель потоків повинна:
- підтримувати паралельні мережеві активності
- тримати shared state під контролем
- робити shutdown-поведінку передбачуваною
- уникати зайвої складності в першій реалізації
- давати основу для подальшого розширення

Модель має бути достатньо простою для MVP, але й достатньо структурованою, щоб уникнути повного redesign після першої робочої версії.

---

## 2. Основні паралельні активності в MVP

MVP-симулятор може потребувати обробки таких паралельних активностей:

- прийом і обробка SIP-повідомлень
- генерація та передача RTP-пакетів
- обробка HTTP-запитів
- підтримка WebSocket-підключень і доставка подій
- виведення логів
- координація завершення роботи

Не всі ці активності обов’язково мають із самого початку працювати в повністю окремих потоках, але вони представляють логічно окремі runtime-діяльності.

---

## 3. Рекомендований підхід до потоків для MVP

Рекомендований підхід для MVP — невеликий багатопотоковий дизайн із чітко обмеженими зонами відповідальності.

Розумна стартова модель така:

### 3.1 Main Thread
Відповідає за:
- запуск процесу
- завантаження конфігурації
- ініціалізацію модулів
- з’єднання компонентів між собою
- координацію завершення роботи

### 3.2 SIP Handling Thread
Відповідає за:
- прослуховування SIP-трафіку
- прийом SIP-повідомлень
- передачу валідованих сигнальних подій у внутрішню логіку

### 3.3 RTP Worker Thread
Відповідає за:
- RTP timing loop
- генерацію пакетів
- передачу пакетів під час активної сесії
- зупинку RTP після завершення сесії

### 3.4 HTTP/WebSocket Service Thread
Відповідає за:
- обробку HTTP-запитів
- підтримку WebSocket-підключень
- доставку event notifications

За потреби цей потік можна буде розділити пізніше, але для MVP одного service thread достатньо.

### 3.5 Optional Logging Worker
Для MVP логування спочатку може залишатися синхронним.  
Якщо пізніше знадобиться, можна додати окремий logging thread.

---

## 4. Зони shared state

Наступні області shared state вимагають чіткого ownership і синхронізації:

### 4.1 Стан пристрою
Спільний між:
- SIP handling
- session logic
- HTTP status access
- WebSocket event generation
- shutdown flow

### 4.2 Контекст сесії
Спільний між:
- SIP handling
- RTP worker
- logging
- diagnostics

### 4.3 Доставка внутрішніх подій
Спільна між:
- producers подій
- WebSocket delivery
- logging

### 4.4 Shutdown signals
Спільні між:
- main thread
- SIP thread
- RTP thread
- HTTP/WebSocket thread

---

## 5. Принципи ownership

Щоб MVP залишався безпечним і зрозумілим, модель потоків повинна дотримуватися таких принципів:

- shared state повинен мати чіткий ownership
- міжпотокова комунікація має бути мінімізована
- прямий несинхронізований доступ до shared state слід уникати
- переходи станів повинні відбуватися в контрольованому місці
- shutdown повинен управлятися централізовано

Дизайн має віддавати перевагу:
- явному ownership
- захищеному доступу
- простим coordination primitives
замість хаотичних взаємодій між потоками.

---

## 6. Стратегія синхронізації для MVP

Точна реалізація може уточнюватися, але MVP, ймовірно, повинен використовувати лише невеликий набір стандартних synchronization tools, наприклад:

- `std::mutex`
- `std::lock_guard`
- `std::unique_lock`
- `std::condition_variable`
- `std::atomic<bool>` для stop flags там, де це доречно

MVP повинен уникати:
- over-engineered asynchronous frameworks
- надто багатьох рівнів блокувань
- неявних правил ownership між потоками
- складних actor-style систем, якщо вони не виправдані пізніше

---

## 7. Рекомендована модель координації

Практична coordination model для MVP така:

- SIP thread отримує повідомлення
- SIP thread виконує parsing і початкову валідацію
- SIP thread у контрольований спосіб оновлює state/session
- SIP thread генерує внутрішню подію
- RTP thread реагує лише тоді, коли стан сесії дозволяє RTP
- HTTP/WebSocket thread у контрольований спосіб читає стан і публікує події
- main thread координує stop signals і виконує join worker threads

Це зберігає signaling path центральним, одночасно дозволяючи RTP і зовнішнім сервісам працювати паралельно.

---

## 8. Shutdown-поведінка

Контрольоване завершення роботи — одна з найважливіших причин визначити модель потоків заздалегідь.

Shutdown-модель повинна гарантувати:
- stop signal поширюється на всі worker threads
- blocking loops можуть завершитися
- RTP-передача коректно зупиняється
- SIP listening коректно зупиняється
- HTTP/WebSocket service коректно зупиняється
- логи, за потреби, скидаються
- worker threads приєднуються через join до завершення процесу

Система повинна уникати:
- detached worker threads
- тихого завершення потоків
- shutdown race-condition навколо shared state

---

## 9. Чому ця модель навмисно мала

MVP-модель потоків навмисно обмежена, тому що:
- перша ціль — коректність, а не високий throughput
- підтримується лише один канонічний session flow
- симулятор повинен залишатися зрозумілим
- debugging конкурентної поведінки уже є нетривіальним навіть у малих системах

Майбутні версії можуть додати:
- finer-grained розділення сервісів
- session-specific workers
- покращені event queues
- окремий diagnostics thread
- performance-oriented покращення конкурентності

Усе це навмисно відкладається.

---

## 10. Рекомендований наступний результат дизайну

На основі цього spike реалізація повинна далі визначити:
- який компонент володіє device state
- який компонент володіє session state
- чи доставляються внутрішні події синхронно чи через queue
- точний stop/exit contract для кожного worker thread

Ці рішення слід ухвалити до початку реалізації core runtime.

---

## Підсумок
Рекомендована MVP-модель потоків — це невеликий, контрольований багатопотоковий дизайн з:

- одним main coordination thread
- одним SIP handling thread
- одним RTP worker thread
- одним HTTP/WebSocket service thread

Цей дизайн віддає пріоритет передбачуваності, контрольованому ownership і чистому shutdown, а не складній конкурентності.