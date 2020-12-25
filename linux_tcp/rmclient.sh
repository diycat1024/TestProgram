#!/bin/bash
n=2000
while (($n>=1750))
do
  docker rm -f client_test"$n" &
  ((n = n - 1))
done
