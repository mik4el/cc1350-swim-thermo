# invoke SourceDir generated makefile for adcsinglechannel.pem3
adcsinglechannel.pem3: .libraries,adcsinglechannel.pem3
.libraries,adcsinglechannel.pem3: package/cfg/adcsinglechannel_pem3.xdl
	$(MAKE) -f /Users/mikael/Code/cc1350-swim-thermo/software/adcsinglechannel_CC1350_SWIMTHERMO_TI/src/makefile.libs

clean::
	$(MAKE) -f /Users/mikael/Code/cc1350-swim-thermo/software/adcsinglechannel_CC1350_SWIMTHERMO_TI/src/makefile.libs clean

