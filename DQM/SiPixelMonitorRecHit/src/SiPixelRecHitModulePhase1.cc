#include "DQM/SiPixelMonitorRecHit/interface/SiPixelRecHitModulePhase1.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQM/SiPixelCommon/interface/SiPixelHistogramId.h"
/// Framework
#include "FWCore/ServiceRegistry/interface/Service.h"
// STL
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdlib.h>

// Data Formats
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelNameUpgrade.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapNameUpgrade.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
//
// Constructors
//
SiPixelRecHitModulePhase1::SiPixelRecHitModulePhase1() : id_(0) { }
///
SiPixelRecHitModulePhase1::SiPixelRecHitModulePhase1(const uint32_t& id) : 
  id_(id)
{ 
}

//
// Destructor
//
SiPixelRecHitModulePhase1::~SiPixelRecHitModulePhase1() {}
//
// Book histograms
//
void SiPixelRecHitModulePhase1::book(const edm::ParameterSet& iConfig,DQMStore::IBooker & iBooker, int type, 
                               bool twoD, bool reducedSet) {

  bool barrel = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
  bool endcap = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
  bool isHalfModule = false;
  if(barrel){
    isHalfModule = PixelBarrelNameUpgrade(DetId(id_)).isHalfModule(); 
  }

  std::string hid;
  // Get collection name and instantiate Histo Id builder
  edm::InputTag src = iConfig.getParameter<edm::InputTag>( "src" );
  // Get DQM interface


  if(type==0){
    SiPixelHistogramId* theHistogramId = new SiPixelHistogramId( src.label() );
	if(!reducedSet)
	{
    if(twoD){
      // XYPosition
      hid = theHistogramId->setHistoId("xypos",id_);
      meXYPos_ = iBooker.book2D(hid,"XY Position",100,-1.,1,100,-4,4);
      meXYPos_->setAxisTitle("X Position",1);
      meXYPos_->setAxisTitle("Y Position",2);
    }
    else{
      // projections of XYPosition
      hid = theHistogramId->setHistoId("xypos",id_);
      meXYPos_px_ = iBooker.book1D(hid+"_px","X Position",100,-1.,1);
      meXYPos_px_->setAxisTitle("X Position",1);
      meXYPos_py_ = iBooker.book1D(hid+"_py","Y Position",100,-4,4);
      meXYPos_py_->setAxisTitle("Y Position",1);
    }
	}
    hid = theHistogramId->setHistoId("ClustX",id_);
    meClustX_ = iBooker.book1D(hid, "RecHit X size", 10, 0., 10.);
    meClustX_->setAxisTitle("RecHit size X dimension", 1);
    hid = theHistogramId->setHistoId("ClustY",id_);
    meClustY_ = iBooker.book1D(hid, "RecHit Y size", 15, 0., 15.);
    meClustY_->setAxisTitle("RecHit size Y dimension", 1); 

    hid = theHistogramId->setHistoId("ErrorX",id_);
    meErrorX_ = iBooker.book1D(hid, "RecHit error X", 100,0.,0.02);
    meErrorX_->setAxisTitle("RecHit error X", 1);
    hid = theHistogramId->setHistoId("ErrorY",id_);
    meErrorY_ = iBooker.book1D(hid, "RecHit error Y", 100,0.,0.02);
    meErrorY_->setAxisTitle("RecHit error Y", 1);

    //Removed to save offline memory
    //hid = theHistogramId->setHistoId("nRecHits",id_);
    //menRecHits_ = iBooker.book1D(hid, "# of rechits in this module", 8, 0, 8);
    //menRecHits_->setAxisTitle("number of rechits",1);  
    delete theHistogramId;
  }

  if(type==1 && barrel){
    uint32_t DBladder;
    DBladder = PixelBarrelNameUpgrade(DetId(id_)).ladderName();
    char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
    hid = src.label() + "_" + sladder;
    if(isHalfModule) hid += "H";
    else hid += "F";
	if(!reducedSet)
	{
    if(twoD){
      meXYPosLad_ = iBooker.book2D("xypos_" + hid,"XY Position",100,-1.,1,100,-4,4);
      meXYPosLad_->setAxisTitle("X Position",1);
      meXYPosLad_->setAxisTitle("Y Position",2);
    }
    else{
      // projections of XYPosition
      meXYPosLad_px_ = iBooker.book1D("xypos_"+hid+"_px","X Position",100,-1.,1);
      meXYPosLad_px_->setAxisTitle("X Position",1);
      meXYPosLad_py_ = iBooker.book1D("xypos_"+hid+"_py","Y Position",100,-4,4);
      meXYPosLad_py_->setAxisTitle("Y Position",1);
    }
	}
    meClustXLad_ = iBooker.book1D("ClustX_" +hid, "RecHit X size", 10, 0., 10.);
    meClustXLad_->setAxisTitle("RecHit size X dimension", 1);
    meClustYLad_ = iBooker.book1D("ClustY_" +hid,"RecHit Y size", 15, 0.,15.);
    meClustYLad_->setAxisTitle("RecHit size Y dimension", 1);
    meErrorXLad_ = iBooker.book1D("ErrorX_"+hid, "RecHit error X", 100,0.,0.02);
    meErrorXLad_->setAxisTitle("RecHit error X", 1);
    meErrorYLad_ = iBooker.book1D("ErrorY_"+hid, "RecHit error Y", 100,0.,0.02);
    meErrorYLad_->setAxisTitle("RecHit error Y", 1);
    menRecHitsLad_ = iBooker.book1D("nRecHits_"+hid, "# of rechits in this module", 8, 0, 8);
    menRecHitsLad_->setAxisTitle("number of rechits",1);

  }

  if(type==2 && barrel){
    
    uint32_t DBlayer;
    DBlayer = PixelBarrelNameUpgrade(DetId(id_)).layerName();
    char slayer[80]; sprintf(slayer,"Layer_%i",DBlayer);
    hid = src.label() + "_" + slayer;
    
	if(!reducedSet)
	{
    if(twoD){
      meXYPosLay_ = iBooker.book2D("xypos_" + hid,"XY Position",100,-1.,1,100,-4,4);
      meXYPosLay_->setAxisTitle("X Position",1);
      meXYPosLay_->setAxisTitle("Y Position",2);
    }
    else{
      // projections of XYPosition
      meXYPosLay_px_ = iBooker.book1D("xypos_"+hid+"_px","X Position",100,-1.,1);
      meXYPosLay_px_->setAxisTitle("X Position",1);
      meXYPosLay_py_ = iBooker.book1D("xypos_"+hid+"_py","Y Position",100,-4,4);
      meXYPosLay_py_->setAxisTitle("Y Position",1);
    }
	}

    meClustXLay_ = iBooker.book1D("ClustX_" +hid, "RecHit X size", 10, 0., 10.);
    meClustXLay_->setAxisTitle("RecHit size X dimension", 1);
    meClustYLay_ = iBooker.book1D("ClustY_" +hid,"RecHit Y size", 15, 0.,15.);
    meClustYLay_->setAxisTitle("RecHit size Y dimension", 1);
    meErrorXLay_ = iBooker.book1D("ErrorX_"+hid, "RecHit error X", 100,0.,0.02);
    meErrorXLay_->setAxisTitle("RecHit error X", 1);
    meErrorYLay_ = iBooker.book1D("ErrorY_"+hid, "RecHit error Y", 100,0.,0.02);
    meErrorYLay_->setAxisTitle("RecHit error Y", 1);
    menRecHitsLay_ = iBooker.book1D("nRecHits_"+hid, "# of rechits in this module", 8, 0, 8);
    menRecHitsLay_->setAxisTitle("number of rechits",1);

  }

  if(type==3 && barrel){
    uint32_t DBmodule;
    DBmodule = PixelBarrelNameUpgrade(DetId(id_)).moduleName();
    char smodule[80]; sprintf(smodule,"Ring_%i",DBmodule);
    hid = src.label() + "_" + smodule;
    
	if(!reducedSet)
	{
    if(twoD){
      meXYPosPhi_ = iBooker.book2D("xypos_" + hid,"XY Position",100,-1.,1,100,-4,4);
      meXYPosPhi_->setAxisTitle("X Position",1);
      meXYPosPhi_->setAxisTitle("Y Position",2);
    }
    else{
      // projections of XYPosition
      meXYPosPhi_px_ = iBooker.book1D("xypos_"+hid+"_px","X Position",100,-1.,1);
      meXYPosPhi_px_->setAxisTitle("X Position",1);
      meXYPosPhi_py_ = iBooker.book1D("xypos_"+hid+"_py","Y Position",100,-4,4);
      meXYPosPhi_py_->setAxisTitle("Y Position",1);
    }
	}
    meClustXPhi_ = iBooker.book1D("ClustX_" +hid, "RecHit X size", 10, 0., 10.);
    meClustXPhi_->setAxisTitle("RecHit size X dimension", 1);
    meClustYPhi_ = iBooker.book1D("ClustY_" +hid,"RecHit Y size", 15, 0.,15.);
    meClustYPhi_->setAxisTitle("RecHit size Y dimension", 1);
    meErrorXPhi_ = iBooker.book1D("ErrorX_"+hid, "RecHit error X", 100,0.,0.02);
    meErrorXPhi_->setAxisTitle("RecHit error X", 1);
    meErrorYPhi_ = iBooker.book1D("ErrorY_"+hid, "RecHit error Y", 100,0.,0.02);
    meErrorYPhi_->setAxisTitle("RecHit error Y", 1);
    menRecHitsPhi_ = iBooker.book1D("nRecHits_"+hid, "# of rechits in this module", 8, 0, 8);
    menRecHitsPhi_->setAxisTitle("number of rechits",1);

  }

  if(type==4 && endcap){
    uint32_t blade;
    blade= PixelEndcapNameUpgrade(DetId(id_)).bladeName();
    
    char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
    hid = src.label() + "_" + sblade;
//     meXYPosBlade_ = iBooker.book2D("xypos_" + hid,"XY Position",100,-1.,1,100,-4,4);
//     meXYPosBlade_->setAxisTitle("X Position",1);
//     meXYPosBlade_->setAxisTitle("Y Position",2);

    meClustXBlade_ = iBooker.book1D("ClustX_" +hid, "RecHit X size", 10, 0., 10.);
    meClustXBlade_->setAxisTitle("RecHit size X dimension", 1);
    meClustYBlade_ = iBooker.book1D("ClustY_" +hid,"RecHit Y size", 15, 0.,15.);
    meClustYBlade_->setAxisTitle("RecHit size Y dimension", 1);
    meErrorXBlade_ = iBooker.book1D("ErrorX_"+hid, "RecHit error X", 100,0.,0.02);
    meErrorXBlade_->setAxisTitle("RecHit error X", 1);
    meErrorYBlade_ = iBooker.book1D("ErrorY_"+hid, "RecHit error Y", 100,0.,0.02);
    meErrorYBlade_->setAxisTitle("RecHit error Y", 1);
    menRecHitsBlade_ = iBooker.book1D("nRecHits_"+hid, "# of rechits in this module", 8, 0, 8);
    menRecHitsBlade_->setAxisTitle("number of rechits",1);

  }
  if(type==5 && endcap){
    uint32_t disk;
    disk = PixelEndcapNameUpgrade(DetId(id_)).diskName();
    
    char sdisk[80]; sprintf(sdisk, "Disk_%i",disk);
    hid = src.label() + "_" + sdisk;
//     meXYPosDisk_ = iBooker.book2D("xypos_" + hid,"XY Position",100,-1.,1,100,-4,4);
//     meXYPosDisk_->setAxisTitle("X Position",1);
//     meXYPosDisk_->setAxisTitle("Y Position",2);

    meClustXDisk_ = iBooker.book1D("ClustX_" +hid, "RecHit X size", 10, 0., 10.);
    meClustXDisk_->setAxisTitle("RecHit size X dimension", 1);
    meClustYDisk_ = iBooker.book1D("ClustY_" +hid,"RecHit Y size", 15, 0.,15.);
    meClustYDisk_->setAxisTitle("RecHit size Y dimension", 1);
    meErrorXDisk_ = iBooker.book1D("ErrorX_"+hid, "RecHit error X", 100,0.,0.02);
    meErrorXDisk_->setAxisTitle("RecHit error X", 1);
    meErrorYDisk_ = iBooker.book1D("ErrorY_"+hid, "RecHit error Y", 100,0.,0.02);
    meErrorYDisk_->setAxisTitle("RecHit error Y", 1);
    menRecHitsDisk_ = iBooker.book1D("nRecHits_"+hid, "# of rechits in this module", 8, 0, 8);
    menRecHitsDisk_->setAxisTitle("number of rechits",1);

  }

  if(type==6 && endcap){
    uint32_t panel;
    uint32_t module;
    panel= PixelEndcapNameUpgrade(DetId(id_)).pannelName();
    module= PixelEndcapNameUpgrade(DetId(id_)).plaquetteName();
    
    char slab[80]; sprintf(slab, "Panel_%i_Ring_%i",panel, module);
    hid = src.label() + "_" + slab;
    
	if(!reducedSet)
	{
    if(twoD){
      meXYPosRing_ = iBooker.book2D("xypos_" + hid,"XY Position",100,-1.,1,100,-4,4);
      meXYPosRing_->setAxisTitle("X Position",1);
      meXYPosRing_->setAxisTitle("Y Position",2);
    }
    else{
      // projections of XYPosition
      meXYPosRing_px_ = iBooker.book1D("xypos_"+hid+"_px","X Position",100,-1.,1);
      meXYPosRing_px_->setAxisTitle("X Position",1);
      meXYPosRing_py_ = iBooker.book1D("xypos_"+hid+"_py","Y Position",100,-4,4);
      meXYPosRing_py_->setAxisTitle("Y Position",1);
    }
	}
    meClustXRing_ = iBooker.book1D("ClustX_" +hid, "RecHit X size", 10, 0., 10.);
    meClustXRing_->setAxisTitle("RecHit size X dimension", 1);
    meClustYRing_ = iBooker.book1D("ClustY_" +hid,"RecHit Y size", 15, 0.,15.);
    meClustYRing_->setAxisTitle("RecHit size Y dimension", 1);
    meErrorXRing_ = iBooker.book1D("ErrorX_"+hid, "RecHit error X", 100,0.,0.02);
    meErrorXRing_->setAxisTitle("RecHit error X", 1);
    meErrorYRing_ = iBooker.book1D("ErrorY_"+hid, "RecHit error Y", 100,0.,0.02);
    meErrorYRing_->setAxisTitle("RecHit error Y", 1);
    menRecHitsRing_ = iBooker.book1D("nRecHits_"+hid, "# of rechits in this module", 8, 0, 8);
    menRecHitsRing_->setAxisTitle("number of rechits",1);

  }

}
//
// Fill histograms
//
void SiPixelRecHitModulePhase1::fill(const float& rechit_x, const float& rechit_y, 
                               const int& sizeX, const int& sizeY, 
			       const float& lerr_x, const float& lerr_y, 
			       bool modon, bool ladon, bool layon, bool phion, 
			       bool bladeon, bool diskon, bool ringon, 
			       bool twoD, bool reducedSet) {

  bool barrel = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
  bool endcap = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);

  //std::cout << rechit_x << " " << rechit_y << " " << sizeX << " " << sizeY << std::endl;
  if(modon){
    meClustX_->Fill(sizeX);
    meClustY_->Fill(sizeY);
    meErrorX_->Fill(lerr_x);
    meErrorY_->Fill(lerr_y);  
	if(!reducedSet)
	{
    if(twoD) meXYPos_->Fill(rechit_x, rechit_y);
    else {
      meXYPos_px_->Fill(rechit_x); 
      meXYPos_py_->Fill(rechit_y);
    }
	}
  }
  //std::cout<<"number of detector units="<<numberOfDetUnits<<std::endl;

  if(ladon && barrel){
    meClustXLad_->Fill(sizeX);
    meClustYLad_->Fill(sizeY);
    meErrorXLad_->Fill(lerr_x);
    meErrorYLad_->Fill(lerr_y);  
	if(!reducedSet)
	{
    if(twoD) meXYPosLad_->Fill(rechit_x, rechit_y);
    else{
      meXYPosLad_px_->Fill(rechit_x); 
      meXYPosLad_py_->Fill(rechit_y);
    }
	}
  }

  if(layon && barrel){
    meClustXLay_->Fill(sizeX);
    meClustYLay_->Fill(sizeY);
    meErrorXLay_->Fill(lerr_x);
    meErrorYLay_->Fill(lerr_y); 
	if(!reducedSet)
	{
    if(twoD) meXYPosLay_->Fill(rechit_x, rechit_y);
    else{
      meXYPosLay_px_->Fill(rechit_x); 
      meXYPosLay_py_->Fill(rechit_y);
    }
	}
  }

  if(phion && barrel){
    meClustXPhi_->Fill(sizeX);
    meClustYPhi_->Fill(sizeY);
    meErrorXPhi_->Fill(lerr_x);
    meErrorYPhi_->Fill(lerr_y); 
    if(!reducedSet)
	{
    if(twoD) meXYPosPhi_->Fill(rechit_x, rechit_y);
    else{
      meXYPosPhi_px_->Fill(rechit_x); 
      meXYPosPhi_py_->Fill(rechit_y);
    }
	}	
  }

  if(bladeon && endcap){
    //meXYPosBlade_->Fill(rechit_x, rechit_y);
    meClustXBlade_->Fill(sizeX);
    meClustYBlade_->Fill(sizeY);
    meErrorXBlade_->Fill(lerr_x);
    meErrorYBlade_->Fill(lerr_y); 
  }

  if(diskon && endcap){
    //meXYPosDisk_->Fill(rechit_x, rechit_y);
    meClustXDisk_->Fill(sizeX);
    meClustYDisk_->Fill(sizeY);
    meErrorXDisk_->Fill(lerr_x);
    meErrorYDisk_->Fill(lerr_y); 
  }

  if(ringon && endcap){
    meClustXRing_->Fill(sizeX);
    meClustYRing_->Fill(sizeY);
    meErrorXRing_->Fill(lerr_x);
    meErrorYRing_->Fill(lerr_y); 
	if(!reducedSet)
	{
    if(twoD) meXYPosRing_->Fill(rechit_x, rechit_y);
    else{
      meXYPosRing_px_->Fill(rechit_x); 
      meXYPosRing_py_->Fill(rechit_y);
    }
	}	
  }
}

void SiPixelRecHitModulePhase1::nfill(const int& nrec, bool modon, bool ladon, bool layon, bool phion, bool bladeon, bool diskon, bool ringon) {
  
  bool barrel = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
  bool endcap = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);

  //if(modon) menRecHits_->Fill(nrec);
  //barrel
  if(ladon && barrel) menRecHitsLad_->Fill(nrec);
  if(layon && barrel) menRecHitsLay_->Fill(nrec);
  if(phion && barrel) menRecHitsPhi_->Fill(nrec);
  //endcap
  if(bladeon && endcap) menRecHitsBlade_->Fill(nrec);
  if(diskon && endcap) menRecHitsDisk_->Fill(nrec);
  if(ringon && endcap) menRecHitsRing_->Fill(nrec);
}
