#!/bin/bash

export GOPATH=$PWD
echo $GO_PATH
go install gen_table
go install gen_feng_table
go install mjlib_test
