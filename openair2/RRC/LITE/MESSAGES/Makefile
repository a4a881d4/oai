OPENAIR2_TOP = ../../..
include $(OPENAIR2_DIR)/RRC/LITE/MESSAGES/Makefile.inc

EXTRA_CFLAGS += -I$(OPENAIR1_DIR) -I/usr/realtime/include -D__need_Emath -D__NO_ASSERT_H__ 

ifdef RTAI
# Get the RTAI variables
CCC = $(shell rtai-config --cc)
RTAI_SUBVERSION=$(shell rtai-config --version | sed -e 's/^..\(.\).*$$/\1/')
IS_RTAI_SUBVERSION_LESS_THAN_FIVE=$(shell if [ $(RTAI_SUBVERSION) -lt 5 ] ; then echo true ; fi)
SUBVERSION=$(shell echo `grep '^SUBLEVEL =' /usr/src/linux/Makefile | sed -e 's, ,,g' | sed -e 's/SUBLEVEL=//'`)
IS_KERNEL_SUBVERSION_GREATER_THAN_20=$(shell if [ $(SUBVERSION) -ge 20 ] ; then echo true ; fi)


EXTRA_CFLAGS += -D__IN_RTAI__ $(shell rtai-config --module-cflags) -g -march=pentium4 -DNODE_RG -DBIGPHYSAREA -DRTAI -DRTAI_ENABLED $(if $(IS_KERNEL_SUBVERSION_GREATER_THAN_20),-mregparm=3 -fno-stack-protector,) -DHW_PREFIX_REMOVAL -Wall -D__KERNEL__ -DMODULE -D_LOOSE_KERNEL_NAMES -DWIDENS_DLC -I/lib/modules/$(shell uname -r)/build/include -I/lib/modules/$(shell uname -r)/build/include/asm/mach-default -include /lib/modules/$(shell uname -r)/build/include/linux/autoconf.h $(if $(IS_RTAI_SUBVERSION_LESS_THAN_FIVE),-DRTAI_ISNT_POSIX,)
endif


obj-m = asn1_msg_kern.o
asn1_msg_kern-objs = $(ASN1_CONVERTER_OBJS) $(ASN1_MODULE_OBJS) bsearch.o rtai_mem.o

clean:
	rm -rf $(asn1_msg_kern-objs) $(obj-m)
	rm -f Makefile.inc.generated

test:
	echo $(asn1_msg_kern-objs) $(obj-m)

regen: regenerate-from-asn1-source

regenerate-from-asn1-source:
	asn1c -gen-PER -fcompound-names -fnative-types -fskeletons-copy asn1c/ASN1_files/EUTRA-RRC-Definitions.asn
