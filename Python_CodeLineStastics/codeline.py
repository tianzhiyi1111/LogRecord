# 1. 遍历该目录下所有的文件。
# 2. 判断文件是否以“.cpp”/".h"结尾。（以C++代码为例）
# 3. 打开.cpp和.h文件（切忌勿用W+，W+会清空文件内容）
# 4. 循环读取文件的每一行
# 5. 判断每一行的内容：
    #(a) 注释: 以//或/*开头。
    #(b) 空行：除空白字符无其他。
    #(c) 代码行：除空白字符之后还剩下其他字符。
# 6. 判断是否为文件末尾,    
# 7. 关闭文件, 返回结果. 

#_*_coding:utf-8_*_

# 统计代码行数, 空行, 注释.

import os
def code_lines_count(path):
    code_lines = 0
    comm_lines = 0
    space_lines = 0
    # 此处依次遍历子文件夹下的文件
    for root,dirs,files in os.walk(path):
        for item in files:
            file_abs_path = os.path.join(root,item)
            postfix = os.path.splitext(file_abs_path)[1]
            if postfix == '.py':
                #print 'Start: ',file_abs_path
                with open(file_abs_path) as fp:
                    while True:
                        line = fp.readline()
                        if not line:
                            #print 'break here,%r' %line
                            break
                        elif line.strip().startswith('//') or line.strip().startswith('/*'):
                            #print '1, here',line
                            comm_lines += 1  
                        elif line.strip():
                            #print '5, here',line
                            code_lines += 1
                        else:
                            #print '6, here',line
                            space_lines +=1
                #print 'Done',file_abs_path
    return code_lines,comm_lines,space_lines
#test
print "Code lines: %d\nComments lines: %d\nWhiteSpace lines: %d" %code_lines_count(r'D:\exercises')