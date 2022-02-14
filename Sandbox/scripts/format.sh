#!/bin/bash

if which clang-format >/dev/null; then

foldersToExclude=(Vendor vendor imgui)
extensionsToProcess=(cpp hpp h)

command="find . \( "
for i in ${!extensionsToProcess[@]}; do
    extension=${extensionsToProcess[$i]}
    command+="-name '*.$extension' "
    if [ "$((${#extensionsToProcess[@]}-1))" -ne "$i" ]; then
        command+=" -or "
    fi
done
command+=" \) -and \( "
for i in ${!foldersToExclude[@]}; do
    folder=${foldersToExclude[$i]}
    command+="-not -path '**/$folder/*' "
    if [ "$((${#foldersToExclude[@]}-1))" -ne "$i" ]; then
        command+=" -and "
    fi
done
command+="\)"

files=$(eval ${command})

echo "Formatting files:"
for file in $files; do
    echo $file
    clang-format -i "$file"
done
echo "Formatting done. Thanks to clang-format ❤️"

else
echo "warning: ClangFormat not installed"
fi
