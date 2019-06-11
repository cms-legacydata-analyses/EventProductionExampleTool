# How to produce 2011 MC Drell-Yan events from scratch with no Pile-up

We will produce these events in three steps.  First we perform the simulation up to the *SIM* step, then another intermedite step
up to the HLT simulation, and then the reconstruction. 

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

Next, identify the configuration fragment that determines what physics event generator we wish to use and what topology we intend to generate.  In
this example we will use the `DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff.py` fragment , which can be found in the [/Configuration/Generator/python](https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/Configuration/Generator/python) area of CMSSW.

##### Step 0: Generation and simulation

- Execute the *cmsDriver* command as:

```
cmsDriver.py DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff.py --mc --eventcontent=RAWSIM --datatier=GEN-SIM --conditions=START53_LV6A1::All --step=GEN,SIM --python_filename=gensimDY.py --no_exec --number=10 --fileout=gensimDY.root
```

Note that we put the naked name of our input fragment (*DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff.py*) because the script will look, by default, in
the */Configuration/Generator/python* area of the CMSSW release.  More information about the *--datatier* used can be found in the [CMS Workbook](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookDataFormats); that is the level of information we need/want in our ROOT output file.

Notice also that wee have used
the `START53_LV6A1::All` conditions, because this is the snapshot of the conditions database we need.  More information about this can
be found in the [CMS Guide for Conditions](http://opendata.cern.ch/docs/cms-guide-for-condition-database) documentation.  As noted above, for this first step, step 0, we
only do the *GEN* and *SIM* parts of the whole chain.  We only generate 10 events for this example and choose the name of *gensimDY* for the output files
in order to identify them correctly.

After executing this command, we will get the *gensimDY.py* configuration file, which will be run with the *cmsRun* executable.  First, however, we need
to do a few modifications.

- Note that we need to be able to locate the database conditions as required by the *--conditions* switch.  Therefore, we need to make the following
 symbolic links:

```
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1 START53_LV6A1

ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db START53_LV6A1.db
```

- Make sure the `cms-opendata-conddb.cern.ch` directory has actually expanded in your VM.  One way of doing this is executing:

```
ls -l
ls -l /cvmfs/
```

You should now see the `cms-opendata-conddb.cern.ch` link in the `/cvmfs` area.

- Open the *gensimDY.py* config file with your favorite text editor and replace the line

```
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_LV6A1::All', '')
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db')
process.GlobalTag.globaltag = 'START53_LV6A1::All'
```

- Run the CMSSW executable in the background

```
cmsRun gensimDY.py > gensimDY.log 2>&1 &
``` 

- Optionally, check the development of the job. You can do this periodically and exit at any time with CTRL+C:

```
tailf gensimDY.log
```


##### Step 1: HLT


- Execute the *cmsDriver* command as:

```
cmsDriver.py step1 --filein file:gensimDY.root --step=DIGI,L1,DIGI2RAW,HLT:2011 --datatier GEN-RAW --conditions=START53_LV6A1::All --fileout=hltDY.root --eventcontent RAWSIM --python_filename hltDY.py --number=10 --mc --no_exec
```

Note here that the ROOT file *gensimDY.root*, which was obtained in the last step (step 0), serves as input for step 1.  
We now process the event up to the high level trigger (HLT) simulation.  This command produces a file, *hltDY.py*, which needs to be modified
like we did above.  I.e.,

- open the *hltDY.py* config file with your favorite text editor and replace the line

```
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_LV6A1::All', '')
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db')
process.GlobalTag.globaltag = 'START53_LV6A1::All'
```

- Now, run the CMSSW executable in the background

```
cmsRun hltDY.py > hltDY.log 2>&1 &
``` 

- Optionally, check the development of the job. You can do this periodically and exit at any time with CTRL+C:

```
tailf hltDY.log
```


##### step 2: RECO (AOD)

- Execute the *cmsDriver* command as:

```
cmsDriver.py step2 --filein file:hltDY.root --step RAW2DIGI,L1Reco,RECO,VALIDATION:validation_prod,DQM:DQMOfflinePOGMC --datatier AODSIM,DQM --conditions START53_LV6::All --fileout=recoDY.root --mc --eventcontent AODSIM,DQM  --python_filename recoDY.py --no_exec -n 10 
```

Note here that the ROOT file *hltDY.root*, which was obtained in the last step (step 1), serves as input for step 2.  
We now process the event up to the final step: the reconstruction (RECO).  This command produces a file, *recoDY.py*, which needs to be modified
like we did above.  I.e.,

- open the *recoDY.py* config file with your favorite text editor and replace the line

```
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_LV6A1::All', '')
```

with

```
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db')
process.GlobalTag.globaltag = 'START53_LV6A1::All'
```

- Now, run the CMSSW executable in the background

```
cmsRun recoDY.py > recoDY.log 2>&1 &
``` 

- Optionally, check the development of the job. You can do this periodically and exit at any time with CTRL+C:

```
tailf recoDY.log
```

The resulting ROOT file, *recoDY.root*, is in the same format as 
the MC and Data released by CMS.
