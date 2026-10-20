execute=${1:-0}
directory=${2:-*}

if [[ $execute == 1001485 ]]; then
	rg "\"setting_id\": \"[^\"]*\"" -g "!resources/profiles/BBL/" -l0 resources/profiles/"$directory" | while IFS= read -r -d '' file; do
		p1=$(echo "$file" | sd "resources/profiles/" "" | sd "/" "{}" | sd ".json" "")
		IFS="{}" read -r -a p2 <<< "$p1"
		p3=$(python quickgen.py autopilot "${p2[0]}" "${p2[2]}" "${p2[-1]}")
		echo "For ${p2[0]} ${p2[2]} ${p2[-1]}"
		echo $p3
		sd "^([ \t]*)\"setting_id\": \"[^\"]*\"" "\$1${p3}" "$file"
	done
elif [[ $execute == 1 ]]; then
	echo "You are about to update every single setting_id as you run this script."
	echo "If a directory is specified, only that directory is affected."
	echo "To reset afterwards, type: git reset --hard && git clean -fd"
	echo "This process is quite destructive. Think twice before going forward."
	echo "Bambu Lab profiles are not affected."
	echo "To start, your password is 1001485"
	exit 1001485
else
	echo "This script is strictly locked for safety."
	echo "See manual for details."
	exit 1
fi
