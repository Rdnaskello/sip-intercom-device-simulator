# Project Vision

## Project Name
SIP Intercom Device Simulator

## Overview
SIP Intercom Device Simulator is a C++-based Linux application that simulates the behavior of a network intercom / access control device.  
The project is designed to model device-side communication logic, including SIP signaling, RTP stream emulation, HTTP/WebSocket control interfaces, structured logging, and network diagnostics.

The simulator is intended as an engineering-focused project that demonstrates practical skills relevant to embedded and communication-oriented software development.

## Project Goal
The main goal of the project is to design and implement a realistic simulator of a SIP-based intercom device that can participate in communication scenarios similar to those used in modern building access systems.

The project should serve two purposes:
1. Provide a technically realistic platform for studying communication protocols and device behavior.
2. Demonstrate software engineering, networking, diagnostics, and C++ development skills in a form suitable for portfolio and interview presentation.

## Problem Statement
Modern intercom and access control systems rely on multiple communication layers:
- signaling and session control
- media transport
- external control APIs
- diagnostics and logging
- concurrent processing of multiple internal tasks

Building a simulator of such a device helps explore how these systems behave, how communication is established, how failures are diagnosed, and how modular communication software can be designed.

## Target System
The simulated system represents a software-based intercom device that:
- accepts and processes SIP signaling messages
- manages call-related device states
- emulates RTP media transmission
- exposes HTTP API endpoints for external control
- publishes device events through WebSocket
- records structured logs and diagnostic information
- runs on Linux in a reproducible environment

## Primary Objectives
- Build a modular C++ application for Linux
- Simulate core SIP call flows relevant to an intercom device
- Emulate RTP packet transmission for an active session
- Expose control and monitoring interfaces through HTTP and WebSocket
- Provide structured logs for debugging and troubleshooting
- Support packet-level analysis through external tools such as tcpdump and Wireshark
- Demonstrate multithreaded system design and safe internal coordination between modules

## MVP Scope
The first version of the project should include:
- application startup and configuration loading
- device state machine
- minimal SIP signaling support for a basic call scenario
- RTP test stream emulation
- HTTP API for device status and door control actions
- WebSocket event notifications
- structured logging
- Docker-based development/runtime environment
- basic documentation and test scenarios

## Out of Scope for MVP
The first version will not include:
- real audio capture or playback
- production-grade SIP stack completeness
- advanced authentication and security mechanisms
- graphical user interface
- direct hardware integration
- full enterprise deployment features
- support for all possible SIP edge cases

## Expected Engineering Value
This project is intended to demonstrate:
- C++ software design
- Linux-based development
- communication protocol handling
- debugging of signaling and network behavior
- structured logging and diagnostics
- multithreaded architecture
- engineering documentation and project organization

## Success Criteria
The project will be considered successful at MVP level if:
- a basic SIP session flow can be simulated end-to-end
- RTP packet flow can be generated and observed
- device state changes can be controlled and monitored through APIs
- logs and network captures can be used to analyze system behavior
- the repository contains clear documentation, modular structure, and reproducible setup instructions

# Бачення проєкту

## Назва проєкту
SIP Intercom Device Simulator

## Загальний опис
SIP Intercom Device Simulator — це застосунок на C++ для Linux, який імітує поведінку мережевого домофонного / контрольного пристрою доступу.  
Проєкт призначений для моделювання логіки зв’язку на стороні пристрою, включаючи SIP-сигналізацію, емуляцію RTP-потоку, HTTP/WebSocket-інтерфейси керування, структуроване логування та мережеву діагностику.

Цей симулятор задуманий як інженерно-орієнтований проєкт, що демонструє практичні навички, релевантні для розробки embedded- та комунікаційного програмного забезпечення.

## Мета проєкту
Головна мета проєкту — спроєктувати та реалізувати реалістичний симулятор SIP-базованого домофонного пристрою, який може брати участь у сценаріях зв’язку, подібних до тих, що використовуються в сучасних системах контролю доступу до будівель.

Проєкт має виконувати дві основні функції:
1. Надавати технічно реалістичну платформу для вивчення комунікаційних протоколів і поведінки пристрою.
2. Демонструвати навички програмної інженерії, роботи з мережами, діагностики та розробки на C++ у форматі, придатному для портфоліо та співбесід.

## Формулювання проблеми
Сучасні домофонні та системи контролю доступу спираються на декілька рівнів комунікації:
- сигналізація та керування сесією
- передача медіаданих
- зовнішні API для керування
- діагностика та логування
- паралельна обробка кількох внутрішніх задач

Створення симулятора такого пристрою допомагає дослідити, як поводяться подібні системи, як встановлюється зв’язок, як діагностуються збої та як можна спроєктувати модульне комунікаційне програмне забезпечення.

## Цільова система
Симульована система представляє собою програмний домофонний пристрій, який:
- приймає та обробляє SIP-повідомлення сигналізації
- керує станами пристрою, пов’язаними з викликом
- емуляє передачу RTP-медіапотоку
- надає HTTP API для зовнішнього керування
- публікує події пристрою через WebSocket
- веде структуровані журнали подій і діагностичну інформацію
- працює на Linux у відтворюваному середовищі

## Основні цілі
- Побудувати модульний застосунок на C++ для Linux
- Імітувати основні SIP-сценарії викликів, релевантні для домофонного пристрою
- Емулювати передачу RTP-пакетів під час активної сесії
- Надати інтерфейси керування та моніторингу через HTTP і WebSocket
- Реалізувати структуроване логування для налагодження та діагностики
- Забезпечити можливість аналізу трафіку на рівні пакетів за допомогою зовнішніх інструментів, таких як tcpdump і Wireshark
- Продемонструвати багатопотоковий дизайн системи та безпечну внутрішню координацію між модулями

## Межі MVP
Перша версія проєкту повинна включати:
- запуск застосунку та завантаження конфігурації
- автомат станів пристрою
- мінімальну підтримку SIP-сигналізації для базового сценарію виклику
- емуляцію тестового RTP-потоку
- HTTP API для отримання стану пристрою та виконання дій керування дверима
- WebSocket-сповіщення про події
- структуроване логування
- середовище розробки та запуску на основі Docker
- базову документацію та тестові сценарії

## Що не входить у MVP
Перша версія не включатиме:
- реальне захоплення або відтворення аудіо
- повноцінну production-grade реалізацію SIP stack
- розширені механізми автентифікації та безпеки
- графічний інтерфейс користувача
- пряму інтеграцію з апаратним забезпеченням
- повний набір enterprise-функцій для розгортання
- підтримку всіх можливих edge-case сценаріїв SIP

## Очікувана інженерна цінність
Цей проєкт має продемонструвати:
- проєктування програмного забезпечення на C++
- розробку під Linux
- роботу з комунікаційними протоколами
- налагодження сигналізації та мережевої поведінки
- структуроване логування та діагностику
- багатопотокову архітектуру
- інженерну документацію та організацію проєкту

## Критерії успіху
Проєкт вважатиметься успішним на рівні MVP, якщо:
- базовий SIP-сценарій сесії можна буде змоделювати end-to-end
- RTP-потік пакетів можна буде згенерувати та спостерігати
- зміни стану пристрою можна буде контролювати й відстежувати через API
- логи та мережеві захоплення можна буде використовувати для аналізу поведінки системи
- репозиторій міститиме зрозумілу документацію, модульну структуру та відтворювані інструкції з запуску