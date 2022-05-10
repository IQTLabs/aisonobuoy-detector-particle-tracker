
# Particle Tracker Buoy Firmware

## About

A sensor buoy based on the Particle Tracker reference application.

Part of the [AISonobuoy](https://github.com/IQTLabs/AISonobuoy) project.

## Compile and Flash with VS Code / Particle Workbench extension

1. Clone this repository `$ git@github.com:IQTLabs/aisonobuoy-particle-tracker.git && cd ./aisonobuoy-particle-tracker`
2. Init & Update Submodules `$ git submodule update --init --recursive`
3. Open Particle Workbench
4. Run the `Particle: Import Project` command, follow the prompts, and wait for the project to load
5. Run the `Particle: Configure Workspace for Device` command and select a compatible Device OS version and the `tracker` platform when prompted ([docs](https://docs.particle.io/tutorials/developer-tools/workbench/#cloud-build-and-flash))
6. Connect your device
7. Compile & Flash!

## Wiring

```
┌───────────────┐     ┌────────────┐     ┌────────────┐     ┌───────────┐     ┌────────────┐
│   [SATCOM]    │     │[C&C /TELEM]│     │  [ML MCU]  │     │[AUDIO AMP]│     │[HYDROPHONE]│
│               │     │            │     │            │     │           │     │            │
│    Myriota    │     │  Particle  │     │  nRF52840  │     │  MAX4466  │     │  Aquarian  │
│      Dev      │     │  Tracker   │     │   Sense    │     │           │     │    H2A     │
│      Kit      │     │            │     │            │     │           │     │            │
│               │     │            │     │            │     │           │     │            │
│               │     │            │     │            │     │           │     │            │
│   26/LEUART TX├─────┤UART RX     │     │          A0├─────┤VCC        │     │            │
│   28/LEUART RX├─────┤UART TX     │     │          A1├─────┤OUT────────┼─────┤RED         │
│               │     │            │     │            │     │           │     │            │
│               │     │      A0/SDA├───┬─┤A4/SDA      │     │        MIC├─────┤GREEN       │
│               │     │            │   │ │            │     │           │     │            │
│               │     │      A1/SCL├─┬─┼─┤A5/SCL      │     │        GND├─────┤BLACK       │
│               │     │            │ │ │ │            │     │           │     │            │
│               │     │            │ │ │ │            │     │           │     │            │
│ VEXT          │     │            │ 1 1 │            │     │           │     │            │
│ /3.3      GND │     │ 3.3    GND │ 0 0 │ 3.3    GND │     │      GND  │     │            │
└──┬─────────┬──┘     └──┬──────┬──┘ k k └──┬──────┬──┘     └───────┬───┘     └────────────┘
   │         │           │      │    │ │    │      │                │
   │                     │           │ │    │
   └─────────────────────┴───────────┴─┴────┘
```
## Hardware

![model](https://github.com/IQTLabs/AISonobuoy/blob/main/ParticleTrackerBuoy/media/4in-PVC-Particle-Tracker-Buoy.gif?raw=true)

Hardware details can be found here: [AISonobuoy](https://github.com/IQTLabs/AISonobuoy)

## Particle Tracker Reference Application

- [Latest Release](https://github.com/particle-iot/tracker-edge/releases)