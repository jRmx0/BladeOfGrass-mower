# BladeOfGrass-mower
An ESP32-S3-based autonomous lawn mower controller that leverages GNSS positioning, sensors, and user inputs for precision grass cutting without perimeter wires.

## Project Goal
This controller executes all operational aspects from route planning to motor control using three primary data sources:

| Data source      | Functional Purpose |
| ---------------- | - |
| Internet         | Wi-Fi connection for ThingsBoard integration enabling remote monitoring and control |
| RTK              | Centimeter-level location tracking for accurate navigation |
| On-board sensors | <ul> <li>Location interpolation between GNSS updates</li> <li>Obstacle detection and avoidance</li> <li>System status monitoring</li> </ul> |

The mower navigates using centimeter-accurate positioning from the UM980 GNSS module, maintaining location awareness between updates by calculating position from speed and directional data.

Users simply define a virtual boundary, and the mower automatically:
- Plans and executes efficient cutting paths
- Manages multi-session mowing for large areas
- Returns to the charging station when needed

## Project Timeline
For a detailed view of the project roadmap and development progress, please see [TIMELINE](./TIMELINE.md) file.

## License
This project is licensed under the terms of the [LICENSE](./LICENSE) file.