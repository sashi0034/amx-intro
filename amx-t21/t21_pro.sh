
for i in {1..4}
do
  echo "Execution $i"
  python t21_profile.py
  python t21_summary.py
  python t21_table.py >> t.txt
  echo "Execution $i completed"
done

echo "All executions completed"
