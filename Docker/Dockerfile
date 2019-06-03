FROM achain/actx:base as builder
ARG branch=master
ARG symbol=ACTX

RUN git clone -b $branch https://github.com/Achain-Dev/Achain2.0.git --recursive \
    && cd Achain2.0 && echo "$branch:$(git rev-parse HEAD)" > /etc/actx-version \
    && cmake -H. -B"/tmp/build" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/tmp/build -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /tmp/build --target install


FROM ubuntu:18.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib/* /usr/local/lib/
COPY --from=builder /tmp/build/bin /opt/actx/bin
COPY --from=builder /tmp/build/contracts /contracts
COPY --from=builder /etc/actx-version /etc
COPY --from=builder /Achain2.0/Docker/nodeosd.sh /opt/actx/bin/nodeosd.sh
ENV ACTX_ROOT=/opt/actx
RUN chmod +x /opt/actx/bin/nodeosd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
ENV PATH /opt/actx/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
WORKDIR .
COPY   config.ini /
