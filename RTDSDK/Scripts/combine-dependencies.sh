source `dirname $0`/env.sh

MVN_ARTIFACTS_DIR=target
DEST_DIR=IPMessagingLib

# Helper to build either release or debug multi-arch static libraries depending on parameter
function merge_archs() {
  CONFIG=$1

  for lib in `find ${MVN_ARTIFACTS_DIR} -name \*a -path "*/${CONFIG}/*" | xargs basename | sort | uniq`; do
    rm -f ${DEST_DIR}/lib/${lib}
    lipo -create -output ${DEST_DIR}/lib/${lib} `find ${MVN_ARTIFACTS_DIR} -name ${lib} -path "*/${CONFIG}/*"`
  done
}

pushd `dirname $0`/..

# This shouldn't be necessary, mvn artifacts with non snapshot release version shouldn't change over time but we don't get latest artifacts without it
rm -rf ~/.m2/repository/com/twilio/rtd
mvn clean

# Call maven to populate target directory from RTD team's artifacts
mvn -Dbuild.platform=android || exit 1

# Set up directory structure
if [ ! -z "${DEST_DIR}" -a -d ./${DEST_DIR} ]; then
  rm -rf ./${DEST_DIR}
fi
mkdir -p ${DEST_DIR}/include/ ${DEST_DIR}/lib/

# Merge in include files
for x in `find ${MVN_ARTIFACTS_DIR} -name include`; do
  rsync -axp ${x}/ ${DEST_DIR}/include/
done

# Merge in include files
for x in `find ${MVN_ARTIFACTS_DIR} -name lib`; do
    rsync -axp ${x}/ ${DEST_DIR}/lib/${lib}
done


# Create multi-arch release libraries for all deps
# merge_archs "release"

if [ "${USE_DEBUG_RTD_LIBS}" -eq "1" ]; then
  # If USE_DEBUG_RTD_LIBS is 1, replace release versions of some libraries with debug (not available for all deps)
  echo "##########"
  echo "Merging in debug libraries where available"
  echo "##########"
  merge_archs "debug"
fi



popd
