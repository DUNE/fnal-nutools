////////////////////////////////////////////////////////////////////////
/// \file  RootEnv.h
/// \brief Setup the root environment
///
/// \version $Id: RootEnv.h,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
/// \author  messier@indiana.edu
////////////////////////////////////////////////////////////////////////
#ifndef EVDB_ROOTENV_H
#define EVDB_ROOTENV_H
class TRint;

namespace evdb {
  /// Configure the ROOT environment
  class RootEnv {
  public:
    RootEnv(int argc, char** argv);
    ~RootEnv();

    int Run();
    
  private:
    void InterpreterConfig();
    void SignalConfig();
    void LoadIncludes();
    void LoadClasses();
    
  private:
  };
}
#endif // EVDROOTENV_H
////////////////////////////////////////////////////////////////////////
