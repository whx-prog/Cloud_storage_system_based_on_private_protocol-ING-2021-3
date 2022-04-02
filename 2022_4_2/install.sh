#!/bin/bash
sh unistall.sh

cd  ./src/sever/
make
cd ../client/
make

