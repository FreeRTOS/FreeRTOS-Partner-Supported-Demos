include imports.mak

all: libs
	$(MAKE) -C examples/test_demo/

libs:
	$(MAKE) -C kernel -f makefile.am64x.a53.freertos-smp
	$(MAKE) -C drivers -f makefile.am64x.a53.gcc-aarch64

clean: libs-clean
	$(MAKE) -C examples/test_demo/ clean

libs-clean:
	$(MAKE) -C kernel -f makefile.am64x.a53.freertos-smp clean
	$(MAKE) -C drivers -f makefile.am64x.a53.gcc-aarch64 clean

scrub: libs-scrub
	$(MAKE) -C examples/test_demo/ scrub

libs-scrub:
	$(MAKE) -C kernel -f makefile.am64x.a53.freertos-smp scrub
	$(MAKE) -C drivers -f makefile.am64x.a53.gcc-aarch64 scrub
