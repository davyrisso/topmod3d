# topmod3d

## Build


### Pre-requisites

Register an account at [qt.io](https://www.qt.io/) (yes, you have to).


### MacOS X

**Note**: Tested on MacOS X Ventura 13.5

Build steps:
 1. Download and install [Qt 5.13](qt-opensource-mac-x64-5.13.0.dmg) at https://download.qt.io/archive/qt/5.13/5.13.0/.
    - Alias `qmake` with the local installation (by default `/Users/[user]/Qt5.13.0/5.13.0/clang_64/bin/qmake`)
 2. Clone this repo:
    - `$ git clone git@github.com:davyrisso/topmod3d.git`
    - `$ cd topmod3d`
 3. Build:
    - `$ qmake -makefile`
    - `$ make`
 4. Launch `topmod.app`

 Alternatively, use the open source Qt Creator bundled in Qt 5.13 and open `TopMod.pro`.

 
 ### Linux

 **Note**: Tested on 
  - Ubuntu 22.04 LTS

Build steps:
   1. Install Qt 5.13
      1. Download the Qt unified installer from https://download.qt.io/archive/online_installers/4.6/qt-unified-linux-x64-4.6.0-online.run
         - `$ wget https://download.qt.io/archive/online_installers/4.6/qt-unified-linux-x64-4.6.0-online.run`
         - `$ chmod +x qt-unified-linux-x64-4.6.0-online.run`
      2. Install dependencies:
         - `$ sudo apt-get install libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-render-util0 libxcb-xinerama0 libxcb-xkb-dev libxkbcommon-x11-0`
      3. Install Qt5 5.13
         - `$ ./qt-unified-linux-x64-4.6.0-online.run install qt.qt5.5130 --accept-licenses --confirm-command --accept-messages --email [Qt account email] --pw [Qt account password]`
      4. Alias `qmake` with the local installation (by default `/home/[user]/Qt/5.13.0/gcc_64/bin/qmake`)
   2. Install dependencies:
      - `$ sudo apt-get install libgl-dev libglu-dev`
   3. Clone this repo:
      - `$ git clone git@github.com:davyrisso/topmod3d.git`
      - `$ cd topmod3d`
   4. Build:
      - `$ qmake -makefile`
      - `$ make`

 Alternatively, use the open source Qt Creator bundled in Qt 5.13 and open `TopMod.pro`.
