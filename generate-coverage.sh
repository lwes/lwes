#!/bin/bash

make maintainer-clean;
./bootstrap \
&& ./configure --enable-coverage \
&& make \
&& (make check || echo "some tests failed!") \
&& lcov --capture --directory . --output-file coverage-all.info \
&& lcov --remove coverage-all.info src/lwes_esf_parser*.c --output-file coverage.info \
&& genhtml coverage.info --output-directory out \
&& echo "point your browser to `pwd`/out/index.html"

