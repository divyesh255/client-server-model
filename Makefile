# -s Option is only for not display enter into directory and leaving from directory.
# -C tells to make command directory should be change before processing the Makefile.
# -f tells to 'make' for use the 'Mymakefile' instead of default 'Makefile'.

#Run Makefile from the individual directory.
all:
	@make -s -C client-server/build -f Makefile
	
#clean generated files from all directory.
clean:
	@rm -rf client-server/bin
