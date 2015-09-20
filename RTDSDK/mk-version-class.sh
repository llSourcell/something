#!/bin/bash

set -e

. ../tools/functions.sh

[ "$1" ] && sdk_version=$1 || sdk_version=$(get_sdk_version)
[ "$sdk_version" ]

mkdir -p gen/com/twilio/ipmessaging

echo "New SDK version is ${sdk_version}"

cat >gen/com/twilio/ipmessaging/Version.java <<EOF
package com.twilio.ipmessaging;

public abstract class Version
{
    /**
     * The current version of the Twilio Client SDK.
     */
    public static final String SDK_VERSION = "${sdk_version}";
}
EOF
