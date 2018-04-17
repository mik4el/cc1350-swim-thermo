## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/pinInterrupt_pem3.oem3

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/pinInterrupt_pem3.xdl
	$(SED) 's"^\"\(package/cfg/pinInterrupt_pem3cfg.cmd\)\"$""\"/Users/mikael/Code/cc1350-swim-thermo/software/pinInterrupt_CC1350_SWIMTHERMO/Debug/configPkg/\1\""' package/cfg/pinInterrupt_pem3.xdl > $@
	-$(SETDATE) -r:max package/cfg/pinInterrupt_pem3.h compiler.opt compiler.opt.defs
