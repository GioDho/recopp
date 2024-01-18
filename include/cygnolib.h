#ifndef __CYGNO_LIB_H__
#define __CYGNO_LIB_H__

#include "midasio.h"
#include "mvodb.h"
#include "TMidasEvent.h"
#include <zlib.h>
#include <stdio.h>
#include <chrono>
#include <string>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <list>

namespace cygnolib {
    
    class Picture {
    public:
        Picture(int height = 2304, int width = 2304);
        ~Picture();
        
        int GetNRows();
        int GetNColumns();
        std::vector<std::vector<uint16_t>> GetFrame();
        void SetFrame(std::vector<std::vector<uint16_t>> inputframe);
        void Print(int a, int b);
        void SavePng(std::string filename, int vmin = 99, int vmax = 130);
        
    private:
        std::vector<std::vector<uint16_t>> frame;
        int nrows;
        int ncolumns;
    };
    // Appunti:
    // Almeno C++17/C++20 , con i vector
    // #include <algorithm> <----- algoritmi per i vector automaticamente parallelizzati
    // smart pointers (funzioni ritornano smart pointer invece che oggetti per ridurre memory usage)
    
    class DGHeader {
    public:
        
        DGHeader(std::vector<uint32_t> rawheader);
        ~DGHeader();
        
        void Print();
        
        int nboards;
        std::vector<int> board_model;
        std::vector<int> nwaveforms;
        std::vector<int> nchannels;
        std::vector<int> nsamples;
        std::vector<int> vertical_resolution;
        std::vector<int> sampling_rate;
        std::vector<std::vector<int>> offsets;
        std::vector<std::vector<int>> TTT;
        std::vector<std::vector<int>> SIC;
        
    };
    
    // ======= to decide =========
    class Board {
    public:
        Board(DGHeader DGH, std::vector<uint16_t> rawwaveforms, int board_model);
        ~Board();
        
        std::vector<std::vector<std::vector<uint16_t>>> GetData();
        
    private:
        DGHeader fDGH;
        int fboard_model;
        int fnchannels;
        int fnsamples;
        int fnwaveforms;
        //....
        std::vector<std::vector<std::vector<uint16_t>>> fData;
    
    };
    
    class PMTData {
    public:
        
        PMTData(DGHeader DGH, std::vector<uint16_t> rawwaveforms);
        ~PMTData();
        
        static std::list<Board> data;
        
        std::vector<std::vector<std::vector<uint16_t>>> GetWaveforms(int board_model);
        
    private:
        DGHeader fDGH;
    };
    
    // Mettere insieme PMTData e Board --> togliere copia di DGH etc
    
    
    // ======= to decide =========
    
    
    void foo();
    
    TMReaderInterface* OpenMidasFile(std::string filename);
    bool FindBankByName(TMidasEvent &event, std::string bname, bool verbose = false);
    
    bool FindODBDumpBOR(TMidasEvent &event, bool verbose = false);
    MVOdb* GetODBDumpBOR(TMidasEvent &event,  MVOdbError *odberror = NULL);
    
    void InitializePMTReadout(std::string filename, bool *DRS4correction, std::vector<float> *channels_offsets);
    // implementazione correzione to-do
    Picture  daq_cam2pic(TMidasEvent &event, std::string cam_model = "fusion");
    DGHeader daq_dgh2head(TMidasEvent &event); //Header
    PMTData daq_dig2PMTData(TMidasEvent &event, DGHeader DGH);
    
    
}

#endif
