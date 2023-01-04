#include <lely/ev/loop.hpp>
#include <lely/io2/linux/can.hpp>
#include <lely/io2/posix/poll.hpp>
#include <lely/io2/sys/io.hpp>
#include <lely/io2/sys/sigset.hpp>
#include <lely/io2/sys/timer.hpp>
#include "CameraSlave.hpp"

int main(int argc, char const *argv[])
{
    // Create an I/O context to synchronize I/O services during shutdown.
    io::Context ctx;
    // Create an platform-specific I/O polling instance to monitor the CAN bus, as
    // well as timers and signals.
    io::Poll poll(ctx);
    // Create a polling event loop and pass it the platform-independent polling
    // interface. If no tasks are pending, the event loop will poll for I/O
    // events.
    ev::Loop loop(poll.get_poll());
    // I/O devices only need access to the executor interface of the event loop.
    auto exec = loop.get_executor();
    // Create a timer using a monotonic clock, i.e., a clock that is not affected
    // by discontinuous jumps in the system time.
    io::Timer timer(poll, exec, CLOCK_MONOTONIC);
    // Create a virtual SocketCAN CAN controller and channel, and do not modify
    // the current CAN bus state or bitrate.
    io::CanController ctrl("vcan0");
    io::CanChannel chan(poll, exec);
    
    chan.open(ctrl);

    // Create a CANopen slave with node-ID 2.
    CameraSlave slave(timer, chan, "cpp-slave.eds", "", 2);
    slave.initialize();

    // Create a signal handler.
    io::SignalSet sigset(poll, exec);
    // Watch for Ctrl+C or process termination.
    sigset.insert(SIGHUP);
    sigset.insert(SIGINT);
    sigset.insert(SIGTERM);

    // Submit a task to be executed when a signal is raised. We don't care which.
    sigset.submit_wait([&](int /*signo*/) {
        // If the signal is raised again, terminate immediately.
        sigset.clear();
        // Perform a clean shutdown.
        ctx.shutdown();
    });

    // Start the NMT service of the slave by pretending to receive a 'reset node'
    // command.
    slave.Reset();

    // Run the event loop until no tasks remain (or the I/O context is shut down).
    loop.run();

    return 0;
}
