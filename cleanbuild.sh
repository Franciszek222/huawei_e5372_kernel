#!/bin/bash
if ! [ -e .no_warn ]; then

	echo "############################# "
	echo "WARNING! This script will erase all"
	echo "your local changes! You have been warned"
	echo "This may include your local kernel config (.config"
	echo ""
	echo "You can now commit your changes"
	echo "#############################"
 
read  -r -p 'Press any key to continue...' ans
touch .no_warn

fi

	git clean -dffx
	git reset --hard
	exec ./build.sh
