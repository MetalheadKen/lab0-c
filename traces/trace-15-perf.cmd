# Test performance of insert_tail, size, reverse, and sort
option fail 0
option malloc 0
# For test natural sort, we extend the amount of time limit
option time 3
new
ih dolphin 1000000
it gerbil 1000000
size 1000
reverse
sort
size 1000

