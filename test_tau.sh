TBC=./build/Release/tbc
data_folder=data
result_folder=tau_results
datasets=(Ionosphere)
clusters=(2)
tau=(0, 5, 10, 15,20, 25, 30)
seed=(348684)
seed_length=${#seed[@]}

for i in ${!datasets[@]}
do
  cluster=${clusters[$i]}
  dataset=${datasets[$i]}
  input=./${data_folder}/${dataset}/${dataset}.csv
  output=./${result_folder}/${dataset}
  if [ ! -d $output ]; then
    mkdir -p $output
  fi

  for j in ${!seed[@]}
  do
    # index=$(($j+1))
    # echo '<' $dataset' > '  seed: ${seed[j]}  '('${index}'/'${seed_length}')'
    for t in ${!tau[@]}
    do
        ${TBC} ${input} ${output}/${dataset}-tau${t}.csv ${cluster} ${tau[t]} ${seed} 
    done
  done

  echo ''
done
