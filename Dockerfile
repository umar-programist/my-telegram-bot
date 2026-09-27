FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN g++ bot.cpp -o mybot -lcurl

CMD ["./mybot"]
