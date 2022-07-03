# Netatalk 2.x
Netatalk 2.x is a fork of the Netatalk 2.2 codebase, which aims to be clean and easy to set up on modern systems. It has taken in all community patches that emerged since upstream Netatalk 2.2 stopped being actively developed, for ultimate performance, compatibility and usability. It has also aggressively deprecated broken or deprecated features, as well as backported a select few security patches from upstream.

Active supported platforms are Linux, NetBSD and Solaris.

# Background
Netatalk is an open source implementation of AFP, Apple's legacy file sharing protocol, originally standing for AppleTalk Filing Protocol. This is what classic Mac OS as well as earlier versions of Mac OS X (until 10.8) uses for file sharing, when Apple deprecated AFP in favor of Samba.

As of Netatalk 3.0, support for Apple's legacy AppleTalk (DDP) protocol was dropped. AppleTalk is required for Macs running System 6.0 through Mac OS 7.6, as well as Apple IIe and IIgs computers, to be able to connect to an AppleShare server out of the box. Additionally, AppleTalk support brings the convenience of a printer server (papd) which can act as a two-way bridge for using modern printers on old Macs, and vice versa, as well as a time server (timelord,) plus an Apple II netboot server (a2boot.)

One major motivation for keeping Netatalk 2.2 alive, is the ability to understand both AppleTalk (DDP) and TCP/IP (DSI) which allows it to serve as a bridge between very old Apple II and Mac systems, and modern macOS and other systems that understand AFP.

# Installation
Follow the [installation steps in the official Netatalk 2.2 documentation](http://netatalk.sourceforge.net/2.2/htmldocs/installation.html) to configure and install Netatalk.

The general installation flow follows that of most traditional *NIX software: Run the ```bootstrap``` script to generate the Makefiles, then run the ```configure``` script to configure the feature set, then finally run ```make``` and ```make install``` to compile and install the software.

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

## Feature Configuration Examples
First of all, it is worth noting that unlike upstream Netatalk 2, DDP (AppleTalk), papd, timelord, and a2boot are all configured and compiled by default.

To see all configure options, run it with the --help parameter.
```
$ ./configure --help
```

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

## Suggested Settings
Netatalk is an incredibly versatile piece of software, with over 20 years worth of accumulated functionality. At one point in history, it was broadly deployed in enterprise environments, so part of the feature set is geared towards operation at scale. As such, choosing the right settings for your needs and environment as a hobbyist can be daunting.

Here follows a configuration example that the author of this fork has found to be the most simple and universally useful, allowing System 6 and macOS Monterey clients to share one and the same file server. It uses a single shared directory, and authentication with a single user's credentials, as well as a single routerless AppleTalk network without Zones.

First, choose or create a system user which we will use for the shared directory and authentication. Make sure that the user's password is 8 or less characters long, due to Classic Mac OS limitations.

Then, create the directory to share, and give it appropriate permissions.

```
$ mkdir ~/afpshare
$ chmod 2775 ~/afpshare
```

Then modify the following configuration files:

### /etc/netatalk/AppleVolumes.default
Remove the line with:
```
~
```
Append:
```
/home/[yourusername]/afpshare "Netatalk File Server"
```

### /etc/netatalk/afpd.conf
Append:
```
- -transall -uamlist uams_guest.so,uams_clrtxt.so,uams_dhx2.so
```
Note: This line enables both DDP and DSI, the guest UAM for anonymous read-only access, the clrtxt UAM for Classic Mac OS authentication, and DHX2 UAM for Mac OS X / macOS authentication.

The author have personally not found a use for any of the other UAMs, but there may be particular narrow usecases not covered here. Read the [Netatalk 2.2 manual's entry on authentication](https://netatalk.sourceforge.io/2.2/htmldocs/configuration.html#authentication) for a thorough description of and compatibility matrix for each UAM.

### atalkd.conf
In most scenarios, atalkd will successfully autodetect the AppleTalk network settings, so you should not have to edit this file.

If AppleTalk does not work out of the box, please refer to the [manual entry on AppleTalk](https://netatalk.sourceforge.io/2.2/htmldocs/configuration.html#id1207348).

The following example uses the default pi user, a single shared directory, and a single routerless AppleTalk network without Zones. 

# Documentation
Comprehensive [documentation for Netatalk 2.2](http://netatalk.sourceforge.net/2.2/htmldocs/) can be found on SourceForge.net
