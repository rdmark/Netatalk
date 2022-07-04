# Netatalk 2.x
Netatalk 2.x is a fork of the Netatalk 2.2 codebase, which aims to be clean and easy to set up on modern systems. It has taken in all community patches that emerged since upstream Netatalk 2.2 stopped being actively developed, for ultimate performance, compatibility and usability. It has also aggressively deprecated broken or deprecated features, as well as backported a select few security patches from upstream.

Actively supported platforms are Linux, NetBSD and Solaris.

# Background
Netatalk is an open source implementation of AFP, Apple's legacy file sharing protocol, originally standing for AppleTalk Filing Protocol. This is what classic Mac OS as well as earlier versions of Mac OS X (until 10.8) uses for file sharing, when Apple deprecated AFP in favor of Samba.

As of Netatalk 3.0, support for Apple's legacy AppleTalk (DDP) protocol was dropped. AppleTalk is required for Macs running System 6.0 through Mac OS 7.6, as well as Apple IIe and IIgs computers, to be able to connect to an AppleShare server out of the box. Additionally, AppleTalk support brings the convenience of a printer server (papd) which can act as a two-way bridge for using modern printers on old Macs, and vice versa, as well as a time server (timelord,) plus an Apple II netboot server (a2boot.)

One major motivation for keeping Netatalk 2.2 alive, is the ability to understand both AppleTalk (DDP) and TCP/IP (DSI) which allows it to serve as a bridge between very old Apple II and Mac systems, and modern macOS and other systems that understand AFP.

