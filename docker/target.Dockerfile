FROM gcc:latest

WORKDIR $HOME/src/
ADD ./* $HOME/src/
RUN g++ main.cpp watershell.cpp -o watershell
CMD ./watershell -l 8080 eth0