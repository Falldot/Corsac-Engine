FROM debian:bullseye

RUN apt-get -qq update && \
    apt-get install -qqy --no-install-recommends \
        ca-certificates \
        autoconf automake cmake dpkg-dev file git make patch \
        libc-dev libc++-dev libgcc-10-dev libstdc++-10-dev  \
        dirmngr gnupg2 lbzip2 wget xz-utils libtinfo5 && \
    rm -rf /var/lib/apt/lists/*

ENV GPG_KEYS 09C4E7007CB2EFFB A2C794A986419D8A B4468DF4E95C63DC D23DD2C20DD88BA2 8F0871F202119294 0FC3042E345AD05D

RUN gpg --batch --keyserver keyserver.ubuntu.com --recv-keys $GPG_KEYS

ENV LLVM_RELEASE 14
ENV LLVM_VERSION 14.0.0

COPY build/install.sh .
RUN ./install.sh