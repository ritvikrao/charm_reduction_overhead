CHARMC =/ccs/home/rrao/charm/ofi-linux-x86_64-cxi-slurmpmi2cray-gcc/bin/charmc $(OPTS)
CHARMC_SMP =/ccs/home/rrao/charm/ofi-linux-x86_64-cxi-slurmpmi2cray-smp-gcc/bin/charmc $(OPTS)

BINARY=reductiontest

reductiontest: reduction.cpp reduction.ci
	$(CHARMC_SMP) reduction.ci
	$(CHARMC_SMP) $(CHARMCFLAGS) $< -o $@

clean:
	rm -f *.o *.decl.h *.def.h $(BINARY) charmrun* *.stamp *.out

