# 🟢 Display Application – Marble Detection UI

A Qt-based user interface for a real-time marble classification system.  
It visualizes system state, training progression, and inference results, allowing users to monitor and evaluate both learning and detection performance.

---

## 🧩 System Overview

The application interacts with two processing cores, each providing different data streams:

| Core | Communication | Purpose |
|------|---------------|---------|
| **HPC Core** | Named Pipe (JSON) | Training & federated learning status |
| **Management Core** | Shared Memory | System state & current marble prediction |

All communication uses **JSON**, ensuring readability and seamless integration.

---

## 🎛️ UI Modes

### **Training Mode**
- Displays expected vs. predicted marbles (sliding window visualization)
- Shows **training accuracy over time** (trend graph)
- Presents **class-wise accuracy changes** (per marble type analysis)
- Helps assess how well the system is learning

### **Inference Mode**
- Simplified layout for **real-time recognition**
- Shows the currently predicted marble
- Displays current machine state and context information

---

## 🎨 UI Components
- Dial-based **state selector** for user feedback on active mode/profile  
- Prediction box with runtime correctness indicators  
- Accuracy visualization (line and bar graphs)  
- Configurable layout and styles stored in `ui_elements/`

---

## 📁 Folder Structure

displayapplication_master/
├── src/ # Core UI & communication logic
├── include/ # Header files
├── ui_elements/ # Qt .ui files, images, stylesheets
├── config/ # JSON configuration files
└── CMakeLists.txt # Build configuration

## ⚙️ Building the Project

### Requirements
- Qt 5 or Qt 6
- C++17
- CMake
- (Optional) VTK with Qt support (for visualization extensions)
