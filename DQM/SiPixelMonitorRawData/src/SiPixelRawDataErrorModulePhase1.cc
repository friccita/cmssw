#include "DQM/SiPixelMonitorRawData/interface/SiPixelRawDataErrorModulePhase1.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQM/SiPixelCommon/interface/SiPixelHistogramId.h"
// Framework
#include "FWCore/ServiceRegistry/interface/Service.h"
// STL
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
 
// Data Formats
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelNameUpgrade.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapNameUpgrade.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
 
using namespace std;
 
const int SiPixelRawDataErrorModulePhase1::LINK_bits = 6;
const int SiPixelRawDataErrorModulePhase1::ROC_bits  = 5;
const int SiPixelRawDataErrorModulePhase1::DCOL_bits = 5;
const int SiPixelRawDataErrorModulePhase1::PXID_bits = 8;
const int SiPixelRawDataErrorModulePhase1::ADC_bits  = 8;
const int SiPixelRawDataErrorModulePhase1::DataBit_bits = 1;
 
const int SiPixelRawDataErrorModulePhase1::ADC_shift  = 0;
const int SiPixelRawDataErrorModulePhase1::PXID_shift = ADC_shift + ADC_bits;
const int SiPixelRawDataErrorModulePhase1::DCOL_shift = PXID_shift + PXID_bits;
const int SiPixelRawDataErrorModulePhase1::ROC_shift  = DCOL_shift + DCOL_bits;
const int SiPixelRawDataErrorModulePhase1::LINK_shift = ROC_shift + ROC_bits;
const int SiPixelRawDataErrorModulePhase1::DB0_shift = 0;
const int SiPixelRawDataErrorModulePhase1::DB1_shift = DB0_shift + DataBit_bits;
const int SiPixelRawDataErrorModulePhase1::DB2_shift = DB1_shift + DataBit_bits;
const int SiPixelRawDataErrorModulePhase1::DB3_shift = DB2_shift + DataBit_bits;
const int SiPixelRawDataErrorModulePhase1::DB4_shift = DB3_shift + DataBit_bits;
const int SiPixelRawDataErrorModulePhase1::DB5_shift = DB4_shift + DataBit_bits;
const int SiPixelRawDataErrorModulePhase1::DB6_shift = DB5_shift + DataBit_bits;
const int SiPixelRawDataErrorModulePhase1::DB7_shift = DB6_shift + DataBit_bits;
 
const uint32_t SiPixelRawDataErrorModulePhase1::LINK_mask = ~(~uint32_t(0) << LINK_bits);
const uint32_t SiPixelRawDataErrorModulePhase1::ROC_mask  = ~(~uint32_t(0) << ROC_bits);
const uint32_t SiPixelRawDataErrorModulePhase1::DCOL_mask = ~(~uint32_t(0) << DCOL_bits);
const uint32_t SiPixelRawDataErrorModulePhase1::PXID_mask = ~(~uint32_t(0) << PXID_bits);
const uint32_t SiPixelRawDataErrorModulePhase1::ADC_mask  = ~(~uint32_t(0) << ADC_bits);
const uint32_t SiPixelRawDataErrorModulePhase1::DataBit_mask = ~(~uint32_t(0) << DataBit_bits);
 
const int SiPixelRawDataErrorModulePhase1::TRLRBGN_bits = 32;
const int SiPixelRawDataErrorModulePhase1::EVTLGT_bits  = 24;
const int SiPixelRawDataErrorModulePhase1::TRLREND_bits = 8;
 
const int SiPixelRawDataErrorModulePhase1::TRLRBGN_shift = 0;
const int SiPixelRawDataErrorModulePhase1::EVTLGT_shift  = TRLRBGN_shift + TRLRBGN_bits;
const int SiPixelRawDataErrorModulePhase1::TRLREND_shift = EVTLGT_shift + EVTLGT_bits;
 
