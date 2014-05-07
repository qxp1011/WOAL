#!/bin/sh

CURRENT=`pwd`
export ROOTDIR=$CURRENT/../
export SOUCEDIR=$ROOTDIR

#######################ACE########################
export ACE_ROOT=$ROOTDIR/ace/ACE_wrappers
export ACE_LIB_PATH=$ACE_ROOT/lib
export ACE_INCLUDE=$ACE_ROOT

#######################GTEST##############################
export GTEST_ROOT=$ROOTDIR/gtest
export GTEST_INCLUDE=$GTEST_ROOT/include
export GTEST_LIB_PATH=$GTEST_ROOT/lib/.libs

#######################LIBOPENFLOW_ACE############################
# Reconstruct the openflow lib using ACE.
export LIBOPENFLOW_ACE_ROOT=$ROOTDIR/libopenflow
export LIBOPENFLOW_ACE_INCLUDE=$LIBOPENFLOW_ACE_ROOT/include/openflow
export LIBOPENFLOW_ACE_LIB_PATH=$LIBOPENFLOW_ACE_ROOT/target

#######################LIBOFCONNECTOR#############################
# Clients can use this lib to connect with the controller.
export LIBOFCONNECTOR_ROOT=$ROOTDIR/libopenflow
export LIBOFCONNECTOR_INCLUDE=$LIBOFCONNECTOR_ROOT/include/ofConnector
export LIBOFCONNECTOR_LIB_PATH=$LIBOFCONNECTOR_ROOT/target


LD_LIBRARY_PATH=$ACE_LIB_PATH:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH;
