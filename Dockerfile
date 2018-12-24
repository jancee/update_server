FROM ubuntu:16.04

LABEL MAINTAINER Jancee Wang <jancee.wang@qq.com>


ENV PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/delta_generator:/opt/chromite/bin
ENV LD_LIBRARY_PATH=/opt/delta_generator:/opt/lib:/root/clang_and_llvm/lib

# RUN df -h
# RUN dpkg --add-architecture i386
RUN apt-get update
# RUN apt-get dist-upgrade

RUN apt-get install -y \
        apt-utils
RUN apt-get install -y \
        python-dev \
        python-pip \
        python-software-properties
RUN apt-get install -y \
        libc++abi1
RUN apt-get install -y \
        libc++1
RUN apt-get install -y \
        libbrotli-dev \
        parted \
        libdbus-1-dev \
        libglib2.0-dev \
        git \
        wget \
        vim \
        m4 \
        zip \
        unzip

# RUN apt-get install -y \
#         iputils-ping \
#         vim \
#         git \
#         libc++abi1 \ 
#         libc++1 \
#         rapidsvn \
#         python-dev \
#         python-pip \
#         openssl \
#         wget \
#         lib32stdc++6 \
#         lib32z1 \
#         libbrotli-dev \
#         build-essential \
#         openssh-server \
#         lib32c++ \
#         libc++.so \
#         autoconf \
#         curl \
#         lib32z1-dev \
#         lib32ncurses5 \
#         libc6-dev \
#         libgmp-dev \
#         libmpc-dev \
#         libmpfr-dev \
#         libxslt-dev \
#         libxml2-dev \
#         m4 \
#         ncurses-dev \
#         ocaml \
#         openssh-client \
#         pkg-config \
#         python-software-properties \
#         ruby-full \
#         software-properties-common \
#         unzip \
#         zip \
#         zlib1g-dev \
#         cmake \
#         libstdc++6-4.7-dev \
#         llvm \
#         clang \
#         git-core \
#         gnupg \
#         flex \
#         bison \
#         gperf \
#         zlib1g-dev \
#         gcc-multilib \
#         g++-multilib \
#         libc6-dev-i386 \
#         lib32ncurses5-dev \
#         x11proto-core-dev \
#         libx11-dev \
#         lib32z-dev \
#         libgl1-mesa-dev \
#         libxml2-utils \
#         xsltproc \
#         libevent-2.0-5 \
#         libevent-core-2.0-5 \
#         gedit

# Install Python Dependency
RUN pip install --upgrade pip
RUN pip install portend==2.3 && pip install protobuf==3.6.1
RUN pip install cherrypy==17.4.1
RUN pip install psutil==5.4.8

# RUN ln -sf /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libc++.so
# RUN ln -sf /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libc++.so.1

# RUN ln -sf /usr/lib/x86_64-linux-gnu/libevent-2.0.so.5 /usr/lib/x86_64-linux-gnu/libevent-2.1.so.6
# RUN ln -sf /usr/lib/x86_64-linux-gnu/libevent_core-2.0.so.5 /usr/lib/x86_64-linux-gnu/libevent_core-2.1.so.6

# download and unzip, libcxx and libcxxabi src
# RUN cd /root && \
#         wget http://releases.llvm.org/4.0.1/libcxx-4.0.1.src.tar.xz && \
#         wget http://releases.llvm.org/4.0.1/libcxxabi-4.0.1.src.tar.xz && \
#         xz -d libcxx-4.0.1.src.tar.xz && \
#         xz -d libcxxabi-4.0.1.src.tar.xz && \
#         tar xvf libcxx-4.0.1.src.tar && \
#         tar xvf libcxxabi-4.0.1.src.tar

# build libcxx
# RUN cd /root/libcxx-4.0.1.src && \
#         mkdir tmp && cd tmp && \
#         cmake -DCMAKE_BUILD_TYPE=Release \
#                 -DCMAKE_INSTALL_PREFIX=/usr \
#                 -DCMAKE_C_COMPILER=clang \
#                 # -DCMAKE_CXX_COMPILER=g++ \
#                 -DCMAKE_CXX_COMPILER=clang \
#                 -DLIBCXXABI_LIBCXX_INCLUDES=../../libcxx-4.0.1.src/include \
#                 .. \
#                 && \
#         make install

# RUN cd /root && \
#         wget http://releases.llvm.org/3.7.0/clang+llvm-3.7.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz \
#         xz -d clang+llvm-3.7.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz && \
#         tar xvf clang+llvm-3.7.0-x86_64-linux-gnu-ubuntu-14.04.tar && \
#         mv clang+llvm-3.7.0-x86_64-linux-gnu-ubuntu-14.04 clang_and_llvm

# build libcxxabi
# RUN cd /root/libcxxabi-4.0.1.src && \
#         mkdir tmp && cd tmp && \
#         cmake -DCMAKE_BUILD_TYPE=Release \
#                 -DCMAKE_INSTALL_PREFIX=/usr \
#                 -DCMAKE_C_COMPILER=clang \
#                 -DCMAKE_CXX_COMPILER=clang++ \
#                 -DLIBCXXABI_LIBCXX_INCLUDES=../../libcxx-4.0.1.src/include \
#                 .. \
#                 && \
#         make install

# move libcxx and libcxxabi so to LD Lib Path

EXPOSE 8080

COPY . /opt/
VOLUME [ "/extern-volume/input", "/extern-volume/output" ]
WORKDIR /opt

CMD [ "/opt/run.sh" ]



















