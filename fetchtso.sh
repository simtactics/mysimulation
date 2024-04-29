#!/bin/bash
clear
echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
          ############
          ## FetchTSO ##
 -------------------------------
   Command-line installer for
        The Sims Online™
 -------------------------------
_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
"

runsetup=0
runhelp=0
runversion=0
runwebsite=0

verify=1
preserve=0
attempts=15
verbose=1

directory1=""
directory2=""
directoriesadded=2 # Defaulting to 2 for compatibility with the interactive setup.

if [ $# -eq 0 ]
then
	echo -e "You ran FetchTSO without any command line arguments.\nWhat would you like to do?\n"
	opt1="Run the interactive setup"
	opt2="Show the help page"
	opt3="Show version and licensing"
	opt4="Visit Niotso's home page"
	opt5="Exit"

	select opt in "$opt1" "$opt2" "$opt3" "$opt4" "$opt5"
	do
		case $opt in
			"$opt1" )
				runsetup=1
				break
				;;
			"$opt2" )
				runhelp=1
				break
				;;
			"$opt3" )
				runversion=1
				break;
				;;
			"$opt4" )
				runwebsite=1
				break;
				;;
			"$opt5" )
				exit 0
				;;
		esac
	done
else
	directoriesadded=0
	i=1
	while [ $i -le $# ]
	do
		case ${!i} in
			"-f" | "--no-verify" )
				verify=0
				;;

			"-p" | "--preserve" )
				preserve=1
				;;

			"-q" | "--quiet" )
				verbose=0
				;;

			"-v" | "--verbose" )
				verbose=1
				;;

			"-h" | "--help" )
				runhelp=1
				;;

			"-V" | "--version" | "-l" | "--license" )
				runversion=1
				;;

			"-a" )
				if [ $i -eq $# ]
				then
					echo "FetchTSO: Missing value to -a parameter.\nUse -h or --help for help."
					exit 1
				fi
				i=$[i+1]
				if [ -z `echo ${!i} | grep "^[0-9]*$"` ]
				then
					echo "FetchTSO: The attempts amount must be a positive integer.\nUse -h or --help for help."
					exit 1
				fi
				attempts=${!1}
				;;

			* )
				if [[ "${!i:0:11}" == "--attempts=" ]]
				then
					if [ -z `echo ${!i:11} | grep "^[0-9]*$"` ]
					then
						echo "FetchTSO: The attempts amount must be a positive integer.\nUse -h or --help for help."
						exit 1
					fi
					attempts=${!i:11}
				elif [[ "${!i:0:1}" == "-" || $directoriesadded -eq 2 ]]
				then
					echo -e "FetchTSO: ${!i}: Invalid option.\nUse -h or --help for help."
					exit 1
				else
					eval "directory$i=${!i}"
					directoriesadded=$[directoriesadded+1]
				fi
				;;
		esac
		i=$[i+1]
	done
fi

if [ $runhelp -eq 1 ]
then
echo "Usage: ./FetchTSO.sh [OPTIONS] DownloadDirectory InstallDirectory

Unless otherwise specified, Cab files are downloaded to the
download directory, extracted to the install directory, and
then deleted.

Options:
  -f, --no-verify   Skip CRC and size validation of Cab files.
  -p, --preserve    Preserve Cab files after installation.
                    Default behavior is to delete them.
  -a N,             Number of times to attempt downloading a
    --attempts=N    file; specify 0 to try indefinitely;
                    default is 15
  -q, --quiet       Show file download progress visibly.
  -v, --verbose     Show wget output. This is the default.
  -h, --help        Display this help and exit.
  -V, --version,    Display version and licensing information
    -l, --license   and exit.

Report bugs to <X-Fi6@phppoll.org>.
FetchTSO is maintained by the Niotso project.
Home page: <http://www.niotso.org/>"
exit 0

elif [ $runversion -eq 1 ]
then
echo "FetchTSO version 1.0.101
Date: November 21, 2011
Author: Fatbag <X-Fi6@phppoll.org>

