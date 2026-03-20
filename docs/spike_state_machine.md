# Spike: Device State Machine Prototype

## Overview
This document defines the initial device state machine prototype for the MVP version of the SIP Intercom Device Simulator.

The purpose of this spike is to define a small, clear, and enforceable state model that can be used consistently by the signaling, session, RTP, and API-related parts of the simulator.

The state machine is intentionally minimal. Its goal is to support the canonical MVP communication flow without introducing unnecessary state complexity too early.

---

## 1. Goals of the State Machine

The MVP state machine should:
- represent the current high-level device state
- allow only valid transitions
- make SIP-driven behavior predictable
- define when RTP is allowed or forbidden
- provide a clear basis for logging and diagnostics
- remain simple enough to reason about during early implementation

---

## 2. Proposed MVP States

The initial MVP state model contains the following states:

### 2.1 `Idle`
The default ready state of the simulator.

Meaning:
- no active call session exists
- the device is ready to receive an incoming SIP `INVITE`
- RTP is not active

### 2.2 `Ringing`
The device has received a valid incoming `INVITE` and is in the pre-session signaling phase.

Meaning:
- a candidate session is being established
- provisional SIP responses may be sent
- the session is not yet fully active
- RTP is not active

### 2.3 `ActiveSession`
A session has been successfully established.

Meaning:
- `INVITE` has been processed
- final success response has been sent
- `ACK` has been received
- the device is considered to be in an active communication session
- RTP may be active

### 2.4 `Error`
A safe error state for conditions where the simulator detects an unrecoverable state inconsistency or critical runtime issue.

Meaning:
- the system cannot safely continue normal flow in its current condition
- recovery behavior depends on implementation policy
- this state exists mainly for defensive design and diagnostics

---

## 3. Canonical State Transition Flow

The canonical MVP transition sequence is:

`Idle -> Ringing -> ActiveSession -> Idle`

This corresponds to the supported SIP session lifecycle:
- incoming `INVITE`
- session establishment
- session termination

The `Error` state is not part of the normal happy path, but it is available as a protective fallback.

---

## 4. Allowed Transitions

### From `Idle`
Allowed transitions:
- `Idle -> Ringing`
- `Idle -> Error`

Not allowed:
- `Idle -> ActiveSession`

### From `Ringing`
Allowed transitions:
- `Ringing -> ActiveSession`
- `Ringing -> Idle`
- `Ringing -> Error`

Notes:
- `Ringing -> Idle` may happen if session setup fails, is rejected, or times out.

### From `ActiveSession`
Allowed transitions:
- `ActiveSession -> Idle`
- `ActiveSession -> Error`

Not allowed:
- `ActiveSession -> Ringing`

### From `Error`
Allowed transitions:
- implementation-defined
- for MVP, the safest assumption is that recovery may require reset or controlled shutdown

---

## 5. Transition Triggers

### `Idle -> Ringing`
Triggered by:
- receipt of a valid supported SIP `INVITE`

### `Ringing -> ActiveSession`
Triggered by:
- successful completion of the supported signaling sequence
- final `200 OK` sent
- corresponding `ACK` received

### `Ringing -> Idle`
Triggered by:
- invalid or incomplete session establishment
- timeout during setup
- explicit internal rejection policy
- safe cleanup after failed pre-session flow

### `ActiveSession -> Idle`
Triggered by:
- receipt of valid SIP `BYE`
- successful session termination flow
- cleanup of session and RTP resources

### `Any -> Error`
Triggered by:
- internal state inconsistency
- critical parsing or session handling failure
- unrecoverable runtime problem according to implementation policy

---

## 6. RTP Rules by State

### In `Idle`
- RTP must not run

### In `Ringing`
- RTP must not run

### In `ActiveSession`
- RTP may run
- RTP start is allowed only after confirmed session establishment

### In `Error`
- RTP must be stopped or remain disabled

This keeps RTP tightly coupled to valid session state.

---

## 7. HTTP/API Behavior by State

For the MVP, HTTP control operations may be partially independent from SIP state, but state awareness is still useful.

Example assumptions:
- status endpoint is always allowed
- door-open action may be allowed in `Idle`, `Ringing`, or `ActiveSession`
- critical administrative operations may later become state-dependent

