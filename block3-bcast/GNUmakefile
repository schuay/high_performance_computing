# The bench targets assume a NECmpi installation with all quirks of the
# jupiter system. The remote .bashrc extends PATH with the NECmpi bin64
# directory.

RSYNC = rsync
SSH = ssh
SCP = scp

SFLAGS = -F "../.ssh-cfg"
RFLAGS = --recursive --progress --copy-links --exclude=build/ -e "$(SSH) $(SFLAGS)"

HOST = jupiter
REMOTEDIR = ~/block3-bcast
SOURCE = .

BENCH_DIR = results
BENCH_FILE = $(BENCH_DIR)/bench.csv
PROCESS_COUNTS = 20 80 160 320 500 576
NS = 1000000 1048576 4000000 4194304 5000000 16777216 40000000 67108864 111111111 268435456
SEED=0
HOSTS=jupiter0,jupiter1,jupiter2,jupiter3,jupiter4,jupiter5,jupiter6,jupiter7,jupiter8,jupiter9,jupiter10,jupiter11,jupiter12,jupiter13,jupiter14,jupiter15,jupiter16,jupiter17,jupiter18,jupiter19,jupiter20,jupiter21,jupiter22,jupiter23,jupiter24,jupiter25,jupiter26,jupiter27,jupiter28,jupiter29,jupiter30,jupiter31,jupiter32,jupiter33,jupiter34,jupiter35

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE="Debug" ..; make -j6

check: bake
	cd build; CK_FORK=no ctest --output-on-failure

bench: bake
	mkdir -p $(BENCH_DIR)
	for N in $(NS); do \
		for P in $(PROCESS_COUNTS); do \
			echo N: $${N} P: $${P}...; \
			MPILX_NODELIST="$(HOSTS)" mpiexec \
				-node 0-35 \
				-n $${P} \
				build/src/bcast_bench -almo -s $(SEED) -n $${N} >> $(BENCH_FILE); \
			if [ "x$$?" != "x0" ]; then \
				exit 1; \
			fi \
		done \
	done

hand-in:
	$(RSYNC) $(RFLAGS) $(SOURCE) $(HOST):$(REMOTEDIR)

remote-bench: hand-in
	$(SSH) $(SFLAGS) $(HOST) 'cd $(REMOTEDIR); make bench'
	mkdir -p $(BENCH_DIR)
	$(SCP) $(SFLAGS) $(HOST):$(REMOTEDIR)/$(BENCH_FILE) $(BENCH_FILE)

clean:
	rm -rf build
