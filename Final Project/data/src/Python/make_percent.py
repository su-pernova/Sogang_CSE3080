# get major korean name
major_KOR = []
file = open('[0]학생설계전공_리스트.txt', 'r', encoding = 'UTF8')
data = file.readlines()
for x in data:
    major_KOR.append(x[:-1])
file.close()

# start iteration
percent_file = open('[2]percentage.txt', 'w')
for y in range(len(major_KOR)):
    
    # get major name
    major_str = []
    file = open('[1]기존전공_리스트_Eng.txt', 'r')
    data = file.readlines()
    for x in data:
        major_str.append(x[:-1])

    # compare major name
    major_num = [0 for i in range(len(major_str))]
    file = open(major_KOR[y] + '.txt', 'r')
    line = file.readline()
    while True:
        line = file.readline()
        if not line: break
        for i in range(len(major_str)):
            if line.startswith(major_str[i]):
                major_num[i] += 1
                break

    # caculate percentage
    total = 0
    percent_num = [0 for i in range(len(major_str))]
    for i in range(len(major_num)):
        total += major_num[i]
    for i in range(len(major_num)):
        percent_num[i] = round(major_num[i] / total * 100)
        
    # save result in text file
    for i in range(len(major_num)):
        percent_file.write(str(percent_num[i]) + ' ')
    percent_file.write('\n')
        
percent_file.close()
print('caculation complete.')



            
