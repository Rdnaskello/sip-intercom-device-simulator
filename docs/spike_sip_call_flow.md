# Spike: SIP Call Flow Prototype

## Overview
This document defines the canonical SIP call flow prototype for the MVP version of the SIP Intercom Device Simulator.

The purpose of this spike is to reduce uncertainty around the signaling sequence that the simulator must support in its first meaningful implementation.

The goal is not to implement full SIP behavior, but to define the smallest realistic signaling scenario that is sufficient for:
- session establishment
- device state transitions
- RTP start/stop coordination
- logging and diagnostics
- end-to-end MVP demonstration

---

## 1. Scope of This Spike

This spike focuses on:
- the minimum SIP signaling flow for MVP
- supported SIP methods and responses
- mapping between signaling steps and device state transitions
- clear boundaries for what is intentionally excluded from MVP

This spike does not attempt to define:
- full SIP RFC compliance
- complete SDP negotiation
- authentication flows
- retransmission handling
- advanced error recovery
- compatibility with all external SIP systems

---

## 2. Roles in the Signaling Scenario

The canonical MVP scenario contains two logical participants:

### 2.1 SIP Peer
The external party that initiates and terminates the SIP session.

### 2.2 SIP Intercom Device Simulator
The simulated device that receives signaling messages, updates internal state, and coordinates session behavior.

In the MVP, the simulator acts primarily as the called device side in the basic incoming session scenario.

---

## 3. Canonical MVP Call Flow

The canonical SIP signaling flow for the MVP is defined as follows:

1. SIP Peer sends `INVITE`
2. Simulator sends `100 Trying`
3. Simulator sends `180 Ringing`
4. Simulator sends `200 OK`
5. SIP Peer sends `ACK`
6. Session becomes active
7. RTP stream starts
8. SIP Peer sends `BYE`
9. Simulator sends `200 OK`
10. Session is terminated
11. RTP stream stops
12. Device returns to `Idle`

---

## 4. Supported SIP Methods and Responses for MVP

### Supported Requests
- `INVITE`
- `ACK`
- `BYE`

### Supported Responses
- `100 Trying`
- `180 Ringing`
- `200 OK`

This set is intentionally minimal and sufficient for the first supported end-to-end scenario.

---

## 5. State Transition Mapping

The signaling flow maps to device states as follows:

### Initial State
- `Idle`

### After receiving `INVITE`
- transition from `Idle` to `Ringing`

### After sending final `200 OK` and receiving `ACK`
- transition from `Ringing` to `ActiveSession`

### After receiving `BYE` and sending `200 OK`
- transition from `ActiveSession` to `Idle`

---

## 6. RTP Coordination Rules

The RTP stream is tied to the signaling flow using the following rules:

- RTP does not start immediately after `INVITE`
- RTP does not start after provisional responses
- RTP starts only after the active session is confirmed by `ACK`
- RTP stops immediately when the session termination flow completes

This keeps RTP lifecycle clearly dependent on confirmed session state.

---

## 7. Logging Expectations

The following events must be logged in the MVP flow:

- receipt of `INVITE`
- sending of `100 Trying`
- sending of `180 Ringing`
- sending of `200 OK`
- receipt of `ACK`
- transition to `ActiveSession`
- RTP stream start
- receipt of `BYE`
- sending of final `200 OK` for termination
- RTP stream stop
- transition back to `Idle`

This logging sequence is important for both debugging and demonstration.

---

## 8. Explicitly Excluded from This MVP Flow

The following items are outside the scope of the canonical MVP SIP flow:

- `REGISTER`
- `CANCEL`
- `OPTIONS`
- `INFO`
- `REFER`
- re-INVITE
- SIP authentication
- advanced SDP negotiation
- retransmission timers
- support for multiple simultaneous dialogs
- support for all SIP error responses

These may be added in later iterations if needed.

---

## 9. MVP Interpretation Notes

The MVP flow intentionally simplifies some real-world SIP behavior.

For the purposes of this simulator:
- the signaling scenario is treated as a controlled engineering flow
- full protocol completeness is not required
- consistency, traceability, and architectural clarity are more important than breadth of SIP support

The chosen flow is realistic enough for communication-oriented system design while still being small enough to implement safely in an MVP.

---

## 10. Recommended Next Technical Outcome

Based on this spike, the next implementation-oriented spike should define:
- the minimum SIP message fields required for `INVITE`, `ACK`, and `BYE`
- the minimum response fields required for `100 Trying`, `180 Ringing`, and `200 OK`
- how parsed messages will be represented internally

That next step corresponds to the SIP parser boundary spike.

---

## Summary
The canonical MVP SIP flow for the SIP Intercom Device Simulator is:

`INVITE -> 100 Trying -> 180 Ringing -> 200 OK -> ACK -> ActiveSession -> BYE -> 200 OK -> Idle`

This flow is the baseline signaling scenario for the first meaningful end-to-end version of the project.

# Spike: прототип SIP call flow

## Загальний опис
Цей документ визначає канонічний прототип SIP call flow для MVP-версії SIP Intercom Device Simulator.

Мета цього spike — зменшити невизначеність щодо сигнальної послідовності, яку симулятор повинен підтримувати у своїй першій змістовній реалізації.

