#!../../bin/linux-x86_64/hp8xxxa

< envPaths

cd ${TOP}

dbLoadDatabase "dbd/hp8xxxa.dbd"
hp8xxxa_registerRecordDeviceDriver pdbbase

epicsEnvSet("STREAM_PROTOCOL_PATH","../../hp8xxxaApp/Db:.")
epicsEnvSet("PROTOFILE","hp8xxxa.proto")
epicsEnvSet("LOC",    "SNS")
epicsEnvSet("GPIB",   "7")
epicsEnvSet("PORT",   "L0")
epicsEnvSet("IP",     "esb-gpib-05")
epicsEnvSet("P",      "SNS:HPPM0")
epicsEnvSet("DELAY1", "0.1")
epicsEnvSet("DELAY2", "0.1")
epicsEnvSet("SCAN1",  "2 second")
epicsEnvSet("SCAN2",  "10 second")

save_restoreSet_status_prefix( "")
save_restoreSet_IncompleteSetsOk( 1)
save_restoreSet_DatedBackupFiles( 1)
set_savefile_path( "/nfs/slac/g/testfac/esb/$(IOC)","autosave")
set_pass0_restoreFile( "hp8xxxa.sav")
set_pass1_restoreFile( "hp8xxxa.sav")

#vxi11Configure("$(PORT)","$(IP)",0,0.0,"gpib0")
vxi11Configure("$(PORT)","$(IP)",0,0.0,"hpib")

#asynSetTraceMask("$(PORT)","$(GPIB)",0x9)
#asynSetTraceIOMask("$(PORT)","$(GPIB)",0x2)

dbLoadRecords("db/hp8xxxa.db","IOC=${IOC},LOC=$(LOC),P=$(P),PROTOFILE=$(PROTOFILE),PORT=$(PORT),DELAY1=$(DELAY1),DELAY2=$(DELAY2),SCAN1=$(SCAN1),SCAN2=$(SCAN2),L=$(PORT),A=$(GPIB)")
dbLoadRecords("db/hp8xxxaExtras.db","P=SNS:LBAND")
dbLoadRecords("db/asynRecord.db","P=$(P):,R=asyn,PORT=$(PORT),ADDR=$(GPIB),OMAX=0,IMAX=0")

cd ${AUTOSAVE}
dbLoadRecords( "db/save_restoreStatus.db","P=$(P)")

cd ${TOP}/iocBoot/${IOC}
iocInit()

create_monitor_set( "hp8xxxa.req",30,"P=$(P)")

