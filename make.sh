# !/bin/bash

function main()
{
	export buildPath=".build/"
	export libraryPath="${buildPath}usr/local/lib"
	export includePath="${buildPath}usr/local/include/potent-code"

	function build()
	{
		echo "Building Delta..."
		bam ${BAMFLAGS} -a Mode=Debug || exit 1
	}
	
	function install()
	{
		# install libraries
		pushd "${libraryPath}" || exit 1
		for i in *; do
			echo "${i} -> /usr/local/lib/${i}"
			sudo cp -f ${i} /usr/local/lib/${i} || exit 1
		done
		popd || exit 1
	
		# install headers
		pushd "${includePath}" || exit 1
		sudo mkdir -p /usr/local/include/potent-code
		for i in *; do
			echo "${i} -> /usr/local/include/potent-code/${i}"
			sudo cp -f ${i} /usr/local/include/potent-code/${i} || exit 1
		done
		popd || exit 1
	}
	
	function clean()
	{
		echo "removing ${buildPath}"
		rm -fR ${buildPath} || exit 1
	}
	
	function usage()
	{
		echo "Usage:"
		echo -e "\t${0}\t\tBuild"
		echo -e "\t${0} install\tInstall to /usr/local/lib and /usr/local/include"
		echo -e "\t${0} clean\t\tClean build outputs"
	}
	
	# if no arguments are given, we just want to build
	if [ -z "${1}" ]; then
		build
	elif [ "${1}" == "install" ]; then
		if [ ! -d "${buildPath}" ]; then
			build
		fi

		install
	elif [ "${1}" == "clean" ]; then
		clean
	else
		usage ${@}
	fi
}

main ${@}
