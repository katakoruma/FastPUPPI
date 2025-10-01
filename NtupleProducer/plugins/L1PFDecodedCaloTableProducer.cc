// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/L1TParticleFlow/interface/PFCluster.h"
#include "DataFormats/L1TCalorimeterPhase2/interface/CaloCrystalCluster.h"
#include "DataFormats/L1THGCal/interface/HGCalMulticluster.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/NanoAOD/interface/FlatTable.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"


#include <algorithm>

class L1PFDecodedCaloTableProducer : public edm::global::EDProducer<>  {
    public:
        explicit L1PFDecodedCaloTableProducer(const edm::ParameterSet&);
        ~L1PFDecodedCaloTableProducer();

    private:
        virtual void produce(edm::StreamID id, edm::Event& iEvent, const edm::EventSetup& iSetup) const override;


        std::string name_;
        edm::EDGetTokenT<l1t::PFClusterCollection> clusters_;
        StringCutObjectSelector<l1t::PFCluster> sel_;

};

L1PFDecodedCaloTableProducer::L1PFDecodedCaloTableProducer(const edm::ParameterSet& iConfig) :
    name_(iConfig.getParameter<std::string>("name")),
    clusters_(consumes<l1t::PFClusterCollection>(iConfig.getParameter<edm::InputTag>("src"))),
    sel_(iConfig.getParameter<std::string>("cut"), true)
{
    produces<nanoaod::FlatTable>();

}

L1PFDecodedCaloTableProducer::~L1PFDecodedCaloTableProducer() { }

// ------------ method called for each event  ------------
    void
