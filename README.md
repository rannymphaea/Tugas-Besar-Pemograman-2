````markdown id="engineer_final_full"
# Traffic Light Simulation System (C++)

## Course: Pemrograman 2

This project is a group assignment for the Pemrograman 2 course. It implements a console-based traffic light simulation using C++ to demonstrate structured programming concepts, sequential logic, and real-world system modeling.

---

## 1. Project Background

Traffic light systems are essential components in transportation infrastructure. They regulate vehicle flow at intersections to ensure safety and efficiency.

This project simulates a simplified version of a traffic light system using a console-based program. The simulation represents real-world behavior through sequential signal switching.

---

## 2. Objectives

The objectives of this project are:

- To simulate a traffic light system using C++
- To apply structured programming concepts (sequence, selection, iteration)
- To understand real-world system modeling
- To improve algorithmic thinking and problem-solving skills
- To develop teamwork and collaboration in software development
- To use Git and GitHub for version control

---

## 3. System Overview

The system simulates three main traffic light states:

- Red Light: Vehicles must stop  
- Yellow Light: Transition / warning phase  
- Green Light: Vehicles may proceed  

The system runs continuously in a loop to mimic real traffic behavior.

---

## 4. System Architecture

### Program Flow

1. Initialize system  
2. Set state to RED  
3. Wait for duration  
4. Set state to YELLOW  
5. Wait for duration  
6. Set state to GREEN  
7. Wait for duration  
8. Repeat cycle  

---

## 5. Flowchart (System Design)

```mermaid id="flow_final"
flowchart TD
    A[Start Program] --> B[Initialize System]

    B --> C[RED Light ON]
    C --> D[Display: STOP]
    D --> E[Delay]

    E --> F[YELLOW Light ON]
    F --> G[Display: WARNING]
    G --> H[Delay]

    H --> I[GREEN Light ON]
    I --> J[Display: GO]
    J --> K[Delay]

    K --> L[Loop Back]
    L --> C
````

---

## 6. Code Structure (OOP Upgrade Version)

To make the system more modular and professional, the design can be extended using Object-Oriented Programming:

```cpp id="oop_version"
class TrafficLight {
private:
    string state;

public:
    void setRed() {
        state = "RED";
        cout << "STOP" << endl;
    }

    void setYellow() {
        state = "YELLOW";
        cout << "WARNING" << endl;
    }

    void setGreen() {
        state = "GREEN";
        cout << "GO" << endl;
    }

    void runCycle() {
        while (true) {
            setRed();
            // delay
            setYellow();
            // delay
            setGreen();
            // delay
        }
    }
};
```

---

## 7. Sample Program Output

````
=============================
 TRAFFIC LIGHT SIMULATION
=============================

[RED LIGHT]
STOP - Vehicles must stop
Waiting...

[YELLOW LIGHT]
WARNING - Prepare to stop or go
Waiting...

[GREEN LIGHT]
GO - Vehicles may proceed
Waiting...

(repeats continuously)
``` id="output_final"

---

## 8. Technology Stack

- Language: C++
- IDE: Visual Studio Code
- Compiler: GCC / MinGW
- Version Control: Git & GitHub

---

## 9. Project Structure

````

ngoding bersama wapres/
├── src/
│   └── traffic_light.cpp
├── README.md
└── .gitignore

````id="structure_final"

---

## 10. Group Members

- Ahmad Adzani Gibran (2510953018)  
- Faruq Habibi (2510953014)  
- Rangga Pramudya (2510953038)  
- Nabila Nasywa Putri (2510953030)  

---

## 11. Lecturers

- Dr. Darmawan, S.T., M.Sc  
- Amirul Luthfi, S.T., M.T  

---

## 12. Build and Run Instructions

### Compile
```bash
g++ src/traffic_light.cpp -o traffic_light
``` id="compile_final"

### Run (Windows)
```bash
traffic_light.exe
``` id="run_win_final"

### Run (Linux / macOS)
```bash
./traffic_light
``` id="run_linux_final"

---

## 13. Learning Outcomes

- Implementation of sequential control systems in C++
- Understanding real-world simulation modeling
- Application of structured and object-oriented programming concepts
- Experience using Git and GitHub for version control
- Collaborative software development workflow

---

## 14. Conclusion

This project demonstrates how a simple traffic light system can be modeled using programming concepts. It bridges theoretical learning with real-world simulation and introduces structured software engineering practices.

The extended object-oriented design provides a foundation for more advanced systems such as adaptive traffic control and multi-intersection simulations.
````

---
