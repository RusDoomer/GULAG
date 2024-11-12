# GULAG
GPU Utilizing Layout Analyzer and Generator : a GPU accelerated keyboard layout optimizer
---
# High level plan
Have an array with the frequency of each ngram (split by ngrams, so monograms is its own array)

Then have a datastructure holding each stat, which holds in it a list of position ngrams which are associated with that stat

To analyze a layout you simply take each stat, iterate over the position ngrams, grab the frequency of each from the frequency array and sum it together

Repeat for each stat, multiply by weights, and comtbine to analyze a full layout


Some notes:
we want a new datastructure for each type of stat (based on ngram) and initialize a new one for each stat we want, it should hold the name and the position ngrams that count, and also its weight

we want scoring to be proportional, so we need to calculate the maximum and minimum scores possible and then normalize scores