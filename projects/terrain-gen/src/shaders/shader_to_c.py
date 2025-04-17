#!/usr/bin/env python3

import os
import sys
import glob
    
direct = "gen/"

# infiles = [
    # "default.vert",
    # "default.frag",
    # "simple.vert",
    # "simple.frag",
# ]

header = "shaders.h"

warning = "\n /*\n  *    ==== DO NOT EDIT THIS FILE ====\n  */\n\n"



#curr = os.sep.join(sys.argv[0].split(os.sep)[:-1])+'/'
curr = os.path.dirname(os.path.realpath(__file__)) + '/'

if not os.path.exists(curr+direct):
    os.mkdir(curr+direct)

header = direct + header

#if os.path.exists(header):
#    os.remove(header)



infiles = (
    glob.glob(os.path.join(curr, '*.vert')) + 
    glob.glob(os.path.join(curr, '*.frag')))

infiles = [os.path.basename(path) for path in infiles]
    



# with open(curr+header, 'w') as fout:
    # fout.write(warning)
    # fout.write("#ifndef GEN_SHADERS_H\n#define GEN_SHADERS_H\n")

head_body = ""

change = False
for fname in infiles:
    src_time = os.path.getmtime(curr+fname)
    if (os.path.exists(curr+direct+fname+'.c')):
        dst_time = os.path.getmtime(curr+direct+fname+'.c')
    else:
        dst_time = 0
    if (src_time > dst_time):
        change = True
        with open(curr+fname, 'r') as fin, open(curr+direct+fname+'.c', 'w') as fout:
            fout.write(warning)
            fout.write('const char ' + fname.replace('.', '_') + '[]=')
            for line in fin:
                nline = line.strip().replace('"', '\\"').replace('\\', '\\\\')
                #if nline != "" and nline[0:2] != "//":
                fout.write('"'+nline+'\\n"\n')
            fout.write(';\n')
        print(fname+' -> '+direct+fname+'.c')
        

if (change):
    with open(curr+header, 'w') as fout:
        fout.write(warning)
        fout.write("#ifndef GEN_SHADERS_H\n#define GEN_SHADERS_H\n")
        for fname in infiles:
            fout.write('extern const char ' + fname.replace('.', '_') + '[];\n')
        fout.write("#endif\n")
    print("generated "+header)
else:
    print("nothing to be done for shaders")

# with open(curr+header, 'a') as fout:
    # fout.write("#endif\n")
