#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Run "python utf8togbk.py" to convert source code files to the GBK format on Windows.
#

import os, codecs, sys

reload(sys)
sys.setdefaultencoding('gbk')

def utf8togbk(dir):
    resultfn = ''
    for fn in os.listdir(dir):
        sfile = os.path.join(dir, fn)
        if os.path.isdir(sfile):
            resultfn += utf8togbk(sfile)
            continue
        if fn.endswith('.h') or fn.endswith('.cpp'):
            if os.path.exists(sfile + '.utf8'):
                continue
            try:
                text = open(sfile,'r',-1,'utf-8').read()
            except UnicodeDecodeError:
                continue
            except TypeError:
                text = open(sfile).read()
                try:
                    if text[:3] == codecs.BOM_UTF8:
                        u = text[3:].decode('utf-8')
                    else:
                        u = text.decode('utf-8')
                    newtext = u.encode('gbk')
                except UnicodeEncodeError:
                    continue
                except UnicodeDecodeError:
                    continue
                newtext = newtext.replace('\r','\n')
                if newtext == text:
                    continue
                text = newtext
                
            try:
                os.rename(sfile, sfile + '.utf8')
                open(sfile, 'wb').write(text.replace('\r','\n'))
                resultfn += fn + ' '
                st = os.stat(sfile + '.utf8')
                os.utime(sfile, (st.st_atime, st.st_mtime))
            except:
                print('except for %s' %(fn,))
    return resultfn

if __name__=="__main__":
    resultfn = utf8togbk(os.path.abspath('..'))
    resultfn += utf8togbk(os.path.abspath('../../core'))
    if resultfn != '': print('utf8->gbk: ' + resultfn)
