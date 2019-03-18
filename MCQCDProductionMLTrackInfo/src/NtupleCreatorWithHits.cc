// -*- C++ -*-
//
// Package:    NtupleCreatorWithHits
// Class:      NtupleCreatorWithHits
// 
/**\class NtupleCreatorWithHits NtupleCreatorWithHits.cc NtupleCreatorWithHits/NtupleCreatorWithHits/src/NtupleCreatorWithHits.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Emanuele Usai,,,
//         Created:  Tue Aug 21 07:28:17 CEST 2018
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//added
#include <string>
#include <iostream>
#include <vector>

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

//#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

#include "TTree.h"
#include "TLorentzVector.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h" 
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"




//
// class declaration
//

class NtupleCreatorWithHits : public edm::EDAnalyzer {
   public:
      explicit NtupleCreatorWithHits(const edm::ParameterSet&);
      ~NtupleCreatorWithHits();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      unsigned int getLayer(const DetId& detid) ;

      // ----------member data ---------------------------
      TFile * tf1;
      TTree * hits_tree;
      //TTree * tracks_tree;

      edm::ParameterSet conf_;

      std::vector<float> hit_global_x;
      std::vector<float> hit_global_y;
      std::vector<float> hit_global_z;
      std::vector<float> hit_local_x;
      std::vector<float> hit_local_y;
      std::vector<float> hit_local_x_error;
      std::vector<float> hit_local_y_error;
      // std::vector<float> hit_cluster_global_x;
      // std::vector<float> hit_cluster_global_y;
      // std::vector<float> hit_cluster_global_z;
      // std::vector<float> hit_cluster_local_x;
      // std::vector<float> hit_cluster_local_y;
      // std::vector<float> hit_cluster_local_x_error;
      // std::vector<float> hit_cluster_local_y_error;
      // std::vector<float> hit_cluster_charge;
      // std::vector<unsigned int>  hit_cluster_size;
      // std::vector<unsigned int>  hit_cluster_size_x;
      // std::vector<unsigned int>  hit_cluster_size_y;
      std::vector<unsigned int>  hit_sub_det; //1 PixelBarrel, 2 PixelEndcap, 3 TIB, 4 TOB, 5 TID, 6 TEC
      std::vector<unsigned int>  hit_layer;
      std::vector<unsigned int>  hit_type; // 0 pixel, 1 "rphiRecHit", 2 "stereoRecHit", 3 "rphiRecHitUnmatched" 4 "stereoRecHitUnmatched"

      std::vector<int>  hit_simtrack_id;
      std::vector<unsigned int>  hit_simtrack_index;
      std::vector<bool>          hit_simtrack_match;
      std::vector<unsigned int>  hit_genparticle_id;
      std::vector<int>           hit_pdgid;
      std::vector<unsigned int>  hit_recotrack_id;
      std::vector<bool>          hit_recotrack_match;
      std::vector<bool>          hit_genparticle_match;
      std::vector<unsigned int>  hit_genjet_id;
      std::vector<bool>          hit_genjet_match;

      std::vector<unsigned int> simtrack_id;
      std::vector<int>          simtrack_pdgid;
      std::vector<int>          simtrack_charge;

      std::vector<float> simtrack_px;
      std::vector<float> simtrack_py;
      std::vector<float> simtrack_pz;
      std::vector<float> simtrack_energy;
      
      std::vector<unsigned int> simtrack_vtxid;
      std::vector<unsigned int> simtrack_genid;
      std::vector<uint32_t> simtrack_evtid;

      std::vector<int> genpart_collid;
      std::vector<int> genpart_pdgid;
      std::vector<int> genpart_charge;

      std::vector<float> genpart_px;
      std::vector<float> genpart_py;
      std::vector<float> genpart_pz;
      std::vector<float> genpart_energy;

      std::vector<int> genpart_status;

      std::vector<float> genjet_px;
      std::vector<float> genjet_py;
      std::vector<float> genjet_pz;
      std::vector<float> genjet_energy;

      std::vector<float> genjet_emEnergy;
      std::vector<float> genjet_hadEnergy;
      std::vector<float> genjet_invisibleEnergy;
      std::vector<float> genjet_auxiliaryEnergy;
      std::vector<std::vector<int> > genjet_const_collid;
      std::vector<std::vector<int> > genjet_const_pdgid;
      std::vector<std::vector<int> > genjet_const_charge;

      std::vector<std::vector<float> > genjet_const_px;
      std::vector<std::vector<float> > genjet_const_py;
      std::vector<std::vector<float> > genjet_const_pz;
      std::vector<std::vector<float> > genjet_const_energy;


      std::vector<float> track_chi2;
      std::vector<float> track_ndof;
      std::vector<float> track_chi2ndof;
      std::vector<float> track_charge;
      std::vector<float> track_momentum;
      std::vector<float> track_pt;
      std::vector<float> track_pterr;
      std::vector<unsigned int>  track_hitsvalid;
      std::vector<unsigned int>  track_hitslost;
      std::vector<float> track_theta;
      std::vector<float> track_thetaerr;
      std::vector<float> track_phi;
      std::vector<float> track_phierr;
      std::vector<float> track_eta;
      std::vector<float> track_etaerr;
      std::vector<float> track_dxy;
      std::vector<float> track_dxyerr;
      std::vector<float> track_dsz;
      std::vector<float> track_dszerr;
      std::vector<float> track_qoverp;
      std::vector<float> track_qoverperr;
      std::vector<float> track_vx;
      std::vector<float> track_vy;
      std::vector<float> track_vz;
      std::vector<Int_t>   track_algo;
      std::vector<std::vector<float> > track_hit_global_x;
      std::vector<std::vector<float> > track_hit_global_y;
      std::vector<std::vector<float> > track_hit_global_z;
      std::vector<std::vector<float> > track_hit_local_x;
      std::vector<std::vector<float> > track_hit_local_y;
      std::vector<std::vector<float> > track_hit_local_x_error;
      std::vector<std::vector<float> > track_hit_local_y_error;
      std::vector<std::vector<unsigned int> >  track_hit_sub_det; //1 PixelBarrel, 2 PixelEndcap, 3 TIB, 4 TOB, 5 TID, 6 TEC
      std::vector<std::vector<unsigned int> >  track_hit_layer;
      //std::vector<std::vector<unsigned int> >  track_hit_type; // 0 pixel, 1 "rphiRecHit", 2 "stereoRecHit", 3 "rphiRecHitUnmatched" 4 "stereoRecHitUnmatched"


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

unsigned int NtupleCreatorWithHits::getLayer(const DetId& detid)
{

  unsigned int subid=detid.subdetId();

          switch(subid)
          {
            case 1://BPIX
            {
              PXBDetId pdetId = PXBDetId(detid);
              return pdetId.layer();
            }

            case 2://FPIX
            {
              PXFDetId pdetId = PXFDetId(detid.rawId());
              return pdetId.disk();
            }

            case 3://TIB
            {
              TIBDetId pdetId = TIBDetId(detid);
              return pdetId.layer();
            }
            break;

            case 4://TID
            {
              TIDDetId pdetId = TIDDetId(detid);
              return pdetId.wheel();
            }
            break;

            case 5://TOB
            {
              TOBDetId pdetId = TOBDetId(detid);
              return pdetId.layer();
            }
            break;

            case 6://TEC
            {
              TECDetId pdetId = TECDetId(detid);
              return pdetId.wheel();
            }
            break;
          }
          return 999;

}

//
// constructors and destructor
//
NtupleCreatorWithHits::NtupleCreatorWithHits(const edm::ParameterSet& iConfig)

{
  conf_=iConfig;
   //now do what ever initialization is needed

   tf1         = new TFile("NtupleQCD.root", "RECREATE");  
   hits_tree   = new TTree("hits_tree","hits_tree");
   //tracks_tree = new TTree("tracks_tree","tracks_tree");

   hits_tree->Branch("hit_global_x",&hit_global_x);
   hits_tree->Branch("hit_global_y",&hit_global_y);
   hits_tree->Branch("hit_global_z",&hit_global_z);
   hits_tree->Branch("hit_local_x",&hit_local_x);
   hits_tree->Branch("hit_local_y",&hit_local_y);
   hits_tree->Branch("hit_local_x_error",&hit_local_x_error);
   hits_tree->Branch("hit_local_y_error",&hit_local_y_error);
   // hits_tree->Branch("hit_cluster_global_x",&hit_cluster_global_x);
   // hits_tree->Branch("hit_cluster_global_y",&hit_cluster_global_y);
   // hits_tree->Branch("hit_cluster_global_z",&hit_cluster_global_z);
   // hits_tree->Branch("hit_cluster_local_x",&hit_cluster_local_x);
   // hits_tree->Branch("hit_cluster_local_y",&hit_cluster_local_y);
   // hits_tree->Branch("hit_cluster_local_x_error",&hit_cluster_local_x_error);
   // hits_tree->Branch("hit_cluster_local_y_error",&hit_cluster_local_y_error);
   // hits_tree->Branch("hit_cluster_charge",&hit_cluster_charge);
   // hits_tree->Branch("hit_cluster_size",&hit_cluster_size);
   // hits_tree->Branch("hit_cluster_size_x",&hit_cluster_size_x);
   // hits_tree->Branch("hit_cluster_size_y",&hit_cluster_size_y);
   hits_tree->Branch("hit_sub_det",&hit_sub_det);
   hits_tree->Branch("hit_layer",&hit_layer);
   hits_tree->Branch("hit_type",&hit_type);

   hits_tree->Branch("hit_simtrack_id",&hit_simtrack_id);
   hits_tree->Branch("hit_simtrack_index",&hit_simtrack_index);
   hits_tree->Branch("hit_simtrack_match",&hit_simtrack_match);
   hits_tree->Branch("hit_genparticle_id",&hit_genparticle_id);
   hits_tree->Branch("hit_pdgid",&hit_pdgid);
   hits_tree->Branch("hit_recotrack_id",&hit_recotrack_id);
   hits_tree->Branch("hit_recotrack_match",&hit_recotrack_match);
   hits_tree->Branch("hit_genparticle_match",&hit_genparticle_match);
   hits_tree->Branch("hit_genjet_id",&hit_genjet_id);
   hits_tree->Branch("hit_genjet_match",&hit_genjet_match);


   hits_tree->Branch("simtrack_id",&simtrack_id);
   hits_tree->Branch("simtrack_pdgid",&simtrack_pdgid);
   hits_tree->Branch("simtrack_charge",&simtrack_charge);

   hits_tree->Branch("simtrack_px",&simtrack_px);
   hits_tree->Branch("simtrack_py",&simtrack_py);
   hits_tree->Branch("simtrack_pz",&simtrack_pz);
   hits_tree->Branch("simtrack_energy",&simtrack_energy);

   hits_tree->Branch("simtrack_vtxid",&simtrack_vtxid);
   hits_tree->Branch("simtrack_genid",&simtrack_genid);
   hits_tree->Branch("simtrack_evtid",&simtrack_evtid);

   hits_tree->Branch("genpart_collid",&genpart_collid);
   hits_tree->Branch("genpart_pdgid",&genpart_pdgid);
   hits_tree->Branch("genpart_charge",&genpart_charge);

   hits_tree->Branch("genpart_px",&genpart_px);
   hits_tree->Branch("genpart_py",&genpart_py);
   hits_tree->Branch("genpart_px",&genpart_pz);
   hits_tree->Branch("genpart_energy",&genpart_energy);
   
   hits_tree->Branch("genpart_status",&genpart_status);

   hits_tree->Branch("genjet_px",&genjet_px);
   hits_tree->Branch("genjet_py",&genjet_py);
   hits_tree->Branch("genjet_pz",&genjet_pz);
   hits_tree->Branch("genjet_energy",&genjet_energy);

   hits_tree->Branch("genjet_emEnergy",&genjet_emEnergy);
   hits_tree->Branch("genjet_hadEnergy",&genjet_hadEnergy);
   hits_tree->Branch("genjet_invisibleEnergy",&genjet_invisibleEnergy);
   hits_tree->Branch("genjet_auxiliaryEnergy",&genjet_auxiliaryEnergy);
   hits_tree->Branch("genjet_const_collid",&genjet_const_collid);
   hits_tree->Branch("genjet_const_pdgid",&genjet_const_pdgid);
   hits_tree->Branch("genjet_const_charge",&genjet_const_charge);

   hits_tree->Branch("genjet_const_px",&genjet_const_px);
   hits_tree->Branch("genjet_const_py",&genjet_const_py);
   hits_tree->Branch("genjet_const_pz",&genjet_const_pz);
   hits_tree->Branch("genjet_const_energy",&genjet_const_energy);



   hits_tree->Branch("track_chi2",&track_chi2);
   hits_tree->Branch("track_ndof",&track_ndof);
   hits_tree->Branch("track_chi2ndof",&track_chi2ndof);
   hits_tree->Branch("track_charge",&track_charge);
   hits_tree->Branch("track_momentum",&track_momentum);
   hits_tree->Branch("track_pt",&track_pt);
   hits_tree->Branch("track_pterr",&track_pterr);
   hits_tree->Branch("track_hitsvalid",&track_hitsvalid);
   hits_tree->Branch("track_hitslost",&track_hitslost);
   hits_tree->Branch("track_theta",&track_theta);
   hits_tree->Branch("track_thetaerr",&track_thetaerr);
   hits_tree->Branch("track_phi",&track_phi);
   hits_tree->Branch("track_phierr",&track_phierr);
   hits_tree->Branch("track_eta",&track_eta);
   hits_tree->Branch("track_etaerr",&track_etaerr);
   hits_tree->Branch("track_dxy",&track_dxy);
   hits_tree->Branch("track_dxyerr",&track_dxyerr);
   hits_tree->Branch("track_dsz",&track_dsz);
   hits_tree->Branch("track_dszerr",&track_dszerr);
   hits_tree->Branch("track_qoverp",&track_qoverp);
   hits_tree->Branch("track_qoverperr",&track_qoverperr);
   hits_tree->Branch("track_vx",&track_vx);
   hits_tree->Branch("track_vy",&track_vy);
   hits_tree->Branch("track_vz",&track_vz);
   hits_tree->Branch("track_algo",&track_algo);
   hits_tree->Branch("track_hit_global_x",&track_hit_global_x);
   hits_tree->Branch("track_hit_global_y",&track_hit_global_y);
   hits_tree->Branch("track_hit_global_z",&track_hit_global_z);
   hits_tree->Branch("track_hit_local_x",&track_hit_local_x);
   hits_tree->Branch("track_hit_local_y",&track_hit_local_y);
   hits_tree->Branch("track_hit_local_x_error",&track_hit_local_x_error);
   hits_tree->Branch("track_hit_local_y_error",&track_hit_local_y_error);
   hits_tree->Branch("track_hit_sub_det",&track_hit_sub_det); //1 PixelBarrel, 2 PixelEndcap, 3 TIB, 4 TOB, 5 TID, 6 TEC
   hits_tree->Branch("track_hit_layer",&track_hit_layer);
   //hits_tree->Branch("track_hit_type",&track_hit_type);


}


NtupleCreatorWithHits::~NtupleCreatorWithHits()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  tf1->cd();
  hits_tree->Write();
  //tracks_tree->Write();
  tf1->Write();
  tf1->Close(); 
}


//
// member functions
//

// ------------ method called for each event  ------------
void
NtupleCreatorWithHits::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  hit_global_x.clear();
  hit_global_y.clear();
  hit_global_z.clear();
  hit_local_x.clear();
  hit_local_y.clear();
  hit_local_x_error.clear();
  hit_local_y_error.clear();
  // hit_cluster_global_x.clear();
  // hit_cluster_global_y.clear();
  // hit_cluster_global_z.clear();
  // hit_cluster_local_x.clear();
  // hit_cluster_local_y.clear();
  // hit_cluster_local_x_error.clear();
  // hit_cluster_local_y_error.clear();
  // hit_cluster_charge.clear();
  // hit_cluster_size.clear();
  // hit_cluster_size_x.clear();
  // hit_cluster_size_y.clear();
  hit_sub_det.clear();
  hit_layer.clear();
  hit_type.clear();

  hit_simtrack_id.clear();
  hit_simtrack_index.clear();
  hit_simtrack_match.clear();
  hit_genparticle_id.clear();
  hit_genparticle_match.clear();
  hit_pdgid.clear();
  hit_genjet_id.clear();
  hit_genjet_match.clear();
  hit_recotrack_id.clear();
  hit_recotrack_match.clear();

  simtrack_id.clear();
  simtrack_pdgid.clear();
  simtrack_charge.clear();

  simtrack_px.clear();
  simtrack_py.clear();
  simtrack_pz.clear();
  simtrack_energy.clear();

  simtrack_vtxid.clear();
  simtrack_genid.clear();
  simtrack_evtid.clear();

  genpart_collid.clear();
  genpart_pdgid.clear();
  genpart_charge.clear();

  genpart_px.clear();
  genpart_py.clear();
  genpart_pz.clear();
  genpart_energy.clear();
 
  genpart_status.clear();

  genjet_px.clear();
  genjet_py.clear();
  genjet_pz.clear();
  genjet_energy.clear();

  genjet_emEnergy.clear();
  genjet_hadEnergy.clear();
  genjet_invisibleEnergy.clear();
  genjet_auxiliaryEnergy.clear();
  genjet_const_collid.clear();
  genjet_const_pdgid.clear();
  genjet_const_charge.clear();

  genjet_const_px.clear();
  genjet_const_py.clear();
  genjet_const_pz.clear();
  genjet_const_energy.clear();

  track_chi2.clear();
  track_ndof.clear();
  track_chi2ndof.clear();
  track_charge.clear();
  track_momentum.clear();
  track_pt.clear();
  track_pterr.clear();
  track_hitsvalid.clear();
  track_hitslost.clear();
  track_theta.clear();
  track_thetaerr.clear();
  track_phi.clear();
  track_phierr.clear();
  track_eta.clear();
  track_etaerr.clear();
  track_dxy.clear();
  track_dxyerr.clear();
  track_dsz.clear();
  track_dszerr.clear();
  track_qoverp.clear();
  track_qoverperr.clear();
  track_vx.clear();
  track_vy.clear();
  track_vz.clear();
  track_algo.clear();
  track_hit_global_x.clear();
  track_hit_global_y.clear();
  track_hit_global_z.clear();
  track_hit_local_x.clear();
  track_hit_local_y.clear();
  track_hit_local_x_error.clear();
  track_hit_local_y_error.clear();
  track_hit_sub_det.clear();
  track_hit_layer.clear();
  //track_hit_type.clear();

  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);


//sim tracks
  edm::Handle<std::vector<SimTrack> > simtracks;
  iEvent.getByLabel( edm::InputTag("g4SimHits") , simtracks);
  std::vector<SimTrack>::const_iterator simtrackIterator      = (simtracks.product())->begin();
  std::vector<SimTrack>::const_iterator simtrackIteratorEnd   = (simtracks.product())->end();

  for ( ; simtrackIterator != simtrackIteratorEnd; simtrackIterator++)
  {

    simtrack_id.push_back(simtrackIterator->trackId());
    simtrack_pdgid.push_back(simtrackIterator->type());
    simtrack_charge.push_back(simtrackIterator->charge());
    //TLorentzVector v(simtrackIterator->momentum().px(),simtrackIterator->momentum().py(),simtrackIterator->momentum().pz(),simtrackIterator->momentum().energy());
    
    simtrack_px.push_back(simtrackIterator->momentum().px());
    simtrack_py.push_back(simtrackIterator->momentum().py());
    simtrack_pz.push_back(simtrackIterator->momentum().pz());
    simtrack_energy.push_back(simtrackIterator->momentum().energy());
    
    simtrack_vtxid.push_back(simtrackIterator->vertIndex());
    simtrack_genid.push_back(simtrackIterator->genpartIndex());
    simtrack_evtid.push_back(simtrackIterator->eventId().rawId());

  }

//genparticles
  edm::Handle<std::vector<reco::GenParticle> > genparticles;
  iEvent.getByLabel( edm::InputTag("genParticles") , genparticles);
  std::vector<reco::GenParticle>::const_iterator genpartIterator      = (genparticles.product())->begin();
  std::vector<reco::GenParticle>::const_iterator genpartIteratorEnd   = (genparticles.product())->end();

  for ( ; genpartIterator != genpartIteratorEnd; genpartIterator++)
  {

    genpart_collid.push_back(genpartIterator->collisionId());
    genpart_pdgid.push_back(genpartIterator->pdgId());
    genpart_charge.push_back(genpartIterator->charge());

    genpart_px.push_back(genpartIterator->px());
    genpart_py.push_back(genpartIterator->py());
    genpart_pz.push_back(genpartIterator->pz());
    genpart_energy.push_back(genpartIterator->energy());

    genpart_status.push_back(genpartIterator->status());

  }

//genjets
  edm::Handle<std::vector<reco::GenJet> > genjets;
  iEvent.getByLabel( edm::InputTag("ak5GenJets") , genjets);
  std::vector<reco::GenJet>::const_iterator genjetIterator      = (genjets.product())->begin();
  std::vector<reco::GenJet>::const_iterator genjetIteratorEnd   = (genjets.product())->end();

  for ( ; genjetIterator != genjetIteratorEnd; genjetIterator++)
  {

    genjet_px.push_back(genjetIterator->px());
    genjet_py.push_back(genjetIterator->py());
    genjet_pz.push_back(genjetIterator->pz());
    genjet_energy.push_back(genjetIterator->energy());

    genjet_emEnergy.push_back(genjetIterator->emEnergy());
    genjet_hadEnergy.push_back(genjetIterator->hadEnergy());
    genjet_invisibleEnergy.push_back(genjetIterator->invisibleEnergy());
    genjet_auxiliaryEnergy.push_back(genjetIterator->auxiliaryEnergy());

    std::vector<int> this_collid;
    std::vector<int> this_pdgid;
    std::vector<int> this_charge;
    std::vector<float> this_px;
    std::vector<float> this_py;
    std::vector<float> this_pz;
    std::vector<float> this_energy;
    const std::vector <const reco::GenParticle*> & constituents = genjetIterator->getGenConstituents();
     for(unsigned int i=0; i<constituents.size();i++)
     {
       this_collid.push_back(constituents[i]->collisionId());
       this_pdgid.push_back(constituents[i]->pdgId());
       this_charge.push_back(constituents[i]->charge());
       
       this_px.push_back(constituents[i]->px());
       this_py.push_back(constituents[i]->py());
       this_pz.push_back(constituents[i]->pz());
       this_energy.push_back(constituents[i]->energy());
     }
    genjet_const_collid.push_back(this_collid);
    genjet_const_pdgid.push_back(this_pdgid);
    genjet_const_charge.push_back(this_charge);
    genjet_const_px.push_back(this_px);
    genjet_const_py.push_back(this_py);
    genjet_const_pz.push_back(this_pz);
    genjet_const_energy.push_back(this_energy);

  }


//sipixel
  edm::Handle<SiPixelRecHitCollection> recHitColl;
  iEvent.getByLabel( edm::InputTag("siPixelRecHits") , recHitColl);

  SiPixelRecHitCollection::const_iterator recHitIdIterator      = (recHitColl.product())->begin();
  SiPixelRecHitCollection::const_iterator recHitIdIteratorEnd   = (recHitColl.product())->end();

  for ( ; recHitIdIterator != recHitIdIteratorEnd; recHitIdIterator++)
  {
    SiPixelRecHitCollection::DetSet detset = *recHitIdIterator;
    DetId detId = DetId(detset.detId()); // Get the Detid object

    TrackerHitAssociator  associate(iEvent,conf_);


    //unsigned int detType=detId.det();    // det type, tracker=1
    unsigned int subid=detId.subdetId(); //subdetector type, barrel=1, fpix=2
    unsigned int layer = getLayer(detId);

    // if(subid==1)
    // {  // Subdet it, pix barrel=1 
      
    //   PXBDetId pdetId = PXBDetId(detId);
    //   // unsigned int detTypeP=pdetId.det();
    //   // unsigned int subidP=pdetId.subdetId();
    //   // Barell layer = 1,2,3
    //   layer=pdetId.layer();
    // }  
    // else
    // {
    //   PXFDetId pdetId = PXFDetId(detId.rawId());
    //   layer=pdetId.disk(); //1,2,3
    // } // end BPix FPix if

    const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );

    SiPixelRecHitCollection::DetSet::const_iterator pixeliter=detset.begin();
    SiPixelRecHitCollection::DetSet::const_iterator rechitRangeIteratorEnd   = detset.end();
    for(;pixeliter!=rechitRangeIteratorEnd;++pixeliter)
    {//loop on the rechit

      if (pixeliter->isValid())
      {

      //simtrack match
      int simtrack_id=-1;
      unsigned int simtrack_index=0;
      bool simtrack_match=false;

      std::vector<SimHitIdpr> matched;
      // SimHitIdpr closest;
      // float mindist = 999999;
      // float dist;
      matched = associate.associateHitId(*pixeliter);
      // if(!matched.empty()) std::cout<<"passing here"<<std::endl;
      //std::cout << " Rechit = " << pixeliter->localPosition() << std::endl; 
      if(!matched.empty())
      {
        //std::cout << " matched = " << matched.size() << std::endl;
        for(std::vector<SimHitIdpr>::const_iterator m=matched.begin(); m<matched.end(); m++)
        {
          bool found = false;
          simtrack_id=m->first;
          for(std::vector<SimTrack>::const_iterator st=(simtracks.product())->begin();st<(simtracks.product())->end();st++)
          {
            if (m->first==st->trackId())
            {
              simtrack_index=st-simtracks.product()->begin();
              //std::cout<<m->second.rawId()<<" "<<m->first<<" "<<st->type()<<" "<<st->genpartIndex()<<std::endl;
              found=true;
              break;
            }
            // st->type(); //pid
            // st->genpartIndex();
            // st->noGenpart();
          }
          if (found)
          {
            simtrack_match=true;
            break;
          }
          // std::cout << " simtrack ID = " << (*m).trackId() << " Simhit x = " << (*m).localPosition() << std::endl;
          // dist = fabs(pixeliter->localPosition().x() - (*m).localPosition().x());
          // if(dist<mindist)
          // {
          //   mindist = dist;
          //   closest = (*m);
          // }
        }  
        //std::cout << " Closest Simhit = " << closest.localPosition() << std::endl;
      }

      //genparticle match
      unsigned int    genparticle_id=0;
      int             genparticle_pdgid=9999;
      bool            genparticle_match=false;
      if (simtrack_match)
      {
        if (!simtracks.product()->at(simtrack_index).noGenpart())
        {
          unsigned int the_gen_index=simtracks.product()->at(simtrack_index).genpartIndex();
          if (the_gen_index<genparticles.product()->size())
          {
            genparticle_id=the_gen_index;
            genparticle_pdgid=genparticles.product()->at(the_gen_index).pdgId();
            genparticle_match=true;
          }
        }
        //std::cout<<genparticle_match<<" "<<genparticle_pdgid<<" "<<simtracks.product()->at(simtrack_index).genpartIndex()<<"\n";
      }

      //genjet match
      unsigned int  genjet_id=0;
      bool          genjet_match=false;
      if (genparticle_match)
      {
        bool found=false;
        reco::GenParticle the_gen_particle = genparticles.product()->at(genparticle_id);
        for(std::vector<reco::GenJet>::const_iterator gj=(genjets.product())->begin();gj<(genjets.product())->end();gj++)
        {
          const std::vector <const reco::GenParticle*> & constituents = gj->getGenConstituents();
          for(unsigned int i=0; i<constituents.size();i++)
          {
            if (constituents[i]->collisionId()==the_gen_particle.collisionId()
                &&constituents[i]->pdgId()==the_gen_particle.pdgId()
                &&constituents[i]->status()==the_gen_particle.status()
                &&fabs(constituents[i]->px()-the_gen_particle.px() )<0.01
                &&fabs(constituents[i]->py()-the_gen_particle.py() )<0.01
                &&fabs(constituents[i]->pz()-the_gen_particle.pz() )<0.01)
            {
              found=true;
              //std::cout<<"aaa "<<fabs(constituents[i]->px()-the_gen_particle.px() )<<" "<<fabs(constituents[i]->py()-the_gen_particle.py() )<<" "<<fabs(constituents[i]->pz()-the_gen_particle.pz() )<<"\n";
              break;
            }//good match found
          }//constituents loop
          if (found)
          {
            genjet_id=gj-genjets.product()->begin();
            genjet_match=true;
            break;
          }
        }//genjets loop
        //std::cout<<genjet_match<<" "<<genparticle_pdgid<<" "<<genjet_id<<"\n";
      }//if hit has a match gen particle


      LocalPoint lp = pixeliter->localPosition();
      LocalError le = pixeliter->localPositionError();
      // float xRecHit = lp.x();
      // float yRecHit = lp.y();

      // MeasurementPoint mp = topol->measurementPosition(xRecHit,yRecHit);
      GlobalPoint GP = theGeomDet->surface().toGlobal(Local3DPoint(lp));
      //theGeomDet->toGlobal(lp).x()
      //theStripDet->toGlobal(hit.localPosition()).x();
      //dynamic_cast<const StripGeomDetUnit*>( theTrackerGeometry->idToDet( hit.geographicalId() ) );
      //std::cout<<"["<<GP.x()<<", "<<GP.y()<<", "<<GP.z()<<"],"<<std::endl;
      //edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = pixeliter->cluster();

      //std::cout<<"["<<lp.x()<<", "<<clust->x()<<", "<<lp.y()<<"],"<<clust->y()<<std::endl;
      
        hit_sub_det.push_back(subid);
        hit_layer.push_back(layer);
        hit_type.push_back(0);
        hit_global_x.push_back(GP.x());
        hit_global_y.push_back(GP.y());
        hit_global_z.push_back(GP.z());
        hit_local_x.push_back(lp.x());
        hit_local_y.push_back(lp.y());
        hit_local_x_error.push_back(sqrt(le.xx()));
        hit_local_y_error.push_back(sqrt(le.yy()));
        hit_recotrack_id.push_back(0);
        hit_recotrack_match.push_back(false);
        hit_simtrack_id.push_back(simtrack_id);
        hit_simtrack_index.push_back(simtrack_index);
        hit_simtrack_match.push_back(simtrack_match);
        hit_genparticle_id.push_back(genparticle_id);
        hit_pdgid.push_back(genparticle_pdgid);
        hit_genparticle_match.push_back(genparticle_match);
        hit_genjet_id.push_back(genjet_id);
        hit_genjet_match.push_back(genjet_match);
      }
      // hit_cluster_global_x.push_back();
      // hit_cluster_global_y.push_back();
      // hit_cluster_global_z.push_back();
      // hit_cluster_local_x.push_back(clust->x());
      // hit_cluster_local_y.push_back(clust->y());
      // hit_cluster_local_x_error.push_back(0);
      // hit_cluster_local_y_error.push_back(0);
      // hit_cluster_charge.push_back((clust->charge())/1000.);
      // hit_cluster_size.push_back(clust->size());
      // hit_cluster_size_x.push_back(clust->sizeX());
      // hit_cluster_size_y.push_back(clust->sizeY());
      


    } //rechits
  } //detid

//sistrip
  std::vector<std::string> collections;
  collections.push_back("rphiRecHit");
  collections.push_back("stereoRecHit");
  collections.push_back("rphiRecHitUnmatched");
  collections.push_back("stereoRecHitUnmatched");

for (unsigned int the_collection=0; the_collection<collections.size();the_collection++){
  edm::Handle<SiStripRecHit2DCollection> stripRecHitColl;
  iEvent.getByLabel( edm::InputTag("siStripMatchedRecHits",collections[the_collection]) , stripRecHitColl);

  SiStripRecHit2DCollection::const_iterator stripRecHitIdIterator      = (stripRecHitColl.product())->begin();
  SiStripRecHit2DCollection::const_iterator stripRecHitIdIteratorEnd   = (stripRecHitColl.product())->end();

  for (; stripRecHitIdIterator != stripRecHitIdIteratorEnd; ++stripRecHitIdIterator)
  { 
     SiStripRecHit2DCollection::DetSet detset = *stripRecHitIdIterator;
     DetId detId = DetId(detset.detId()); // Get the Detid object
     //unsigned int detType=detId.det();    // det type, tracker=1
     unsigned int subid=detId.subdetId(); //subdetector type, barrel=1, fpix=2
     unsigned int layer = getLayer(detId);

     //const StripGeomDetUnit * theGeomDet = dynamic_cast<const StripGeomDetUnit*> (theTracker.idToDet(detId) );

     SiStripRecHit2DCollection::DetSet::const_iterator stripiter=detset.begin();
     SiStripRecHit2DCollection::DetSet::const_iterator stripRechitRangeIteratorEnd   = detset.end();
     TrackerHitAssociator  associate(iEvent,conf_);

     for(;stripiter!=stripRechitRangeIteratorEnd;++stripiter)
     //for(SiStripMatchedRecHit2DCollection::DetSet::const_iterator iter=detunit_iterator->begin(), end = detunit_iterator->end(); iter != end; ++iter)
     {
      if (stripiter->isValid())
      {



        //simtrack match
        int simtrack_id=-1;
        unsigned int simtrack_index=0;
        bool simtrack_match=false;
        std::vector<SimHitIdpr> matched;
        matched = associate.associateHitId(*stripiter); 
        if(!matched.empty())
        {
          for(std::vector<SimHitIdpr>::const_iterator m=matched.begin(); m<matched.end(); m++)
          {
            bool found = false;
            simtrack_id=m->first;
            for(std::vector<SimTrack>::const_iterator st=(simtracks.product())->begin();st<(simtracks.product())->end();st++)
            {
              if (m->first==st->trackId())
              {
                simtrack_index=st-simtracks.product()->begin();
                found=true;
                break;
              }
            }
            if (found)
            {
              simtrack_match=true;
              break;
            }
          }  
        }

        //genparticle match
        unsigned int    genparticle_id=0;
        int             genparticle_pdgid=9999;
        bool            genparticle_match=false;
        if (simtrack_match)
        {
          if (!simtracks.product()->at(simtrack_index).noGenpart())
          {
            unsigned int the_gen_index=simtracks.product()->at(simtrack_index).genpartIndex();
            if (the_gen_index<genparticles.product()->size())
            {
              genparticle_id=the_gen_index;
              genparticle_pdgid=genparticles.product()->at(the_gen_index).pdgId();
              genparticle_match=true;
            }
          }
        }

        //genjet match
        unsigned int  genjet_id=0;
        bool          genjet_match=false;
        if (genparticle_match)
        {
          bool found=false;
          reco::GenParticle the_gen_particle = genparticles.product()->at(genparticle_id);
          for(std::vector<reco::GenJet>::const_iterator gj=(genjets.product())->begin();gj<(genjets.product())->end();gj++)
          {
            const std::vector <const reco::GenParticle*> & constituents = gj->getGenConstituents();
            for(unsigned int i=0; i<constituents.size();i++)
            {
              if (constituents[i]->collisionId()==the_gen_particle.collisionId()
                  &&constituents[i]->pdgId()==the_gen_particle.pdgId()
                 &&constituents[i]->status()==the_gen_particle.status()
                 &&fabs(constituents[i]->px()-the_gen_particle.px() )<0.01
                 &&fabs(constituents[i]->py()-the_gen_particle.py() )<0.01
                 &&fabs(constituents[i]->pz()-the_gen_particle.pz() )<0.01)
              {
                found=true;
                break;
              }//good match found
            }//constituents loop
            if (found)
            {
              genjet_id=gj-genjets.product()->begin();
              genjet_match=true;
              break;
            }
          }//genjets loop
        }//if hit has a match gen particle


        const StripGeomDetUnit* theGeomDet = dynamic_cast<const StripGeomDetUnit*>( theTracker.idToDet( stripiter->geographicalId() ) );
        LocalPoint lp = stripiter->localPosition();
        LocalError le = stripiter->localPositionError();
        //std::cout<<lp.x()<<std::endl;
        GlobalPoint GP = theGeomDet->surface().toGlobal(Local3DPoint(lp));
        //std::cout<<theGeomDet->toGlobal(lp).x();
        // //theStripDet->toGlobal(hit.localPosition()).x();
        // //dynamic_cast<const StripGeomDetUnit*>( theTrackerGeometry->idToDet( hit.geographicalId() ) );
        // //std::cout<<"["<<GP.x()<<", "<<GP.y()<<", "<<GP.z()<<"],"<<std::endl;
        // //edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = pixeliter->cluster();
        //std::cout<<lp.x()<<std::endl;
      
        //   std::cout<<lp.x()<<std::endl;
        hit_sub_det.push_back(subid);
        hit_layer.push_back(layer);
        hit_type.push_back(the_collection+1);
        hit_global_x.push_back(GP.x());
        hit_global_y.push_back(GP.y());
        hit_global_z.push_back(GP.z());
        hit_local_x.push_back(lp.x());
        hit_local_y.push_back(lp.y());
        hit_local_x_error.push_back(sqrt(le.xx()));
        hit_local_y_error.push_back(sqrt(le.yy()));
        hit_recotrack_id.push_back(0);
        hit_recotrack_match.push_back(false);
        hit_simtrack_id.push_back(simtrack_id);
        hit_simtrack_index.push_back(simtrack_index);
        hit_simtrack_match.push_back(simtrack_match);
        hit_genparticle_id.push_back(genparticle_id);
        hit_pdgid.push_back(genparticle_pdgid);
        hit_genparticle_match.push_back(genparticle_match);
        hit_genjet_id.push_back(genjet_id);
        hit_genjet_match.push_back(genjet_match);
      }
     }
  }
}

  // Handle<TrackCollection> tracks;
  // iEvent.getByLabel("TrackRefitter", tracks);
  edm::ESHandle<TransientTrackingRecHitBuilder> theTrackerRecHitBuilder;
  edm::Handle<std::vector<reco::Track> > trackColl;
  iEvent.getByLabel( edm::InputTag("TrackRefitter") , trackColl);
  iSetup.get<TransientRecHitRecord>().get(conf_.getParameter<std::string>("TrackerRecHitBuilder"),theTrackerRecHitBuilder);
//tracks


  // edm::Handle<std::vector<reco::Track> > trackColl;
  // iEvent.getByLabel( edm::InputTag("generalTracks") , trackColl);
  std::vector<reco::Track>::const_iterator trackIterator      = (trackColl.product())->begin();
  std::vector<reco::Track>::const_iterator trackIteratorEnd   = (trackColl.product())->end();

  for ( ; trackIterator != trackIteratorEnd; trackIterator++)
  {

    track_chi2.push_back(      trackIterator->chi2());
    track_ndof.push_back(      trackIterator->ndof());
    track_chi2ndof.push_back(  trackIterator->chi2()/trackIterator->ndof());
    track_charge.push_back(    trackIterator->charge());
    track_momentum.push_back(  trackIterator->p());
    track_pt.push_back(        trackIterator->pt());
    track_pterr.push_back(     trackIterator->ptError());
    track_hitsvalid.push_back( trackIterator->numberOfValidHits());
    track_hitslost.push_back(  trackIterator->numberOfLostHits());
    track_theta.push_back(     trackIterator->theta());
    track_thetaerr.push_back(  trackIterator->thetaError());
    track_phi.push_back(       trackIterator->phi());
    track_phierr.push_back(    trackIterator->phiError());
    track_eta.push_back(       trackIterator->eta());
    track_etaerr.push_back(    trackIterator->etaError());
    track_dxy.push_back(       trackIterator->dxy());
    track_dxyerr.push_back(    trackIterator->dxyError());
    track_dsz.push_back(       trackIterator->dsz());
    track_dszerr.push_back(    trackIterator->dszError());
    track_qoverp.push_back(    trackIterator->qoverp());
    track_qoverperr.push_back( trackIterator->qoverpError());
    track_vx.push_back(        trackIterator->vx());
    track_vy.push_back(        trackIterator->vy());
    track_vz.push_back(        trackIterator->vz());
    track_algo.push_back(      (Int_t) trackIterator->algo());

    std::vector<float> this_track_hit_global_x;
    std::vector<float> this_track_hit_global_y;
    std::vector<float> this_track_hit_global_z;
    std::vector<float> this_track_hit_local_x;
    std::vector<float> this_track_hit_local_y;
    std::vector<float> this_track_hit_local_x_error;
    std::vector<float> this_track_hit_local_y_error;
    std::vector<unsigned int> this_track_hit_sub_det; //1 PixelBarrel, 2 PixelEndcap, 3 TIB, 4 TOB, 5 TID, 6 TEC
    std::vector<unsigned int> this_track_hit_layer;


    // trackingRecHit_iterator rechit_it = trackIterator->recHitsBegin();
    // trackingRecHit_iterator rechit_end = trackIterator->recHitsEnd();
    // for (;rechit_it!=rechit_end;rechit_it++)
    const reco::HitPattern& p = trackIterator->hitPattern();
    for (int i=0; i<p.numberOfHits(); i++) 
    {
      TrackingRecHitRef rhit = trackIterator->recHit(i);
      if (rhit->isValid())
      {
        DetId the_detid = rhit->geographicalId();
        if (the_detid.det()!=1) continue; //only tracker
        unsigned int subid= the_detid.subdetId();
        LocalPoint lp;
        GlobalPoint GP;
        LocalError le;

        TransientTrackingRecHit::RecHitPointer tthit = theTrackerRecHitBuilder->build(&*rhit);
        GP =  tthit->globalPosition();
        lp = tthit->localPosition();
        le = tthit->localPositionError();

        // if (subid==1 || subid==2)
        // {
        //   const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*>( theTracker.idToDet( (*rechit_it)->geographicalId() ) );
        //   lp = (*rechit_it)->localPosition();
        //   le = (*rechit_it)->localPositionError();
        //   GP = theGeomDet->surface().toGlobal(Local3DPoint(lp));
        //   //std::cout<<GP.x()<<std::endl;

        // }
        // if (subid>=3 && subid<=6)
        // {
        //   const StripGeomDetUnit* theGeomDet = dynamic_cast<const StripGeomDetUnit*>( theTracker.idToDet( (*rechit_it)->geographicalId() ) );
        //   lp = (*rechit_it)->localPosition();
        //   le = (*rechit_it)->localPositionError();
        //   GP = theGeomDet->surface().toGlobal(Local3DPoint(lp));

        //   //std::cout<<GP.x()<<std::endl;
        // }
        unsigned int layer = getLayer(the_detid);
        this_track_hit_layer.push_back(layer);
        this_track_hit_sub_det.push_back(subid);
        this_track_hit_global_x.push_back(GP.x());
        this_track_hit_global_y.push_back(GP.y());
        this_track_hit_global_z.push_back(GP.z());
        this_track_hit_local_x.push_back(lp.x());
        this_track_hit_local_y.push_back(lp.y());
        this_track_hit_local_x_error.push_back(sqrt(le.xx()));
        this_track_hit_local_y_error.push_back(sqrt(le.yy()));
        //int nmatches=0;
        for (unsigned int i=0;i<hit_global_x.size();i++)
        {
          if (hit_sub_det[i]==subid &&
              hit_layer[i]==layer &&
              fabs((float)hit_global_x[i]-(float)GP.x())<0.01 &&
              fabs((float)hit_global_y[i]-(float)GP.y())<0.01 &&
              fabs((float)hit_global_z[i]-(float)GP.z())<0.01 
              )
          {
            hit_recotrack_match[i]=true;
            hit_recotrack_id[i]=track_chi2.size()-1;
            //std::cout<<"track match "<<hit_global_x[i]<<" "<<GP.x()<<std::endl;
            //nmatches++;
          }

        }

//qui


        // switch (nmatches)
        // {
        //   case 0:
        //   std::cout<<"no matches"<<std::endl;
        //   break;
        //   case 1:
        //   //std::cout<<"one match"<<std::endl;
        //   break;
        //   default:
        //   std::cout<<nmatches<<" matches"<<std::endl;
        //   break;


        // }
        //std::cout<<(*rechit_it)->localPosition().x()<<std::endl;

  //        id_type rawId() const { return m_id;}
  // DetId geographicalId() const {return m_id;}
  
  // virtual LocalPoint localPosition() const = 0;
  
  // virtual LocalError localPositionError() const = 0;
      }
      

    } //track rechits loop

   track_hit_global_x.push_back(this_track_hit_global_x);
   track_hit_global_y.push_back(this_track_hit_global_y);
   track_hit_global_z.push_back(this_track_hit_global_z);
   track_hit_local_x.push_back(this_track_hit_local_x);
   track_hit_local_y.push_back(this_track_hit_local_y);
   track_hit_local_x_error.push_back(this_track_hit_local_x_error);
   track_hit_local_y_error.push_back(this_track_hit_local_y_error);
   track_hit_sub_det.push_back(this_track_hit_sub_det);
   track_hit_layer.push_back(this_track_hit_layer);

  }

  hits_tree->Fill();
  //tracks_tree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
NtupleCreatorWithHits::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NtupleCreatorWithHits::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
NtupleCreatorWithHits::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
NtupleCreatorWithHits::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
NtupleCreatorWithHits::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
NtupleCreatorWithHits::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NtupleCreatorWithHits::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(NtupleCreatorWithHits);
