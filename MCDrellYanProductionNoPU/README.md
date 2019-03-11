# How to produce 2010 MC Drell-Yan events from scratch with no Pile-up

The objective of this example is to show how to generate Drell-Yan events *from scratch* and go through the full chain of
production in order to obtain *reconstructed* events suitable for analysis.  All the python configuration files and ROOT files generated reside, respectively, in the *python* and *data* folders. 

For 2010 MC samples, **we will not perform the HLT simulation**.  We will produce these events in three steps.  First we perform the simulation up to the *SIM* step, then another intermedite step
up to the L1 (level 1 trigger) simulation, and then the reconstruction. 

To start, first create a [VM](http://opendata.cern.ch/record/250 "CMS Open Data Portal") from the CMS Open Data website 
and open de slc5 CMS shell terminal available in the Desktop.

Then follow these steps:

- Create a CMSSW environment: 

    ```
    cmsrel CMSSW_4_2_8
    ```

- Change to the CMSSW_4_2_8/src/ directory:

    ```
    cd CMSSW_4_2_8/src/
    ```

- Initialize the CMSSW environment:

  ```
  cmsenv
  ```

Next, identify the configuration fragment that determines what physics event generator we wish to use and what topology we intend to generate.  In
this example we will use the `DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff.py` fragment , which can be found in the [/Configuration/Generator/python](https://github.com/cms-sw/cmssw/tree/CMSSW_4_2_X/Configuration/Generator/python) area of CMSSW.  More information on the parameters within this
fragment can be found in the [MC production overview](/docs/cms-mc-production-overview) documentation.

##### Step 0: Generation and simulation

- Execute the *cmsDriver* command as:

```
cmsDriver.py DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff.py --mc --eventcontent=RAWSIM --datatier=GEN-SIM --conditions=START42_V17B::All --step=GEN,SIM --python_filename=gensimDY.py --no_exec --number=10 --fileout=gensimDY.root
```

Note that we put the naked name of our input fragment (*DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff.py*) because the script will look, by default, in
the */Configuration/Generator/python* area of the CMSSW release.  More information about the *--datatier* used can be found at the [CMS Workbook](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookDataFormats); that is the level of information we need/want in our ROOT output file.  

Notice also that wee have used
the `START42_V17B::All` conditions, because this is the snapshot of the conditions database we need.  More information about this can
be found at the [CMS Guide for Conditions](docs/cms-guide-for-condition-database) documentation.  As noted above, for this first step, step 0, we
only do the *GEN* and *SIM* parts of the whole chain.  We only generate 10 events for this example and choose the name of *gensimDY* for the output files
in order to identify them properly.

After executing this command, we will get the *gensimDY.py* configuration file, which will be run with the *cmsRun* executable.  First, however, we need
to do a few modifications.

- Note that we need to be able to locate the database conditions as required by the *--conditions* switch.  Therefore, we need to make the following
 symbolic links:

```
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START42_V17B START42_V17B

ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START42_V17B.db START42_V17B.db
```

- Make sure the `cms-opendata-conddb.cern.ch` directory has actually expanded in your VM.  One way of doing this is executing:

```
ls -l
ls -l /cvmfs/
```

You should now see the `cms-opendata-conddb.cern.ch` link in the `/cvmfs` area.

- Open the *gensimDY.py* config file with your favorite text editor and replace the line

```
process.GlobalTag.globaltag = 'START42_V17B::All'
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START42_V17B.db')
process.GlobalTag.globaltag = 'START42_V17B::All'
```

- Run the CMSSW executable in the background

```
cmsRun gensimDY.py > gensimDY.log 2>&1 &
``` 

- Check the development of the job:

```
tailf gensimDY.log
```

##### Step 1: DIGI, L1

- Execute the *cmsDriver* command as:

```
cmsDriver.py step1 --filein file:gensimDY.root --step=DIGI,L1,DIGI2RAW --datatier GEN-SIM-RAW --conditions=START42_V17B::All --fileout=digiDY.root --eventcontent RAWSIM --python_filename hltDY.py --number=10 --mc --no_exec
```

Note here that the ROOT file *gensimDY.root*, which was obtained in the last step (step 0), serves as input for step1.  
We now process the event up to the L1 (level 1 trigger) simulation.  No HLT simulation is performed for MC datsets for 2010.  

The command produces a file, *digiDY.py*, which needs to be modified
like we did above.  I.e.,

- open the *digiDY.py* config file with your favorite text editor and replace the line

```
process.GlobalTag.globaltag = 'START42_V17B::All'
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START42_V17B.db')
process.GlobalTag.globaltag = 'START42_V17B::All'
```

- Now, run the CMSSW executable in the background

```
cmsRun digiDY.py > digiDY.log 2>&1 &
``` 

- Check the development of the job:

```
tailf digiDY.log
```

##### step 2: RECO (AOD)

- Execute the *cmsDriver* command as:

```
cmsDriver.py step2 --filein file:digiDY.root --fileout recoDY.root --mc --eventcontent AODSIM --pileup NoPileUp --customise Configuration/GlobalRuns/reco_TLR_42X.customisePPMC,Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --conditions START42_V14B::All --step RAW2DIGI,L1Reco,RECO --python_filename recoDY.py --no_exec -n 10
```

Note here that the ROOT file *gensimDY.root*, which was obtained in the last step (step 0), serves as input for step1.  In addition, the configuration file
is customized in the same fashion as the latest CMS production for this epoch. 
We now process the event up to the final step: the reconstruction (RECO).  This command produces a file, *recoDY.py*, which needs to be modified
like we did above.  I.e.,

- open the *recoDY.py* config file with your favorite text editor and replace the line

```
process.GlobalTag.globaltag = 'START42_V17B::All'
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START42_V17B.db')
process.GlobalTag.globaltag = 'START42_V17B::All'
```

- Additionally, add these lines at the end of the *recoDY.py* configuration file:

```
#modify the localreco sequence so it does not requiere luminosity production
process.localreco.remove(process.lumiProducer)

#for consistency, remove products from event content
process.AODSIMoutput.outputCommands.remove('keep LumiSummary_lumiProducer_*_*')
process.AODSIMoutput.outputCommands.append('drop LumiSummary_lumiProducer_*_*')
```

- Now, run the CMSSW executable in the background

```
cmsRun recoDY.py > recoDY.log 2>&1 &
``` 

- Check the development of the job:

```
tailf recoDY.log
```

The resulting ROOT file, *recoDY.root*, is in the same format as 
the MC and Data released by CMS.
