# Configuration file for single particle generation
#
# $Id: evdb.py,v 1.1.1.1 2010-12-22 16:18:52 p-nusoftart Exp $
#
# Original author B. Rebel
#
# Spacing is not signficant in this file.

# Define the default configuration for the framework.
import FWCore.ParameterSet.python.Config as evd

# Give this job a name.  
process = evd.Process("Event Display")

# Maximum number of events to do.
process.maxEvents = evd.untracked.PSet(
    input = evd.untracked.int32(9999)
)

# Load the standard message logger configuration.
# Threshold=Info. Limit of 5 per category; then exponential backoff.
#process.load("Config/MessageLogger_cfi")

# Load the service that manages root files for histograms.
process.TFileService = evd.Service(
    "TFileService",
    fileName = evd.string("output_hist.root"),
    closeFileFast = evd.untracked.bool(False)
)

# Define the geometry.
process.Geometry = evd.Service(
    "Geometry",
    GDML     = evd.FileInPath("Geometry/gdml/ndos.gdml"),
    BigBoxUsed  = evd.bool(True),
    BigBoxRange = evd.double(1500)
)

# Configure the event display service
process.EventDisplay = evd.Service(
    "EventDisplay",
    AutoAdvanceInterval=evd.uint32(1000)
)

# Define the service for passing simulation parameters
process.RandomHandler = evd.Service(
    "RandomHandler"
)

# Define and configure some modules to do work on each event.
# Modules are just defined for now, the are scheduled later.

#
# A sample module configuration. v = vector.
#
# Make some particles to go through
process.singlegen = evd.EDProducer(
    "SingleGen",
    PDG          =evd.vint32(13),
    P0           =evd.vdouble(6.),
    SigmaP       =evd.vdouble(0.),
    PDist        =evd.vint32(1),
    X0           =evd.vdouble(0.),
    Y0           =evd.vdouble(0.),
    Z0           =evd.vdouble(20.),
    T0           =evd.vdouble(0.),
    SigmaX       =evd.vdouble(0.),
    SigmaY       =evd.vdouble(0.),
    SigmaZ       =evd.vdouble(0.0),
    SigmaT       =evd.vdouble(0.0),
    PosDist      =evd.vint32(0),
    Theta0XZ     =evd.vdouble(0.),   #angle in XZ plane (degrees)
    Theta0YZ     =evd.vdouble(-3.3), #angle in YZ plane (degrees)
    SigmaThetaXZ =evd.vdouble(0.),   #in degrees
    SigmaThetaYZ =evd.vdouble(0.),   #in degrees
    AngleDist    =evd.vint32(1)
)

# Start each new event with an empty event.
process.source = evd.Source("PoolSource",
                            fileNames=evd.untracked.vstring('single_gen.root')
                            )

# Write the events to the output file.
#process.output = evd.OutputModule(
#    "PoolOutputModule",
#    fileName = evd.untracked.string('file:single_gen.root'),
#)

####### End of the section that defines and configures modules.#########

# Tell the system to execute all paths.
process.doit = evd.EndPath( process.singlegen )

