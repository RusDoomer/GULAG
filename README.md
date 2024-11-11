# GULAG
GPU Utilizing Layout Analyzer and Generator : a GPU accelerated keyboard layout optimizer
---
# High level plan
Have an array with the frequency of each ngram (split by ngrams, so monograms is its own array)

Then have a datastructure holding each stat, which holds in it a list of position ngrams which are associated with that stat

To analyze a layout you simply take each stat, iterate over the position ngrams, grab the frequency of each from the frequency array and sum it together

Repeat for each stat, multiply by weights, and comtbine to analyze a full layout
