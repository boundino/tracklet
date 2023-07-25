
input=$1

output=${input%%.txt}_shuf.txt

echo $output

shuf $input > $output