# Installation
**For Debian Linux (and derivates) users, there is an [automated installation script](https://github.com/rdmark/Netatalk-2.x/blob/branch-netatalk-2-x/contrib/shell_utils/debian_install.sh) that configures and installs the universal Netatalk setup described below.**

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

## Print Server
Netatalk provides an AppleTalk compatible printer server daemon called papd. It can leverage the CUPS backend to share modern printers with vintage Macs.

In order to use it, install the CUPS backend on your system before configuring Netatalk.

```
$ sudo apt install cups
```

This guide will not cover all scenarios for setting up CUPS, since [CUPS's own documentation](http://www.cups.org/doc/admin.html) should serve this purpose. However, we will cover some key steps and caveats that are specific to the Netatalk setup.

Once you have installed CUPS packages and started the daemons, or enabled the systemd services for CUPS (including the cups-browser web interface, for convenience), you will want to add yourself to the CUPS admin group, and set a few options to make remote administration possible. Skip these steps if you want a more secure setup.

```
$ sudo usermod -a -G lpadmin $USER
$ cupsctl --remote-admin WebInterface=yes
```

You may want to tweak the settings in ```/etc/cups/cupsd.conf``` to suite your environment, and tastes. Two recommended tweaks are to enable log rotation, and to disable the indefinite storing of print jobs, to avoid continuous increase in disk space taken up.

```
MaxLogSize 1000
PreserveJobHistory No
```

Note: In the author's environment TCP port 631 (CUPS Web Interface default) was not open, so you may have to either edit ```/etc/cups/cupsd.conf``` or open the port in your firewall if you want to use the CUPS Web Interface.

Once you have configured everything to your satisfaction, just start or restart the cups service. If the package installer didn't enable the services for you, you may need to use systemctl to reload daemons and enable new services first. 

### Sharing a modern printer over AppleTalk

Note that for this to work, your printer must be CUPS (Apple AirPrint) compatible. At the time of writing, the majority of modern printers are compatible. You can test this by after going through the steps above pipe some text to lp. First, list the printers that CUPS can see, then configure the default printer (HP Tango in my case), finally pipe a stream to lp for printing.

```
$ lpstat -p -d
printer HP_Tango_333AB4_ is idle.  enabled since Fri 07 Jan 2022 06:47:09 PST
system default destination: HP_Tango_333AB4_
$ lpoptions -d HP_Tango_333AB4_
device-uri=ipps://HP%20Tango%20%5B333AB4%5D._ipps._tcp.local/ printer-info='HP Tango [333AB4]' printer-location printer-make-and-model='HP Tango' printer-type=16781324
$ echo "TESTING" | lp
request id is HP_Tango_333AB4_-3 (0 file(s))
```

The next step is to configure papd. Edit ```/etc/netatalk/papd.conf``` and add the following line to the bottom of the file:

```
cupsautoadd:op=root:
```

Save, exit, and restart papd.

At this stage, you want to test that the CUPS printer is shared over AppleTalk. This can be done using this command:

```
$ nbplkup
```

The output should look something like this:

```
                     rascsi3b:ProDOS16 Image                     65280.205:3
             HP Tango 333AB4 :LaserWriter                        65280.205:130
                     rascsi3b:Apple //e Boot                     65280.205:3
                     rascsi3b:AFPServer                          65280.205:131
                     rascsi3b:TimeLord                           65280.205:129
                     rascsi3b:Apple //gs                         65280.205:3
```

If you see your printer here, the only thing left is to configure the printer on your vintage Mac in the Chooser. This part may vary depending on AppleTalk version, printer drivers on your system, and model and make of printer. The author has tested their HP Tango printer with LaserWriter 7 on System 7.1.1, and LaserWriter 8 on Mac OS 8.6:

1. Pick the LaserWriter driver in the Chooser. Your printer should appear in the list of PostScript printers.
    1. If using LaserWriter 7 or earlier, just selecting the printer should be enough.
    2. If using LaserWriter 8, click Setup.
2. When given the option to pick PPD (printer description file) choose plain LaserWriter from the list.
3. Print!

### Troubleshooting
If the LaserWriter driver fails to properly spool the print job to the printer, you may want to try to configure is differently through CUPS. Here it is helpful to have the web interface ready. Go to the Administration page, Add Printer, then pick the printer you want to share from the list of detected ones. Note that one printer may expose several interfaces, so choose the one that seems the most likely to work with LaserWriter spool queues. Afterwards, check with the nbplkup command which printers are available over AppleTalk, and have a look in the Chooser if a different printer has been detected now.

You may also try different PPD files in the Chooser and see it that makes a difference. 

## Time Server
Netatalk also comes bundled with a [Timelord](https://web.archive.org/web/20010303220117/http://www.cs.mu.oz.au/appletalk/readmes/TMLD.README.html) compatible daemon that can be used to continuously synchronize vintage Macs' system clocks, enabling you to operate them without a PRAM battery, for instance.

Once the timelord daemon is running, you'd want to install the [Tardis Chooser extension](https://macintoshgarden.org/apps/tardis-and-timelord) on your Mac and reboot. Now you should have a 'tardis' option in the Chooser, from where you can choose available time servers to sync with. The extension will automatically sync the system clock on bootup against the chosen server.

## Apple II Netboot
Netatalk comes with the a2boot daemon for netbooting Apple //e and //gs computers.

The [A2SERVER](http://ivanx.com/a2server/) project has comprehensive information on a2boot. 

# Connect to AppleShare from a Mac client
This section provides some pointers to how to connect to the AppleShare server provided by Netatalk.

## AppleTalk
AppleTalk is compatible with the oldest version of the Macintosh System Software that supported networking, as well as network enabled Apple II and Lisa systems, up until Mac OS X 10.8.

AFP over classic AppleTalk (DDP) is plug and play, with available servers detected automatically if the ```atalkd``` daemon is running and has registered the AFP server.

On System 6.0.x and later, open up Chooser and select AppleTalk. The file server should be detected automatically.

On Mac OS X 10.8 or earlier, open the Finder and select Network from the left drawer. The file server should be detected automatically. 

## TCP/IP
AFP over TCP (DSI) can be used on Mac OS 7.1 and later. Mac OS 8.1 and later supports AFP over TCP out of the box, but on older versions you need to install [AppleShare Client 3.7.4](https://macintoshgarden.org/apps/appleshare-client-372) (or later.) You may also need to install [Open Transport 1.3](https://macintoshgarden.org/apps/open-transport-13-gold-master) if a compatible version of OT is not already installed.

Mac OS X 10.0 onwards supports service discovery for AFP over TCP, but in Classic Mac OS you have to enter the IP address to the AFP file server manually in the Chooser.

Note that the AppleShare Client 3.7.4 installer will refuse to install on a 7.1 System, so you will have to copy the AppleShare extension over manually.

## Mac Emulators
The AFP server can be accessed also from within a Mac emulator with a network bridge, such as Basilisk II.

In Basilisk II, make sure you configure the emulator with the slirp network interface, and install the AppleShare Client / Open Transport software on the emulated system as instructed above. In the TCP/IP control panel, configure DHCP and make sure you can ping the host with OTTool or similar utility. At this point, you should be able to reach the shared drive through TCP, if not AppleTalk.

# See Also
Comprehensive [documentation for Netatalk 2.2](http://netatalk.sourceforge.net/2.2/htmldocs/) can be found on SourceForge.net
