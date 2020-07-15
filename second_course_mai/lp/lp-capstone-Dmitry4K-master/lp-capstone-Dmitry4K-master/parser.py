

def parsing(filename):
    persons = []
    families = []
    id = None
    name = None
    second_name = None
    sex = None
    f_id = None
    f_hus = None
    f_wife = None
    list = []
    print(1)
    with open(filename, encoding='utf-8') as file:
        for line in file:
            if 'INDI' in line:
                id = line.split()[1]
            elif 'NAME' in line:
                name = line.split()[2]
            elif 'SURN' in line:
                second_name = line.split()[2]
            elif 'SEX' in line:
                sex = line.split()[2]
                persons.append([id, name, second_name, sex])
            elif 'FAM\n' in line:
                if list != []:
                    families.append(list)
                    list = []
                f_id = line.split()[1]
            elif 'HUSB' in line:
                f_hus = line.split()[2]
            elif 'WIFE' in line:
                f_wife = line.split()[2]
                list.append(f_id)
                list.append(f_hus)
                list.append(f_wife)
            elif 'CHIL' in line:
                list.append(line.split()[2])
    return persons, families


def facts(filename, persons, families):
    with open(filename, 'w') as file:
        for person in persons:
            if person[3] == 'M':
                file.write('male('+person[0]+')\n')
            if person[3] == 'F':
                file.write('female('+person[0]+')\n')
        for family in families:
            for j in range(2):
                for i in range(len(family)):
                    try:
                        file.write('child('+family[i+3]+', '+family[j+1]+')\n')
                    except:
                        a = 5


persons, families = parsing('tree.ged')
print(persons)
print(families)
facts('facts.txt', persons, families)