const long long SiPixelRawDataErrorModulePhase1::TRLREND_mask = ~(~(long long)(0) << TRLREND_bits);
const long long SiPixelRawDataErrorModulePhase1::EVTLGT_mask  = ~(~(long long)(0) << EVTLGT_bits);
const long long SiPixelRawDataErrorModulePhase1::TRLRBGN_mask = ~(~(long long)(0) << TRLRBGN_bits);
//
// Constructors
//
SiPixelRawDataErrorModulePhase1::SiPixelRawDataErrorModulePhase1() : 
  id_(0),
  ncols_(416),
  nrows_(160) 
{ 
  _debug_ = false;
}
//
SiPixelRawDataErrorModulePhase1::SiPixelRawDataErrorModulePhase1(const uint32_t& id) : 
  id_(id),
  ncols_(416),
  nrows_(160)
{ 
  _debug_ = false;
}
//
SiPixelRawDataErrorModulePhase1::SiPixelRawDataErrorModulePhase1(const uint32_t& id, const int& ncols, const int& nrows) : 
  id_(id),
  ncols_(ncols),
  nrows_(nrows)
{ 
  _debug_ = false;
} 
//
// Destructor
//
SiPixelRawDataErrorModulePhase1::~SiPixelRawDataErrorModulePhase1() {}
//
// Book histograms for errors with detId
//
void SiPixelRawDataErrorModulePhase1::book(const edm::ParameterSet& iConfig, DQMStore::IBooker & iBooker, int type) {}
//
// Fill histograms
//
int SiPixelRawDataErrorModulePhase1::fill(const edm::DetSetVector<SiPixelRawDataError>& input, std::map<std::string,MonitorElement**> *meMapFEDs, bool modon, bool ladon, bool bladeon) {
  //std::cout<<"Entering SiPixelRawDataErrorModulePhase1::fill: "<<std::endl;
  bool barrel = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
  bool endcap = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
   
  // Get DQM interface
   
  unsigned int numberOfSeriousErrors = 0;
   
  edm::DetSetVector<SiPixelRawDataError>::const_iterator isearch = input.find(id_); // search  errors of detid
   
  if( isearch != input.end() ) {  // Not at empty iterator
    // Look at errors now
    edm::DetSet<SiPixelRawDataError>::const_iterator  di;
    for(di = isearch->data.begin(); di != isearch->data.end(); di++) {
      int FedId = di->getFedId();                  // FED the error came from
      int chanNmbr = 0;
      int errorType = di->getType();               // type of error
      int TBMType=-1; int TBMMessage=-1; int evtSize=-1; int evtNbr=-1; int fullType=-1;
      //bool notReset = true;
      const int LINK_bits = 6;
      const int LINK_shift = 26;
      const uint32_t LINK_mask = ~(~(uint32_t)(0) << LINK_bits);
 
      if(modon){
	if(errorType == 32 || errorType == 33 || errorType == 34) {
 	  long long errorWord = di->getWord64();     // for 64-bit error words
 	  chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
 	  if(errorType == 34) evtSize = (errorWord >> EVTLGT_shift) & EVTLGT_mask;
	} else {
 	  uint32_t errorWord = di->getWord32();      // for 32-bit error words
 	  chanNmbr = (errorWord >> LINK_shift) & LINK_mask; // default way to get channel number.  Only different for case 29 below.
 	  switch(errorType) {  // fill in the appropriate monitorables based on the information stored in the error word
 	  case(28) : {
 	    int NFa = (errorWord >> DB0_shift) & DataBit_mask;
 	    int NFb = (errorWord >> DB1_shift) & DataBit_mask;
 	    int NFc = (errorWord >> DB2_shift) & DataBit_mask;
 	    int NFd = (errorWord >> DB3_shift) & DataBit_mask;
 	    int NFe = (errorWord >> DB4_shift) & DataBit_mask;
 	    int NF2 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int L1A = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (NFa==1) fullType = 1; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFb==1) fullType = 2; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFc==1) fullType = 3; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFd==1) fullType = 4; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFe==1) fullType = 5; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NF2==1) fullType = 6; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (L1A==1) fullType = 7; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    chanNmbr = 0;  // signifies channel not known
 	    break; }
 	  case(29) : {
 	    int CH1 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int CH2 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int CH3 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int CH4 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int CH5 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int BLOCK_bits      = 3;
 	    int BLOCK_shift     = 8;
 	    uint32_t BLOCK_mask = ~(~uint32_t(0) << BLOCK_bits);
 	    int BLOCK = (errorWord >> BLOCK_shift) & BLOCK_mask;
 	    int localCH = 1*CH1+2*CH2+3*CH3+4*CH4+5*CH5;
 	    if (BLOCK%2==0) chanNmbr=(BLOCK/2)*9+localCH;
 	    else chanNmbr = ((BLOCK-1)/2)*9+4+localCH;
 	    if ((chanNmbr<1)||(chanNmbr>36)) chanNmbr=0;  // signifies unexpected result
 	    break; }
 	  case(30) : {
 	    int T0 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int T1 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int T2 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int T3 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int T4 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int T5 = (errorWord >> DB5_shift) & DataBit_mask;
 	    int T6 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int T7 = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (T0==1) TBMMessage=0;
 	    if (T1==1) TBMMessage=1;
 	    if (T2==1) TBMMessage=2;
 	    if (T3==1) TBMMessage=3;
 	    if (T4==1) TBMMessage=4;
 	    if (T5==1) TBMMessage=5;
 	    if (T6==1) TBMMessage=6;
 	    if (T7==1) TBMMessage=7;
 	    //if(TBMMessage==5 || TBMMessage==6) notReset=false;
 	    int StateMach_bits      = 4;
 	    int StateMach_shift     = 8;
 	    uint32_t StateMach_mask = ~(~uint32_t(0) << StateMach_bits);
 	    int StateMach = (errorWord >> StateMach_shift) & StateMach_mask;
 	    switch(StateMach) {
 	    case(0) : {
 	      TBMType = 0;
 	      break; }
 	    case(1) : case(9) : {
		TBMType = 1;
		break; }
 	    case(2) : case(4) : case(6) : {
		TBMType = 2;
		break; }
 	    case(8) : {
 	      TBMType = 3;
 	      break; }
 	    default : TBMType = 4;
 	    };
 	    break; }
 	  case(31) : {
 	    evtNbr = (errorWord >> ADC_shift) & ADC_mask;
 	    break; }
 	  case(36) : {
 	    //ROCId = (errorWord >> ROC_shift) & ROC_mask;
 	    break; }
 	  case(37) : {
 	    //DCOLId = (errorWord >> DCOL_shift) & DCOL_mask;
 	    //PXId = (errorWord >> PXID_shift) & PXID_mask;
 	    break; }
 	  case(38) : {
 	    //ROCNmbr = (errorWord >> ROC_shift) & ROC_mask;
 	    break; }
 	  default : break;
 	  };
 	}//end if not double precision  
      }//end if modon
 
      if(ladon && barrel){
	if(errorType == 32 || errorType == 33 || errorType == 34){
 	  long long errorWord = di->getWord64();     // for 64-bit error words
 	  if(errorType == 34) evtSize = (errorWord >> EVTLGT_shift) & EVTLGT_mask;
 	  chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
	} else {
 	  uint32_t errorWord = di->getWord32();      // for 32-bit error words
 	  chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
 	  switch(errorType) {  // fill in the appropriate monitorables based on the information stored in the error word
 	  case(28) : {
 	    int NFa = (errorWord >> DB0_shift) & DataBit_mask;
 	    int NFb = (errorWord >> DB1_shift) & DataBit_mask;
 	    int NFc = (errorWord >> DB2_shift) & DataBit_mask;
 	    int NFd = (errorWord >> DB3_shift) & DataBit_mask;
 	    int NFe = (errorWord >> DB4_shift) & DataBit_mask;
 	    int NF2 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int L1A = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (NFa==1) fullType = 1; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFb==1) fullType = 2; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFc==1) fullType = 3; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFd==1) fullType = 4; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFe==1) fullType = 5; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NF2==1) fullType = 6; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (L1A==1) fullType = 7; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    chanNmbr = 0;
 	    break; }
 	  case(29) : {
 	    int CH1 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int CH2 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int CH3 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int CH4 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int CH5 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int BLOCK_bits      = 3;
 	    int BLOCK_shift     = 8;
 	    uint32_t BLOCK_mask = ~(~uint32_t(0) << BLOCK_bits);
 	    int BLOCK = (errorWord >> BLOCK_shift) & BLOCK_mask;
 	    int localCH = 1*CH1+2*CH2+3*CH3+4*CH4+5*CH5;
 	    if (BLOCK%2==0) chanNmbr=(BLOCK/2)*9+localCH;
 	    else chanNmbr = ((BLOCK-1)/2)*9+4+localCH;
 	    if ((chanNmbr<1)||(chanNmbr>36)) chanNmbr=0;  // signifies unexpected result
 	    break; }
 	  case(30) : {
 	    int T0 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int T1 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int T2 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int T3 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int T4 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int T5 = (errorWord >> DB5_shift) & DataBit_mask;
 	    int T6 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int T7 = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (T0==1) TBMMessage=0;
 	    if (T1==1) TBMMessage=1;
 	    if (T2==1) TBMMessage=2;
 	    if (T3==1) TBMMessage=3;
 	    if (T4==1) TBMMessage=4;
 	    if (T5==1) TBMMessage=5;
 	    if (T6==1) TBMMessage=6;
 	    if (T7==1) TBMMessage=7;
 	    int StateMach_bits      = 4;
 	    int StateMach_shift     = 8;
 	    uint32_t StateMach_mask = ~(~uint32_t(0) << StateMach_bits);
 	    int StateMach = (errorWord >> StateMach_shift) & StateMach_mask;
 	    switch(StateMach) {
 	    case(0) : {
 	      TBMType = 0;
 	      break; }
 	    case(1) : case(9) : {
		TBMType = 1;
		break; }
 	    case(2) : case(4) : case(6) : {
		TBMType = 2;
		break; }
 	    case(8) : {
 	      TBMType = 3;
 	      break; }
 	    default : TBMType = 4;
 	    };
 	    break; }
 	  case(31) : {
 	    evtNbr = (errorWord >> ADC_shift) & ADC_mask;
 	    break; }
 	  case(36) : {
 	    //int ROCId = (errorWord >> ROC_shift) & ROC_mask;
 	    break; }
 	  case(37) : {
 	    //int DCOLId = (errorWord >> DCOL_shift) & DCOL_mask;
 	    //int PXId = (errorWord >> PXID_shift) & PXID_mask;
 	    break; }
 	  case(38) : {
 	    //int ROCNmbr = (errorWord >> ROC_shift) & ROC_mask;
 	    break; }
 	  default : break;
 	  };
 	}
      }//end if ladderon
 
      if(bladeon && endcap){
	if(errorType == 32 || errorType == 33 || errorType == 34){
 	  long long errorWord = di->getWord64();     // for 64-bit error words
 	  if(errorType == 34) evtSize = (errorWord >> EVTLGT_shift) & EVTLGT_mask;
 	  chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
	} else {
 	  uint32_t errorWord = di->getWord32();      // for 32-bit error words
 	  chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
 	  switch(errorType) {  // fill in the appropriate monitorables based on the information stored in the error word
 	  case(28) : {
 	    int NFa = (errorWord >> DB0_shift) & DataBit_mask;
 	    int NFb = (errorWord >> DB1_shift) & DataBit_mask;
 	    int NFc = (errorWord >> DB2_shift) & DataBit_mask;
 	    int NFd = (errorWord >> DB3_shift) & DataBit_mask;
 	    int NFe = (errorWord >> DB4_shift) & DataBit_mask;
 	    int NF2 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int L1A = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (NFa==1) fullType = 1; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFb==1) fullType = 2; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFc==1) fullType = 3; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFd==1) fullType = 4; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NFe==1) fullType = 5; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (NF2==1) fullType = 6; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    if (L1A==1) fullType = 7; ((*meMapFEDs)["meFullType_"][FedId])->Fill((int)fullType);
 	    chanNmbr = 0;
 	    break; }
 	  case(29) : {
 	    int CH1 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int CH2 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int CH3 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int CH4 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int CH5 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int BLOCK_bits      = 3;
 	    int BLOCK_shift     = 8;
 	    uint32_t BLOCK_mask = ~(~uint32_t(0) << BLOCK_bits);
 	    int BLOCK = (errorWord >> BLOCK_shift) & BLOCK_mask;
 	    int localCH = 1*CH1+2*CH2+3*CH3+4*CH4+5*CH5;
 	    if (BLOCK%2==0) chanNmbr=(BLOCK/2)*9+localCH;
 	    else chanNmbr = ((BLOCK-1)/2)*9+4+localCH;
 	    if ((chanNmbr<1)||(chanNmbr>36)) chanNmbr=0;  // signifies unexpected result
 	    break; }
 	  case(30) : {
 	    int T0 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int T1 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int T2 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int T3 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int T4 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int T5 = (errorWord >> DB5_shift) & DataBit_mask;
 	    int T6 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int T7 = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (T0==1) TBMMessage=0;
 	    if (T1==1) TBMMessage=1;
 	    if (T2==1) TBMMessage=2;
 	    if (T3==1) TBMMessage=3;
 	    if (T4==1) TBMMessage=4;
 	    if (T5==1) TBMMessage=5;
 	    if (T6==1) TBMMessage=6;
 	    if (T7==1) TBMMessage=7;
 	    int StateMach_bits      = 4;
 	    int StateMach_shift     = 8;
 	    uint32_t StateMach_mask = ~(~uint32_t(0) << StateMach_bits);
 	    int StateMach = (errorWord >> StateMach_shift) & StateMach_mask;
 	    switch(StateMach) {
 	    case(0) : {
 	      TBMType = 0;
 	      break; }
 	    case(1) : case(9) : {
		TBMType = 1;
		break; }
 	    case(2) : case(4) : case(6) : {
		TBMType = 2;
		break; }
 	    case(8) : {
 	      TBMType = 3;
 	      break; }
 	    default : TBMType = 4;
 	    };
 	    break; }
 	  case(31) : {
 	    evtNbr = (errorWord >> ADC_shift) & ADC_mask;
 	    break; }
 	  case(36) : {
 	    //int ROCId = (errorWord >> ROC_shift) & ROC_mask;
 	    break; }
 	  case(37) : {
 	    //int DCOLId = (errorWord >> DCOL_shift) & DCOL_mask;
 	    //int PXId = (errorWord >> PXID_shift) & PXID_mask;
 	    break; }
 	  case(38) : {
 	    //int ROCNmbr = (errorWord >> ROC_shift) & ROC_mask;
 	    break; }
 	  default : break;
 	  };
 	}
      }//end if bladeon
       
      if(!(FedId==38&&chanNmbr==7)){ // mask slow channel that spits out lots of EventNumber errors even when in STDBY!
 	if(errorType==29 || (errorType==30 && TBMType==1)){ // consider only TIMEOUT and OVERFLOW as serious errors right now
	  //if(!(errorType==30) || notReset){
	  //cout<<"ERROR: "<<errorType<<" "<<TBMType<<endl;
	  std::string hid;
	  static const char chNfmt[] = "Pixel/AdditionalPixelErrors/FED_%d/FedChNErrArray_%d";
	  char chNbuf[sizeof(chNfmt) + 2*32]; // 32 digits is enough for up to 2^105 + sign.
	  sprintf(chNbuf, chNfmt, FedId, chanNmbr);
	  hid = chNbuf;
	  if((*meMapFEDs)["meFedChNErrArray_"][FedId*37 + chanNmbr]) (*meMapFEDs)["meFedChNErrArray_"][FedId*37 + chanNmbr]->Fill((*meMapFEDs)["meFedChNErrArray_"][FedId*37 + chanNmbr]->getIntValue()+1);
 
	  static const char chLfmt[] = "Pixel/AdditionalPixelErrors/FED_%d/FedChLErrArray_%d";
	  char chLbuf[sizeof(chLfmt) + 2*32]; // 32 digits is enough for up to 2^105 + sign.
	  sprintf(chLbuf, chLfmt, FedId, chanNmbr);
	  hid = chLbuf;
	  if((*meMapFEDs)["meFedChLErrArray_"][FedId*37 + chanNmbr]) (*meMapFEDs)["meFedChLErrArray_"][FedId*37 + chanNmbr]->Fill(errorType); 
 
	  numberOfSeriousErrors++;
	  int messageType = 99;
	  if(errorType<30) messageType = errorType-25;
	  else if(errorType>30) messageType = errorType-19;
	  else if(errorType==30 && TBMMessage==0) messageType = errorType-25;
	  else if(errorType==30 && TBMMessage==1) messageType = errorType-24;
	  else if(errorType==30 && (TBMMessage==2 || TBMMessage==3 || TBMMessage==4)) messageType = errorType-23;
	  else if(errorType==30 && TBMMessage==7) messageType = errorType-22;
	  else if(errorType==30 && TBMType==1) messageType = errorType-21;
	  else if(errorType==30 && TBMType==2) messageType = errorType-20;
	  else if(errorType==30 && TBMType==3) messageType = errorType-19;
	  if(messageType<=20){
	    static const char fmt[] = "Pixel/AdditionalPixelErrors/FED_%d/FedETypeNErrArray_%d";
	    char buf[sizeof(fmt) + 2*32]; // 32 digits is enough for up to 2^105 + sign.
	    sprintf(buf, fmt, FedId, messageType);
	    hid = buf;
	    if((*meMapFEDs)["meFedETypeNErrArray_"][FedId*21+messageType]) (*meMapFEDs)["meFedETypeNErrArray_"][FedId*21+messageType]->Fill((*meMapFEDs)["meFedETypeNErrArray_"][FedId*21+messageType]->getIntValue()+1); 
	  }
 	}
 
 	(*meMapFEDs)["meNErrors_"][FedId]->Fill((int)numberOfSeriousErrors);
 	(*meMapFEDs)["meTBMMessage_"][FedId]->Fill((int)TBMMessage);
 	(*meMapFEDs)["meTBMType_"][FedId]->Fill((int)TBMType);
	(*meMapFEDs)["meErrorType_"][FedId]->Fill((int)errorType);
	(*meMapFEDs)["meFullType_"][FedId]->Fill((int)fullType);
	(*meMapFEDs)["meEvtNbr_"][FedId]->Fill((int)evtNbr);
	(*meMapFEDs)["meEvtSize_"][FedId]->Fill((int)evtSize);
      }
    }//end for loop over all errors on module
  }//end if not an empty iterator
  return numberOfSeriousErrors;
}
 
