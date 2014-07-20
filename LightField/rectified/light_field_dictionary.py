import os
import re

def main():
    lst = []
    pat = re.compile('out_([0-9]{2})_([0-9]{2})_([0-9\-\.]+)_([0-9\-\.]+)\.png')

    nr = 0
    nc = 0
    for name in os.listdir():
        mat = pat.match(name)
        if mat is not None:
            r = int(mat.group(1))
            c = int(mat.group(2))
            x = float(mat.group(3))
            y = float(mat.group(4))
            lst.append((r, c, x, y, name))
            nr = max(nr, r)
            nc = max(nc, c)

    nr += 1
    nc += 1
    with open('dataset.txt', 'w') as fp:
        fp.write('{0} {1}\n'.format(nr, nc))
        for item in lst:
            fp.write('{0} {1} {2} {3} {4}\n'.format(item[0], item[1], item[2], item[3], item[4]))
        

if __name__=='__main__':
    main()
