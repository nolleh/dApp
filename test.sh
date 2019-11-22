#!/bin/bash
docker run --rm -it -v ${PWD}:/dApp nolleh/eosio-ut:v1.7.0 /bin/bash -c \
	"cd dApp && ./clean.sh && dos2unix ./tools/*.sh \ 
	cd build && cmake .. -Deosio_DIR=/root/opt/eosio/lib/cmake/eosio && cd .. && \
	./build.sh"
