import re, sys
from subprocess import Popen, PIPE

def append_postfix(name, postfix):
	return "{0}-{2}.{1}".format(*name.rsplit('.', 1) + [postfix])

def load_file(name):
	with open(name, 'r') as f: 
		lines = f.read().splitlines()
	return lines
	
def save_file(name, str):
	with open(name, 'w') as f: 
		f.write(str)
	
def macro_test(str):

	# match macro define
	xx = [(m.start(0), m.group(0)) for m in 
		re.finditer(r'[\w#]+|[^\s\w#]', str)]
	if len(xx) <= 3: return False;
	if xx[0][1].lower() != '#define': return False;
	if xx[2][1] != '(': return False
	
	# get end of arguments
	for x in xx:
		if x[1] == ')': return x[0]+1
	raise ValueError('bad line: ' + str)
	
def macro_fix(str, index):
	base = str[:index]; tail = str[index:]
	tail = tail.replace(' ', '`')
	tail = tail.replace('\t', '$')
	cont = tail.endswith('\\')
	if cont: tail = tail[:-1]+'@\\'
	str = base+tail
	return (cont, str)
	
def encode_file(lines):
	inMacro = False;
	for i in range(len(lines)):
		str = lines[i]
		if inMacro:
			inMacro,lines[i] = macro_fix(str, 0)
		else:
			index = macro_test(str)
			if index != 0:
				inMacro,lines[i] = macro_fix(str, index)
				
def decode_file(str):
	str = str.replace('`', ' ')
	str = str.replace('$', '\t')
	str = str.replace('@', '\n')	
	return str
				
def execute_mcpp(file):
	proc = Popen('mcpp -P "%s"' % (file), stdout=PIPE)
	return proc.communicate()[0]

def do_encode(file):
	lines = load_file(file)
	encode_file(lines)
	file = append_postfix(file, 'x')
	save_file(file, '\n'.join(lines))	
	
def do_decode(file):
	str = execute_mcpp(file)
	str = decode_file(str)
	str = '\n'.join([x.rstrip() for x in str.splitlines()])
	file = append_postfix(file, 'x')
	save_file(file, str)	

do_encode(sys.argv[1])
do_decode(sys.argv[2])
