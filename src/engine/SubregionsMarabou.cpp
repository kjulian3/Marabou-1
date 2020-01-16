/*********************                                                        */
/*! \file SubregionsMarabou.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Kyle Julian
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 **/

#include "File.h"
#include "MStringf.h"
#include "Options.h"
#include "PropertyParser.h"
#include "MarabouError.h"
#include "QueryLoader.h"
#include "AcasParser.h"
#include "SubregionsMarabou.h"

SubregionsMarabou::SubregionsMarabou( unsigned verbosity)
    : _acasParser( NULL )
    , _engine( verbosity )
    , _initialState()
{
}

SubregionsMarabou::~SubregionsMarabou()
{
    if ( _acasParser )
    {
        delete _acasParser;
        _acasParser = NULL;
    }
}
void SubregionsMarabou::prepareInputQuery()
{
    String inputQueryFilePath = Options::get()->getString( Options::INPUT_QUERY_FILE_PATH );
    if ( inputQueryFilePath.length() > 0 )
    {
        /*
          Step 1: extract the query
        */
        if ( !File::exists( inputQueryFilePath ) )
        {
            printf( "Error: the specified inputQuery file (%s) doesn't exist!\n", inputQueryFilePath.ascii() );
            throw MarabouError( MarabouError::FILE_DOESNT_EXIST, inputQueryFilePath.ascii() );
        }
        _inputQuery = QueryLoader::loadQuery(inputQueryFilePath);
    }
    else
    {
        /*
          Step 1: extract the network
        */
        String networkFilePath = Options::get()->getString( Options::INPUT_FILE_PATH );
        if ( !File::exists( networkFilePath ) )
        {
            printf( "Error: the specified network file (%s) doesn't exist!\n", networkFilePath.ascii() );
            throw MarabouError( MarabouError::FILE_DOESNT_EXIST, networkFilePath.ascii() );
        }

        // For now, assume the network is given in ACAS format
        _acasParser = new AcasParser( networkFilePath );
        _acasParser->generateQuery( _inputQuery );

        /*
          Step 2: extract the property in question
        */
        String propertyFilePath = Options::get()->getString( Options::PROPERTY_FILE_PATH );
        if ( propertyFilePath != "" )
        {
            PropertyParser().parse( propertyFilePath, _inputQuery );
        }
    }
}

void SubregionsMarabou::load_inputs(int reg, int inputSize, float *u, float *l, float tau, float *hs, float *vowns, float *vints, int lenVOWNS, int lenVINTS)
{
    int hInd = reg/lenVOWNS/lenVINTS;
    reg -= hInd*lenVOWNS*lenVINTS;
    int vownInd = reg/lenVINTS;
    int vintInd = reg - vownInd*lenVINTS;
    
    //if (tau<=0.0 ) {
    //    tau = 0.01;
    //if (tau<1.0 ) {
    //    tau = 1.0;
    //}
    float upper[] = {hs[hInd+1],vowns[vownInd+1],vints[vintInd+1],tau};
    float lower[] = {hs[hInd],  vowns[vownInd],  vints[vintInd], tau}; //-1.0};
    memcpy(u, upper, sizeof(float)*inputSize);
    memcpy(l, lower, sizeof(float)*inputSize);
}

