bsd=$1
execute=${2:-0}
directory=$3

# It runs in background. Compared to sd, it consumes less RAM

# MIGRATION IN PROGRESS! Things may work differently than expected for a few weeks from now. Thank you for your patience.
	if [ $execute = 1001485 ]; then
		fileCOUNT=$(rg "\"setting_id\": \"[^\"]*\"" -g "!resources/profiles/BBL/" -l resources/profiles/"$directory" | wc -l)
		currentPOS=0
		rg "\"setting_id\": \"[^\"]*\"" -g "!resources/profiles/BBL/" -l0 resources/profiles/"$directory" | while IFS= read -r -d '' file; do
		echo $p3
		if [ $bsd = 'bsd' ]; then
			(( currentPOS++ ))
			p1=$(echo "$file" | sed '' 's|resources/profiles/||g' | sed '' 's|/|{}|g' | sed '' 's|.json||g')
			IFS="{}" read -r -a p2 <<< $p1
			p3=$(python quickgen.py autopilot "${p2[0]}" "${p2[2]}" "${p2[-1]}")
			printf "[$currentPOS/$fileCOUNT] ${p2[0]} ${p2[2]} ${p2[-1]}"
			sed -E -i '' "s/^([[:blank:]]*)\"setting_id\": \"[^\"]*\"|\1${p3}/g" "$file"
		elif [ $bsd = 'gnu' ]; then
			(( currentPOS++ ))
			p1=$(printf "$file" | sed 's|resources/profiles/||g' | sed 's|/|{}|g' | sed 's|.json||g')
			#echo $p1
			IFS="{}" read -r -a p2 <<< "$p1"
			p3=$(python3 quickgen.py autopilot "${p2[0]}" "${p2[2]}" "${p2[-1]}")
			printf "[$currentPOS/$fileCOUNT] ${p2[0]} ${p2[2]} ${p2[-1]}"
			sed -E -i "s|^([[:blank:]]*)\"setting_id\": \"[^\"]*\"|\1${p3}|g" "$file"
		else
			echo "BSD and GNU has different sed builds. Specify your sed."
			exit 1234
		fi
	done

# BEFORE SED/GREP
#        rg "\"setting_id\": \"[^\"]*\"" -g "!resources/profiles/BBL/" -l0 resources/profiles/"$directory" | while IFS= read -r -d '' file; do
#                p1=$(echo "$file" | sd "resources/profiles/" "" | sd "/" "{}" | sd ".json" "")
#                IFS="{}" read -r -a p2 <<< "$p1"
#                p3=$(python quickgen.py autopilot "${p2[0]}" "${p2[2]}" "${p2[-1]}")
#                echo "For ${p2[0]} ${p2[2]} ${p2[-1]}"
#                echo $p3
#                sd "^([ \t]*)\"setting_id\": \"[^\"]*\"" "\$1${p3}" "$file"

elif [ $execute = 1 ]; then
	echo "This process is destructive. Make sure you have a backup or a git repository set up."
	echo "Bambu Lab profiles are not affected."
	echo "To start, your password is 1001485"
	exit 1001485
else
	echo "This script is strictly locked for safety."
	echo "See manual for details."
	exit 1
fi
