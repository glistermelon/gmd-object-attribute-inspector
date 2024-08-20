import json

data = {}

def get_type(s : str):
    if s == '**integer**': return 'int'
    if s == '**float**': return 'float'
    if s == '**bool**': return 'bool'
    if s == '**[HSV]()**': return 'color'
    if s == '**integer array**': return 'array'
    if s == '**string**': return 'str'
    if 'option' in s: return 'int'
    print('Unknown type:', s)
    exit()

for ln in open('md.md'):
    ln = [s.strip() for s in ln.strip().split('|')[1:-1]]
    print(ln)
    data[ln[0]] = {
        "name": ln[1],
        "type": get_type(ln[2]),
        "desc": ln[3] if len(ln) >= 4 else None
    }

with open('docs.json', 'w') as f:
    f.write(json.dumps(data, separators=(',',':')))