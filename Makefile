aSNAP = snap-core
SNAP = Snap-4.1/$(aSNAP)
SRC = src
TEST = test
TOOLS = tools


all: $(SNAP)/Snap.o src test tools ##Compile all
clean: clean-src clean-test 
clean-all: clean-snap clean
run-test:
	$(MAKE) run -C $(TEST)
run-test-repeat:
	$(MAKE) test-repeat -C $(TEST)
run-src:
	$(MAKE) run -C $(SRC)

$(SNAP)/Snap.o: 
	make -C $(SNAP)

src:
	make -C $(SRC)

test:
	make -C $(TEST)

tools:
	make -C $(TOOLS)

clean-snap:
	$(MAKE) clean -C $(SNAP)

clean-src:
	$(MAKE) clean -C $(SRC)

clean-test:
	$(MAKE) clean -C $(TEST)

clean-tools:
	$(MAKE) clean -C $(TOOLS)


.PHONY: all clean clean-all clean-src clean-test clean-tools src test tools run-test run-src run-test-repeat help
