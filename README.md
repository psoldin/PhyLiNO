# PhyLiNO

This is the software implementation of the framework `PhyLiNO`, as described in [this publication](https://arxiv.org/abs/2502.15253).

## Abstract

We present a framework for the analysis of data from neutrino oscillation experiments. The framework performs a profile likelihood fit and employs a forward folding technique to optimize its model with respect to the oscillation parameters. It is capable of simultaneously handling multiple datasets from the same or different experiments and their correlations. The code of the framework is optimized for performance and allows for convergence times of a few seconds handling hundreds of fit parameters, thanks to multi-threading and usage of GPUs. The framework was developed in the context of the Double Chooz experiment, where it was successfully used to fit three- and four-flavor models to the data, as well as in the measurement of the energy spectrum of reactor neutrinos. We demonstrate its applicability to other experiments by applying it to a study of the oscillation analysis of a medium baseline reactor experiment similar to JUNO.

The software implementation is done, an example for simulated Double Chooz like data is coming very soon!
