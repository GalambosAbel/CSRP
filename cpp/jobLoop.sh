#inputs are: 1: name of the word to be processed; 2: number of layers to process (starting from 0); 3: the type of ordering heuristic to use
outPath="../out/$1/$3"
mkdir -p $outPath
echo "-------------------------------" >> "$outPath/logs.txt"
echo "Logs of the run initated at time \"$(date +"%c")\":" >> "$outPath/logs.txt"
echo "Run initated at time: $(date +"%c")"
for i in `seq 0 $2`
do
    ./csrp -e "abel.galambos@gmail.com" -i "../signature_dists/$1/layer_$i.in" -o "$outPath/layer_$i.in" -v "$outPath/layer_$i.png" -s -$3 |& tee -a "$outPath/logs.txt"
done
echo "The run concluded at time \"$(date +"%c")\"" >> "$outPath/logs.txt"
echo "Run concluded at time: $(date +"%c")"