#!/bin/bash -xv

# With constant sky subtraction, we have to clean hidden files 
# from a previous run, so that they can be recreated with a new 
# set of parameters. This has to be done before the parallel
# processing starts

# $1: main directory
# $2: science directory

\rm /$1/$2/.*.sky
