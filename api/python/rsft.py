#%% sftools
import m8r as sf
import numpy as np
import subprocess
import os
from matplotlib import pyplot as plt
import string

def cmd(command, return_output=False):
    """Run command and pipe what you would see in terminal into the output cell.
    If return_output=True the output is also returned as a string.
    
    Args:
    input: command (string) to be executed in the terminal
    output: nothing, or the output from the terminal (return_output=True)
    
    """
    process = subprocess.Popen(command, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
    while True:
        output = process.stderr.readline().decode('utf-8')
        if output == '' and process.poll() is not None:
            # this prints the stdout in the end
            output2 = process.stdout.read().decode('utf-8')
            print(output2.strip())
            break
        if output:
            print(output.strip())
    rc = process.poll()
    if return_output:
        return (output2+output)
    
    return rc

def to_np(fname):
    '''
    returns numpy array read from .rsf file
    
    input: file name
    output: numpy array axis 0 is matching
    '''
    f = sf.Input(fname)
    np_arr = f.read().T
    assert np_arr.shape[0] == int(f.get("n1"))
    return np_arr

def vd(fname):
    '''
    parses header from the fname to variable dictionary
    
    input: file name
    output : variable dictionary vd, 
    e.g. vd["n1"] contains the first dimension in rsf file
    '''
    header = cmd(f"sfin {fname}", return_output=True)
    print(header)
    return parse_header_to_dict(header)

def write_vd(fname, vd):
    '''
    writes variable dictionary to header of the file fname
    
    input: file name, variable dictionary
    '''
    for key, val in vd.items():
        cmd(f"sfput < {fname} {key}={val} > .tmp.{fname}")
        cmd(f"sfmv .tmp.{fname}  {fname}")
    return 0

def np_to_rsf(vel, model_output, d1 = 1, d2 = 1):
    ''' Write 2D numpy array vel to rsf file model_output '''
    yy = sf.Output(model_output)
    yy.header = ''
    yy.put('n1',np.shape(vel)[1])
    yy.put('n2',np.shape(vel)[0])
    yy.put('d1',d1)
    yy.put('d2',d2)
    yy.put('o1',0)
    yy.put('o2',0)
    yy.headerflushed = True
    yy.write(vel)
    yy.close()

def write_arr(fname, arr, vd=None):
    '''
    writes array to a file fname
    (?need to check shapes before writing?)
    
    input:  file name, numpy array
    '''
    f = sf.Output(fname)
    f.header = ''
    i = 1
    for n in arr.shape:
        print(f"set n{i}={n}")
        f.put(f"n{i}", n)
        if vd==None:
            f.put(f"d{i}",1.0)
            f.put(f"o{i}",0.0)
        else:
            assert vd[f"n{i}"]==n
        i += 1
    
    if vd != None:
        for key, val in vd.items():
            print(f"{key}={val}")
            #if (key[0] in ["n","o","d"]) and (key[1] in string.digits):
            f.put(key, val)
    f.write(arr.T)
    f.close()
    return 0

def view2d(fname):
    plt.imshow(to_np(fname))
    plt.axis("tight")
    plt.colorbar()
    plt.show()
    

def parse_header_to_dict(header):
    """Parse RSF header into a dictionary of variables, modified from m8r.py
    
    input: header -- string
    output: dictionary of variables
    """
    vd = {}  # variable dictionary
    ilist = header.split()
    pos = 0
    squot = "'"
    dquot = '"'
    while pos < len(ilist):
        if '=' in ilist[pos]:
            tokenlist = ilist[pos].split('=')
            lhs = tokenlist[0]
            rhs = tokenlist[1]
            quotmark = None
            if rhs[0] in (squot, dquot):
                if rhs[0] == squot:
                    quotmark = squot
                else:
                    quotmark = dquot
                if rhs[-1] == quotmark:
                    rhs_out = rhs.strip(quotmark)
                    pos += 1
                else:
                    rhs_out = rhs.lstrip(quotmark)
                    while pos < len(ilist):
                        pos += 1
                        rhs_out += ' '
                        if ilist[pos][-1] == quotmark:
                            rhs_out += ilist[pos][:-1]
                            break
                        else:
                            rhs_out += ilist[pos]
            else:
                rhs_out = rhs
                pos += 1
            if lhs[0]=="n" and (lhs[1] in string.digits):
                rhs_out = int(rhs_out)
            elif (lhs[0] in ["d","o"]) and (lhs[1] in string.digits):
                rhs_out = float(rhs_out)
            vd[lhs] = rhs_out   
        else:
            pos += 1
    return vd

class rsf():
    """ class for rsf = fname + numpy array + variable dictionary """
    def __init__(self, fname):
        self.fname = fname
        self.read()
    
    def write(self):
        #write_vd(self.fname, self.vd)
        write_arr(self.fname, self.arr, self.vd)
    
    def read(self):
        fname = self.fname
        self.arr = sf.Input(fname).read()
        self.vd = vd(fname)        

