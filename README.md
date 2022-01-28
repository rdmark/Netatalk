# Netatalk 2.2.x
A fork of the Netatalk 2.2 codebase which takes in post-2.2.6 downstream patches from various sources, while adding some modern conveniences such as [systemd unit configurations for all daemons](https://github.com/rdmark/Netatalk/tree/branch-netatalk-2-2-x/distrib/initscripts).

The primary focus of this fork is to have a version of Netatalk 2.2.x that runs well on modern Open-Source systems, in particular Debian Linux and NetBSD.

As of Netatalk 3.0, support for Apple's legacy AppleTalk (DDP) protocol was dropped. AppleTalk is required for Macs running System 6.0 through Mac OS 7.6, as well as supported Apple II systems, to be able to connect to a Netatalk AppleShare out of the box. Additionally, AppleTalk support brings the convenience of auto-detection of available shares on a network, as well as the ability to netboot an Apple II. Hence, the demand among the vintage Apple community to keep maintaining the Netatalk 2.2 codebase.

# Installation
Follow the installation steps in the [official Netatalk 2.2 documenation](http://netatalk.sourceforge.net/2.2/htmldocs/installation.html) to configure and install Netatalk.

As supplementary information when installing on Debian derivate Linux distros, you need at least these apt packages:
```
libssl-dev libdb-dev libcups2-dev autotools-dev automake libtool libgcrypt20-dev
```

An example configuration that enables systemd and all the AppleTalk daemons, plus zeroconf for service discovery on Mac OS X (requires libavahi):
```
$ ./configure --enable-systemd --enable-ddp --enable-a2boot --enable-cups --enable-timelord --enable-zeroconf
```

# Documentation
Comprehensive [documentation for Netatalk 2.2](http://netatalk.sourceforge.net/2.2/htmldocs/) can be found on SourceForge.net