L1PFDecodedCaloTableProducer::produce(edm::StreamID id, edm::Event& iEvent, const edm::EventSetup& iSetup) const
{
    edm::Handle<l1t::PFClusterCollection> clusters;
    iEvent.getByToken(clusters_, clusters);

    std::vector<const l1t::PFCluster *> selected;


    for (const l1t::PFCluster &cl : *clusters)
        if(sel_(cl)) selected.push_back(&cl);

    
    // create the table
    unsigned int ncands = selected.size();
    auto out = std::make_unique<nanoaod::FlatTable>(ncands, name_, false, true);

    std::vector<float> vals_empt, vals_srrTot, vals_hwSrrTot, vals_meanz, 
        vals_hwMeanZ, vals_hoe, vals_piIdProb, vals_PuIdProb, vals_EmIdProb, 
        vals_caloIso, vals_showerShape; 
     std::vector<float> vals_showerlength,
        vals_coreshowerlength, vals_emf, vals_hw_emf, vals_abseta, 
        vals_hw_abseta, 
        vals_hw_meanz, 
        vals_sigmaetaeta, vals_hw_sigmaetaeta, 
        vals_sigmaphiphi, vals_hw_sigmaphiphi, vals_sigmazz, vals_hw_sigmazz,
        vals_firstlayer, vals_maxlayer, vals_hw_maxlayer,
        vals_sigmarr, vals_hw_sigmarr, vals_sigmarrmax, vals_hw_sigmarrmax,
        vals_sigmarrmean, vals_hw_sigmarrmean, vals_zbarycenter, vals_ebm0,
        vals_ebm1, vals_hbm,
        vals_first1layers, vals_first3layers, vals_first5layers,
        vals_firstHcal1layers, vals_firstHcal3layers, vals_firstHcal5layers,
        vals_last1layers, vals_last3layers, vals_last5layers;

    vals_empt.resize(ncands);
    vals_srrTot.resize(ncands);
    vals_hwSrrTot.resize(ncands);
    vals_meanz.resize(ncands);
    vals_hwMeanZ.resize(ncands);
    vals_hoe.resize(ncands);
    vals_piIdProb.resize(ncands);
    vals_PuIdProb.resize(ncands);
    vals_EmIdProb.resize(ncands);
    vals_caloIso.resize(ncands);
    vals_showerShape.resize(ncands);
    vals_showerlength.resize(ncands);
    vals_coreshowerlength.resize(ncands);
    vals_emf.resize(ncands);
    vals_hw_emf.resize(ncands);
    vals_abseta.resize(ncands);
    vals_hw_abseta.resize(ncands);
    vals_hw_meanz.resize(ncands);
    vals_sigmaetaeta.resize(ncands);
    vals_hw_sigmaetaeta.resize(ncands);
    vals_sigmaphiphi.resize(ncands);
    vals_hw_sigmaphiphi.resize(ncands);
    vals_sigmazz.resize(ncands);
    vals_hw_sigmazz.resize(ncands);

    vals_firstlayer.resize(ncands);
    vals_maxlayer.resize(ncands);
    vals_hw_maxlayer.resize(ncands);
    vals_sigmarr.resize(ncands);
    vals_hw_sigmarr.resize(ncands);
    vals_sigmarrmax.resize(ncands);
    vals_hw_sigmarrmax.resize(ncands);
    vals_sigmarrmean.resize(ncands);
    vals_hw_sigmarrmean.resize(ncands);
    vals_zbarycenter.resize(ncands);
    vals_ebm0.resize(ncands);
    vals_ebm1.resize(ncands);
    vals_hbm.resize(ncands);
    vals_first1layers.resize(ncands);
    vals_first3layers.resize(ncands);
    vals_first5layers.resize(ncands);
    vals_firstHcal1layers.resize(ncands);
    vals_firstHcal3layers.resize(ncands);
    vals_firstHcal5layers.resize(ncands);
    vals_last1layers.resize(ncands);
    vals_last3layers.resize(ncands);
    vals_last5layers.resize(ncands);

    for (unsigned int i = 0; i < ncands; ++i) {
        const auto cand = selected[i];
        auto obj = cand->caloDigiObj();

        if(auto digi = std::get_if<l1ct::EmCaloObj>(&obj)){
            vals_srrTot[i] =   digi->floatSrrTot();
            vals_hwSrrTot[i] = digi->hwSrrTot.to_float();
            vals_meanz[i] =    digi->floatMeanZ();
            vals_hwMeanZ[i] =  digi->hwMeanZ.to_float();
            vals_hoe[i] =      digi->floatHoe();
            vals_piIdProb[i] = digi->floatPiProb();
            vals_PuIdProb[i] = digi->floatPuProb();
            vals_EmIdProb[i] = digi->floatEmProb();

            const l1tp2::CaloCrystalCluster *crycl = dynamic_cast<const l1tp2::CaloCrystalCluster *>(cand->constituentsAndFractions().front().first.get());
            if(crycl) {
                vals_caloIso[i] = crycl->isolation();
                vals_showerShape[i] = crycl->e2x5() / crycl->e5x5();
            }
        } else if(auto digi = std::get_if<l1ct::HadCaloObj>(&obj)){
            vals_empt[i] = digi->floatEmPt();

            vals_srrTot[i] = digi->floatSrrTot();
            vals_hwSrrTot[i] = digi->hwSrrTot.to_float();
            vals_meanz[i] = digi->floatMeanZ();
            vals_hwMeanZ[i] = digi->hwMeanZ.to_float();
            vals_hoe[i] = digi->floatHoe();
            vals_piIdProb[i] = digi->floatPiProb();
            vals_PuIdProb[i] = digi->floatPuProb();
            vals_EmIdProb[i] = digi->floatEmProb();

        }

        // Now fill variables which are not yet decoded by Correlator and need the upstream collection
        const l1tp2::CaloCrystalCluster *crycl = dynamic_cast<const l1tp2::CaloCrystalCluster *>(cand->constituentsAndFractions().front().first.get());
        if(crycl) {
            vals_caloIso[i] = crycl->isolation();
            vals_showerShape[i] = crycl->e2x5() / crycl->e5x5();
        }

        const l1t::HGCalMulticluster *hgcalcl = dynamic_cast<const l1t::HGCalMulticluster *>(cand->constituentsAndFractions().front().first.get());
        if(hgcalcl) {
            static constexpr float ETAPHI_LSB = M_PI / 720;
            static constexpr float SIGMAZZ_LSB = 778.098 / (1 << 7);
            static constexpr float SIGMAPHIPHI_LSB = 0.12822 / (1 << 7);
            static constexpr float SIGMAETAETA_LSB = 0.148922 / (1 << 5);
            static constexpr float SIGMARR_LSB = 0.2 / 10;
            static constexpr float FIRST_LAYERS = 0.1 / (1 << 5);
            static constexpr float HGCAL_LAYERS = 0.1667 / (1 << 6);
            static constexpr float LAST_LAYERS = 0.5 / (1 << 8);

            ap_uint<6> w_showerlenght = hgcalcl->showerLength();
            ap_uint<6> w_coreshowerlenght = hgcalcl->coreShowerLength();
            ap_uint<8> w_emf = std::min(round(hgcalcl->eot() * 256), float(255.));
            ap_uint<10> w_abseta = round(fabs(hgcalcl->eta()) / ETAPHI_LSB);
            ap_ufixed<12, 11, AP_RND_CONV, AP_SAT> w_meanz_f = fabs(hgcalcl->zBarycenter()) - 320;  // LSB = 0.5cm
            ap_uint<12> w_meanz = w_meanz_f.range();
            ap_uint<5> w_sigmaetaeta = round(hgcalcl->sigmaEtaEtaTot() / SIGMAETAETA_LSB);
            ap_uint<7> w_sigmaphiphi = round(hgcalcl->sigmaPhiPhiTot() / SIGMAPHIPHI_LSB);
            ap_uint<7> w_sigmazz = round(hgcalcl->sigmaZZ() / SIGMAZZ_LSB);
            
            ap_uint<6> w_firstlayer = hgcalcl->firstLayer();
            ap_uint<7> w_maxlayer = hgcalcl->maxLayer();

            ap_uint<7> w_sigmarr = round(hgcalcl->sigmaRRTot() / SIGMARR_LSB);
            ap_uint<7> w_sigmarrmax = round(hgcalcl->sigmaRRMax() / SIGMARR_LSB);
            ap_uint<7> w_sigmarrmean = round(hgcalcl->sigmaRRMean() / SIGMARR_LSB);

            ap_uint<7> w_zbarycenter = round(hgcalcl->zBarycenter() / 1.); 

            ap_uint<7> w_ebm0 = hgcalcl->ebm0();
            ap_uint<7> w_ebm1 = hgcalcl->ebm1();
            ap_uint<7> w_hbm = hgcalcl->hbm();

            ap_uint<7> w_first1layers = round(hgcalcl->first1layers() / FIRST_LAYERS);
            ap_uint<7> w_first3layers = round(hgcalcl->first3layers() / FIRST_LAYERS);
            ap_uint<7> w_first5layers = round(hgcalcl->first5layers() / FIRST_LAYERS);
            ap_uint<7> w_firstHcal1layers = round(hgcalcl->firstHcal1layers() / HGCAL_LAYERS);
            ap_uint<7> w_firstHcal3layers = round(hgcalcl->firstHcal3layers() / HGCAL_LAYERS);
            ap_uint<7> w_firstHcal5layers = round(hgcalcl->firstHcal5layers() / HGCAL_LAYERS);
            ap_uint<7> w_last1layers = round(hgcalcl->last1layers() / LAST_LAYERS);
            ap_uint<7> w_last3layers = round(hgcalcl->last3layers() / LAST_LAYERS);
            ap_uint<7> w_last5layers = round(hgcalcl->last5layers() / LAST_LAYERS);


            vals_showerlength[i] = w_showerlenght.to_int();
            vals_coreshowerlength[i] = w_coreshowerlenght.to_int();
            vals_emf[i] = w_emf / 256.;
            vals_hw_emf[i] = w_emf.to_float();
            vals_abseta[i] = w_abseta * ETAPHI_LSB;
            vals_hw_abseta[i] = w_abseta.to_float();
            vals_hw_meanz[i] = w_meanz*0.5;
            vals_sigmaetaeta[i] = w_sigmaetaeta * SIGMAETAETA_LSB;
            vals_hw_sigmaetaeta[i] = w_sigmaetaeta.to_float();
            vals_sigmaphiphi[i] = w_sigmaphiphi * SIGMAPHIPHI_LSB;
            vals_hw_sigmaphiphi[i] = w_sigmaphiphi.to_float();
            vals_sigmazz[i] = w_sigmazz * SIGMAZZ_LSB;
            vals_hw_sigmazz[i] = w_sigmazz.to_float();
            vals_firstlayer[i] = w_firstlayer.to_float();
            vals_maxlayer[i] = w_maxlayer.to_float();
            vals_sigmarr[i] = w_sigmarr * SIGMARR_LSB;
            vals_hw_sigmarr[i] = w_sigmarr.to_float();
            vals_sigmarrmax[i] = w_sigmarrmax * SIGMARR_LSB;
            vals_hw_sigmarrmax[i] = w_sigmarrmax.to_float();
            vals_sigmarrmean[i] = w_sigmarrmean * SIGMARR_LSB;
            vals_hw_sigmarrmean[i] = w_sigmarrmean.to_float();
            vals_zbarycenter[i] = w_zbarycenter.to_float();
            vals_ebm0[i] = w_ebm0.to_float();
            vals_ebm1[i] = w_ebm1.to_float();
            vals_hbm[i] = w_hbm.to_float();
            vals_first1layers[i] = w_first1layers.to_float();
            vals_first3layers[i] = w_first3layers.to_float();
            vals_first5layers[i] = w_first5layers.to_float();
            vals_firstHcal1layers[i] = w_firstHcal1layers.to_float();
            vals_firstHcal3layers[i] = w_firstHcal3layers.to_float();
            vals_firstHcal5layers[i] = w_firstHcal5layers.to_float();
            vals_last1layers[i] = w_last1layers.to_float();
            vals_last3layers[i] = w_last3layers.to_float();
            vals_last5layers[i] = w_last5layers.to_float();
        }
    }


    out->addColumn<float>("empt", vals_empt, "");
    out->addColumn<float>("srrTot", vals_srrTot, "");
    out->addColumn<float>("hwSrrTot", vals_hwSrrTot, "");
    out->addColumn<float>("meanz", vals_meanz, "");
    out->addColumn<float>("hwMeanZ", vals_hwMeanZ, "");
    out->addColumn<float>("hoe", vals_hoe, "");
    out->addColumn<float>("piIdProb", vals_piIdProb, "");
    out->addColumn<float>("PuIdProb", vals_PuIdProb, "");
    out->addColumn<float>("EmIdProb", vals_EmIdProb, "");
    out->addColumn<float>("caloIso", vals_caloIso, "");
    out->addColumn<float>("showerShape", vals_showerShape, "");

    out->addColumn<float>("showerlength", vals_showerlength, "");
    out->addColumn<float>("coreshowerlength", vals_coreshowerlength, "");
    out->addColumn<float>("emf", vals_emf, "");
    out->addColumn<float>("hwEmf", vals_hw_emf, "");
    out->addColumn<float>("abseta", vals_abseta, "");
    out->addColumn<float>("hwAbseta", vals_hw_abseta, "");
    out->addColumn<float>("hwFPMeanz", vals_hw_meanz, "");    
    out->addColumn<float>("sigmaetaeta", vals_sigmaetaeta, "");
    out->addColumn<float>("hwSigmaetaeta", vals_hw_sigmaetaeta, "");
    out->addColumn<float>("sigmaphiphi", vals_sigmaphiphi, "");
    out->addColumn<float>("hwSigmaphiphi", vals_hw_sigmaphiphi, "");
    out->addColumn<float>("sigmazz", vals_sigmazz, "");
    out->addColumn<float>("hwSigmazz", vals_hw_sigmazz, "");

    out->addColumn<float>("firstlayer", vals_firstlayer, "");
    out->addColumn<float>("maxlayer", vals_maxlayer, "");
    out->addColumn<float>("hwMaxlayer", vals_hw_maxlayer, "");
    out->addColumn<float>("sigmarr", vals_sigmarr, "");
    out->addColumn<float>("hwSigmarr", vals_hw_sigmarr, "");
    out->addColumn<float>("sigmarrmax", vals_sigmarrmax, "");
    out->addColumn<float>("hwSigmarrmax", vals_hw_sigmarrmax, "");
    out->addColumn<float>("sigmarrmean", vals_sigmarrmean, "");
    out->addColumn<float>("hwSigmarrmean", vals_hw_sigmarrmean, "");
    out->addColumn<float>("zbarycenter", vals_zbarycenter, "");
    out->addColumn<float>("ebm0", vals_ebm0, "");
    out->addColumn<float>("ebm1", vals_ebm1, "");
    out->addColumn<float>("hbm", vals_hbm, "");
    out->addColumn<float>("first1layers", vals_first1layers, "");
    out->addColumn<float>("first3layers", vals_first3layers, "");
    out->addColumn<float>("first5layers", vals_first5layers, "");
    out->addColumn<float>("firstHcal1layers", vals_firstHcal1layers, "");
    out->addColumn<float>("firstHcal3layers", vals_firstHcal3layers, "");
    out->addColumn<float>("firstHcal5layers", vals_firstHcal5layers, "");
    out->addColumn<float>("last1layers", vals_last1layers, "");
    out->addColumn<float>("last3layers", vals_last3layers, "");
    out->addColumn<float>("last5layers", vals_last5layers, "");

    // save to the event branches
    iEvent.put(std::move(out));

   
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1PFDecodedCaloTableProducer);
