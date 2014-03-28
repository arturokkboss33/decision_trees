decision_trees
==============

C++ code for binary decision trees

description
===========

This version of binary trees was done completely by the author.

For testing, the dataset in http://archive.ics.uci.edu/ml/datasets/SPECT+Heart was used, as well as simple self-made example.

With the algorithm a 79.7% accuracy is achieved for the mentioned dataset; not far from the 84.0% accuracy achieved by CLIP3 (described in the same URL).

installation
============

You only need to install g++ compiler.

how to
======

Download the files. 

Run make. 

Run the exe file generated with the next instruction:

./dectree_exe FILE

Where FILE is the base name of your training and test dataset (without extension).

Name your training dataset as FILE.train and your test dataset as FILE.test, and see the examples in the project to ensure their correct parsing. (And save the files in the correct folder!)

The first column in the files is the output classification, and the rest is the binary value of the cases' attributes.
