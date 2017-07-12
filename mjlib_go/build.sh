#!/bin/bash

export GOPATH=/data/yun/mygit/qipai/mjlib_go
echo $GO_PATH
go install gen_table
go install gen_feng_table
go install mjlib_test
