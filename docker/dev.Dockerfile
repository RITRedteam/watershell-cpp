FROM gcc:latest

RUN apt-get update && apt-get install python3
WORKDIR $HOME/src/