This command-line tool is published directly into the public domain
with no warranty of any kind, express or implied.

The setup components downloaded by this tool are created by
Alex Zvenigorodsky and copyright © 2004 Maxis.

The game material packaged by these components is
copyright © 2002-2004 Maxis.

The Sims™ and The Sims Online™ are trademarks belonging to
Electronic Arts, Inc. (EA).

FetchTSO is maintained by the Niotso project.
Home page: <http://www.niotso.org/>"
exit 0

elif [ $runwebsite -eq 1 ]
then
	xdg-open "http://www.niotso.org/" & > /dev/null
	exit 0
fi

if [ $directoriesadded -lt 2 ]
then

	if [ $directoriesadded -eq 0 ]
	then
		echo "FetchTSO: Missing download directory and install directory."
		exitcode=2
	else
		echo "FetchTSO: Missing install directory."
		exitcode=3
	fi

	echo "For help, use -h or --help.

For other information, including the interactive setup,
run without using any parameters."
	exit $exitcode
fi
downdir="$directory1"
extractdir="$directory2"

############################
## Interactive Setup Code ##
## Starts here            ##
############################

if [ $runsetup -eq 1 ]
then

	verify=1
	preserve=1
	attempts=15
	verbose=0
	confirmed=0

	while [ $confirmed -eq 0 ]
	do

echo "
--------
Interactive Setup
--------

To quit the setup at any time, you are safe to close this window.

Your input should take about 1 minute. After that, the game's
contents will be downloaded and extracted.

At the end of the configure setup is a confirmation screen.
If you mess up anywhere along the setup, you can choose to start
over when you reach the confirmation screen.

To begin, press enter."
read

other="Other (Please specify after typing this number)"
opt1dir="./Download"
opt1="$opt1dir (Recommended) (In the FetchTSO folder)"
opt2dir="~/Downloads/TSO"
opt2="$opt2dir (In my Downloads folder)"
opt3dir="/var/tmp/TSO"
opt3="$opt3dir (In my temp folder)"
echo "================
Where do you want to download the installation files?
================"
select opt in "$opt1" "$opt2" "$opt3" "$other"
do
	case $opt in
		"$opt1" )
			downdir="$opt1dir"
			break
			;;
		"$opt2" )
			downdir="$opt2dir"
			break
			;;
		"$opt3" )
			downdir="$opt3dir"
			break
			;;
		"$other" )
			echo "Custom directory: "
			read downdir
			break
			;;
	esac
done
if [ -e "$downdir" ]
then
	if ! [ -d "$downdir" ]
	then
		echo "This is not a directory."
		select opt in "Delete it and make the directory" "Continue anyway"
		do
			case $opt in
				"Delete it and make the directory" )
					rm -i "$downdir"
					if [ $? -ne 0 ]
					then
						echo "This file could not be deleted."
					else
						mkdir -p "$downdir"
						if [ $? -ne 0 ]
						then
							echo "This file was deleted, but the folder could not be created."
						fi
					fi
					break
					;;
				"Continue anyway" )
					confirmed=1
					break
					;;
			esac
		done
	fi
else
	echo "The directory you specified does not exist."
	select opt in "Make it" "Continue anyway"
	do
		case $opt in
			"Make it" )
				mkdir -p "$downdir"
				if [ $? -ne 0 ]
				then
					echo "This file was deleted, but the folder could not be created."
				fi
				break
				;;
			"Continue anyway" )
				confirmed=1
				break
				;;
		esac
	done
fi

opt1dir="./The Sims Online"
opt1="$opt1dir (Recommended) (In the FetchTSO folder)"
opt2dir="~/The Sims Online"
opt2="$opt2dir (In my home folder)"
echo "
================
Where do you want to extract the installation files to?
================"
select opt in "$opt1" "$opt2" "$other"
do
	case $opt in
		"$opt1" )
			extractdir="$opt1dir"
			break
			;;
		"$opt2" )
			extractdir="$opt2dir"
			break
			;;
		"$other" )
			echo "Custom directory: "
			read extractdir
			break
			;;
	esac
