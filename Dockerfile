FROM ubuntu:22.10

# We update the cache and install our needed packages
RUN apt-get update && \
    apt-get install -y \
      build-essential \
      python3 \
      strace
