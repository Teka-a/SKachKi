FROM debian:12

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime &&\
    echo $TZ > /etc/timezone &&\
    apt-get update &&\
    apt-get install --no-install-recommends qtbase5-dev=5.15.8+dfsg-11 -y &&\
    apt-get install --no-install-recommends libqt5sql5-psql=5.15.8+dfsg-11 -y &&\
    apt-get install --no-install-recommends qt5-qmake=5.15.8+dfsg-11 -y &&\
    apt-get install --no-install-recommends qtbase5-dev-tools=5.15.8+dfsg-11 -y &&\
    apt-get install --no-install-recommends qtchooser=66-2 -y &&\
    apt-get install --no-install-recommends build-essential=12.9 -y &&\
    apt-get install --no-install-recommends libtool=2.4.7-5 -y &&\
    apt-get install --no-install-recommends libssl-dev=3.0.11-1~deb12u2 -y &&\
    apt-get install wget -y &&\
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/*

WORKDIR /root/
RUN mkdir /root/server/
COPY *.cpp /root/server/
COPY *.h /root/server/
COPY *.pro /root/server/

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.bz2
RUN tar --bzip2 -xf boost_1_83_0.tar.bz2
RUN cd  boost_1_83_0 && ./bootstrap.sh  && ./b2 install


WORKDIR /root/server/
RUN qmake echoServer.pro &&\
    make
ENTRYPOINT ["./echoServer"]
