FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    automake \
    autotools-dev \
    bc \
    build-essential \
    cpio \
    curl \
    file \
    fzf \
    git \
    libncurses-dev \
    libtool \
    libcurl4-openssl-dev \
    lzop \
    make \
    rsync \
    unzip \
    wget \
    libssl-dev \
    whiptail \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /build

# Copy the OpenIPC firmware source
COPY . /build/

# Create output directory
RUN mkdir -p output

# Set environment variables
ENV TARGET=/build/output
ENV PWD=/build

# Build xmdp
RUN cd /build/general/package/xmdp/src && \
    make clean && \
    make && \
    chmod +x xmdp

# Install Python dependencies for exploit scripts
RUN pip3 install --no-cache-dir \
    requests \
    python-nmap

# Set working directory to xmdp
WORKDIR /build/general/package/xmdp/src

# Add xmdp to PATH
ENV PATH="/build/general/package/xmdp/src:${PATH}"

# Set default command to bash
CMD ["/bin/bash"] 