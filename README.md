# Display Application — Marble Detection UI
A Qt-based interface for visualizing system states, training progress, and inference results in a real-time marble classification system.

---

## Overview

This application provides the user interface for a hardware-driven system that classifies marbles based on their acoustic characteristics.  
It displays system state, training progression, and inference outcomes in real time using data streamed from two processing cores.

The UI supports two operational modes:

| Mode | Purpose | Key Visual Elements |
|------|---------|--------------------|
| Training Mode | Monitor learning performance | Accuracy trend chart, expected vs. predicted marble visualization |
| Inference Mode | Real-time marble classification | Live result display, system state info |

---

## System Communication

The system communicates with two separate cores:

| Core | Method | Data Provided |
|------|--------|---------------|
| HPC Core | Named Pipe (JSON) | Training epoch, federated learning epoch |
| Management Core | Shared Memory | Expected marble order, current system state, inference output |

JSON is used for structured and human-readable communication.

---

## UI Functionality

### Training Mode
- Compares **expected vs. detected** marble sequences
- Displays **accuracy growth** over time
- Shows **accuracy per marble type** (progress indicators)

### Inference Mode
- Focuses on current detection output
- Shows text-based state feedback from management core
- Uses simplified prediction visualization

---

## Project Structure
displayapplication_master/
├── src/ # Source code (logic, processing, communication)
├── include/ # Header files
├── ui_elements/ # Qt .ui files, icons, stylesheets
├── config/ # JSON configuration files
└── CMakeLists.txt # Build configuration

---

## Build Instructions

### Requirements
- Qt 5 or Qt 6
- CMake
- C++17
- (Optional) VTK with Qt support (for extended UI visualization)