Ціль не в тому, щоб реалізувати повну SIP-поведінку, а в тому, щоб визначити найменший реалістичний сигнальний сценарій, достатній для:
- встановлення сесії
- переходів станів пристрою
- координації запуску/зупинки RTP
- логування та діагностики
- end-to-end демонстрації MVP

---

## 1. Межі цього spike

Цей spike фокусується на:
- мінімальному SIP signaling flow для MVP
- підтримуваних SIP-методах і відповідях
- мапінгу між сигнальними кроками та переходами станів пристрою
- чітких межах того, що свідомо виключається з MVP

Цей spike не намагається визначити:
- повну відповідність SIP RFC
- повноцінну SDP-negotiation
- authentication flows
- обробку retransmission
- просунуте error recovery
- сумісність з усіма зовнішніми SIP-системами

---

## 2. Ролі в сигнальному сценарії

Канонічний MVP-сценарій містить двох логічних учасників:

### 2.1 SIP Peer
Зовнішня сторона, яка ініціює та завершує SIP-сесію.

### 2.2 SIP Intercom Device Simulator
Симульований пристрій, який приймає сигнальні повідомлення, оновлює внутрішній стан і координує поведінку сесії.

У межах MVP симулятор переважно виступає як сторона викликаного пристрою в базовому сценарії вхідної сесії.

---

## 3. Канонічний MVP call flow

Канонічний SIP signaling flow для MVP визначається так:

1. SIP Peer надсилає `INVITE`
2. Simulator надсилає `100 Trying`
3. Simulator надсилає `180 Ringing`
4. Simulator надсилає `200 OK`
5. SIP Peer надсилає `ACK`
6. Сесія стає активною
7. RTP-потік запускається
8. SIP Peer надсилає `BYE`
9. Simulator надсилає `200 OK`
10. Сесія завершується
11. RTP-потік зупиняється
12. Пристрій повертається в `Idle`

---

## 4. Підтримувані SIP-методи та відповіді для MVP

### Підтримувані requests
- `INVITE`
- `ACK`
- `BYE`

### Підтримувані responses
- `100 Trying`
- `180 Ringing`
- `200 OK`

Цей набір навмисно мінімальний і достатній для першого підтримуваного end-to-end сценарію.

---

## 5. Мапінг переходів станів

Сигнальний сценарій мапиться на стани пристрою так:

### Початковий стан
- `Idle`

### Після отримання `INVITE`
- перехід з `Idle` у `Ringing`

### Після надсилання фінального `200 OK` і отримання `ACK`
- перехід з `Ringing` у `ActiveSession`

### Після отримання `BYE` і надсилання `200 OK`
- перехід з `ActiveSession` у `Idle`

---

## 6. Правила координації RTP

RTP-потік прив’язується до сигнального сценарію за такими правилами:

- RTP не запускається одразу після `INVITE`
- RTP не запускається після provisional responses
- RTP запускається лише після підтвердження активної сесії через `ACK`
- RTP зупиняється одразу після завершення сценарію завершення сесії

Це робить життєвий цикл RTP чітко залежним від підтвердженого стану сесії.

---

## 7. Очікування щодо логування

У межах MVP-сценарію мають логуватися такі події:

- отримання `INVITE`
- надсилання `100 Trying`
- надсилання `180 Ringing`
- надсилання `200 OK`
- отримання `ACK`
- перехід у `ActiveSession`
- запуск RTP-потоку
- отримання `BYE`
- надсилання фінального `200 OK` для завершення
- зупинка RTP-потоку
- перехід назад у `Idle`

Ця послідовність логування важлива і для налагодження, і для демонстрації.

---

## 8. Що явно виключено з цього MVP flow

Наступні елементи не входять у межі канонічного MVP SIP flow:

- `REGISTER`
- `CANCEL`
- `OPTIONS`
- `INFO`
- `REFER`
- re-INVITE
- SIP authentication
- advanced SDP negotiation
- retransmission timers
- підтримка кількох одночасних dialogs
- підтримка всіх SIP error responses

Усе це можна буде додати в наступних ітераціях за потреби.

---

## 9. Примітки до трактування MVP

MVP-flow навмисно спрощує деякі аспекти реальної SIP-поведінки.

Для цілей цього симулятора:
- сигнальний сценарій розглядається як контрольований інженерний flow
- повна протокольна завершеність не є обов’язковою
- узгодженість, трасованість і архітектурна ясність важливіші за широту SIP-підтримки

Обраний flow достатньо реалістичний для communication-oriented system design і водночас достатньо малий, щоб його можна було безпечно реалізувати в межах MVP.

---

## 10. Рекомендований наступний технічний результат

На основі цього spike наступний implementation-oriented spike повинен визначити:
- мінімальні SIP message fields, потрібні для `INVITE`, `ACK` і `BYE`
- мінімальні response fields, потрібні для `100 Trying`, `180 Ringing` і `200 OK`
- спосіб внутрішнього представлення parsed messages

Цей наступний крок відповідає spike-задачі з визначення меж SIP-parser-а.

---

## Підсумок
Канонічний MVP SIP flow для SIP Intercom Device Simulator такий:

`INVITE -> 100 Trying -> 180 Ringing -> 200 OK -> ACK -> ActiveSession -> BYE -> 200 OK -> Idle`

Цей flow є базовим сигнальним сценарієм для першої змістовної end-to-end версії проєкту.