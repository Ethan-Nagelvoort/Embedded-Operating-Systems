import sys

choice = input("Enter 1 if you would like to use data1.txt. Enter 2 if you would like to use data2.txt\n")
if int(choice) == 1:
    f = open('data1.txt', 'r')
elif int(choice) == 2:
    f = open('data2.txt', 'r')
else:
    print("Invalid option was chosen")
    sys.exit(0)

lines = f.readlines()
for line in lines:
    parts = line.split()
    pn = int(parts[0])
    add = int(parts[1])
    op = parts[2]
f.close()
sys.exit(0)



