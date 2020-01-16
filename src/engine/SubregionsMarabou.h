/*********************                                                        */
/*! \file SubregionsMarabou.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Kyle Julian
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]

 **/

#ifndef __SubregionsMarabou_h__
#define __SubregionsMarabou_h__

#include "AcasParser.h"
#include "Engine.h"
#include "InputQuery.h"

class SubregionsMarabou
{
public:
    SubregionsMarabou( unsigned verbosity );
    ~SubregionsMarabou();

    /*
      Entry point of this class
    */
    void run();

private:
    InputQuery _inputQuery;

    /*
      Extract the input files: network and property, and use them
      to generate the input query
    */
    void prepareInputQuery();

    /*
      Invoke the engine to solve the input query
    */
    void solveQuery();
    
    /* load the input bounds */
    void load_inputs(int PROPERTY, int inputSize, float *u, float *l, float tau, float *hs, float *vowns, float *vints, int lenVOWNS, int lenVINTS);

    /*
      ACAS network parser
    */
    AcasParser *_acasParser;

    /*
      The solver
    */
    Engine _engine;
    
    /*
      The state of the engine after pre-processing
    */
    EngineState _initialState;
};

#endif // __Marabou_h__

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//