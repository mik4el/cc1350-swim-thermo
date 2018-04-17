# invoke SourceDir generated makefile for pinInterrupt.pem3
pinInterrupt.pem3: .libraries,pinInterrupt.pem3
.libraries,pinInterrupt.pem3: package/cfg/pinInterrupt_pem3.xdl
	$(MAKE) -f /Users/mikael/Code/cc1350-swim-thermo/software/pinInterrupt_CC1350_SWIMTHERMO/src/makefile.libs

clean::
	$(MAKE) -f /Users/mikael/Code/cc1350-swim-thermo/software/pinInterrupt_CC1350_SWIMTHERMO/src/makefile.libs clean

