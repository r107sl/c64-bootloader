# C64 bootloader

A C64 kernal ROM that lists the ROM images held on a One ROM and boots the one
you choose.

By default it boots the first kernal image configured on One ROM. Enter the
menu by holding down Commodore, Run/Stop or Q during booting.  One ROM stores
your choice and boots that image on subsequent boots if you don't enter the menu.

The C64 talks to the One ROM using RBCP, which carries commands from the C64 to
One ROM as ROM address reads and carries replies back in a region of the image
One ROM is serving.

The protocol is described at
[rom-bus-control-protocol](https://github.com/piersfinlayson/rom-bus-control-protocol).

## Building

Requires [cc65](https://cc65.github.io/).

```
make                  # 8KB kernal ROM          -> c64_bootloader.bin
make TARGET=c64c      # 16KB C64C combined ROM  -> c64c_bootloader.bin
```

Objects go under `build/<target>/`, so the two builds do not tread on each
other. `make clean` removes the whole `build` directory.

## Licence

MIT, see [LICENSE](LICENSE). Individual files carry their own copyright
headers.
