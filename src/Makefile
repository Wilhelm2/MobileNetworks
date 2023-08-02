#
# OMNeT++/OMNEST Makefile for MNwithStationFailures
#
# This file was generated with the command:
#  opp_makemake -f --deep -KINET_PROJ=/home/wilhelm/Documents/omnetpp-5.5.1/samples/inet -DINET_IMPORT -I$$\(INET_PROJ\)/src -L$$\(INET_PROJ\)/src -lINET$$\(D\)
#

# Name of target to be created (-o option)
TARGET = MNwithStationFailures$(D)$(EXE_SUFFIX)
TARGET_DIR = .

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I$(INET_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)$(INET_PROJ)/src  -lINET$(D)

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/BroadcastController.o \
    $O/MessageFactory.o \
    $O/simulationParameters.o \
    $O/Stats.o \
    $O/MobileHosts/HostCommunications.o \
    $O/MobileHosts/LinearMobilityInsideCells.o \
    $O/MobileHosts/MH.o \
    $O/MobileHosts/MHRBuffer.o \
    $O/MobileHosts/MHSBuffer.o \
    $O/MobileHosts/MHState.o \
    $O/MobileSupportStations/Communications.o \
    $O/MobileSupportStations/MobileHostsMap.o \
    $O/MobileSupportStations/MSS.o \
    $O/MobileSupportStations/MSSSBuffer.o \
    $O/MobileSupportStations/OpenSocketMap.o \
    $O/MobileSupportStations/Session.o \
    $O/MobileSupportStations/StationAppCoSendingBuffer.o \
    $O/MobileSupportStations/StationGroup.o \
    $O/MobileSupportStations/StationReceiveBuffer.o \
    $O/MobileSupportStations/StationSendingBuffer.o \
    $O/Messages/Ack_h_m.o \
    $O/Messages/Ack_S_m.o \
    $O/Messages/AckSS_m.o \
    $O/Messages/AlgoTimeout_m.o \
    $O/Messages/App_resp_m.o \
    $O/Messages/AppCo_m.o \
    $O/Messages/AppCoAck_m.o \
    $O/Messages/Apph_S_m.o \
    $O/Messages/AppS_h_m.o \
    $O/Messages/AppS_S_m.o \
    $O/Messages/Delete_m.o \
    $O/Messages/FIFOCom_m.o \
    $O/Messages/HostMessage_m.o \
    $O/Messages/Init_m.o \
    $O/Messages/InitAck_m.o \
    $O/Messages/RecoverMSS_m.o \
    $O/Messages/RecoverMSSReq_m.o \
    $O/Messages/Register_m.o \
    $O/Messages/Req1_m.o \
    $O/Messages/Req2_m.o \
    $O/Messages/Rsp1_m.o \
    $O/Messages/Rsp2_m.o \
    $O/Messages/StationWirelessMessage_m.o \
    $O/Messages/timeout_m.o \
    $O/Messages/TimeoutAppCo_m.o \
    $O/Messages/TimeoutApph_S_m.o \
    $O/Messages/TimeoutAppS_h_m.o \
    $O/Messages/timeoutHandoff_m.o

# Message files
MSGFILES = \
    Messages/Ack_h.msg \
    Messages/Ack_S.msg \
    Messages/AckSS.msg \
    Messages/AlgoTimeout.msg \
    Messages/App_resp.msg \
    Messages/AppCo.msg \
    Messages/AppCoAck.msg \
    Messages/Apph_S.msg \
    Messages/AppS_h.msg \
    Messages/AppS_S.msg \
    Messages/Delete.msg \
    Messages/FIFOCom.msg \
    Messages/HostMessage.msg \
    Messages/Init.msg \
    Messages/InitAck.msg \
    Messages/RecoverMSS.msg \
    Messages/RecoverMSSReq.msg \
    Messages/Register.msg \
    Messages/Req1.msg \
    Messages/Req2.msg \
    Messages/Rsp1.msg \
    Messages/Rsp2.msg \
    Messages/StationWirelessMessage.msg \
    Messages/timeout.msg \
    Messages/TimeoutAppCo.msg \
    Messages/TimeoutApph_S.msg \
    Messages/TimeoutAppS_h.msg \
    Messages/timeoutHandoff.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET_PROJ=/home/wilhelm/Documents/omnetpp-5.5.1/samples/inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(TOOLCHAIN_NAME),clangc2)
LIBS += -Wl,-rpath,$(abspath $(INET_PROJ)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
MSGC:=$(MSGC) --msg6

# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)