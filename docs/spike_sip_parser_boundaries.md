# Spike: SIP Parser Boundaries

## Overview
This document defines the boundaries of the SIP parser for the MVP version of the SIP Intercom Device Simulator.

The goal of this spike is to determine the minimum parsing scope required to support the canonical MVP SIP call flow without introducing unnecessary protocol complexity too early.

This spike focuses on:
- which SIP messages must be parsed
- which fields are required for MVP behavior
- which fields may be ignored in the first implementation
- how parsed SIP messages may be represented internally

---

## 1. Purpose of the Parser in MVP

The SIP parser in the MVP is not intended to be a full SIP protocol implementation.

Its purpose is to:
- recognize supported SIP request types
- extract the minimum required metadata from incoming messages
- support session lifecycle decisions
- provide enough structured information for logging and state transitions
- enable generation of valid enough SIP responses for the supported flow

The parser should be intentionally small, maintainable, and easy to extend later.

---

## 2. Supported Incoming SIP Messages

For the MVP, the parser only needs to support the following incoming SIP requests:

- `INVITE`
- `ACK`
- `BYE`

No other SIP request methods are required for the first implementation.

---

## 3. Minimum Fields Required for Incoming Requests

The following fields should be parsed for supported incoming requests where applicable.

### 3.1 Start Line
The parser shall extract:
- request method
- request URI
- SIP version

Example:
`INVITE sip:intercom@example.com SIP/2.0`

### 3.2 Required Headers
The parser should extract at least:

- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

These headers are sufficient for:
- identifying message type
- associating messages with a session
- constructing basic responses
- logging meaningful signaling context

### 3.3 Optional / Nice-to-Have Headers for MVP
The parser may also extract if present:

- `Contact`
- `Content-Type`
- `Content-Length`

These are useful but not strictly required for the first minimal signaling logic.

### 3.4 Message Body
For the MVP, full SDP parsing is not required.

The parser may:
- preserve the body as raw text if present
- store `Content-Type`
- store `Content-Length`

But it does not need to interpret SDP fields in the first version.

---

## 4. Fields Required for Each Supported Method

### 4.1 INVITE
Required:
- start line
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Optional:
- `Contact`
- `Content-Type`
- `Content-Length`
- raw body

### 4.2 ACK
Required:
- start line
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Optional:
- `Contact`

### 4.3 BYE
Required:
- start line
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Optional:
- `Contact`

---

## 5. Fields That Can Be Ignored in MVP

The following SIP elements may be ignored in the first implementation unless later needed:

- `Max-Forwards`
- `User-Agent`
- `Allow`
- `Supported`
- `Route`
- `Record-Route`
- `Authorization`
- `Proxy-Authorization`
- `Expires`
- `Subject`
- `Accept`
- `Session-Expires`
- advanced SDP fields

These fields may still be logged or preserved as raw headers if convenient, but they do not need active behavioral handling in the MVP.

---

## 6. Parser Output Model

The parser should produce a structured internal representation of the received SIP message.

A possible MVP-oriented internal model is:

