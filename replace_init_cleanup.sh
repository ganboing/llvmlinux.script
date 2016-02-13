#!/bin/bash

MODULE_BC=$1.o.bc
MODULE_LL=$1.o.ll

llvm-dis $MODULE_BC
sed -i "s/init_module/init_module_$1/g" $MODULE_LL
sed -i "s/cleanup_module/cleanup_module_$1/g" $MODULE_LL
