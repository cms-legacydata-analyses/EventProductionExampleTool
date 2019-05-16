# How to produce 2012 MC QCD events with Tracker Hits info from scratch with Pile-up (good for Machine Learning studies) 
This sample is an special one in the sense that it uses low-level tracking information to create event-level or jetlevel classifiers for ML; it also gives us position of pixel and strip tracker hits in simple ntuple-like format, this is useful because in AOD opendata files  no tracker rechits are available.


We will produce these events in four steps.  First we perform the simulation up to the *SIM* step, then another step
up to the *HLT* simulation, then up to the reconstruction *RECO*, and then finaly extract information from a CMS root file type EDM  by
an EDAnalyzer in ROOT *ntuple* format. 

To start, first create a [VM](http://opendata.cern.ch/record/252 "CMS Open Data Portal") from the CMS Open Data website.

Then follow these steps:

- Create a CMSSW environment: 

    ```
    cmsrel CMSSW_5_3_32
    ```

- Change to the CMSSW_5_3_32/src/ directory:

    ```
    cd CMSSW_5_3_32/src/
    ```

- Initialize the CMSSW environment:

  ```
  cmsenv
  ```   

In this example we will use the code of an EDAnalyzer that will give us the information in NTUPLE format. Therefore, start with these steps:

- Obtain the code from git:

  ```   
  git clone -b 2012 git@github.com:cms-legacydata-analyses/EventProductionExamplesTool.git 
  ```   
- Change to EventProductionExamplesTool/MCQCDProductionMLTrackInfo directory 

  ```
  cd EventProductionExamplesTool/MCQCDProductionMLTrackInfo
  ```
- Compile everything:

  ```
  scram b
  ```
  
<!--Next, identify the configuration fragment that determines what physics event generator we wish to use and what topology we intend to generate. -->
In this example we will use the `QCDForPF_8TeV_cfi.py` fragment , which can be found in the [/Configuration/Generator/python](https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/Configuration/Generator/python) area of CMSSW.  

##### Step 0: Generation and simulation

- Execute the *cmsDriver* command as:

```
cmsDriver.py QCDForPF_8TeV_cfi.py --fileout file:gensimQCD.root --mc --eventcontent RAWSIM --customise Configuration/StandardSequences/SimWithCastor_cff.customise,Configuration/DataProcessing/Utils.addMonitoring --beamspot Realistic8TeVCollision --datatier GEN-SIM --conditions=START53_V27::All --step GEN,SIM --datamix NODATAMIXER --python_filename gensimQCD.py --no_exec -n 10
```

Note that we put the naked name of our input fragment (*QCDForPF_8TeV_cfi.py*) because the script will look, by default, in
the */Configuration/Generator/python* area of the CMSSW release.  More information about the *--datatier* used can be found at the [CMS Workbook](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookDataFormats); that is the level of information we need/want in our ROOT output file.

Notice also that we have used
the `START53_V27::All` conditions, because this is the snapshot of the conditions database we need.  More information about this can
be found at the [CMS Guide for Conditions]( http://opendata.cern.ch/docs/cms-guide-for-condition-database) documentation.  As noted above, for this first step, step 0, we
only do the *GEN* and *SIM* parts of the whole chain.  We only generate 10 events that for this example is the minimum and choose the name of *gensimQCD* for the output files
in order to identify them correctly.

After executing this command, we will get the *gensimQCD.py* configuration file, which will be run with the *cmsRun* executable.  First, however, we need
to do a few modifications.

- Note that we need to be able to locate the database conditions as required by the *--conditions* switch.  Therefore, we need to make the following
 symbolic links:

```
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_V27 START53_V27

ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_V27.db START53_V27.db
```

- Make sure the `cms-opendata-conddb.cern.ch` directory has actually expanded in your VM.  One way of doing this is executing:

```
ls -l
ls -l /cvmfs/
```

You should now see the `cms-opendata-conddb.cern.ch` link in the `/cvmfs` area.

- Open the *gensimQCD.py* config file with your favorite text editor and change the line

```
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_V27.db')
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')
```
This sample is dedicated to studying 'boosted' events so we need change the parameter *pThat* threshold  in a pythia generator v6 from pThat>15 to pThat>600 , for more information of this parameters visit [pythia documentation](http://home.thep.lu.se/Pythia/).

- For this in the *gensimQCD.py* change the line

```
'CKIN(3)=15.           ! minimum pt hat for hard interactions',
```
with

```
'CKIN(3)=600.           ! minimum pt hat for hard interactions',
```

- Run the CMSSW executable in the background

```
cmsRun gensimQCD.py > gensimQCD.log 2>&1 &
``` 

- Check the development of the job:

```
tailf gensimQCD.log
```


##### Step 1: HLT


- Execute the *cmsDriver* command as:

```
cmsDriver.py step1 --filein file:gensimQCD.root --fileout file:hltQCD.root --mc --eventcontent RAWSIM --runsScenarioForMC Run2012_AB_C_D_oneRunPerEra --datatier GEN-SIM-RAW --conditions=START53_V27::All --step DIGI,L1,DIGI2RAW,HLT:7E33v2 --python_filename hltQCD.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 10 --pileup_input root://eospublic.cern.ch//eos/opendata/cms/MonteCarlo2012/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/FEF2F4CC-0E6A-E111-96F6-0030487F1C57.root --pileup 2012_Summer_50ns_PoissonOOTPU
```

Note here that the ROOT file *gensimQCD.root*, which was obtained in the last step (step 0), serves as input for step 1.  

The MinBias file with pile-up events used is a particular example made available for these instructions. Be aware that these files may or may not be available as part of an official release, and that one may need to simulate those as well.

We now process the event up to the high level trigger (HLT) simulation.  This command produces a file, *hltQCD.py*, which needs to be modified
like we did above.  I.e.,

- open the *hltQCD.py* config file with your favorite text editor and change the line

```
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_V27.db')
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')
```
- In order to save the tracking hits information, change the line:

```
outputCommands = process.RAWSIMEventContent.outputCommands
```
with

```
outputCommands = process.RAWSIMEventContent.outputCommands+cms.untracked.vstring('keep *_simSiPixelDigis_*_*','keep *_simSiStripDigis_*_*'),
```

- Now, run the CMSSW executable in the background

```
cmsRun hltQCD.py > hltQCD.log 2>&1 &
``` 

- Check the development of the job:

```
tailf hltQCD.log
```


##### step 2: RECO (AOD)

- Execute the *cmsDriver* command as:

```
cmsDriver.py step2 --filein file:hltQCD.root --step RAW2DIGI,L1Reco,RECO,VALIDATION:validation_prod,DQM:DQMOfflinePOGMC --datatier AODSIM,DQM --conditions START53_V27::All --fileout file:recoQCD.root --mc --eventcontent AODSIM,DQM --python_filename recoQCD.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 10
```

Note here that the ROOT file *hltQCD.root*, which was obtained in the last step (step 1), serves as input for step2.  
We now process the event up to the final step: the reconstruction (RECO).  This command produces a file, *recoQCD.py*, which needs to be modified
like we did above.  I.e.,

- open the *recoQCD.py* config file with your favorite text editor and change the line

```
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_V27.db')
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')
```

and change the line
```
outputCommands = process.AODSIMEventContent.outputCommands,
```
with
```
outputCommands = process.AODSIMEventContent.outputCommands+cms.untracked.vstring('keep *_simSiPixelDigis_*_*','keep *_simSiStripDigis_*_*','keep *_siPixelClusters_*_*','keep *_siStripClusters_*_*','keep *_g4SimHits_*_*','keep *_generalTracks_*_*'),
```

- Now, run the CMSSW executable in the background

```
cmsRun recoQCD.py > recoQCD.log 2>&1 &
``` 

- Check the development of the job:

```
tailf recoQCD.log
```

The resulting ROOT file, *recoQCD.root*, is in the same format as 
the MC and Data released by CMS.

##### step 3: NTUPLE
The instructions to write your own EDAnalyzer are [here](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookWriteFrameworkModule).
For this example use an existing EDAnalyzer called SaveHits. For more information on the parameters within this can be found in this [repository](https://github.com/emanueleusai/opendatadnn/tree/master/SaveHits/SaveHits).

- Make a soft link to the python configuration file
  ```
  ln -s python/MLntupleQCD.py .
  ```

- Run the CMSSW configuration file
  ```
  cmsRun MLntupleQCD.py
  ```

As a result you will get a NtupleQCD.root file with simple variables and histograms that can be used for machine learning studies.

