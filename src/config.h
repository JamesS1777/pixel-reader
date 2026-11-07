#ifndef CONFIG_H
#define CONFIG_H

#include "pixel.h"

#define MAX_SLOTS 10
#define MAX_DEPLOY_STEPS 30

typedef struct {
    int x;
    int y;
    int set;
} Point;

typedef struct {
    Point point;
    Color color;
} StateCheck;

typedef struct {
    int slot;
    int x;
    int y;
    int count;
    int set;
} DeployStep;

typedef struct {
    char window_title[128];

    int dry_run;
    int stop_key;
    char stop_key_name[8];
    int max_attacks;
    int max_unknown;
    int battle_timeout_s;

    int loop_delay_ms;
    int click_delay_ms;
    int deploy_delay_ms;

    int color_tolerance;

    StateCheck home_check;
    StateCheck scout_check;
    StateCheck results_check;

    Point btn_attack;
    Point btn_find_match;
    Point btn_return_home;

    Point slots[MAX_SLOTS];
    DeployStep deploy[MAX_DEPLOY_STEPS];
} Config;

int config_load(const char *path, Config *cfg);
void config_print(const Config *cfg);

#endif