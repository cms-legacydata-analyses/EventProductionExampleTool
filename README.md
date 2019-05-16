# Guide for Event Production with CMS Infrastructure

## Description
This is a guide on how to produce [CMSSW](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCMSSWFramework)
python configuration files that can be used to perform the generation, simulation and/or reconstruction of Monte Carlo (MC)(or real) collision events.  These events can be used later to
perform physics analyses.  Take into account, however, that a great amount of
MC samples have been released as Open Data by the CMS Collaboration, and that the reconstruction of RAW real data is not generally needed (nor RAW samples available.)

The examples are a bit different depending on the epoch (2010, 2011, etc.) of the CMS data.  One can pick the appropiate branch in order to get the correct version.

Each package in this repository guides you to produce a few events of the process referenced in the name of the package.  The instructions in each allow you to produce the necessary configuration files that are needed and which have been stored in the repository for comparison.  We also show you how to execute the relevant jobs.

The simple tools and examples presented
here are the easiest way to deal with the processing of MC-generated collisions or
real LHC beam collisions.  Obviously, and since most of the Open Data released by CMS is ready for analysis, these
instructions are likely to be more useful for MC production.

The present directives are essentially a short summary of the more-detailed documentation found in Chapter 6 of the [CMS Offline Workbook](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBook), which is publicly available.

In this document the general strategy for event processing is first described.  Then, the `cmsDriver.py` script is presented. This is the steering script, which is used to generate almost any configuration needed.  Finally, one can follow the simple examples for MC production and real data reconstruction in the individual packages in the repository.

## General strategy for event production

Physics event generation and detector simulation are the earliest steps in the event processing chain that leads to producing MC samples suitable for physics analysis.  The reconstruction of these events (and also possibly of real
*RAW* data from the LHC) is also understood as part of event production.  The strategy presented here is a general one
for processing events. Please take into account
 that many changes may occur with time, so the instructions bellow are just simple starting points. The given references, above or bellow, should be consulted in order to expand the required knowledge.

The strategy is presented as a list of points (steps) that need to be considered:

- The final objective is to produce python configuration files that can be run under CMSSW in order to perform the 
    required actions; namely, generation, simulation and/or reconstruction.  For this, the `cmsDriver.py` tool is used. 
    This utility will appear in the user's *PATH* (just like cmsRun) once she has setup the 
    appropiate CMSSW environment.
- There is a difference between processing MC events and real events.  For the former, the `cmsDriver.py` tool 
    helps in putting together configuration files
     for the generation of such events, the simulation of the interaction with the
    detector, and its possible reconstruction.  For the latter, there will be little need of doing anything because 
    most of the Open Data released by CMS is already *reconstructed*, ready to be analyzed.  However, for a series of
    small *RAW* samples, one might need to use the `cmsDriver.py` tool to put together configuration files that can
    perform the full reconstruction.
- These instructions focus on MC event production.  The idea is to use the `cmsDriver.py` to be able to build *any* 
    configuration file out of the large collection of CMSSW pre-fabricated configuration application fragments.  Thus, 
    in most cases a user will only need to know how find the right components, to compose them together, and to execute.
- Since the `cmsDriver.py` tool is part of CMSSW, then the very first step would be to setup an appropiate
     CMSSW release area. Many examples are given bellow.
