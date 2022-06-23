#!/bin/sh
which clang-format 1>/dev/null 2>&1
if [ ${?} -ne 0 ] ; then
	echo "clang-format not installed or not on path."
	exit 1

fi

function do_format() {
	Dir=${1}
	HeaderFiles=`find ${Dir} -name "*.h"`
	SourceFiles=`find ${Dir} -name "*.c"`
	for FilePath in ${HeaderFiles} ${SourceFiles}
	do
		echo ${FilePath}
		cat ${FilePath} | clang-format > ${FilePath}
	done
}

TargetDirectories="Core"
for TargetDirectory in ${TargetDirectories}
do
	do_format ${TargetDirectory}
done

