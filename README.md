# Space-Saving-Algorithm

Space saving algorithm is used for finding the k most frequent items in a data stream . This algorithm is effectively used for preventing DOS attacks. This algorithm was implemented as an exercise to understand various algorithms for counting frequent items in a data stream as a part of GSOC 2018  project discussion.

The procedure for running the algorithm is as follows :

1) Run the test.c script to generate a biased dataset for testing. Make sure the number of ouputs is between [ 1, 10^6 ] otherwise runtime error occurs.

2) Run space_saving_tree.c script to generate the list of numbers to be deleted ( ip addresses to be blackholed ). The script generates output1.txt file as output.

3) Run space_saving_table.c script to generate the list of numbers to be deleted ( ip addresses to be blackholed ). The script generates output.txt file as output.

4) The above two scrits use the space saving algorithm for generating the list of numbers. The first script uses an AVL tree for hash map implementation whereas the second script uses a hash table implementation. The second script runs 3-4 times faster than the first one.

5) Run correct.c script to generate the 100% accurate output. Output is written to correct.txt file.
