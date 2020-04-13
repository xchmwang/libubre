import sys


def read_file(filename):
    f = open(filename, 'r')
    content = f.read()
    f.close()
    return content


def time_parser(line):
    time = line.split()[1]
    eles = time.split(':')
    hour = int(eles[0])
    minute = int(eles[1])
    second = float(eles[2])
    return 3600 * hour + 60 * minute + second


def parser(content):
    lines = content.split('\n')
    time_start = time_parser(lines[0])
    time_end = time_parser(lines[1])
    count = int(lines[2])
    return time_start, time_end, count


def all_in_one(content):
    lines = content.split('\n')
    target = [line for line in lines if 'bytecode run ' in line]
    last_line = target[len(target) - 1]

    starts = [line for line in target if 'bytecode run start' in line]
    times = [time_parser(line) for line in starts]
    delta = [time_next - time for time, time_next in zip(times, times[1:])]
    idx_list = [idx for idx, ele in enumerate(delta, start=0) if ele > 1]
    index = max(idx_list) + 1
    first_line = starts[index]

    return time_parser(first_line), time_parser(last_line), len(starts[index:])


def main():
    filename = sys.argv[1]
    cmd = sys.argv[2]

    content = read_file(filename)
    op_code = {
            'manual':'parser(content)',
            'auto':'all_in_one(content)'
            }
    start, end, count = eval(op_code[cmd])
    avg = (end - start) / count
    print start, end, count
    print avg


if __name__ == '__main__':
    main()

