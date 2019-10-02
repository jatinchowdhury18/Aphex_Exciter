# Aphex Type B Aural Exciter

This repository contains simulations and real-time implementation for a
virtual analog model of the Aphex Type B Aural Exciter.

![Pic](https://cdn.shopify.com/s/files/1/2604/0858/products/Aphex_Aural_Exciter_Type_B_1024x1024@2x.jpg?v=1525925116)

# Simulations

Simulations are done in `Jupyter Notebook`. Simulation code for various
sections of the circuit can be found in the 
[Simulations]([./Simulations](https://github.com/jatinchowdhury18/Aphex_Exciter/tree/master/Simulations)) folder.

For an in-depth discussion of the Tone circuit, see [![NBViewer](https://github.com/jupyter/design/blob/master/logos/Badges/nbviewer_badge.svg)](https://nbviewer.jupyter.org/github/jatinchowdhury18/Aphex_Exciter/blob/master/Simulations/Filter.ipynb)

# Implementation
Implementation is being done using [JUCE](https://github.com/WeAreROLI/JUCE).
Individual circuit components are each being made as a separate JUCE project,
as well as a full implementation combining all of the subcomponents.

Circuit schematic: https://i.imgur.com/PT9lUi6.gif
