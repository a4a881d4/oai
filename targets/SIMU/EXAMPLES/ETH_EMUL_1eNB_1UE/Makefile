all: oaisim nasmesh

oaisim: 
	(cd $(OPENAIR_TARGETS)/SIMU/USER && make oaisim NAS_NETLINK=1)
nasmesh:
	(cd $(OPENAIR2_DIR) && make nasmesh_netlink.ko)
	(cd $(OPENAIR2_DIR)/NAS/DRIVER/MESH/RB_TOOL/ && make)

one_eNB_one_UE: oaisim nasmesh

userclean: clean oaisim nasmesh	

clean: 
	(cd $(OPENAIR_TARGETS)/SIMU/USER && make clean)