int SiPixelRawDataErrorModulePhase1::fillFED(const edm::DetSetVector<SiPixelRawDataError>& input, std::map<std::string,MonitorElement**> *meMapFEDs) {
  //std::cout<<"Entering   SiPixelRawDataErrorModulePhase1::fillFED: "<<static_cast<int>(id_)<<std::endl;
  unsigned int numberOfSeriousErrors = 0;
   
  edm::DetSetVector<SiPixelRawDataError>::const_iterator isearch = input.find(0xffffffff); // search  errors of detid
  if( isearch != input.end() ) {  // Not an empty iterator
    // Look at FED errors now	
    edm::DetSet<SiPixelRawDataError>::const_iterator  di;
    for(di = isearch->data.begin(); di != isearch->data.end(); di++) {
      int FedId = di->getFedId();                  // FED the error came from
      int chanNmbr = -1;
      int errorType = 0;               // type of error
      if(FedId==static_cast<int>(id_)) {
 	errorType = di->getType();               // type of error
 	((*meMapFEDs)["meErrorType_"][id_])->Fill((int)errorType);
 	//bool notReset=true;
	int TBMType=-1; int TBMMessage=-1; int evtSize=-1; int fullType=-1;
	const int LINK_bits = 6;
	const int LINK_shift = 26;
	const uint32_t LINK_mask = ~(~(uint32_t)(0) << LINK_bits);
 	if((errorType == 32)||(errorType == 33)||(errorType == 34)) {
 	  long long errorWord = di->getWord64();     // for 64-bit error words
 	  chanNmbr = 0;
 	  switch(errorType) {  // fill in the appropriate monitorables based on the information stored in the error word
 	  case(32) : {
 	    break; }
 	  case(33) : {
 	    break; }
 	  case(34) : {
 	    evtSize = (errorWord >> EVTLGT_shift) & EVTLGT_mask;
 	    if(!(FedId==38&&chanNmbr==7)) ((*meMapFEDs)["meEvtSize_"][id_])->Fill((int)evtSize); 
 	    break; }
 	  default : break;
 	  };
 	} else {
 	  uint32_t errorWord = di->getWord32();      // for 32-bit error words
 	  switch(errorType) {  // fill in the appropriate monitorables based on the information stored in the error word
 	  case(25) : case(39) : {
	      chanNmbr = 0;
	      break; }
 	  case(28) : {
 	    int NFa = (errorWord >> DB0_shift) & DataBit_mask;
 	    int NFb = (errorWord >> DB1_shift) & DataBit_mask;
 	    int NFc = (errorWord >> DB2_shift) & DataBit_mask;
 	    int NFd = (errorWord >> DB3_shift) & DataBit_mask;
 	    int NFe = (errorWord >> DB4_shift) & DataBit_mask;
 	    int NF2 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int L1A = (errorWord >> DB7_shift) & DataBit_mask;
 	    if (NFa==1) fullType = 1; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    if (NFb==1) fullType = 2; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    if (NFc==1) fullType = 3; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    if (NFd==1) fullType = 4; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    if (NFe==1) fullType = 5; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    if (NF2==1) fullType = 6; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    if (L1A==1) fullType = 7; ((*meMapFEDs)["meFullType_"][id_])->Fill((int)fullType);
 	    chanNmbr = 0;
 	    break; }
 	  case(29) : {
 	    int CH1 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int CH2 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int CH3 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int CH4 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int CH5 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int BLOCK_bits      = 3;
 	    int BLOCK_shift     = 8;
 	    uint32_t BLOCK_mask = ~(~uint32_t(0) << BLOCK_bits);
 	    int BLOCK = (errorWord >> BLOCK_shift) & BLOCK_mask;
 	    int localCH = 1*CH1+2*CH2+3*CH3+4*CH4+5*CH5;
 	    if (BLOCK%2==0) chanNmbr=(BLOCK/2)*9+localCH;
 	    else chanNmbr = ((BLOCK-1)/2)*9+4+localCH;
 	    if ((chanNmbr<1)||(chanNmbr>36)) chanNmbr=0;  // signifies unexpected result
 	    break; }
 	  case(30) : {
 	    int T0 = (errorWord >> DB0_shift) & DataBit_mask;
 	    int T1 = (errorWord >> DB1_shift) & DataBit_mask;
 	    int T2 = (errorWord >> DB2_shift) & DataBit_mask;
 	    int T3 = (errorWord >> DB3_shift) & DataBit_mask;
 	    int T4 = (errorWord >> DB4_shift) & DataBit_mask;
 	    int T5 = (errorWord >> DB5_shift) & DataBit_mask;
 	    int T6 = (errorWord >> DB6_shift) & DataBit_mask;
 	    int T7 = (errorWord >> DB7_shift) & DataBit_mask;
 	    if(!(FedId==38&&chanNmbr==7)){
 	      if (T0==1) { TBMMessage=0; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T1==1) { TBMMessage=1; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T2==1) { TBMMessage=2; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T3==1) { TBMMessage=3; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T4==1) { TBMMessage=4; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T5==1) { TBMMessage=5; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T6==1) { TBMMessage=6; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	      if (T7==1) { TBMMessage=7; ((*meMapFEDs)["meTBMMessage_"][id_])->Fill((int)TBMMessage); }
 	    }
 	    //if(TBMMessage==5 || TBMMessage==6) notReset=false;
 	    int StateMach_bits      = 4;
 	    int StateMach_shift     = 8;
 	    uint32_t StateMach_mask = ~(~uint32_t(0) << StateMach_bits);
 	    int StateMach = (errorWord >> StateMach_shift) & StateMach_mask;
 	    switch(StateMach) {
 	    case(0) : {
 	      TBMType = 0;
 	      break; }
 	    case(1) : case(9) : {
		TBMType = 1;
		break; }
 	    case(2) : case(4) : case(6) : {
		TBMType = 2;
		break; }
 	    case(8) : {
 	      TBMType = 3;
 	      break; }
 	    default : TBMType = 4;
 	    };
 	    if(!(FedId==38&&chanNmbr==7)) ((*meMapFEDs)["meTBMType_"][id_])->Fill((int)TBMType);
 	    chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
 	    break; }
 	  case(31) : {
 	    int evtNbr = (errorWord >> ADC_shift) & ADC_mask;
 	    if(!(FedId==38&&chanNmbr==7))((*meMapFEDs)["meEvtNbr_"][id_])->Fill((int)evtNbr);
 	    chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
 	    break; }
 	  case(35) : case(36) : case(37) : case(38) : {
	      chanNmbr = (errorWord >> LINK_shift) & LINK_mask;
	      break; }
 	  default : break;
 	  };
 	}// end if errorType
 
 	//if(!(errorType==30) || notReset){
 	if(errorType==29 || (errorType==30 && TBMType==1)){ // consider only TIMEOUT and OVERFLOW as serious errors right now
	  if(!(FedId==38&&chanNmbr==7)){ // mask slow channel that spits out lots of EventNumber errors even when in STDBY!
	    std::string hid;
	    //cout<<"FEDERROR: "<<errorType<<" "<<TBMType<<endl;
	    static const char chNfmt[] = "Pixel/AdditionalPixelErrors/FED_%d/FedChNErrArray_%d";
	    char chNbuf[sizeof(chNfmt) + 2*32]; // 32 digits is enough for up to 2^105 + sign.
	    sprintf(chNbuf, chNfmt, FedId, chanNmbr);
	    hid = chNbuf;
	    if((*meMapFEDs)["meFedChNErrArray_"][id_*37 + chanNmbr]) (*meMapFEDs)["meFedChNErrArray_"][id_*37 + chanNmbr]->Fill((*meMapFEDs)["meFedChNErrArray_"][id_*37 + chanNmbr]->getIntValue()+1);
 
	    static const char chLfmt[] = "Pixel/AdditionalPixelErrors/FED_%d/FedChLErrArray_%d";
	    char chLbuf[sizeof(chLfmt) + 2*32]; // 32 digits is enough for up to 2^105 + sign.
	    sprintf(chLbuf, chLfmt, FedId, chanNmbr);
	    hid = chLbuf;
	    if((*meMapFEDs)["meFedChLErrArray_"][id_*37 + chanNmbr]) (*meMapFEDs)["meFedChLErrArray_"][id_*37 + chanNmbr]->Fill(errorType); 
 
	    numberOfSeriousErrors++;
	    int messageType = 99;
	    if(errorType<30) messageType = errorType-25;
	    else if(errorType>30) messageType = errorType-19;
	    else if(errorType==30 && TBMMessage==0) messageType = errorType-25;
	    else if(errorType==30 && TBMMessage==1) messageType = errorType-24;
	    else if(errorType==30 && (TBMMessage==2 || TBMMessage==3 || TBMMessage==4)) messageType = errorType-23;
	    else if(errorType==30 && TBMMessage==7) messageType = errorType-22;
	    else if(errorType==30 && TBMType==1) messageType = errorType-21;
	    else if(errorType==30 && TBMType==2) messageType = errorType-20;
	    else if(errorType==30 && TBMType==3) messageType = errorType-19;
	    if(messageType<=20){
	      static const char fmt[] = "Pixel/AdditionalPixelErrors/FED_%d/FedETypeNErrArray_%d";
	      char buf[sizeof(fmt) + 2*32]; // 32 digits is enough for up to 2^105 + sign.
	      sprintf(buf, fmt, FedId, messageType);
	      hid = buf;
	      if((*meMapFEDs)["meFedETypeNErrArray_"][id_*21 + messageType]) (*meMapFEDs)["meFedETypeNErrArray_"][id_+21 + messageType]->Fill((*meMapFEDs)["meFedETypeNErrArray_"][id_* 21 + messageType]->getIntValue()+1);
	    }
 	  }//end if bad channel
	}//end if not 30 || notReset
      }//end if
    }//end for
    if(numberOfSeriousErrors>0) ((*meMapFEDs)["meNErrors_"][id_])->Fill((float)numberOfSeriousErrors);
 
  }// end if not an empty iterator
   
  //std::cout<<"...leaving   SiPixelRawDataErrorModulePhase1::fillFED. "<<std::endl;
  return numberOfSeriousErrors;
}
