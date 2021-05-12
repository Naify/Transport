#!/bin/sh

PRO_NAME="transport"
#BUILD_DIR="build"
DISTRIB_DIR="./distrib"

fail() { echo -e "\033[37;1;41m $1 \033[0m"; exit 1;}

convertsecs() {
     ((h=${1}/3600))
     ((m=(${1}%3600)/60))
     ((s=${1}%60))
     printf "%02d:%02d:%02d\n" $h $m $s
}

START=$(date +%s)

OS=`uname -o`

if [ "$OS" == "GNU/Linux" ]; then
    INSTALL_DEPENDS="sudo apt-get -y install"
    INSTALL_CMD="sudo dpkg -i $DISTRIB_DIR/*.deb"
    OS_VERSION=`cat "/etc/astra_version" | cut -d' ' -f2`

    [ "$OS_VERSION" == "1.4" ] && DEPENDS=`cat ./build_depends.astra1.4`
    [ "$OS_VERSION" == "1.6" ] && DEPENDS=`cat ./build_depends.astra1.6`
else
    fail "Не поддерживаемая операционная система"
fi

INSTALL=0
CHECK_DEPENDS=0

while getopts ":ic" opt; do
    case $opt in
        i) INSTALL=1 ;;
        c) CHECK_DEPENDS=1 ;;
    esac
done

echo -e "\033[32m Сборка проекта $PRO_NAME\033[0m"

[ $CHECK_DEPENDS -eq 1 ] && { $INSTALL_DEPENDS $DEPENDS || fail "Ошибка при установке зависимостей" ;}

sh ./build_deb.sh || fail "Ошибка сборки пакета"

[ $INSTALL -eq 1 ] && { $INSTALL_CMD || fail "Ошибка установки пакета" ;}

echo -e "\033[32m Пакет $PRO_NAME собран за $(convertsecs $(($(date +%s)-$START)))\033[0m"

exit 0
