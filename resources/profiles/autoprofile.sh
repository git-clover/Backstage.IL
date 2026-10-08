action=$1
name=$2

if [[ "$action" == "add" ]]; then
    echo "New vendor: $name"
    cp -r ../profiles_template/Template "$name"
    cp ../profiles_template/Template.json "$name.json"
elif [[ "$action" == "remove" ]] then
	echo "Goodbye, $name"
	rm -rf "$name" "$name.json"
fi
