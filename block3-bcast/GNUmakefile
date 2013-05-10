RSYNC = rsync
SSH = ssh
SCP = scp

SFLAGS = -F "../.ssh-cfg"
RFLAGS = --recursive --progress --copy-links --exclude=build/ -e "$(SSH) $(SFLAGS)"

HOST = jupiter
REMOTEDIR = ~/block3-bcast
SOURCE = .

CSV_FILE = data/bench.csv
PROCESS_COUNTS = 20 80 160 320 500
ITERATION_COUNTS = 10 100 1000
N=1000
SEED=0
HOSTS=jupiter0,jupiter1,jupiter2,jupiter3,jupiter4,jupiter5,jupiter6,jupiter7,jupiter8,jupiter9,jupiter10,jupiter11,jupiter12,jupiter13,jupiter14,jupiter15,jupiter16,jupiter17,jupiter18,jupiter19,jupiter20,jupiter21,jupiter22,jupiter23,jupiter24,jupiter25,jupiter26,jupiter27,jupiter28,jupiter29,jupiter30,jupiter31,jupiter32,jupiter33,jupiter34,jupiter35

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE="Debug" ..; make -j6

check: bake
	cd build; CK_FORK=no ctest --output-on-failure

bench: bake
	mkdir -p data
	for ITERS in $(ITERATION_COUNTS); do \
		for P in $(PROCESS_COUNTS); do \
			mpiexec -host $(HOSTS) \
				-loadbalance \
				-np $${P} \
				build/src/bcast_bench -almo -s $(SEED) -n $(N) > $(CSV_FILE); \
		done \
	done

hand-in:
	$(RSYNC) $(RFLAGS) $(SOURCE) $(HOST):$(REMOTEDIR)

remote-bench: hand-in
	$(SSH) $(SFLAGS) $(HOST) 'cd $(REMOTEDIR); \
		export PATH=/opt/cmake/bin/:/opt/openmpi/bin/:${PATH}; \
		MPI_HOME=/opt/openmpi make clean bench'
	mkdir -p data
	$(SCP) $(SFLAGS) $(HOST):$(REMOTEDIR)/$(CSV_FILE) $(CSV_FILE)

clean:
	rm -rf build