done
if [ -e "$extractdir" ]
then
	if ! [ -d "$extractdir" ]
	then
		echo "This is not a directory."
		select opt in "Delete it and make the directory" "Continue anyway"
		do
			case $opt in
				"Delete it and make the directory" )
					rm -i "$extractdir"
					if [ $? -ne 0 ]
					then
						echo "This file could not be deleted."
					else
						mkdir -p "$extractdir"
						if [ $? -ne 0 ]
						then
							echo "This file was deleted, but the folder could not be created."
						fi
					fi
					break
					;;
				"Continue anyway" )
					confirmed=1
					break
					;;
			esac
		done
	fi
else
	echo "The directory you specified does not exist."
	select opt in "Make it" "Continue anyway"
	do
		case $opt in
			"Make it" )
				mkdir -p "$extractdir"
				if [ $? -ne 0 ]
				then
					echo "This file was deleted, but the folder could not be created."
				fi
				break
				;;
			"Continue anyway" )
				confirmed=1
				break
				;;
		esac
	done
fi

echo "
================
Do you want to tweak advanced settings? (Not necessary)
================"
select opt in "Yes" "No"
do
	case $opt in
		"Yes" )
			advanced=1
			break
			;;
		"No" )
			advanced=0
			break
			;;
	esac
done

#Start of advanced settings
if [ $advanced -eq 1 ]
then
echo "
================
Show verbose output? (Will print messages from wget)
================"
select opt in "Yes" "No (Recommended)"
do
	case $opt in
		"Yes" )
			verbose=1
			verbosestr="Yes"
			break
			;;
		"No (Recommended)" )
			verbose=0
			verbosestr="No"
			break
			;;
	esac
done
echo "
================
Verify CRCs and sizes of Cab files?
================"
select opt in "Yes (Recommended)" "No"
do
	case $opt in
		"Yes (Recommended)" )
			verify=1
			verifystr="Yes"
			break
			;;
		"No" )
			verify=0
			verifystr="No"
			break
			;;
	esac
done
echo "
================
Preserve Cab files after installation?
================"
select opt in "Yes" "No (Recommended)"
do
	case $opt in
		"Yes" )
			preserve=1
			preservestr="Yes"
			break
			;;
		"No (Recommended)" )
			preserve=0
			preservestr="No"
			break
			;;
	esac
done
attempts=""
while [ -z `echo $attempts | grep "^[0-9]*$"` ]
do
if [ -z "$attempts" ]
then
echo "
================
How many times should FetchTSO attempt to download each file?
(Default: 15; specify 0 for indefinite)
================"
fi
read attempts
if [ "$attempts" = "0" ]
then
	attemptsstr="Indefinite"
else
	attemptsstr="$attempts"
fi
done
fi
#End of advanced settings

echo "
==============================
FetchTSO - Current configuration
==============================
       | Download to: |
$downdir

       | Extract to:  |
$extractdir"
if [ $advanced -eq 1 ]
then
echo "
         | Verbose: |
$verbosestr

         | Verify:  |
$verifystr

        | Preserve:  |
$preservestr

        | Attempts:  |
$attemptsstr"
fi
echo "------------------------------

Is this okay?
(Yes -> install; No -> start over)"
select opt in "Yes" "No"
do
	case $opt in
		"Yes" )
			confirmed=1
			break
			;;
		"No" )
			confirmed=0
			break
			;;
	esac
done

done
fi

############################
## Interactive Setup Code ##
## Ends here              ##
############################

############################
## Doing the actual       ##
## download and install.  ##
##                        ##
## No more warnings.      ##
## Instead, bad messages  ##
## are errors that play   ##
## the terminal's bell    ##
## (using \a).            ##
############################

clear

if [[ ! -r "$downdir" || ! -w "$downdir" ]]
then
	echo -e "\aThe download directory does not exist or is not readable or writable."
	exit 1