The state machine should at minimum make the current device state observable through the API.

---

## 8. Logging Expectations

State transitions should always be logged.

At minimum, the following should be recorded:
- previous state
- new state
- trigger or reason
- relevant session identifier if applicable

Example:
- `Idle -> Ringing` on incoming `INVITE`
- `Ringing -> ActiveSession` on confirmed session establishment
- `ActiveSession -> Idle` on `BYE`
- transition to `Error` with error reason

This is important for debugging signaling behavior and validating system correctness.

---

## 9. Why This State Model Is Intentionally Small

The MVP state model is intentionally limited because:
- the simulator supports only one canonical SIP scenario
- too many states too early would increase implementation complexity
- the main goal is predictability, not protocol completeness
- additional states can be introduced later if a real need appears

Possible future states might include:
- `Starting`
- `Stopping`
- `TerminatingSession`
- `Rejected`
- `Busy`
- `Recovering`

These are intentionally excluded from the first version.

---

## 10. Recommended Next Design Outcome

Based on this spike, the implementation should next define:
- where state ownership lives
- who is allowed to trigger transitions
- how transition validation is enforced
- whether transitions are performed directly or through a centralized state manager

This should be resolved before deeper signaling implementation begins.

---

## Summary
The recommended MVP device state machine is:

- `Idle`
- `Ringing`
- `ActiveSession`
- `Error`

with the canonical happy-path flow:

`Idle -> Ringing -> ActiveSession -> Idle`

This model is intentionally minimal, predictable, and sufficient for the first end-to-end simulator implementation.

# Spike: прототип автомата станів пристрою

## Загальний опис
Цей документ визначає початковий прототип автомата станів пристрою для MVP-версії SIP Intercom Device Simulator.

Мета цього spike — визначити малу, чітку та контрольовану модель станів, яку можна послідовно використовувати в сигнальній логіці, менеджері сесій, RTP та API-пов’язаних частинах симулятора.

Автомат станів навмисно робиться мінімальним. Його мета — підтримати канонічний MVP-комунікаційний сценарій без додавання зайвої складності станів надто рано.

---

## 1. Цілі автомата станів

MVP-автомат станів повинен:
- представляти поточний високорівневий стан пристрою
- дозволяти лише коректні переходи
- робити SIP-керовану поведінку передбачуваною
- визначати, коли RTP дозволений, а коли заборонений
- давати чітку основу для логування і діагностики
- залишатися достатньо простим для аналізу на ранньому етапі реалізації

---

## 2. Запропоновані стани для MVP

Початкова MVP-модель станів містить такі стани:

### 2.1 `Idle`
Стандартний готовий стан симулятора.

Значення:
- активної сесії виклику немає
- пристрій готовий прийняти вхідний SIP `INVITE`
- RTP не активний

### 2.2 `Ringing`
Пристрій отримав коректний вхідний `INVITE` і перебуває у фазі сигналізації до встановлення сесії.

Значення:
- формується потенційна сесія
- можуть надсилатися provisional SIP responses
- сесія ще не є повністю активною
- RTP не активний

### 2.3 `ActiveSession`
Сесія успішно встановлена.

Значення:
- `INVITE` оброблено
- фінальну успішну відповідь надіслано
- `ACK` отримано
- пристрій вважається таким, що перебуває в активній комунікаційній сесії
- RTP може бути активним

### 2.4 `Error`
Безпечний стан помилки для випадків, коли симулятор виявляє невідновну неузгодженість стану або критичну runtime-проблему.

Значення:
- система не може безпечно продовжувати нормальний flow у поточному стані
- поведінка відновлення залежить від політики реалізації
- цей стан існує насамперед для захисного дизайну та діагностики

---

## 3. Канонічний ланцюжок переходів станів

Канонічна послідовність переходів для MVP:

`Idle -> Ringing -> ActiveSession -> Idle`

Вона відповідає підтримуваному життєвому циклу SIP-сесії:
- вхідний `INVITE`
- встановлення сесії
- завершення сесії

Стан `Error` не є частиною нормального happy path, але існує як захисний fallback.

---

## 4. Дозволені переходи

### З `Idle`
Дозволені переходи:
- `Idle -> Ringing`
- `Idle -> Error`

Недозволені:
- `Idle -> ActiveSession`

