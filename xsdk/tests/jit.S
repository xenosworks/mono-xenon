/* More of the title's .jit pool, beside the 4 MB Mono brings (mono/utils/mono-xsdk.c): JIT_POOL_BYTES of it. */

.section .jit,"awx",@nobits
.balign 0x10000
.skip JIT_POOL_BYTES
