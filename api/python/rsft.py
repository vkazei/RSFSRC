#%% sftools
import m8r as sf
import numpy as np
import subprocess
import os

def cmd(command, return_output=False):
    """Run command and pipe what you would see in terminal into the output cell.
    If return_output=True the output is returned as a string.
    
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
    return numpy array read from .rsf file
    
    input: file name
    output: numpy array axis 0 is matching
    '''
    f = sf.Input(fname)
    np_arr = f.read().T
    
    assert np_arr.shape[0] == f.get("n1")
    return np_arr

def vd(fname):
    '''
    parses header from the fname to variable dictionary
    
    input: file name
    output : variable dictionary vd, 
    e.g. vd["n1"] first dimension in rsf file
    '''
    header = cmd(f"sfin {fname}", return_output=True)
    print(header)
    return parse_header_to_dict(header)

def write_vd(fname, vd):
    '''
    writes variable dictionary to header of the file fname
    
    input: file name, variable dictionary
    '''
    f = sf.Output(fname)
    for key, val in vd.items():
        f.put(key, val)
    f.close()
    return 0

def write_arr(fname, arr):
    '''
    writes array to a file fname
    (?need to check shapes before writing?)
    
    input:  file name, numpy array
    '''
    f = sf.Output(fname)
    f.write(arr)
    f.close()
    assert 
    return 0

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
        write_vd(self.fname, self.vd)
        write_arr(self.fname, self.arr)
    
    def read(self):
        fname = self.fname
        self.arr = sf.Input(fname).read()
        self.vd = vd(fname)        