elif [[ ! -r "$extractdir" || ! -r "$extractdir" ]]
then
	echo -e "\aThe extract directory does not exist or is not readable or writable."
	exit 1
fi

if [ $verbose -eq 0 ]
then
	echo -e "Download progress:\n0 of 1115 files downloaded."
else
	echo -e "Progress: 0 of 1115 files downloaded.\n==========================================\n"
fi

if [[ $downdir == .*/ ]]
then
	downdir="$downdir:0:$[${#downdir}-1]"
fi
if [[ $extractdir == .*/ ]]
then
	extractdir="$extractdir:0:$[${#extractdir}-1]"
fi

if [ $verbose -eq 0 ]
then
	quiet="-q"
else
	quiet=""
fi

downloadingmanifest=0

if ! [ -e "$downdir/manifest.txt" ]
then
	downloadingmanifest=1
	if [ $verbose -eq 0 ]
	then
		sdatestr=`date +"%0s.%0N"`
	fi
	wget $quiet --ftp-user="anonymous" --ftp-password="" -t $attempts -c -N -nH -nd --waitretry=6 --retry-connrefused --directory-prefix="$downdir" "ftp://largedownloads.ea.com/pub/misc/tso/manifest.txt"
	if [ $verbose -eq 0 ]
	then
		fdatestr=`date +"%0s.%0N"`
	fi

	returnvalue="$?"
	if [ $returnvalue -ne 0 ]
	then
		if [ $returnvalue -ne 3 ]
		then
			echo -e "\aThe download for this file failed.\n\nMake sure you are connected to the Internet and try again."
		else
			echo -e "\aThis file could not be saved at the path specified.\n\nMake sure the directory exists, you have read and write permission,\nand the file is not in use, and try again."
		fi
		exit 1
	fi
fi

function calcspeed {
	# Usage: calcspeed size startsecond finishsecond filenumber files
	# where the seconds have exactly 9 decimal digits of floating point precision

	# The goal is, given these parameters, to print the download rate, choosing the
	# best unit from B/s, KiB/s, or MiB/s.

	size=`echo "scale=2; $1" | bc`
	sizeunit="bytes"
	if [ "`echo "$size >= 1024.0" | bc`" -eq 1 ]
	then
		size=`echo "scale=2; $1/1024" | bc`
		if [ "`echo "$size < 1024.0" | bc`" -eq 1 ]
		then
			sizeunit="KiB"
		else
			sizeunit="MiB"
			size=`echo "scale=2; $1/1048576" | bc`
		fi
	fi

	time=`echo "scale=9; $3-$2" | bc`
	speedunit="bytes"
	rate=`echo "scale=0; $1/$time" | bc`
	if [ $rate -ge 1024 ]
	then
		rate=`echo "scale=0; $1/$time/1024" | bc`
		if [ $rate -lt 1024 ]
		then
			speedunit="KiB"
		else
			speedunit="MiB"
			rate=`echo "scale=9; $1/$time/1048576" | bc`
		fi
	fi

	echo "$4 of $5 files downloaded. (Size: ${size} ${sizeunit}; Avg. ${rate} ${speedunit}/s)"
}

if [ $verbose -eq 0 ]
then
	size=`stat --printf="%s" "$downdir/manifest.txt"`

	if [ $downloadingmanifest -eq 1 ]
	then
		calcspeed $size $sdatestr $fdatestr 1 1115
	else
		echo "1 of 1115 files downloaded. (Size: ${size} bytes)"
	fi
else
	clear
	echo -e "Progress: 1 of 1115 files downloaded.\n==========================================\n\n"
fi

function hashcheck {
	# Usage: hashcheck infile correcthash

	filehash=`(
		dataleft=\`stat -c "%s" $1\`
		increment=524288
		insize=0

		while [ $dataleft -ne 0 ]
		do
			if [ $increment -gt $dataleft ]
			then
				increment=$dataleft
			fi
			insize=$(($insize+$increment))
			dataleft=$(($dataleft-$increment))

			for i in {1..2}
			do
				head -c $insize $1 | tail -c $increment
			done
		done
	) | md5sum -b -`

	if [ "${filehash:0:32}" == "$2" ]
	then
		return 0
	else
		return 1
	fi
}