- Then, one needs to simply execute the `cmsDriver.py` script with the appropiate parameters (see examples
    bellow).  Usually, at the 
    starting point, one of these parameters is always a *configuration input file* that uses one of the many 
    CMSSW interfaces to many physics event generators that are of interest to the Collaboration (e.g., 
    [Pythia](http://home.thep.lu.se/~torbjorn/Pythia.html), 
    [Herwig](http://herwig.hepforge.org/), 
    [Tauola](https://tauolapp.web.cern.ch/tauolapp/), etc.)  This file
    itself has many *physics* parameters that can be tuned. 
- Identifying or creating a proper *initial* python configuration file  that defines the kind of process one is interested in is perhaps the most
complicated part of the production chain.  Fortunately, CMSSW has pre-fabricated fragments which are originally located in the gen production areas (e.g., [/Configuration/Generator/python](https://github.com/cms-sw/cmssw/tree/CMSSW_4_2_X/Configuration/Generator/python) for 2010 production.).  
For the majority of applications for producing MC samples the only difference will be this generator-level configuration fragment, while other conditions and steps will be standard. This is a great benefit of using `cmsDriver.py` for composing applications for MC production, as it will ensure that most current setups and conditions will be employed. 
- Note that external generators can also be used by starting the production chain from an LHE file.
- When it comes to the configuration input files, one needs to remember that the event processing usually happens
    in many steps.  This means that the `cmsDriver.py` might need to be used several times to acquire the configuration
    files that are needed for each step.  One step in the event processing chain may create event data (*edm::Event*) 
    that will serve as an input to the subsequent steps, thus it is important to properly order the 
    sequence of steps for execution. 
- One also need to know that some software components may need 
    other *helper* software, thus those services and modules also need to be present in the configuration file. However, most users will not need to worry about such details as in CMSSW there are utilities which 
    will ensure that all service software is included, and the sequence of steps in the event processing chain is correct.


## The cmsDriver tool

The [cmsDriver](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCmsDriver) is a tool to create production-solid configuration files from minimal command line options.  Its code implementation, the [cmsDriver.py](https://github.com/cms-sw/cmssw/blob/master/Configuration/Applications/scripts/cmsDriver.py) script, is part of the CMSSW software.  

A summary of the `cmsDriver.py` wrapper's options, with a detailed message about each one, can be visualized by getting the help:

```
cmsDriver.py --help
```

The options list is divided into two sections according to the user's level of knowledge: Options and Expert Options. Here, only the former, the "standar" Options, are listed:

- `-s STEP, --step=STEP`: this option is useful to indicate what kind of steps the user wants to run and in which order. The most common, possible values for *STEP* are: 

    - GEN : the generator plus the creation of GenParticles and GenJets
    - SIM : Geant4 simulation of the detector (energy deposits in the detector volumes)
    - DIGI : simulation of detector signal response to the energy deposits
    - L1: simulation of the L1 trigger
    - DIGI2RAW : data format conversion of the digi signals into the RAW format that will be provided in the online system
    - HLT : high level trigger,

  which are usually executed in a single job. Remaining building blocks are executed in a *step 2*:

    - RAW2DIGI : data format conversion of the RAW format into digi signals
    - RECO : full event reconstruction
    - ALCA : production of alignment and calibration streams
    - DQM : code run for DQM
    - VALIDATION : code run for validation

  For fast simulation, things are slightly different. As the fast simulation combines a lot of things, there are only two standard steps:

    - GEN
    - FASTSIM

- `--conditions=CONDITIONS`: this option concerns the alignment and calibration conditions the user wants to apply for producing the dataset.

- `--eventcontent=EVENTCONTENT`: the user can select what event content has to be written out in the output by making use of this option.  The user can choose among those available in the files in [Configuration/EventContent/python/](https://github.com/cms-sw/cmssw/tree/master/Configuration/EventContent/python) directory.

- `--filein=FILEIN`: specifying this option the user can indicate the infile name. For instance, if in the previous step the processing of events has been run up to the L1 or HLT, in the next step the reconstruction can be run. In this case the user has to specify what was the output file of the previous generation as infile for the new configuration file.

- `--fileout=FILEOUT`: this option can be used to customize the name for the output file to specify in the configuration file created by cmsDriver.py.

- `-n NUMBER, --number=NUMBER`: indicates the number of events to write out in the event content of the output file. The default is 1.

- `--mc`:  this option, if defined, imposes the processing of simulation. A default value is based on all other defined options.
- `--data`: this option, if defined, imposes the processing of real data. A default value is based on all other defined options.  If neither `--mc` option nor `--data` are specified, it will be determined that the user is requiring simulations.

- `--no_exec`: this option implies to prepare the full python configuration file without execute the *cmsRun* command 
at the end
