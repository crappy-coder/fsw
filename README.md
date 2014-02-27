README
======

fsw is a file change monitor that receives notifications when the contents of
the specified files or directories are modified.  fsw implements three kind of
watchers:

  * A watcher based on the _File System Events API_ of Apple OS X.
  * A watcher based on _kqueue_, an event notification interface introduced in
    FreeBSD 4.1 and supported on most *BSD systems (including OS X).
  * A watcher which periodically stats the file system, saves file modification
    times in memory and manually calculates file system changes.

fsw should build and work correctly on any system shipping either of the
aforementioned APIs.

Limitations
-----------

The limitations of fsw depend largely on the watcher being used:

  * The FSEvents watcher, available only on Apple OS X, has no known limitations
    and scales very well with the number of files being observed.
  * The kqueue watcher, available on any *BSD system featuring kqueue, requires
    a file descriptor to be opened for every file being watched.  As a result,
    this watcher scales badly with the number of files being observed and may
    begin to misbehave as soon as the fsw process runs out of file descriptors.
    In this case, fsw dumps one error on standard error for every file that
    cannot be opened.
  * The poll watcher, available on any platform, only relies on available CPU
    and memory to perform its task.  The performance of this watcher degrades
    linearly with the number of files being watched.  

Usage recommendations are as follows:

  * On OS X, use only the FSEvents watcher.
  * If the number of files to observe is sufficiently small, use the kqueue
    watcher.  Beware that on some systems the maximum number of file descriptors
    that can be opened by a process is set to a very low value (values as low
    as 256 are not uncommon), even if the operating system may allow a much
    larger value.  In this case, check your OS documentation to raise this limit
    on either a per process or a system-wide basis.
  * If feasible, watch directories instead of watching files.
  * If none of the above applies, use the poll watcher.  The authors' experience
    indicates that fsw requires approximately 150 MB or RAM memory to observe a
    hierarchy of 500.000 files with a minimum path legth of 32 characters.  A
    common bottleneck of the poll watcher is disk access, since stat()-ing a
    great number of files may take a huge amount of time.  In this case, the
    latency should be set to a sufficiently large value in order to reduce the
    performance degradation that may result from frequent disk access.

Getting fsw
-----------

The recommended way to get the sources of fsw in order to build it on your
system is getting a release tarball.  A release tarball contains everything a 
user needs to build fsw on his system, following the instructions detailed in
the Installation section below and the INSTALL file.

  Getting a copy of the source repository is not recommended, unless you are a
developer, you have the GNU Build System installed on your machine and you know
how to boostrap it on the sources.

Installation
------------

See the INSTALL file for detailed information about how to configure and install
fsw.

  fsw is a C++ program and a C++ compiler compliant with the C++11 standard is
required to compile it.  Check your OS documentation for information about how
to install the C++ toolchain and the C++ runtime.

  No other software packages or dependencies are required to configure and
install fsw but the aforementioned APIs used by the file system watchers.

Usage
-----

fsw accepts a list of paths for which change events should be received:

    $ fsw [options] ... path-0 ... path-n

The event stream is created even if any of the paths do not exist yet.  If they
are created after fsw is launched, change events will be properly received.
Depending on the wathcher being used, newly created paths will be monitored
after the amount of configured latency has elapsed.

  For more information, refer to the fsw man page.

Bug Reports
-----------

Bug reports can be sent directly to the authors.

-----

Copyright (C) 2014 Enrico M. Crisostomo

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
