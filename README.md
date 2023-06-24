# Netatalk
Netatalk is an implementation of "AFP over TCP".
Netatalk also support the AppleTalk Protocol Suite for legacy Macs.
The current release contains support for EtherTalk Phase I and II, 
DDP, RTMP, NBP, ZIP, AEP, ATP, PAP, ASP, AFP and DSI.
The complete stack looks like this on a BSD-derived system:

```
    AFP                          AFP
     |                            |
    ASP    PAP                   DSI
      \   /                       |
       ATP RTMP NBP ZIP AEP       |
        |    |   |   |   |        |
   -+---------------------------------------------------+- (kernel boundary)
    |                    Socket                         |
    +-----------------------+------------+--------------+
    |                       |     TCP    |    UDP       |
    |          DDP          +------------+--------------+
    |                       |           IP              |
    +-----------------------+---------------------------+
    |                Network-Interface                  |
    +---------------------------------------------------+
```

DSI is a session layer used to carry AFP over TCP.
DDP is in the kernel.  "atalkd" implements RTMP, NBP, ZIP, and AEP.  It
is the AppleTalk equivalent of Unix "routed".  There is also a
client-stub library for NBP.  ATP and ASP are implemented as
libraries.  "papd" allows Macs to spool to "lpd", and "pap" allows Unix
machines to print to AppleTalk connected printers.  "psf" is a
PostScript printer filter for "lpd", designed to use "pap".  "psorder"
is a PostScript reverser, called by "psf" to reverse pages printed to
face-up stacking printers.  "afpd" provides Macs with an interface to
the Unix file system.  Refer to the appropriate man pages for
operational information.

# Netatalk 2.x
Netatalk 2.x is a fork of the Netatalk 2.2 codebase which aims to be clean, safe and easy to set up on modern systems. It has also aggressively deprecated broken or long outdated features.

Actively supported platforms are Debian Linux, NetBSD and Solaris.

# Installation
Follow the [installation instructions in the wiki](https://github.com/rdmark/Netatalk-2.x/wiki/Chapter-2.-Installation) to configure and install Netatalk.

The general installation flow follows that of most traditional *NIX software: Run the ```bootstrap``` script to generate the Makefiles, then run the ```configure``` script to configure the feature set, then finally run ```make``` and ```make install``` to compile and install the software.

## Recommended packages
### Debian Linux
**For Debian Linux (and derivates), there is an [automated installation script](https://github.com/rdmark/Netatalk-2.x/blob/branch-netatalk-2-x/contrib/shell_utils/debian_install.sh) that configures and installs the universal Netatalk setup described below.**

```
$ apt install libssl-dev libdb-dev autotools-dev automake libtool libtool-bin pkg-config
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

### Fedora Server
```
$ dnf install openssl-devel libgcrypt-devel libdb-devel automake libtool avahi-devel cups-devel
```

### OmniOSce
**Note:** These instructions may apply also to other OpenSolaris / Illumos distros, or even to Solaris proper.

```
$ pkg install git libtool automake gcc7 bdb
```

## Bootstrap
**Note:** For systems running legacy versions of autotools, you may need to replace the first few lines of ```configure.ac``` with the commented out legacy syntax before running the ```bootstrap``` script. Has been confirmed on f.e. Ubuntu 18.04 and earlier, and Illumos (Tribblix 0m28).

First of all, it is worth noting that unlike upstream Netatalk 2, DDP (AppleTalk), papd, timelord, and a2boot are all configured and compiled by default.

For most setups, the only parameter you need is for picking the init script option for your OS. Here as an example is the cross-platform *systemd* option, which will install systemd services that you can start once installation is complete. Run ```./configure --help``` to see the other init script options available.
```
$ ./configure --enable-systemd
```

### SLP
If you want to use Netatalk with Mac OS X 10.2 or later, make sure you have installed `libavahi` and `libgcrypt` as per the instructions in the section above before configuring and compiling.

Mac OS X 10.0 and 10.1, however, uses SLP (Service Location Protocol) rather than Zeroconf for service discovery. Netatalk does not support SLP and Zeroconf being enabled at the same time. [See bug #7.](https://github.com/rdmark/Netatalk-2.x/issues/7)

On Debian f.e., the SLP library package is called `libslp-dev`.

After installing the package, configure Netatalk with `--enable-srvloc --disable-zeroconf`.

### OmniOSce
You need to point to the location where the Berkely DB libraries and headers are installed, e.g. for OmniOSce:

```
./configure --with-bdb=/opt/ooce
```

## Example Configuration
Netatalk is an incredibly versatile piece of software, with over 30 years worth of accumulated functionality. Up until the time Apple deprecated AFP, it was deployed in enterprise and educational environments, so part of the feature set is geared towards operation at scale. As such, choosing the right settings for your needs and environment as a hobbyist can be daunting.

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

The author have personally not found a use for any of the other UAMs, but there may be particular narrow usecases not covered here. Read the [Netatalk 2.2 manual's entry on authentication](https://github.com/rdmark/Netatalk-2.x/wiki/Chapter-3.-Setting-up-Netatalk#authentication) for a thorough description of and compatibility matrix for each UAM.

### atalkd.conf
In most scenarios, atalkd will successfully autodetect the AppleTalk network settings, so you should not have to edit this file.

If AppleTalk does not work out of the box, please refer to the [manual entry on AppleTalk](https://github.com/rdmark/Netatalk-2.x/wiki/Chapter-3.-Setting-up-Netatalk#appletalk).
