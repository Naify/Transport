#!/bin/bash

FAKE_ROOT=`which fakeroot`
BUILD_DIR=./build
DISTRIB_DIR=./distrib
RELEASE_DIR=./build/bin
DEB_DIR=./deb
DEB_BIN_DIR=./deb/usr/local/transport
DEB_CTRL_FILE=./deb/DEBIAN/control

echo "Creating Transport control package..."

if [ -d $BUILD_DIR ]; then
    echo "Deleting directory "$BUILD_DIR"..."
    /bin/rm -rf $BUILD_DIR
    if [ $? -ne 0 ]; then
        echo "Error deleting directory "$BUILD_DIR""
        exit 1
    fi
fi

if [ -d $DISTRIB_DIR ]; then
    echo "Deleting directory "$DISTRIB_DIR"..."
    /bin/rm -rf $DISTRIB_DIR
    if [ $? -ne 0 ]; then
        echo "Error deleting directory "$DISTRIB_DIR""
        exit 1
    fi
fi
        
if [ -d $DEB_BIN_DIR/transport_control ]; then
    echo "Deleting directory "$DEB_BIN_DIR"..."
    /bin/rm -rf $DEB_BIN_DIR
    if [ $? -ne 0 ]; then
        echo "Error deleting directory "$DEB_BIN_DIR""
        exit 1
    fi
fi

mkdir -p $BUILD_DIR
if [ $? -ne 0 ]; then
        echo "Error creating directory "$BUILD_DIR""
        exit 1
fi

mkdir -p $DISTRIB_DIR
if [ $? -ne 0 ]; then
        echo "Error creating directory "$DISTRIB_DIR""
        exit 1
fi
        
mkdir -p $DEB_BIN_DIR
if [ $? -ne 0 ]; then
        echo "Error creating directory "$DEB_BIN_DIR""
        exit 1
fi

chmod -R 0755 $DEB_DIR/DEBIAN
if [ $? -ne 0 ]; then
    echo "Error granting rights for DEBAIN"
    exit 1
fi
        
cd $BUILD_DIR
        
qmake -config release ../
if [ $? -ne 0 ]; then
        echo "Error calling qmake"
        exit 1
fi
        
make -j4
if [ $? -ne 0 ]; then
        echo "Error calling make"
        exit 1
fi

cd ../

test -e $RELEASE_DIR/transport_control
if [ $? -ne 0 ]; then
    echo "Build sources first"
    exit 1
fi

cp $RELEASE_DIR/transport_control $DEB_BIN_DIR/transport_control
if [ $? -eq 0 ]; then
    echo "Binary file copied successfully"
else
    echo "Error copying binary file"
    exit 1
fi

chmod 0755 $DEB_BIN_DIR/transport_control
if [ $? -eq 0 ]; then
    echo "Execution rights granted"
else
    echo "Error granting execution rights"
    exit 1
fi

PACKAGE_SIZE=$(du -s --apparent-size $DEB_DIR)
REGEXP="(.+)\s.+/"
if [[ $PACKAGE_SIZE =~ $REGEXP ]]; then 
    PACKAGE_SIZE=${BASH_REMATCH[1]}; 
else
    PACKAGE_SIZE=0
fi

sed -i -e "s/Installed-Size: .*/Installed-Size: $PACKAGE_SIZE/g" $DEB_CTRL_FILE
if [ $? -eq 0 ]; then
    echo "Installed-Size updated"
else
    echo "Error updating installed-size in control file"
    exit 1
fi

$FAKE_ROOT dpkg-deb --build $DEB_DIR $DISTRIB_DIR
if [ $? -eq 0 ]; then
    echo "Package created"
else
    echo "Error creatin deb-package"
    exit 1
fi

exit 0
