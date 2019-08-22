#!/bin/sh
cd /opt/achain/bin

if [ ! -d "/opt/achain/bin/data-dir" ]; then
    mkdir /opt/achain/bin/data-dir
fi

if [ -f '/opt/achain/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/achain/bin/data-dir
fi

if [ -d '/opt/achain/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/achain/bin/data-dir
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
    CONFIG_DIR="--config-dir=/opt/achain/bin/data-dir"
else
    CONFIG_DIR=""
fi

exec /opt/achain/bin/nodeos $CONFIG_DIR "$@"
