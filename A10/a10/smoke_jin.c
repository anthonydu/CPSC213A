#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf(S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void)0)  // do nothing
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t match;
  uthread_cond_t paper;
  uthread_cond_t tobacco;
  uthread_cond_t smoke;
  uthread_cond_t thread_ready_cond;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc(sizeof(struct Agent));
  agent->mutex = uthread_mutex_create();
  agent->paper = uthread_cond_create(agent->mutex);
  agent->match = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke = uthread_cond_create(agent->mutex);
  agent->thread_ready_cond = uthread_cond_create(agent->mutex);
  return agent;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

// **
//  * You might find these declarations helpful.
//  *   Note that Resource enum had values 1, 2 and 4 so you can combine
//  resources;
//  *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
//  */
enum Resource { MATCH = 1, PAPER = 2, TOBACCO = 4 };
char* resource_name[] = {"", "match", "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count[5];  // # of times resource signalled
int smoke_count[5];   // # of times smoker with resource smoked

int current_material_combo = 0;

typedef struct {
  int material_id;
  uthread_cond_t material_cond;
  uthread_cond_t broadcast_cond;
  uthread_cond_t thread_ready_cond;
  uthread_mutex_t agent_mutex;
  uthread_mutex_t smoker_mutex;
} HandleResourceThreadMsg;

HandleResourceThreadMsg* createHandleResourceThreadMsg(
    int material_id,
    uthread_cond_t material_cond,
    uthread_cond_t broadcast_cond,
    uthread_cond_t thread_ready_cond,
    uthread_mutex_t agent_mutex,
    uthread_mutex_t smoker_mutex) {
  HandleResourceThreadMsg* tm = malloc(sizeof(HandleResourceThreadMsg));
  tm->material_id = material_id;
  tm->material_cond = material_cond;
  tm->broadcast_cond = broadcast_cond;
  tm->thread_ready_cond = thread_ready_cond;
  tm->agent_mutex = agent_mutex;
  tm->smoker_mutex = smoker_mutex;
  return tm;
}

typedef struct {
  int material_id;
  int corresponding_current_material_combo;
  uthread_cond_t smoke_cond;
  uthread_cond_t broadcast_cond;
  uthread_cond_t thread_ready_cond;
  uthread_mutex_t agent_mutex;
  uthread_mutex_t smoker_mutex;
} SmokerThreadMsg;

SmokerThreadMsg* createSmokerThreadMsg(int material_id,
                                       int corresponding_current_material_combo,
                                       uthread_cond_t smoke_cond,
                                       uthread_cond_t broadcast_cond,
                                       uthread_cond_t thread_ready_cond,
                                       uthread_mutex_t agent_mutex,
                                       uthread_mutex_t smoker_mutex) {
  SmokerThreadMsg* tm = malloc(sizeof(SmokerThreadMsg));
  tm->material_id = material_id;
  tm->corresponding_current_material_combo
      = corresponding_current_material_combo;
  tm->smoke_cond = smoke_cond;
  tm->broadcast_cond = broadcast_cond;
  tm->thread_ready_cond = thread_ready_cond;
  tm->agent_mutex = agent_mutex;
  tm->smoker_mutex = smoker_mutex;
  return tm;
}

void* handle_resource(void* threadmsgv) {
  HandleResourceThreadMsg* tm = threadmsgv;
  while (1) {
    uthread_mutex_lock(tm->agent_mutex);
    VERBOSE_PRINT("the following handler is waiting: %s\n",
                  resource_name[tm->material_id]);
    num_active_threads++;
    uthread_cond_signal(tm->thread_ready_cond);
    uthread_cond_wait(tm->material_cond);
    current_material_combo = current_material_combo | tm->material_id;
    VERBOSE_PRINT("currently in the following handler: %s\n",
                  resource_name[tm->material_id]);
    VERBOSE_PRINT("current material combo: %d\n", current_material_combo);

    if (current_material_combo == 3 || current_material_combo > 4) {
      uthread_mutex_unlock(tm->agent_mutex);
      VERBOSE_PRINT("signalling smokers...\n");
      uthread_mutex_lock(tm->smoker_mutex);
      uthread_cond_broadcast(tm->broadcast_cond);
      num_active_threads--;
      uthread_mutex_unlock(tm->smoker_mutex);
    } else {
      VERBOSE_PRINT("no smoker can smoke yet...\n");
      num_active_threads--;
      uthread_mutex_unlock(tm->agent_mutex);
    }
  }
}

void* handle_smoke(void* threadmsgv) {
  SmokerThreadMsg* tm = threadmsgv;

  uthread_mutex_lock(tm->agent_mutex);
  num_active_threads++;
  uthread_cond_signal(tm->thread_ready_cond);
  uthread_mutex_unlock(tm->agent_mutex);

  while (1) {
    uthread_mutex_lock(tm->smoker_mutex);
    while (current_material_combo != tm->corresponding_current_material_combo) {
      uthread_cond_wait(tm->broadcast_cond);
    }
    uthread_mutex_unlock(tm->smoker_mutex);
    uthread_mutex_lock(tm->agent_mutex);
    current_material_combo = 0;
    VERBOSE_PRINT("%s smoker smoked!\n", resource_name[tm->material_id]);
    uthread_cond_signal(tm->smoke_cond);
    smoke_count[tm->material_id]++;
    uthread_mutex_unlock(tm->agent_mutex);
  }
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it
 * does is choose 2 random resources, signal their condition variables, and then
 * wait wait for a smoker to smoke.
 */
void* agent(void* av) {
  struct Agent* a = av;
  static const int choices[]
      = {MATCH | PAPER, MATCH | TOBACCO, PAPER | TOBACCO};
  static const int matching_smoker[] = {TOBACCO, PAPER, MATCH};

  srandom(time(NULL));

  uthread_mutex_lock(a->mutex);
  // Wait until all other threads are waiting for a signal
  VERBOSE_PRINT("active threads: %d\n", num_active_threads);
  while (num_active_threads < 6) uthread_cond_wait(a->thread_ready_cond);
  VERBOSE_PRINT("All threads set up!\n");

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    VERBOSE_PRINT("active threads: %d\n", num_active_threads);

    while (num_active_threads < 6) {
      uthread_cond_wait(a->thread_ready_cond);
    }

    int r = random() % 6;
    switch (r) {
      case 0:
        signal_count[TOBACCO]++;
        VERBOSE_PRINT("match available\n");
        uthread_cond_signal(a->match);
        VERBOSE_PRINT("paper available\n");
        uthread_cond_signal(a->paper);
        break;
      case 1:
        signal_count[PAPER]++;
        VERBOSE_PRINT("match available\n");
        uthread_cond_signal(a->match);
        VERBOSE_PRINT("tobacco available\n");
        uthread_cond_signal(a->tobacco);
        break;
      case 2:
        signal_count[MATCH]++;
        VERBOSE_PRINT("paper available\n");
        uthread_cond_signal(a->paper);
        VERBOSE_PRINT("tobacco available\n");
        uthread_cond_signal(a->tobacco);
        break;
      case 3:
        signal_count[TOBACCO]++;
        VERBOSE_PRINT("paper available\n");
        uthread_cond_signal(a->paper);
        VERBOSE_PRINT("match available\n");
        uthread_cond_signal(a->match);
        break;
      case 4:
        signal_count[PAPER]++;
        VERBOSE_PRINT("tobacco available\n");
        uthread_cond_signal(a->tobacco);
        VERBOSE_PRINT("match available\n");
        uthread_cond_signal(a->match);
        break;
      case 5:
        signal_count[MATCH]++;
        VERBOSE_PRINT("tobacco available\n");
        uthread_cond_signal(a->tobacco);
        VERBOSE_PRINT("paper available\n");
        uthread_cond_signal(a->paper);
        break;
    }
    VERBOSE_PRINT("agent is waiting for smoker to smoke\n");
    uthread_cond_wait(a->smoke);
    VERBOSE_PRINT("agent finished waiting for smoker to smoke\n");
    VERBOSE_PRINT("\n");
  }

  uthread_mutex_unlock(a->mutex);
  return NULL;
}

int main(int argc, char** argv) {
  struct Agent* a = createAgent();
  uthread_t agent_thread;
  uthread_t paper_smoker;
  uthread_t tobacco_smoker;
  uthread_t match_smoker;

  uthread_t paper_handler;
  uthread_t tobacco_handler;
  uthread_t match_handler;

  uthread_init(3);

  uthread_mutex_t smoker_mutex = uthread_mutex_create();
  uthread_cond_t broadcast_cond = uthread_cond_create(smoker_mutex);

  HandleResourceThreadMsg* paper_handler_msg
      = createHandleResourceThreadMsg(2,
                                      a->paper,
                                      broadcast_cond,
                                      a->thread_ready_cond,
                                      a->mutex,
                                      smoker_mutex);
  HandleResourceThreadMsg* tobacco_handler_msg
      = createHandleResourceThreadMsg(4,
                                      a->tobacco,
                                      broadcast_cond,
                                      a->thread_ready_cond,
                                      a->mutex,
                                      smoker_mutex);
  HandleResourceThreadMsg* match_handler_msg
      = createHandleResourceThreadMsg(1,
                                      a->match,
                                      broadcast_cond,
                                      a->thread_ready_cond,
                                      a->mutex,
                                      smoker_mutex);

  SmokerThreadMsg* paper_smoker_msg
      = createSmokerThreadMsg(2,
                              5,
                              a->smoke,
                              broadcast_cond,
                              a->thread_ready_cond,
                              a->mutex,
                              smoker_mutex);
  SmokerThreadMsg* tobacco_smoker_msg
      = createSmokerThreadMsg(4,
                              3,
                              a->smoke,
                              broadcast_cond,
                              a->thread_ready_cond,
                              a->mutex,
                              smoker_mutex);
  SmokerThreadMsg* match_smoker_msg
      = createSmokerThreadMsg(1,
                              6,
                              a->smoke,
                              broadcast_cond,
                              a->thread_ready_cond,
                              a->mutex,
                              smoker_mutex);

  paper_smoker = uthread_create(handle_smoke, paper_smoker_msg);
  tobacco_smoker = uthread_create(handle_smoke, tobacco_smoker_msg);
  match_smoker = uthread_create(handle_smoke, match_smoker_msg);

  paper_handler = uthread_create(handle_resource, paper_handler_msg);
  tobacco_handler = uthread_create(handle_resource, tobacco_handler_msg);
  match_handler = uthread_create(handle_resource, match_handler_msg);
  agent_thread = uthread_create(agent, a);

  uthread_join(agent_thread, NULL);

  assert(signal_count[MATCH] == smoke_count[MATCH]);
  assert(signal_count[PAPER] == smoke_count[PAPER]);
  assert(signal_count[TOBACCO] == smoke_count[TOBACCO]);
  assert(smoke_count[MATCH] + smoke_count[PAPER] + smoke_count[TOBACCO]
         == NUM_ITERATIONS);

  printf("Smoke counts: %d matches, %d paper, %d tobacco\n",
         smoke_count[MATCH],
         smoke_count[PAPER],
         smoke_count[TOBACCO]);

  return 0;
}