all: interface
X2_DBG=1

interface: Iface_Process.o gm_init_if.o gm_update.o gm_pc_if.o  message_modules.o message_transport.o openair_inet_sockets.o openair_io_sync.o
	gcc Iface_Process.o gm_init_if.o gm_update.o gm_pc_if.o  message_modules.o message_transport.o openair_inet_sockets.o openair_io_sync.o -o interface -lpthread -D X2_DBG=1

clean:
	rm -rf *o interface


