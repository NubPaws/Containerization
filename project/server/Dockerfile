# Start from a base image with a compiler
FROM ubuntu:latest

# Install dependencies for building and for sockets
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Copy source code
COPY main.cpp /app/main.cpp

# Build
WORKDIR /app
RUN g++ main.cpp -o server

# Expose the port (for documentation)
EXPOSE 5000

# Run the server
CMD ["./server"]
