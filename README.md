compile with:
```bash
 cmake . -B build && cmake --build build
```
# isys_canopen_example
The isys_example project is a minimally modified version of the tutorial slave to have a worker thread running in the background to be used for tasks like reading an IMU or processing camera data. Currently just random numbers are generated. This can be run with the master executable from the tutorial.

The CameraSlave has parents `Device`, `Node`, and `BasicSlave`. You can find the most useful functions for the `BasicSlave` class here at the documentation [BasicSlave](https://lely_industries.gitlab.io/lely-core/doxygen/classlely_1_1canopen_1_1BasicSlave.html).

# tutorial
The tutorial project contains the tutorial found at [lely](https://opensource.lely.com/canopen/docs/cpp-tutorial/).


