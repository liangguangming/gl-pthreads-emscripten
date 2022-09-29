#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <GLES2/gl2.h>
#include <math.h>
#include <assert.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/threading.h>
#include <bits/errno.h>
#include <stdlib.h>

void *ThreadMain(void *arg)
{
  printf("ThreadMain\n");
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.explicitSwapControl = EM_TRUE;
  attr.alpha = 0;
  #if MAX_WEBGL_VERSION >= 2
    attr.majorVersion = 2;
  #endif
  attr.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_FALLBACK;
  attr.renderViaOffscreenBackBuffer = EM_TRUE;
  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx;
  ctx = emscripten_webgl_create_context("#canvas", &attr);
  emscripten_webgl_make_context_current(ctx);

  double color = 0;
  for(int i = 0; i < 100; ++i)
  {
    color += 0.01;
    glClearColor(color, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    EMSCRIPTEN_RESULT r = emscripten_webgl_commit_frame();
    assert(r == EMSCRIPTEN_RESULT_SUCCESS);

    double now = emscripten_get_now();
    while(emscripten_get_now() - now < 16) /*no-op*/;
  }

  emscripten_webgl_make_context_current(0);
  emscripten_webgl_destroy_context(ctx);
  printf("Thread quit\n");
  pthread_exit(0);
}

pthread_t CreateThread()
{
  pthread_t thread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  // emscripten_pthread_attr_settransferredcanvases(&attr, "#canvas");
  int rc = pthread_create(&thread, &attr, ThreadMain, 0);
  if (rc == ENOSYS)
  {
    printf("Test Skipped! OffscreenCanvas is not supported!\n");
#ifdef REPORT_RESULT
    REPORT_RESULT(1);
#endif
    exit(0);
  }  
  if (rc)
  {
    printf("Failed to create thread! error: %d\n", rc);
    exit(0);
  }
  pthread_attr_destroy(&attr);

  return thread;
}

void *mymain(void*)
{
  pthread_t thread = CreateThread();
  pthread_detach(thread);
  return 0;
}

int main()
{
  mymain(0);
  emscripten_exit_with_live_runtime();
  return 0;
}
