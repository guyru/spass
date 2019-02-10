==============
Readme - spass
==============
Secure password/passphrase generator.

Authors
=======
* Author: `Guy Rutenberg`_
* Word list by `Arnold G. Reinhold`_

.. _`Guy Rutenberg`: http://www.guyrutenberg.com
.. _`Arnold G. Reinhold`: http://world.std.com/~reinhold/index.html

Usage
=======
By default spass will generate passwords. You can control the length using the
``-l`` modifier. Examples::

  $ spass
  mqGP0GEZ
  $ ./spass -l 10
  @-9Me6VNnT

If you pass the ``-p`` modifier, spass will output a passphrase instead of a
password::

  $ ./spass -p -l 4
  pecan suey faith signor

For more options see ``spass --help``.

Installation
=============
Dependencies
------------
 * CMake-2.8
 * PortAudio
 * libasound2-dev
 * libboost-program-options-dev
 * libqt4-dev
Linux
-----
Just run::

  mkdir build
  cd build
  cmake ..
  make
  sudo make install

Or use one of the provided binary packages.

Windows
-------
Compiling Boost and PortAudio from source is easy. PortAudio uses CMake (just
generate and compile), and compiling boost is a matter of: ``bootstrap`` and
``.\b2``.

If you use Visual Studio Express 2012, you may need to set the ``Boost_COMPILER``
variable in CMake to ``-vc110``, else it won't be detected properly.

In order to select the right Qt installation, set ``QT_QMAKE_EXECUTABLE`` to
point to the correct path to ``qmake.exe``. CMake will use it to infer the rest
of the paths.

Due to a bug in CMake (actually a regression) setting the SubSystem doesn't work
as it should. It should be set manually in the Project
Properties->Linker->System to "Windows" (at least for release).

Configuration
-------------
``cmake`` can receive the following options::

  WITH_PORTAUDIO          use PortAudio as the audio backend instead of ALSA
                          [default=OFF]
  WITH_OSS                use Open Sound System (OSS) as the audio backend
                          instead of ALSA [default=OFF]
  WITH_ALSA               use Advanced Linux Sound Architecture (ALSA) as the
                          audio backend [default=ON]

The options are exclusive, e.g. only one of them can be used each time. If no
option is provided, the ALSA is used.

Security
========
The security of a password generator like spass, is determined by the quality of
its underlying random number generator. spass employs a true random number
generator based on noise obtained via the microphone. The noise goes through an
unbiasing phase and then every 512 bits are compressed into 128 bits using a
hash function. After those two phases the output bits should have full entropy.

See also `Audio Based True Random Number Generator POC`__.

__ http://www.guyrutenberg.com/2010/05/14/audio-based-true-random-number-generator-poc/

The words for the passphrases are taken from a list containing 8192 words.
Hence, each word provides 13 bits of entropy.

Caveats
-------
The current implementation can't open the audio device for capture in an
exclusive mode (due to ALSA limitation). This means that theoratically, if you
use spass on a system with a malicious user, he could record the same noise
you're using and guess the output of spass. This means that you've to
be in full control of the system your using spass on. While this may seem
problematic, in practice it's a requirement for every other cryptography related
application as well.
