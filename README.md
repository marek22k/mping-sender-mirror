<!--
SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# mping-sender

[![pipeline badge](https://ci.codeberg.org/api/badges/13147/status.svg)](https://ci.codeberg.org/repos/13147)
[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/11574/badge)](https://www.bestpractices.dev/projects/11574)

mping-sender is a simple multicast transmitter that sends a UDP packet every second.

mping-sendet is a multicast sender designed to provide a simple multicast source that periodically sends out UDP packets to test multicast functionality.

mping-sendet sends a UDP packet every second. The source and destination can be specified using command line arguments.
The UDP packet attempts to follow large parts of the [mping](https://github.com/troglobit/mping/tree/main) implementation with an empty payload.

## Building

See [BUILDING.md](BUILDING.md).

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## Security

On Linux, mping-sender uses three sandboxing technologies or restriction technologies: Capabilities, seccomp and landlock, systemd, and AppArmor.
These serve to limit the extent of compromise if mping-sender is compromised by an attack.

### Capabilities

In Linux, capabilities are used to restrict what a program is allowed to do, especially when interacting with the system. mping-sender requires one capability: `CAP_SETPCAP`.
`CAP_SETPCAP` is required to restrict its own capabilities.
systemd and AppArmor ensure on Linux that mping-sender does not receive any other capabilities.
mping-sender drops all granted capabilities, as no special rights are required to send UDP packets via the operating system interface.
Furthermore, libcap-ng performs a "lock": If supported, NoNewPriv and securebits are set.

### Seccomp

mping-sender uses the boost library. Boost use syscalls. However, it is not documented which ones boost use. Creating a whitelist - especially one that is distribution-independent - has therefore proven difficult.
For this reason, seccomp is used to blacklist syscalls that mping-sender does not need.

### Landlock

Landlock can restrict what a program can access. At the beginning, a complete landlock sandbox is created. Only necessary permissions are granted - currently none.

Landlock offers backward compatibility with older kernels through a dynamic ABI query. However, to reduce maintenance effort, a certain Landlock version is required. This should be based on Debian.

### AppArmor

AppArmor restricts mping-sender by determining what mping-sender is allowed to do. Unnecessary operations are therefore blocked by AppArmor.

### systemd

mping-sender comes with a hardened systemd unit that restricts many accesses not used by mping-sender.

See [SECURITY.md](SECURITY.md).

## How it works?

mping-sender uses several libraries to send periodic UDP packets.

Libraries used:

- [Boost.Log](https://www.boost.org/): This library is used as a logger. Various log levels can be set in the configuration file.
- [Boost.Program_options](https://www.boost.org/): This library is used to parse the command line options.
- [Boost.Asio](https://www.boost.org/): This library is used to communicate with the socket of the TAP device and to receive and send several packets asynchronously.

Optional libraries:

- [libcap-ng](https://people.redhat.com/sgrubb/libcap-ng/): Used to discard unnecessary capabilities and restrict the program.
- [libseccomp](https://github.com/seccomp/libseccomp): Used to block potentially dangerous system calls.
- [landlock](https://landlock.io/): Used to restrict access.

Here is how the program works:

1. reading the command line parameters
2. setting the log level
3. output of the versions
4. creation of the UDP socket
5. starting timer

The following happens when the timer triggers:

1. the timer is reset.
2. a UDP packet is sent with the current time.
3. the sequence number is increased by one.

## Configuration

```
mping-sender

Usage: mping-sender [configuration]

Configuration:
  --help                    Help screen
  --log-level arg (=info)   Log level. Valid options are trace, debug, info,
                            warning, error and fatal. Defaults to info.
  --bind-address arg        Address to which the UDP socket is bound. Required.
  --bind-port arg (=4321)   Port to which the UDP socket is bound. Defaults to
                            4321.
  --address arg (=ff2e::42) Address to send packets to. Defaults to ff2e::42
  --port arg (=4321)        Address to which the packages are sent.
  --ttl arg (=@)            TTTL or hop limit with which the packets are sent.
                            Defaults to 64.
  --interface-name arg      Name of the interface from which the packets are
                            sent. Required.

```

The log level can have one of the following values:

- `trace`
- `debug`
- `info`
- `warning`
- `error`
- `fatal`

For production, "info" or lower is recommended.

## Notes for myself

### Create a new release

To create a new release the following is necessary:

- Create a changelog entry
- Create a changelog entry for the Debian package
- Update the SECURITY.md
- Create a new git tag
