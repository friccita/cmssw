//
// $Id: CompositeCandidate.h,v 1.1 2008/10/23 20:21:13 srappocc Exp $
//

#ifndef DataFormats_PatCandidates_CompositeCandidate_h
#define DataFormats_PatCandidates_CompositeCandidate_h

/**
  \class    pat::CompositeCandidate CompositeCandidate.h "DataFormats/PatCandidates/interface/CompositeCandidate.h"
  \brief    Analysis-level particle class

   CompositeCandidate implements an analysis-level particle class within the 'pat'
   namespace.

  \author   Sal Rappoccio
  \version  $Id: CompositeCandidate.h,v 1.1 2008/10/23 20:21:13 srappocc Exp $
*/

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/PATObject.h"


// Define typedefs for convenience
namespace pat {
  class CompositeCandidate;
  typedef std::vector<CompositeCandidate>              CompositeCandidateCollection; 
  typedef edm::Ref<CompositeCandidateCollection>       CompositeCandidateRef; 
  typedef edm::RefVector<CompositeCandidateCollection> CompositeCandidateRefVector; 
}


// Class definition
namespace pat {


  class CompositeCandidate : public PATObject<reco::CompositeCandidate> {

    public:

      /// default constructor
      CompositeCandidate();
      /// constructor from a composite candidate
      CompositeCandidate(const reco::CompositeCandidate & aCompositeCandidate);
      /// destructor
      virtual ~CompositeCandidate();

      /// required reimplementation of the Candidate's clone method
      virtual CompositeCandidate * clone() const { return new CompositeCandidate(*this); }

  };


}

#endif
