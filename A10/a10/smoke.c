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
  uthread_cond_t thread_ready;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc(sizeof(struct Agent));
  agent->mutex = uthread_mutex_create();
  agent->paper = uthread_cond_create(agent->mutex);
  agent->match = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke = uthread_cond_create(agent->mutex);
  agent->thread_ready = uthread_cond_create(agent->mutex);
  return agent;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource { MATCH = 1, PAPER = 2, TOBACCO = 4 };
char* resource_name[] = {"", "match", "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count[5];  // # of times resource signalled
int smoke_count[5];   // # of times smoker with resource smoked

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
  while (num_active_threads < 6) uthread_cond_wait(a->thread_ready);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
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
  }

  uthread_mutex_unlock(a->mutex);
  return NULL;
}

int available_resources[5] = {0, 0, 0, 0, 0};

struct Bridge {
  int resource_monitored;
  struct Agent* agent;
  uthread_cond_t can_smoke[5];
};

struct Bridge* createBridge(int resource_monitored,
                            struct Agent* agent,
                            uthread_cond_t can_smoke[5]) {
  struct Bridge* bridge = malloc(sizeof(struct Bridge));
  bridge->resource_monitored = resource_monitored;
  bridge->agent = agent;
  for (int i = 0; i < 5; i++) bridge->can_smoke[i] = can_smoke[i];
  return bridge;
}

uthread_cond_t cond(struct Agent* agent, int resource) {
  if (resource == MATCH) return agent->match;
  if (resource == TOBACCO) return agent->tobacco;
  if (resource == PAPER) return agent->paper;
}

int count_resources() {
  int count = 0;
  for (int i = 0; i < 5; i++)
    if (available_resources[i] > 0) count += 1;
  return count;
}

void* bridge(void* bv) {
  struct Bridge* b = bv;
  char* name = resource_name[b->resource_monitored];
  uthread_mutex_lock(b->agent->mutex);
  while (1) {
    VERBOSE_PRINT("bridge waiting for %s\n", name);
    num_active_threads++;
    uthread_cond_signal(b->agent->thread_ready);
    uthread_cond_wait(cond(b->agent, b->resource_monitored));
    num_active_threads--;
    available_resources[b->resource_monitored] = 1;

    VERBOSE_PRINT(
        "bridge received %s, new total: %d\n", name, count_resources());

    if (count_resources() != 2) continue;
    VERBOSE_PRINT("enough resources collected\n");

    int resource_held = 0;
    for (int i = 0; i < 5; i++) {
      if (available_resources[i]) resource_held += i;
    }

    if (resource_held == (MATCH | TOBACCO)) {
      VERBOSE_PRINT("signaled %s to smoke\n", resource_name[PAPER]);
      uthread_cond_signal(b->can_smoke[PAPER]);
      VERBOSE_PRINT("cleared resource stash\n");
    }
    if (resource_held == (PAPER | TOBACCO)) {
      VERBOSE_PRINT("signaled %s to smoke\n", resource_name[MATCH]);
      uthread_cond_signal(b->can_smoke[MATCH]);
      VERBOSE_PRINT("cleared resource stash\n");
    }
    if (resource_held == (MATCH | PAPER)) {
      VERBOSE_PRINT("signaled %s to smoke\n", resource_name[TOBACCO]);
      uthread_cond_signal(b->can_smoke[TOBACCO]);
      VERBOSE_PRINT("cleared resource stash\n");
    }
    for (int i = 0; i < 5; i++) available_resources[i] = 0;
  }
  uthread_mutex_lock(b->agent->mutex);
}

struct Smoker {
  int resource_held;
  struct Agent* agent;
  uthread_cond_t can_smoke;
};

struct Smoker* createSmoker(int resource_held,
                            struct Agent* agent,
                            uthread_cond_t can_smoke) {
  struct Smoker* smoker = malloc(sizeof(struct Smoker));
  smoker->resource_held = resource_held;
  smoker->agent = agent;
  smoker->can_smoke = can_smoke;
  return smoker;
}

void* smoker(void* sv) {
  struct Smoker* s = sv;
  char* name = resource_name[s->resource_held];
  uthread_mutex_lock(s->agent->mutex);
  while (1) {
    VERBOSE_PRINT("smoker %s waiting to smoke\n", name);
    num_active_threads++;
    uthread_cond_signal(s->agent->thread_ready);
    uthread_cond_wait(s->can_smoke);
    num_active_threads--;
    VERBOSE_PRINT("smoker %s smoked\n\n", name);
    smoke_count[s->resource_held]++;
    uthread_cond_signal(s->agent->smoke);
  }
  uthread_mutex_unlock(s->agent->mutex);
}

int main(int argc, char** argv) {
  struct Agent* a = createAgent();
  uthread_t agent_thread;

  uthread_init(5);

  // TODO

  uthread_cond_t can_smoke[5] = {NULL,
                                 uthread_cond_create(a->mutex),
                                 uthread_cond_create(a->mutex),
                                 NULL,
                                 uthread_cond_create(a->mutex)};

  struct Bridge* bridges[5];
  bridges[MATCH] = createBridge(MATCH, a, can_smoke);
  bridges[PAPER] = createBridge(PAPER, a, can_smoke);
  bridges[TOBACCO] = createBridge(TOBACCO, a, can_smoke);

  uthread_t bridge_threads[5];
  bridge_threads[MATCH] = uthread_create(bridge, bridges[MATCH]);
  bridge_threads[PAPER] = uthread_create(bridge, bridges[PAPER]);
  bridge_threads[TOBACCO] = uthread_create(bridge, bridges[TOBACCO]);

  struct Smoker* smokers[5];
  smokers[MATCH] = createSmoker(MATCH, a, can_smoke[MATCH]);
  smokers[PAPER] = createSmoker(PAPER, a, can_smoke[PAPER]);
  smokers[TOBACCO] = createSmoker(TOBACCO, a, can_smoke[TOBACCO]);

  uthread_t smoker_threads[5];
  smoker_threads[MATCH] = uthread_create(smoker, smokers[MATCH]);
  smoker_threads[PAPER] = uthread_create(smoker, smokers[PAPER]);
  smoker_threads[TOBACCO] = uthread_create(smoker, smokers[TOBACCO]);

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
