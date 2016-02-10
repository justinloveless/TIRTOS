# invoke SourceDir generated makefile for multi_main.pem4f
multi_main.pem4f: .libraries,multi_main.pem4f
.libraries,multi_main.pem4f: package/cfg/multi_main_pem4f.xdl
	$(MAKE) -f C:\Users\justin\workspace_v6_1\multifile2/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\justin\workspace_v6_1\multifile2/src/makefile.libs clean