filesdownloaded=1
while read line
do
	file=`echo $line | sed -e 's/.*File="//' -e 's/".*//'`
	if [[ "$file" == Data[0-9]*.cab ]]
	then
		size=`echo $line | sed -e 's/.*Size="//' -e 's/".*//'`
		hash=`echo $line | sed -e 's/.*Hash="//' -e 's/,/ /g' -e 's/".*//'`

		if [ ${#hash} -ne 64 ]
		then
			echo -e "\aHash broken in manifest.txt on file $file."
			exit 1
		fi
		hexhash=""
		for decimal in $hash
		do
			num=$((10#$decimal))
			hexhash="$hexhash`printf '%02x' $num`"
		done

		skip=0
		if [ -e "$downdir/$file" ]
		then
			if [ $verify -eq 0 ]
			then
				skip=1
			elif [ $size -eq `stat --printf="%s" "$downdir/$file"` ]
			then
				hashcheck "$downdir/$file" "$hexhash"
				if [ "$?" -eq 0 ]
				then
					skip=1
				fi
			fi
		fi

		if [ $skip -eq 0 ]
		then
			downloaded=0
			i=1
			while [ $downloaded -ne 1 ]
			do
				if [ $verbose -eq 0 ]
				then
					sdatestr=`date +"%0s.%0N"`
				fi
				wget $quiet --ftp-user="anonymous" --ftp-password="" -t $attempts -c -N -nH -nd --waitretry=6 --retry-connrefused --directory-prefix="$downdir" "ftp://largedownloads.ea.com/pub/misc/tso/$file"
				if [ $verbose -eq 0 ]
				then
					fdatestr=`date +"%0s.%0N"`
				fi

				returnvalue="$?"
				if [ $returnvalue -ne 0 ]
				then
					if [ $returnvalue -ne 3 ]
					then
						echo -e "\aThe download for this file failed.\n\nMake sure you are connected to the Internet and try again."
					else
						echo -e "\aThis file could not be saved at the path specified.\n\nMake sure the directory exists, you have read and write permission,\nand the file is not in use, and try again."
					fi
					exit 1
				fi

				if [[ $size -ne `stat --printf="%s" "$downdir/$file"` || `hashcheck "$downdir/$file" "$hexhash"; echo $?` -eq 1 ]]
				then
					echo -e "$file does not match the manifest's description. Trial $i of 5."
					if [ $i -eq 5 ]
					then
						echo -e "\aThis file could not be downloaded."
						exit 1
					fi
					i=$[i+1]
				else
					downloaded=1
				fi
			done
		fi

		filesdownloaded=$[filesdownloaded+1]
		if [ $verbose -eq 0 ]
		then
			if [ $skip -eq 0 ]
			then
				calcspeed $size $sdatestr $fdatestr $filesdownloaded 1115
			else
				echo "$filesdownloaded of 1115 files downloaded. (Size: ${size} bytes)"
			fi
		else
			clear
			echo -e "Progress: $filesdownloaded of 1115 files downloaded.\n==========================================\n\n"
		fi
	fi
done < "$downdir/manifest.txt"

if [ $verbose -eq 0 ]
then
	echo -e "\nExtract progress:"
else
	clear
	echo -e "Progress: Extracting contents of all downloaded files.\n==========================================\n\n"
fi

cabextract -d "$extractdir" "$downdir/Data1.cab"
if [ $? -ne 0 ]
then
	echo "\aNot all files could be extracted."
	exit 1
fi

if [ $preserve -eq 0 ]
then
	rm -f "$downdir/manifest.txt"
	for n in {1..1114}
	do
		rm -f "$downdir/Data$n.cab"
	done
fi
clear
echo -e "\aThe Sims Online has downloaded and extracted without error.\n"
read