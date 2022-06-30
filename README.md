# Netatalk 2.x
A fork of the Netatalk 2 codebase which takes in post-2.2.6 downstream patches from various sources, while adding some modern conveniences such as [systemd unit configurations for all daemons](https://github.com/rdmark/Netatalk/tree/branch-netatalk-2-2-x/distrib/initscripts).

The primary focus of this fork is to have a version of Netatalk 2 that runs well on modern OSes, in particular Linux and NetBSD.

As of Netatalk 3.0, support for Apple's legacy AppleTalk (DDP) protocol was dropped. AppleTalk is required for Macs running System 6.0 through Mac OS 7.6, as well as supported Apple II systems, to be able to connect to an AppleShare server out of the box. Additionally, AppleTalk support brings the convenience of a printer server (papd) which can act as a two-way bridge for using modern printers on old Macs, and vice versa, as well as a time server (timelord,) plus an Apple II netboot server (a2boot.)

At the same time, the ability of Netatalk 2.2 to understand both AppleTalk (DDP) and TCP/IP (DSI) allows it to serve as a bridge between very old Apple II and Mac systems, and modern macOS and other systems that understand AFP.

Hence, the need among the vintage Apple community to keep maintaining the Netatalk 2.2 codebase.

# Installation
Follow the installation steps in the [official Netatalk 2.2 documentation](http://netatalk.sourceforge.net/2.2/htmldocs/installation.html) to configure and install Netatalk.

As supplementary information when installing on a Debian based Linux distros (which is the primary environment the author of this fork is using), you need at least these apt packages:
```
$ apt install libssl-dev libdb-dev autotools-dev automake libtool pkg-config
```

For *papd* printer server support, install CUPS packages.
```
$ apt install libcups2-dev cups
```

For Zeroconf (Bonjour) service discovery in Mac OS X 10.2 or later, install Avahi packages.
```
$ apt install libavahi-client-dev
```

For DHX2 authentication support, required for Mac OS X 10.2 or later, install libgcrypt.
```
$ apt install libgcrypt20-dev
```

## Configuration Examples
First of all, it is worth noting that unlike upstream Netatalk 2, DDP (AppleTalk), papd, timelord, and a2boot are all configured and compiled by default. 

For most setups, the only parameter you need is for picking the init script option for your OS. Here as an example is the *systemd* option, which will install systemd services that you can start once installation is complete. Use ```./configure --help``` to see the other init script options available.
```
$ ./configure --enable-systemd
```

If you want to use Netatalk with Mac OS X 10.2 or later, make sure you have installed *libavahi* and *libgcrypt* as per the instructions in the section above before configuring and compiling.

Mac OS X 10.0 and 10.1 uses SLP rather than Zeroconf for service discovery. Netatalk does not support SLP and Zeroconf being enabled at the same time. [See bug #7.](https://github.com/rdmark/Netatalk-2.x/issues/7)
```
$ apt install libslp-dev
$ ./configure --enable-systemd --enable-srvloc --disable-zeroconf
```

# Documentation
Comprehensive [documentation for Netatalk 2.2](http://netatalk.sourceforge.net/2.2/htmldocs/) can be found on SourceForge.net
