#!/bin/sh

autoreconf --install
git submodule init
git submodule update