void SubregionsMarabou::run()
{
    prepareInputQuery();
    _engine.processInputQuery( _inputQuery );
    _engine.storeState( _initialState, true );
    
    const List<unsigned> inputVariables( _engine.getInputVariables() );
    struct timespec start;
    struct timespec end;
    unsigned long long microSecondsElapsed;
    
    float vints[]  = {-25.000,-24.000,-23.000,-22.000,-21.000,-20.000,-19.000,-18.000,-17.000,-16.000,-15.000,-14.000,-13.000,-12.000,-11.000,-10.000,-9.000,-8.000,-7.000,-6.000,-5.000,-4.000,-3.000,-2.000,-1.000,0.000,1.000,2.000,3.000,4.000,5.000,6.000,7.000,8.000,9.000,10.000,11.000,12.000,13.000,14.000,15.000,16.000,17.000,18.000,19.000,20.000,21.000,22.000,23.000,24.000,25.000};
    float vowns[]  = {-41.667,-41.666,-41.000,-40.000,-39.000,-38.000,-37.000,-36.000,-35.000,-34.000,-33.000,-32.000,-31.000,-30.000,-29.000,-28.000,-27.000,-26.000,-25.001,-24.999,-24.000,-23.000,-22.000,-21.000,-20.000,-19.000,-18.000,-17.000,-16.000,-15.000,-14.000,-13.000,-12.000,-11.000,-10.000,-9.000,-8.000,-7.000,-6.000,-5.000,-4.000,-3.000,-2.000,-1.000,-0.001,0.001,1.000,2.000,3.000,4.000,5.000,6.000,7.000,8.000,9.000,10.000,11.000,12.000,13.000,14.000,15.000,16.000,17.000,18.000,19.000,20.000,21.000,22.000,23.000,24.000,24.999,25.001,26.000,27.000,28.000,29.000,30.000,31.000,32.000,33.000,34.000,35.000,36.000,37.000,38.000,39.000,40.000,41.000,41.666,41.667};
    float hs[] = {-3000.000,-2900.000,-2800.000,-2700.000,-2600.000,-2500.000,-2400.000,-2300.000,-2200.000,-2100.000,-2000.000,-1900.000,-1800.000,-1700.000,-1600.000,-1500.000,-1450.000,-1400.000,-1350.000,-1300.000,-1250.000,-1200.000,-1150.000,-1100.000,-1050.000,-1000.000,-950.000,-900.000,-875.000,-850.000,-825.000,-800.000,-775.000,-750.000,-725.000,-700.000,-690.000,-680.000,-670.000,-660.000,-650.000,-640.000,-630.000,-620.000,-610.000,-600.000,-590.000,-580.000,-570.000,-560.000,-550.000,-540.000,-530.000,-520.000,-510.000,-500.000,-497.000,-494.000,-491.000,-488.000,-485.000,-482.000,-479.000,-476.000,-473.000,-470.000,-467.000,-464.000,-461.000,-458.000,-455.000,-452.000,-449.000,-446.000,-443.000,-440.000,-437.000,-434.000,-431.000,-428.000,-425.000,-422.000,-419.000,-416.000,-413.000,-410.000,-407.000,-404.000,-401.000,-398.000,-395.000,-392.000,-389.000,-386.000,-383.000,-380.000,-377.000,-374.000,-371.000,-368.000,-365.000,-362.000,-359.000,-356.000,-353.000,-350.000,-347.000,-344.000,-341.000,-338.000,-335.000,-332.000,-329.000,-326.000,-323.000,-320.000,-317.000,-314.000,-311.000,-308.000,-305.000,-302.000,-299.000,-296.000,-293.000,-290.000,-287.000,-284.000,-281.000,-278.000,-275.000,-272.000,-269.000,-266.000,-263.000,-260.000,-257.000,-254.000,-251.000,-248.000,-245.000,-242.000,-239.000,-236.000,-233.000,-230.000,-227.000,-224.000,-221.000,-218.000,-215.000,-212.000,-209.000,-206.000,-203.000,-200.000,-198.000,-196.000,-194.000,-192.000,-190.000,-188.000,-186.000,-184.000,-182.000,-180.000,-178.000,-176.000,-174.000,-172.000,-170.000,-168.000,-166.000,-164.000,-162.000,-160.000,-158.000,-156.000,-154.000,-152.000,-150.000,-148.000,-146.000,-144.000,-142.000,-140.000,-138.000,-136.000,-134.000,-132.000,-130.000,-128.000,-126.000,-124.000,-122.000,-120.000,-118.000,-116.000,-114.000,-112.000,-110.000,-108.000,-106.000,-104.000,-102.000,-100.000,-99.000,-98.000,-97.000,-96.000,-95.000,-94.000,-93.000,-92.000,-91.000,-90.000,-89.000,-88.000,-87.000,-86.000,-85.000,-84.000,-83.000,-82.000,-81.000,-80.000,-79.000,-78.000,-77.000,-76.000,-75.000,-74.000,-73.000,-72.000,-71.000,-70.000,-69.000,-68.000,-67.000,-66.000,-65.000,-64.000,-63.000,-62.000,-61.000,-60.000,-59.000,-58.000,-57.000,-56.000,-55.000,-54.000,-53.000,-52.000,-51.000,-50.000,-49.000,-48.000,-47.000,-46.000,-45.000,-44.000,-43.000,-42.000,-41.000,-40.000,-39.000,-38.000,-37.000,-36.000,-35.000,-34.000,-33.000,-32.000,-31.000,-30.000,-29.000,-28.000,-27.000,-26.000,-25.000,-24.000,-23.000,-22.000,-21.000,-20.000,-19.000,-18.000,-17.000,-16.000,-15.000,-14.000,-13.000,-12.000,-11.000,-10.000,-9.000,-8.000,-7.000,-6.000,-5.000,-4.000,-3.000,-2.000,-1.000,0.000,1.000,2.000,3.000,4.000,5.000,6.000,7.000,8.000,9.000,10.000,11.000,12.000,13.000,14.000,15.000,16.000,17.000,18.000,19.000,20.000,21.000,22.000,23.000,24.000,25.000,26.000,27.000,28.000,29.000,30.000,31.000,32.000,33.000,34.000,35.000,36.000,37.000,38.000,39.000,40.000,41.000,42.000,43.000,44.000,45.000,46.000,47.000,48.000,49.000,50.000,51.000,52.000,53.000,54.000,55.000,56.000,57.000,58.000,59.000,60.000,61.000,62.000,63.000,64.000,65.000,66.000,67.000,68.000,69.000,70.000,71.000,72.000,73.000,74.000,75.000,76.000,77.000,78.000,79.000,80.000,81.000,82.000,83.000,84.000,85.000,86.000,87.000,88.000,89.000,90.000,91.000,92.000,93.000,94.000,95.000,96.000,97.000,98.000,99.000,100.000,102.000,104.000,106.000,108.000,110.000,112.000,114.000,116.000,118.000,120.000,122.000,124.000,126.000,128.000,130.000,132.000,134.000,136.000,138.000,140.000,142.000,144.000,146.000,148.000,150.000,152.000,154.000,156.000,158.000,160.000,162.000,164.000,166.000,168.000,170.000,172.000,174.000,176.000,178.000,180.000,182.000,184.000,186.000,188.000,190.000,192.000,194.000,196.000,198.000,200.000,203.000,206.000,209.000,212.000,215.000,218.000,221.000,224.000,227.000,230.000,233.000,236.000,239.000,242.000,245.000,248.000,251.000,254.000,257.000,260.000,263.000,266.000,269.000,272.000,275.000,278.000,281.000,284.000,287.000,290.000,293.000,296.000,299.000,302.000,305.000,308.000,311.000,314.000,317.000,320.000,323.000,326.000,329.000,332.000,335.000,338.000,341.000,344.000,347.000,350.000,353.000,356.000,359.000,362.000,365.000,368.000,371.000,374.000,377.000,380.000,383.000,386.000,389.000,392.000,395.000,398.000,401.000,404.000,407.000,410.000,413.000,416.000,419.000,422.000,425.000,428.000,431.000,434.000,437.000,440.000,443.000,446.000,449.000,452.000,455.000,458.000,461.000,464.000,467.000,470.000,473.000,476.000,479.000,482.000,485.000,488.000,491.000,494.000,497.000,500.000,510.000,520.000,530.000,540.000,550.000,560.000,570.000,580.000,590.000,600.000,610.000,620.000,630.000,640.000,650.000,660.000,670.000,680.000,690.000,700.000,725.000,750.000,775.000,800.000,825.000,850.000,875.000,900.000,950.000,1000.000,1050.000,1100.000,1150.000,1200.000,1250.000,1300.000,1350.000,1400.000,1450.000,1500.000,1600.000,1700.000,1800.000,1900.000,2000.000,2100.000,2200.000,2300.000,2400.000,2500.000,2600.000,2700.000,2800.000,2900.000,3000.000};
    
    
    int lenVINTS = 50;
    int lenVOWNS = 89;
    int lenHS = 610;
    int inputSize = 4;
    float ubs[inputSize], lbs[inputSize];
    int target = 0;
    int ind;
    
    for (float tau=0.0; tau<40.5; tau+=1){
        printf("RA: %d, TAU: %.1f\n",target,tau);
        fflush(stdout); 
        for (int reg=0; reg<lenVINTS*lenVOWNS*lenHS; reg++){ 
    
            start = TimeUtils::sampleMicro();
            _engine.restoreState( _initialState );
            load_inputs(reg, inputSize, ubs, lbs, tau, hs, vowns, vints, lenVOWNS, lenVINTS);
            // Add bound as equations for each input variable
            auto split = new PiecewiseLinearCaseSplit();
            ind = 0;
            for ( const auto &variable : inputVariables )
            {
                split->storeBoundTightening( Tightening( variable, lbs[ind],
                                                         Tightening::LB ) );
                split->storeBoundTightening( Tightening( variable, ubs[ind],
                                                         Tightening::UB ) );
                ind++;
            }
            _engine.applySplit( *split );

            _engine.solve( Options::get()->getInt( Options::TIMEOUT ) );

            if ( _engine.getExitCode() == Engine::SAT )
            {
                printf("%d,\n",reg);
                fflush(stdout); 
            }
            //else if ( _engine.getExitCode() == Engine::UNSAT )
            //    printf("UNSAT\n");
    
        }
        end = TimeUtils::sampleMicro();
        microSecondsElapsed = TimeUtils::timePassed( start, end );
        printf( "time: %llu\n", microSecondsElapsed / 1000000 ); // In seconds
    }
}

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
