FROM achain/actx:base
ARG branch=master
ARG symbol=ACTX

RUN git clone -b $branch https://github.com/Achain-Dev/Achain2.0.git --recursive \
    && cd Achain2.0 && echo "$branch:$(git rev-parse HEAD)" > /etc/actx-version \
    && cmake -H. -B"/opt/actx" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/opt/actx -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /opt/actx --target install \
    && cp /Achain2.0/Docker/config.ini / && cp /Achain2.0/Docker/genesis.json / \
    && ln -s /opt/actx/contracts /contracts && cp /Achain2.0/Docker/nodeosd.sh /opt/actx/bin/nodeosd.sh \
    && ln -s /Achain2.0/tutorials /tutorials

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates vim psmisc python3-pip && rm -rf /var/lib/apt/lists/*
RUN pip3 install numpy
ENV ACTX_ROOT=/opt/actx
RUN chmod +x /opt/actx/bin/nodeosd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
ENV PATH /opt/actx/bin:/usr/actx/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
