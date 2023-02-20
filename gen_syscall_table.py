import re


def read_syscall_define(filename):
    with open(filename) as f:
        lines = f.readlines()
    lines = [x.strip() for x in lines]

    table = {}  # {syscall_num, syscall_name}
    pattern = '^#define\s*__NR_(\w*)\s*(\d*)'
    count = 0
    for line in lines:
        count += 1
        result = re.match(pattern, line)
        if result:
            (name, num) = result.groups()
            # print(name, num, line, count)
            table[int(num)] = name

    return table


def write_syscall_table(filename, table):
    with open(filename, 'a') as f:
        f.write('#ifndef __SYSCALL_TABLE_H__\n')
        f.write('#define __SYSCALL_TABLE_H__\n\n')
        f.write('char syscall_table[][32] = {\n')
        idx = 0
        for num, name in table.items():
            # print("num=" + str(num) +", name=" +name)
            if idx < int(num):
                continue
            f.write(f'\t"{name}",\n')
            idx += 1

        f.write('};\n')
        f.write('\n#endif')


if __name__ == '__main__':
    table = read_syscall_define('/usr/include/asm/unistd_64.h')
    write_syscall_table('syscall_table.h', table)
