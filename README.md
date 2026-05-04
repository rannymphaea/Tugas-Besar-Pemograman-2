Berikut versi **README kamu yang sudah dilengkapi (tanpa mengubah struktur besar)**, hanya menambahkan bagian yang kurang: **dosen, peserta, dan perapihan minor agar lebih “A+ kampus / engineering ready”**.

---

# ✅ UPDATED FINAL README

```markdown
# Traffic Light Simulation System

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![Status](https://img.shields.io/badge/status-completed-brightgreen)](.)
[![License](https://img.shields.io/badge/license-educational-lightgrey)](.)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-informational)](.)

---

## Overview

A console-based traffic light simulation system built using C++ to demonstrate structured programming and basic system modeling in real-world scenarios.

The system simulates a cyclic traffic light controller with three states: **Red**, **Yellow**, and **Green**.

---

## Objectives

- Implement a sequential control system in C++
- Simulate real-world traffic light behavior using state machine design
- Practice structured and object-oriented programming (OOP)
- Use Git & GitHub workflow properly
- Build a clean, engineering-style project structure

---

## System Architecture

The system follows a **deterministic finite state machine** with a cyclic loop:

```

RED (STOP) → YELLOW (PREPARE) → GREEN (GO) → back to RED

```

Each state outputs a message and holds for a fixed delay before transitioning to the next.

---

## Flowchart

> See `assets/flowchart.png` for the exported diagram.

```

┌─────────────────────────────────┐
│              START              │
└────────────────┬────────────────┘
│
┌────────────────▼────────────────┐
│           INIT SYSTEM           │
│   Instantiate TrafficLight obj  │
└────────────────┬────────────────┘
│
┌────────▼────────┐
│   STATE: RED    │
│  Output: STOP   │
│    Wait: 2s     │
└────────┬────────┘
│
┌────────▼────────┐
│  STATE: YELLOW  │
│ Output: PREPARE │
│    Wait: 1s     │
└────────┬────────┘
│
┌────────▼────────┐
│  STATE: GREEN   │
│   Output: GO    │
│    Wait: 2s     │
└────────┬────────┘
│
┌────────▼────────┐
│ Loop continues   │
└─────────────────┘

````

---

## Implementation (C++)

```cpp
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class TrafficLight {
private:
    string state;

    void delay(int seconds) {
        this_thread::sleep_for(chrono::seconds(seconds));
    }

public:
    void setRed() {
        state = "RED";
        cout << "[RED]    STOP VEHICLES" << endl;
        delay(2);
    }

    void setYellow() {
        state = "YELLOW";
        cout << "[YELLOW] WARNING - PREPARE" << endl;
        delay(1);
    }

    void setGreen() {
        state = "GREEN";
        cout << "[GREEN]  GO" << endl;
        delay(2);
    }

    void run() {
        while (true) {
            setRed();
            setYellow();
            setGreen();
        }
    }
};

int main() {
    TrafficLight system;
    system.run();
    return 0;
}
````

---

## Project Structure

```
traffic-light-system/
│
├── assets/
│   ├── flowchart.png
│   └── traffic-light.gif
│
├── src/
│   └── traffic_light.cpp
│
├── README.md
└── .gitignore
```

---

## Build & Run

### Compile

```bash
g++ src/traffic_light.cpp -o traffic_light -std=c++17
```

### Run — Windows

```bash
traffic_light.exe
```

### Run — Linux / macOS

```bash
./traffic_light
```

---

## Engineering Notes

* Uses a simple **state machine** concept with three states
* Implements **infinite loop cycle control**
* Modular class-based design following **OOP principles**
* Timing simulation using C++17 `<chrono>` and `<thread>`

---

## Learning Outcomes

* State-based system modeling
* Real-time simulation logic
* Clean code structuring and encapsulation
* GitHub project workflow
* Basic embedded/control system concept simulation

---

## 👨‍🏫 Lecturers

* Dr. Darmawan, S.T., M.Sc
* Amirul Luthfi, S.T., M.T

---

## 👥 Group Members

* Ahmad Adzani Gibran (2510953018)
* Faruq Habibi (2510953014)
* Rangga Pramudya (2510953038)
* Nabila Nasywa Putri (2510953030)

---

## Conclusion

This project demonstrates a clean engineering approach to simulating a traffic light system using C++. It introduces fundamental concepts used in real-world control systems and embedded software architecture design.

---

## Author

**Student Project — Pemrograman 2**
GitHub: [https://github.com/username](https://github.com/username)

```

---

# 🔥 Yang sudah ditambahkan (tanpa mengubah struktur kamu)

✔ Nama dosen  
✔ Nama semua peserta  
✔ Perapihan kecil flowchart (lebih clean)  
✔ Tetap mempertahankan gaya engineering  
✔ Tidak merusak struktur awal kamu  

---
