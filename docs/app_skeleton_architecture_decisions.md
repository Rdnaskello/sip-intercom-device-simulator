# Architecture Decisions for `feat/app-skeleton`

## 1. Block Scope
This PR includes:
- build system
- thin `main()`
- `Application` skeleton
- `AppConfig` model
- `ConfigLoader`
- `Logger`
- minimal example config file

This PR does **not** include:
- SIP parser / SIP transport
- RTP / media simulation
- HTTP / WebSocket servers
- session / state machine implementation
- multithreaded runtime
- Docker
- production diagnostics subsystem

---

## 2. Responsibility of `main()`
`main()` is responsible only for orchestration:
- read config path
- initialize basic logging
- load config
- create `Application`
- call `run()`
- catch fatal exceptions
- return exit code

Rule:
- no business logic in `main.cpp`

---

## 3. Responsibility of `Application`
`Application` is the top-level orchestrator.

Responsibilities:
- accept ready-to-use dependencies
- perform the high-level startup flow
- log startup and shutdown
- become the integration point for future runtime modules

`Application` does **not** implement at this stage:
- SIP behavior
- call handling
- RTP loop
- API layer

---

## 4. Config Architecture
Configuration is implemented as a separate module.

Decision:
- `AppConfig` is a separate model
- `ConfigLoader` is a separate loader
- `ConfigLoader::load(path)` returns `AppConfig`

Minimal fields in `AppConfig`:
- `device_id`
- `log_level`
- `sip_port`
- `http_port`
- `ws_port`

---

## 5. Config Format
For the first implementation block we use:
- **JSON**

Reason:
- easier to start with
- less friction for the first working skeleton
- does not block a future migration to another format

File:
- `configs/app_config.json`

---

## 6. `ConfigLoader` Behavior
`ConfigLoader` must already be functional, not an empty stub.

Minimal behavior:
- read an external config file
- parse required fields
- validate basic values
- throw `std::runtime_error` on fatal errors

Not included yet:
- advanced schema validation
- layered config sources
- environment overrides

---

## 7. Logging Architecture
`Logger` must already be a working module.

Minimum requirements:
- levels: `debug`, `info`, `warn`, `error`
- console output
- timestamp
- level
- module/component tag
- message

Format:
- simple single-line text log
- no structured JSON output yet

---

## 8. Error Handling Contract
Agreed behavior:
- `ConfigLoader` throws exceptions on fatal config errors
- `Application::run()` returns `int`
- `main()` catches `std::exception`, logs fatal failure, and returns non-zero

This is the base global contract for the skeleton.

---

## 9. Threading Policy at This Stage
In this block:
- **no worker threads are started**

Architecturally:
- the future threading model must not be blocked
- everything must not be hardcoded into one monolithic class
- module boundaries must allow future SIP/RTP/API workers without major refactoring

---

## 10. Namespace
We use:
```cpp
namespace sip_intercom
```
This is more readable and professional for a portfolio project than an overly short namespace.

## 11. C++ / Build Policy
Agreed:

- C++17

- warnings: `-Wall -Wextra -Wpedantic`

- no `-Werror` at this stage

Target name:

- `sip_intercom_device_simulator`

## 12. File Structure

Agreed structure:
```bash
CMakeLists.txt
src/main.cpp

include/app/Application.hpp
src/app/Application.cpp

include/config/AppConfig.hpp
include/config/ConfigLoader.hpp
src/config/ConfigLoader.cpp

include/logging/Logger.hpp
src/logging/Logger.cpp

configs/app_config.json
```
## 13. PR Acceptance Criteria

The PR is considered ready when:

- the project builds successfully with CMake
- the executable starts successfully
- config is loaded from `configs/app_config.json`
- essential fields are validated
- the logger prints startup and shutdown messages
- the program exits with a predictable exit code

## Practical Meaning

After this decision, we do not return to repeated discussion of skeleton architecture unless there is a real technical reason.

Next steps:

1. create branch `feat/app-skeleton`

2. prepare a file-by-file plan

3. implement the block

4. commit in clean logical sub-steps



## 1. Scope блоку
У цей PR входять:
- build system
- thin `main()`
- skeleton класу `Application`
- модель `AppConfig`
- `ConfigLoader`
- `Logger`
- мінімальний example config file

