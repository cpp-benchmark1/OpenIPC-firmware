FROM ubuntu:22.04

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
    gcc \
    git \
    libncurses-dev \
    libtool \
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

# Copy the entire project
COPY . /build/

# Create output directory
RUN mkdir -p /build/output

# Set environment variables
ENV TARGET=/build/output
ENV PWD=/build
ENV PATH="/build/general/package/xmdp/src:${PATH}"

# Build the project
RUN cd /build/general/package/xmdp/src && \
    make clean && \
    make && \
    chmod +x xmdp

# Install Python dependencies for exploit scripts
RUN pip3 install --no-cache-dir \
    requests \
    python-nmap

# Set permissions for exploit script
RUN chmod +x /build/general/package/xmdp/src/exploit_cwe134.py

# Set working directory to xmdp directory
WORKDIR /build/general/package/xmdp/src

# Default command
CMD ["/bin/bash"]

