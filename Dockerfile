FROM ubuntu:20.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire project
COPY . /root/xmdp/

# Set working directory
WORKDIR /root/xmdp/general/package/xmdp/src

# Build the project
RUN make clean && make

# Make exploit script executable
RUN chmod +x exploit_cwe78.py

# Default command
CMD ["/bin/bash"] 