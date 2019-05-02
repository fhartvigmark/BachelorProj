aSNAP = snap-core
SNAP = Snap-4.1/$(aSNAP)
SRC = src
TEST = test


all: $(SNAP)/Snap.o src test ##Compile all
clean: clean-src clean-test
clean-all: clean-snap clean
run-test:
	$(MAKE) run -C $(TEST)
run-src:
	$(MAKE) run -C $(SRC)

$(SNAP)/Snap.o: 
	make -C $(SNAP)

src:
	make -C $(SRC)

test:
	make -C $(TEST)

clean-snap:
	$(MAKE) clean -C $(SNAP)

clean-src:
	$(MAKE) clean -C $(SRC)

clean-test:
	$(MAKE) clean -C $(TEST)


.PHONY: all clean clean-all clean-src clean-test src test run-test run-src help
