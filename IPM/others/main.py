import csv
from collections import Counter

def get_most_used_prefixes_from_csv(file_path):
    words = []

    with open(file_path, newline='', encoding='latin-1') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            if row: 
                words.append(row[0])

    prefixes = [word[:3] for word in words if len(word) >= 3]
    prefix_counts = Counter(prefixes)
    most_used_prefixes = sorted(prefix_counts.items())
    return most_used_prefixes

csv_file_path = "others\\legendas-prefixos.csv"
result = get_most_used_prefixes_from_csv(csv_file_path)

print("Prefixes in alphabetical order:")
i = 0
for prefix, frequency in result:
    i+=1
    print("number: " + str(i))
    print(f"{prefix}: {frequency} times")