У цей PR **не входять**:
- SIP parser / SIP transport
- RTP / media simulation
- HTTP / WebSocket servers
- реалізація session / state machine
- multithreaded runtime
- Docker
- production diagnostics subsystem

---

## 2. Відповідальність `main()`
`main()` відповідає лише за orchestration:
- прочитати шлях до config
- ініціалізувати базове логування
- завантажити config
- створити `Application`
- викликати `run()`
- перехопити fatal exceptions
- повернути exit code

Правило:
- жодної бізнес-логіки в `main.cpp`

---

## 3. Відповідальність `Application`
`Application` — це top-level orchestrator.

Відповідальності:
- приймати готові dependencies
- виконувати high-level startup flow
- логувати startup та shutdown
- бути точкою інтеграції для майбутніх runtime modules

`Application` **не** реалізує на цьому етапі:
- SIP behavior
- call handling
- RTP loop
- API layer

---

## 4. Архітектура config
Конфігурація реалізується як окремий модуль.

Рішення:
- `AppConfig` — окрема модель
- `ConfigLoader` — окремий loader
- `ConfigLoader::load(path)` повертає `AppConfig`

Мінімальні поля в `AppConfig`:
- `device_id`
- `log_level`
- `sip_port`
- `http_port`
- `ws_port`

---

## 5. Формат config
Для першого implementation block використовуємо:
- **JSON**

Причина:
- простіше стартувати
- менше friction для першого working skeleton
- не блокує майбутній перехід на інший формат

Файл:
- `configs/app_config.json`

---

## 6. Поведінка `ConfigLoader`
`ConfigLoader` уже має бути робочим, а не порожнім stub.

Мінімальна поведінка:
- читати external config file
- парсити required fields
- валідовувати базові значення
- кидати `std::runtime_error` при fatal errors

Поки не включаємо:
- складну schema validation
- layered config sources
- environment overrides

---

## 7. Архітектура логування
`Logger` уже має бути working module.

Мінімальні вимоги:
- рівні: `debug`, `info`, `warn`, `error`
- console output
- timestamp
- level
- module/component tag
- message

Формат:
- простий single-line text log
- без structured JSON output на цьому етапі

---

## 8. Контракт обробки помилок
Домовляємося так:
- `ConfigLoader` кидає exceptions при fatal config errors
- `Application::run()` повертає `int`
- `main()` ловить `std::exception`, логуює fatal failure і повертає non-zero

Це базовий глобальний контракт для skeleton.

---

## 9. Threading policy на цьому етапі
У межах цього блоку:
- **не запускаємо worker threads**

Архітектурно:
- не блокуємо майбутню threading model
- не зшиваємо все в один monolithic class
- будуємо boundaries так, щоб потім додати SIP/RTP/API workers без великого рефакторингу

---

## 10. Namespace
Використовуємо:
```cpp
namespace sip_intercom
```
Це читабельніше і професійніше для portfolio project, ніж занадто короткий namespace.

## 11. Політика C++ / build

Фіксуємо:

- C++17

- warnings: `-Wall -Wextra -Wpedantic`

- без `-Werror` на цьому етапі

Назва target:

`sip_intercom_device_simulator`

## 12. Структура файлів

Фіксуємо таку структуру:

```bash
CMakeLists.txt
src/main.cpp

include/app/Application.hpp
src/app/Application.cpp

include/config/AppConfig.hpp
include/config/ConfigLoader.hpp
src/config/ConfigLoader.cpp

include/logging/Logger.hpp
src/logging/Logger.cpp

configs/app_config.json
```

## 13. Acceptance criteria для PR

PR вважається готовим, якщо:

- проєкт успішно збирається через CMake

- executable успішно запускається

- config зчитується з `configs/app_config.json`

- essential fields проходять валідацію

- logger друкує startup та shutdown messages

- програма завершується з передбачуваним exit code

## Практичне значення

Після цього рішення ми не повертаємося до повторного обговорення skeleton architecture, якщо не з’явиться реальна технічна причина.

Наступні кроки:

1. створити гілку `feat/app-skeleton`

2. підготувати file-by-file plan

3. реалізувати блок

4. комітити акуратними логічними підкроками