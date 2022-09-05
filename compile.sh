# '-sUSE_PTHREADS', '-sPTHREAD_POOL_SIZE', '-sOFFSCREENCANVAS_SUPPORT', '-lGL', '-sOFFSCREEN_FRAMEBUFFER'

emcc \
./src/gl_in_pthread.cpp \
-g \
-o ./wasm/glLib.js \
-s USE_PTHREADS=1 \
-s PTHREAD_POOL_SIZE=4 \
-s OFFSCREENCANVAS_SUPPORT=1 \
-s OFFSCREEN_FRAMEBUFFER=1 \