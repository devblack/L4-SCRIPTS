
for (( i=0; i<3000; i++ ))
do
echo "53.$(($RANDOM%255)).$(($RANDOM%255)).$(($RANDOM%255))" >> $1.txt
echo "54.$(($RANDOM%255)).$(($RANDOM%255)).$(($RANDOM%255))" >> $1.txt
echo "55.$(($RANDOM%255)).$(($RANDOM%255)).$(($RANDOM%255))" >> $1.txt
done
