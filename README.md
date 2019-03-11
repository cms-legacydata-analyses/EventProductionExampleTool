# Guide for Event Production with CMS Infrastructure

This is a guide on how to produce [CMSSW](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCMSSWFramework)
python configuration files that can be used to perform the generation, simulation and/or reconstruction of [Monte 
Carlo (MC)](/docs/cms-mc-production-overview) (or real) collision events.  These events can be used later to
perform physics analyses.  Take into account, however, that a great amount of
MC samples have been released as Open Data by the CMS Collaboration, and that the reconstruction of RAW real data is not generally needed (nor RAW samples available.)

Each package in this repository guides you to produce a few events of the process referenced in the name of the package.  The instructions allow you to produce the necessary configuration files that are needed and which have been stored in the repository for comparison.

The examples are a bit different depending on the epoch (2010, 2011, etc.) of the CMS data.  One can pick the appropiate branch in order to get the correct version.