#!/bin/sh
cd /opt/actx/bin

if [ ! -d "/opt/actx/bin/data-dir" ]; then
    mkdir /opt/actx/bin/data-dir
fi

if [ -f '/opt/actx/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/actx/bin/data-dir
fi

if [ -d '/opt/actx/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/actx/bin/data-dir
fi

while :; do
    case $1 in
        --config-dir=?*)
            CONFIG_DIR=${1#*=}
            ;;
        *)
            break
    esac
    shift
done

if [ ! "$CONFIG_DIR" ]; then
    CONFIG_DIR="--config-dir=/opt/actx/bin/data-dir"
else
    CONFIG_DIR=""
fi

exec /opt/actx/bin/nodeos $CONFIG_DIR "$@"
