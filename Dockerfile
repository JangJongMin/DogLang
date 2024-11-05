FROM ubuntu:20.04

RUN apt-get update && \
    apt-get install -y git gcc python3 python3-pip

RUN git clone "https://github.com/JangJongMin/DogLang.git"

RUN gcc -o DogLang main.c