### З `Ringing`
Дозволені переходи:
- `Ringing -> ActiveSession`
- `Ringing -> Idle`
- `Ringing -> Error`

Примітки:
- `Ringing -> Idle` може статися, якщо встановлення сесії не вдалося, було відхилене або завершилося timeout.

### З `ActiveSession`
Дозволені переходи:
- `ActiveSession -> Idle`
- `ActiveSession -> Error`

Недозволені:
- `ActiveSession -> Ringing`

### З `Error`
Дозволені переходи:
- визначаються реалізацією
- для MVP найбезпечніше припустити, що відновлення може вимагати reset або контрольованого shutdown

---

## 5. Тригери переходів

### `Idle -> Ringing`
Тригер:
- отримання коректного підтримуваного SIP `INVITE`

### `Ringing -> ActiveSession`
Тригер:
- успішне завершення підтримуваного signaling sequence
- фінальний `200 OK` надіслано
- відповідний `ACK` отримано

### `Ringing -> Idle`
Тригер:
- невалідне або незавершене встановлення сесії
- timeout під час setup
- явна внутрішня політика відхилення
- безпечне очищення після невдалого pre-session flow

### `ActiveSession -> Idle`
Тригер:
- отримання коректного SIP `BYE`
- успішний сценарій завершення сесії
- очищення ресурсів сесії та RTP

### `Any -> Error`
Тригер:
- внутрішня неузгодженість стану
- критичний збій парсингу або обробки сесії
- невідновна runtime-проблема згідно з політикою реалізації

---

## 6. Правила RTP залежно від стану

### У `Idle`
- RTP не повинен працювати

### У `Ringing`
- RTP не повинен працювати

### У `ActiveSession`
- RTP може працювати
- RTP дозволено запускати лише після підтвердженого встановлення сесії

### У `Error`
- RTP повинен бути зупинений або залишатися вимкненим

Це тримає RTP жорстко прив’язаним до коректного стану сесії.

---

## 7. Поведінка HTTP/API залежно від стану

Для MVP HTTP-керування може бути частково незалежним від SIP-стану, але awareness стану все одно корисний.

Приклади припущень:
- status endpoint завжди дозволений
- дія відкриття дверей може бути дозволена в `Idle`, `Ringing` або `ActiveSession`
- критичні адміністративні операції пізніше можуть стати state-dependent

Автомат станів повинен щонайменше робити поточний стан пристрою видимим через API.

---

## 8. Очікування щодо логування

Переходи станів повинні логуватися завжди.

Щонайменше потрібно записувати:
- попередній стан
- новий стан
- тригер або причину
- релевантний ідентифікатор сесії, якщо застосовно

Приклади:
- `Idle -> Ringing` на вхідний `INVITE`
- `Ringing -> ActiveSession` після підтвердженого встановлення сесії
- `ActiveSession -> Idle` на `BYE`
- перехід у `Error` з причиною помилки

Це важливо для debugging сигнальної поведінки й перевірки коректності системи.

---

## 9. Чому ця модель станів навмисно мала

MVP-модель станів навмисно обмежена, тому що:
- симулятор підтримує лише один канонічний SIP-сценарій
- надто багато станів на старті збільшили б складність реалізації
- головна мета — передбачуваність, а не повна протокольна завершеність
- додаткові стани можна ввести пізніше, якщо з’явиться реальна потреба

Можливі майбутні стани:
- `Starting`
- `Stopping`
- `TerminatingSession`
- `Rejected`
- `Busy`
- `Recovering`

Усі вони навмисно виключені з першої версії.

---

## 10. Рекомендований наступний результат дизайну

На основі цього spike реалізація далі повинна визначити:
- де живе ownership стану
- хто має право ініціювати переходи
- як перевіряється валідність переходів
- чи виконуються переходи напряму, чи через централізований state manager

Це слід вирішити до початку глибшої реалізації сигналізації.

---

## Підсумок
Рекомендований MVP-автомат станів пристрою такий:

- `Idle`
- `Ringing`
- `ActiveSession`
- `Error`

з канонічним happy-path flow:

`Idle -> Ringing -> ActiveSession -> Idle`

Ця модель навмисно мінімальна, передбачувана і достатня для першої end-to-end реалізації симулятора.