fo = open('csim2.log', 'w')

with open('csim.log', 'r') as f:
  while True:
    line = f.readline()
    if not line:
      break
    if line[0] == 'm' or line[0] == 'h':
      fo.write(line)