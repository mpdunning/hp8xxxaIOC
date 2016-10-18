TOP=../..

include $(TOP)/configure/CONFIG

PROD_IOC = hp8xxxa

DBD += hp8xxxa.dbd

hp8xxxa_DBD += base.dbd
hp8xxxa_DBD += stream.dbd
hp8xxxa_DBD += asyn.dbd
hp8xxxa_DBD += asSupport.dbd
hp8xxxa_DBD += hppmSup.dbd
#hp8xxxa_DBD += calcSupport.dbd
#hp8xxxa_DBD += drvAsynIPPort.dbd
hp8xxxa_DBD += drvVxi11.dbd

PROD_SRCS += hp8xxxa_registerRecordDeviceDriver.cpp hppmASub.c
PROD_SRCS_DEFAULT += hp8xxxaMain.cpp

PROD_LIBS += asyn
PROD_LIBS += autosave
PROD_LIBS += stream
#PROD_LIBS += calc

PROD_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES

