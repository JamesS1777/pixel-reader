#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "log.h"

/* Removes spaces from both ends of a string */
static char *trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) end--;
    *end = '\0';
    return s;
}

/* Starting values before the file is read */
static void set_defaults(Config *cfg) {
    memset(cfg, 0, sizeof *cfg);
    strcpy(cfg->window_title, "Clash of Clans");
    cfg->dry_run = 1;
    cfg->stop_key = VK_F12;
    strcpy(cfg->stop_key_name, "F12");
    cfg->max_attacks = 5;
    cfg->max_unknown = 20;
    cfg->battle_timeout_s = 180;
    cfg->loop_delay_ms = 500;
    cfg->click_delay_ms = 800;
    cfg->deploy_delay_ms = 60;
    cfg->color_tolerance = 20;
}

/* "412,288" -> Point */
static int parse_point(const char *value, Point *p) {
    if (sscanf(value, "%d , %d", &p->x, &p->y) != 2) return 0;
    p->set = 1;
    return 1;
}

/* "412,288,255,255,255" -> StateCheck */
static int parse_check(const char *value, StateCheck *c) {
    if (sscanf(value, "%d , %d , %d , %d , %d", &c->point.x, &c->point.y,
               &c->color.r, &c->color.g, &c->color.b) != 5) return 0;
    c->point.set = 1;
    return 1;
}

/* "1,400,300,10" -> DeployStep (slot, x, y, count) */
static int parse_deploy(const char *value, DeployStep *d) {
    if (sscanf(value, "%d , %d , %d , %d", &d->slot, &d->x, &d->y, &d->count) != 4) return 0;
    d->set = 1;
    return 1;
}

/* "F12" -> VK_F12 */
static int parse_f_key(const char *value, Config *cfg) {
    if (toupper((unsigned char)value[0]) != 'F') return 0;
    int n = atoi(value + 1);
    if (n < 1 || n > 12) return 0;
    cfg->stop_key = VK_F1 + (n - 1);
    snprintf(cfg->stop_key_name, sizeof cfg->stop_key_name, "F%d", n);
    return 1;
}

int config_load(const char *path, Config *cfg) {
    set_defaults(cfg);

    FILE *f = fopen(path, "r");
    if (f == NULL) return 0;

    char line[256];
    int line_no = 0;
    while (fgets(line, sizeof line, f) != NULL) {
        line_no++;

        /* Cut off comments */
        char *comment = strpbrk(line, "#;");
        if (comment != NULL) *comment = '\0';

        /* Skip blank lines and [section] headers */
        char *text = trim(line);
        if (text[0] == '\0' || text[0] == '[') continue;

        /* Split "key = value" */
        char *equals = strchr(text, '=');
        if (equals == NULL) {
            log_msg("config line %d: no '=' found, skipped", line_no);
            continue;
        }
        *equals = '\0';
        char *key = trim(text);
        char *value = trim(equals + 1);
        if (value[0] == '\0') continue;   /* left blank on purpose */

        int ok = 1;
        if      (strcmp(key, "window_title") == 0) snprintf(cfg->window_title, sizeof cfg->window_title, "%s", value);
        else if (strcmp(key, "dry_run") == 0)          cfg->dry_run = atoi(value);
        else if (strcmp(key, "stop_key") == 0)         ok = parse_f_key(value, cfg);
        else if (strcmp(key, "max_attacks") == 0)      cfg->max_attacks = atoi(value);
        else if (strcmp(key, "max_unknown") == 0)      cfg->max_unknown = atoi(value);
        else if (strcmp(key, "battle_timeout_s") == 0) cfg->battle_timeout_s = atoi(value);
        else if (strcmp(key, "loop_delay_ms") == 0)    cfg->loop_delay_ms = atoi(value);
        else if (strcmp(key, "click_delay_ms") == 0)   cfg->click_delay_ms = atoi(value);
        else if (strcmp(key, "deploy_delay_ms") == 0)  cfg->deploy_delay_ms = atoi(value);
        else if (strcmp(key, "color_tolerance") == 0)  cfg->color_tolerance = atoi(value);
        else if (strcmp(key, "home_check") == 0)       ok = parse_check(value, &cfg->home_check);
        else if (strcmp(key, "scout_check") == 0)      ok = parse_check(value, &cfg->scout_check);
        else if (strcmp(key, "results_check") == 0)    ok = parse_check(value, &cfg->results_check);
        else if (strcmp(key, "btn_attack") == 0)       ok = parse_point(value, &cfg->btn_attack);
        else if (strcmp(key, "btn_find_match") == 0)   ok = parse_point(value, &cfg->btn_find_match);
        else if (strcmp(key, "btn_return_home") == 0)  ok = parse_point(value, &cfg->btn_return_home);
        else if (strncmp(key, "slot_", 5) == 0) {
            int n = atoi(key + 5);
            if (n >= 1 && n <= MAX_SLOTS) ok = parse_point(value, &cfg->slots[n - 1]);
            else ok = 0;
        }
        else if (strncmp(key, "deploy_", 7) == 0) {
            int n = atoi(key + 7);
            if (n >= 1 && n <= MAX_DEPLOY_STEPS) ok = parse_deploy(value, &cfg->deploy[n - 1]);
            else ok = 0;
        }
        else {
            log_msg("config line %d: unknown setting '%s' (typo?)", line_no, key);
            continue;
        }

        if (!ok) log_msg("config line %d: couldn't read value for '%s'", line_no, key);
    }

    fclose(f);
    return 1;
}

void config_print(const Config *cfg) {
    int slots = 0, steps = 0;
    for (int i = 0; i < MAX_SLOTS; i++) if (cfg->slots[i].set) slots++;
    for (int i = 0; i < MAX_DEPLOY_STEPS; i++) if (cfg->deploy[i].set) steps++;

    log_msg("Config: window \"%s\", dry run %s, stop key %s, max attacks %d",
            cfg->window_title, cfg->dry_run ? "ON" : "OFF", cfg->stop_key_name, cfg->max_attacks);
    log_msg("State checks: home %s, scout %s, results %s",
            cfg->home_check.point.set ? "set" : "BLANK",
            cfg->scout_check.point.set ? "set" : "BLANK",
            cfg->results_check.point.set ? "set" : "BLANK");
    log_msg("Buttons: attack %s, find match %s, return home %s",
            cfg->btn_attack.set ? "set" : "BLANK",
            cfg->btn_find_match.set ? "set" : "BLANK",
            cfg->btn_return_home.set ? "set" : "BLANK");
    log_msg("Troop slots set: %d, deploy steps set: %d", slots, steps);
}