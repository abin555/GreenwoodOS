import sys
bs = 1024
head = int(sys.argv[1])

block_offset = head // bs
block_idx = 0
single_idx = 0
double_idx = 0
local_offset = (head) % bs 

if block_offset < 12:
    block_idx = block_offset
elif 12 <= block_offset < 12 + (bs // 4):
    block_idx = 12
    single_idx = block_offset - 12
elif block_offset >= 12 + (bs // 4):
    block_idx = 13
    offset = block_offset - (11 + (bs // 4))
    single_idx = offset % (bs // 4)
    double_idx = (offset // (bs // 4)) * (bs // 4) % 255

print(head, local_offset, block_idx, single_idx, double_idx)