- message type (`INVITE`, `ACK`, `BYE`, or unknown)
- request URI
- SIP version
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`
- optional `Contact`
- optional `Content-Type`
- optional `Content-Length`
- raw body
- raw original message
- parse status / validation status

This model should be simple enough for:
- session matching
- response construction
- logging
- state machine decisions

---

## 7. Validation Boundaries

For MVP, validation should remain intentionally limited.

The parser should validate:
- supported method exists
- request line format is recognizable
- required headers are present
- required headers are not empty
- the message can be classified as minimally usable or invalid

The parser does not need to validate:
- all header syntax in strict RFC detail
- deep SDP correctness
- transport-level retransmission rules
- full URI correctness for all SIP variants

---

## 8. Response Construction Implications

The parsing scope is directly connected to response generation.

To generate valid enough MVP responses, the implementation will likely need access to:
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

This means these headers are not only parse requirements but also response-generation requirements.

---

## 9. Error Handling Expectations

If an incoming message:
- uses an unsupported method
- has an unrecognized start line
- is missing required headers
- cannot be minimally interpreted

then the parser should classify it as invalid for MVP processing.

The system may then:
- log the parsing failure
- reject the message or ignore it according to MVP policy
- avoid crashing or entering inconsistent state

---

## 10. Recommended Parser Philosophy for MVP

The parser should follow these principles:

- minimal supported scope
- explicit handling of supported methods
- predictable behavior on invalid input
- internal representation oriented to simulator logic, not full protocol completeness
- extension-friendly structure for later SIP support growth

The parser should be "small but solid" rather than broad and fragile.

---

## Summary
For the MVP, the SIP parser only needs to support:
- `INVITE`
- `ACK`
- `BYE`

and only needs to extract the minimum fields required for:
- session identification
- state transitions
- logging
- response construction

All broader SIP support should be intentionally postponed until after the first end-to-end implementation is working.

# Spike: межі SIP parser-а

## Загальний опис
Цей документ визначає межі SIP parser-а для MVP-версії SIP Intercom Device Simulator.

Мета цього spike — визначити мінімальний обсяг парсингу, потрібний для підтримки канонічного MVP SIP call flow, не додаючи зайвої протокольної складності надто рано.

Цей spike фокусується на:
- які SIP-повідомлення потрібно парсити
- які поля обов’язкові для MVP-поведінки
- які поля можна ігнорувати в першій реалізації
- яким може бути внутрішнє представлення розібраних SIP-повідомлень

---

## 1. Призначення parser-а в MVP

SIP parser у межах MVP не призначений бути повною реалізацією SIP-протоколу.

Його мета:
- розпізнавати підтримувані типи SIP requests
- витягувати мінімально потрібні метадані з вхідних повідомлень
- підтримувати рішення, пов’язані з життєвим циклом сесії
- надавати достатньо структурованої інформації для логування та переходів станів
- дозволяти генерувати достатньо коректні SIP responses для підтримуваного flow

Parser має бути навмисно малим, підтримуваним і простим для подальшого розширення.

---

## 2. Підтримувані вхідні SIP-повідомлення

Для MVP parser повинен підтримувати лише такі вхідні SIP requests:

- `INVITE`
- `ACK`
- `BYE`

Жодні інші SIP request methods не потрібні для першої реалізації.

---

## 3. Мінімальні поля, потрібні для вхідних requests

Для підтримуваних вхідних requests потрібно парсити такі поля, де це застосовно.

### 3.1 Start Line
Parser повинен витягувати:
- request method
- request URI
- SIP version

Приклад:
`INVITE sip:intercom@example.com SIP/2.0`

### 3.2 Обов’язкові заголовки
Parser повинен витягувати щонайменше:

- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Цих заголовків достатньо для:
- ідентифікації типу повідомлення
- прив’язки повідомлень до сесії
- побудови базових відповідей
- змістовного логування сигнального контексту

### 3.3 Необов’язкові / бажані заголовки для MVP
Parser може також витягувати, якщо вони присутні:

- `Contact`
- `Content-Type`
- `Content-Length`

Вони корисні, але не є строго обов’язковими для першої мінімальної сигнальної логіки.

### 3.4 Тіло повідомлення
Для MVP повний SDP parsing не потрібен.

Parser може:
- зберігати body як raw text, якщо він присутній
- зберігати `Content-Type`
- зберігати `Content-Length`

Але йому не потрібно інтерпретувати SDP-поля в першій версії.

---

## 4. Поля, потрібні для кожного підтримуваного методу

### 4.1 INVITE
Обов’язкові:
- start line
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Необов’язкові:
- `Contact`
- `Content-Type`
- `Content-Length`
- raw body

### 4.2 ACK
Обов’язкові:
- start line
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Необов’язкові:
- `Contact`

### 4.3 BYE
Обов’язкові:
- start line
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Необов’язкові:
- `Contact`

---

## 5. Поля, які можна ігнорувати в MVP

У першій реалізації можна ігнорувати такі SIP-елементи, якщо вони не знадобляться пізніше:

- `Max-Forwards`
- `User-Agent`
- `Allow`
- `Supported`
- `Route`
- `Record-Route`
- `Authorization`
- `Proxy-Authorization`
- `Expires`
- `Subject`
- `Accept`
- `Session-Expires`
- advanced SDP fields

Ці поля все ще можна логувати або зберігати як raw headers, якщо це зручно, але їх не потрібно активно обробляти в межах MVP.

---

## 6. Модель результату parser-а

Parser повинен повертати структуроване внутрішнє представлення отриманого SIP-повідомлення.

Можлива MVP-орієнтована внутрішня модель:

- message type (`INVITE`, `ACK`, `BYE` або unknown)
- request URI
- SIP version
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`
- optional `Contact`
- optional `Content-Type`
- optional `Content-Length`
- raw body
- raw original message
- parse status / validation status

Ця модель має бути достатньо простою для:
- зіставлення з сесією
- побудови responses
- логування
- рішень автомата станів

---

## 7. Межі валідації

Для MVP валідація має залишатися навмисно обмеженою.

Parser повинен перевіряти:
- що існує підтримуваний method
- що формат request line впізнаваний
- що обов’язкові заголовки присутні
- що обов’язкові заголовки не порожні
- що повідомлення можна класифікувати як мінімально придатне або невалідне

Parser не зобов’язаний перевіряти:
- весь синтаксис заголовків у строгій RFC-деталізації
- глибоку коректність SDP
- transport-level retransmission rules
- повну коректність URI для всіх SIP-варіантів

---

## 8. Наслідки для побудови responses

Межі парсингу напряму пов’язані з генерацією responses.

Щоб формувати достатньо коректні MVP-responses, реалізації, ймовірно, буде потрібен доступ до:
- `Via`
- `From`
- `To`
- `Call-ID`
- `CSeq`

Це означає, що ці заголовки є не лише вимогою для parser-а, а й вимогою для генерації responses.

---

## 9. Очікування щодо обробки помилок

Якщо вхідне повідомлення:
- використовує непідтримуваний method
- має невпізнаваний start line
- не містить обов’язкових заголовків
- не може бути мінімально інтерпретоване

тоді parser повинен класифікувати його як невалідне для MVP-обробки.

Система після цього може:
- залогувати помилку парсингу
- відхилити або проігнорувати повідомлення згідно з MVP-політикою
- не допустити крашу або переходу в неузгоджений стан

---

## 10. Рекомендована філософія parser-а для MVP

Parser має дотримуватися таких принципів:

- мінімальний підтримуваний scope
- явна обробка підтримуваних methods
- передбачувана поведінка на невалідному вводі
- внутрішнє представлення, орієнтоване на логіку симулятора, а не на повну протокольну завершеність
- дружня до розширення структура для подальшого росту SIP-підтримки

Parser має бути “small but solid”, а не широким, але крихким.

---

## Підсумок
Для MVP SIP parser повинен підтримувати лише:
- `INVITE`
- `ACK`
- `BYE`

і витягувати тільки мінімальні поля, потрібні для:
- ідентифікації сесії
- переходів станів
- логування
- побудови responses

Будь-яку ширшу SIP-підтримку слід навмисно відкласти до моменту, коли перша end-to-end реалізація вже працюватиме.