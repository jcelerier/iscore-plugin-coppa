# iscore-plugin-coppa
coppa plugin for i-score

Allows use of OSCQuery in the i-score sequencer.

This is achieved by making a wrapping of the coppa api by the OSSIA api. 

The wrapping is still incomplete and a lot of things won't work.


## Requirements

* See coppa requirements : https://github.com/jcelerier/coppa/blob/master/README.md

## Setup

    $ git clone https://github.com/jcelerier/iscore-plugin-coppa
    $ cd iscore-plugin-coppa
    $ git submodule update --init --recursive
    $ (cd coppa/jeayeson; ./configure)
    
    
## Building
    $ mkdir build; cd build
    $ export JAMOMA_CMAKE_PATH=/usr/local/jamoma/share/cmake/Jamoma/
    $ export QT5_CMAKE_PATH=/home/jcelerier/Qt5.5.0/5.5/gcc_64/lib/cmake/Qt5
    $ export KDNSSD_CMAKE_PATH=/usr/lib/x86_64-linux-gnu/cmake/KF5DNSSD
    $ cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_PREFIX_PATH="$JAMOMA_CMAKE_PATH;$QT5_CMAKE_PATH;$KDNSSD_CMAKE_PATH" -DCMAKE_BUILD_TYPE=Debug -DISCORE_USE_DEV_PLUGINS:Bool=True -DEXAMPLES:Bool=True
    $ make -j8
    
## Testing

    $ cd build/coppa
    $ ./zc_server # ZeroConf test server
    
And open i-score, add a new OSCQuery device, and browse (the default ip will work, too).
